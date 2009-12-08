#ifndef WAGOCONNECT_H
#define WAGOCONNECT_H

#include <QObject>
#include <QtNetwork>
#include <QtGui>

#include "detectip.h"

#define WAGO_FW_VESION          "1.7"

enum { WAGO_CONNECTED, WAGO_DISCONNECTED };
enum { WERROR_NOERROR, WERROR_CONNECT_FAILED, WERROR_NOTCONNECTED, WERROR_TIMEOUT };

class WagoCommand
{
        public:
                WagoCommand(): object_cb(NULL), timeout(0) { }
                WagoCommand(QString cmd, QObject *obj, QString slot):
                        command(cmd), object_cb(obj), slot_name(slot), timeout(0) { }

                void emitCallback()
                {
                        if (object_cb)
                                QMetaObject::invokeMethod(object_cb,
                                                          slot_name.toLocal8Bit().data(),
                                                          Q_ARG(QString, command),
                                                          Q_ARG(QString, response));
                }

                QString command;
                QString response;
                QObject *object_cb;
                QString slot_name;
                int timeout;

                bool needResponse()
                {
                        if (object_cb && slot_name != "")
                                return true;
                        return false;
                }

                bool isNull()
                {
                        if (command == "")
                                return true;
                        return false;
                }
};

class WagoConnect : public QObject
{
        Q_OBJECT

        private:
                int connect_status;
                QString wago_ip, wago_fwversion, calaos_user, calaos_password;
                bool use_proxy;
                QUdpSocket *udpSocket;
                QHttp *httpProxy;
                QTimer *mainTimer, *heartbeatTimer;
                int timeout_counter;

                QQueue<WagoCommand> commands;

                WagoCommand current_cmd;

                WagoConnect(QObject *parent = NULL);

        public:
                static WagoConnect &Instance()
                {
                        static WagoConnect wc;

                        return wc;
                }
                ~WagoConnect();

                /*
                 * Get connection status either WAGO_CONNECTED or WAGO_DISCONNECTED
                 */
                int getConnectionStatus() { return connect_status; }

                /*
                 * Get wago device firmware revision
                 */
                QString getWagoVersion() { return wago_fwversion; }

                /*
                 * Get wago IP address
                 */
                QString getWagoIP() { return wago_ip; }

                /*
                 * Connect to wago device at address <ip>. The connection is kept alive
                 * using WAGO_GET_VERSION.
                 * If proxy is set, connection is made through calaos_server and http requests
                 * It allows to program the wago device from the LAN or even Internet.
                 *
                 */
                void Connect(QString ip = "10.0.0.123", bool proxy = false);

                /*
                 * Disconnect the previously connected wago device.
                 */
                void Disconnect();

                /*
                 * try to send a command
                 */
                void SendCommand(QString cmd, QObject *object = NULL, QString slotName = "");

                void setUsername(QString user) { calaos_user = user; }
                void setPassword(QString pass) { calaos_password = pass; }

        signals:
                /*
                 * Signal emited when connection established successfully
                 */
                void connected(QString &ip, bool proxy);

                /*
                 * Signal emited when disconnected
                 */
                void disconnected();

                /*
                 * Signal emited when the firmware revision on the wago device needs
                 * to be updated.
                 */
                void updateNeeded(QString &version);

                /*
                 * Signal emited when <command> command is sent to device
                 */
                void commandSent(QString &command);

                /*
                 * Signal emited when response has been received
                 */
                void responseReceived(QString &command, QString response);

                /*
                 * Signal emited when an error occured
                 */
                void error(int error);

        private slots:
                void readPendingDatagrams();

                void mainTick();
                void heartbeatTick();

                void heartbeat_cb(QString command, QString response);
};

#endif // WAGOCONNECT_H

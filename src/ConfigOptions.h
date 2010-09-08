#ifndef CONFIGOPTIONS_H
#define CONFIGOPTIONS_H

#include <QtGui>

class ConfigOptions
{
        public:
                static ConfigOptions &Instance()
                {
                        static ConfigOptions c;
                        return c;
                }

                QString getUsername() { return username; }
                void setUsername(QString s) { username = s; }

                QString getPassword() { return password; }
                void setPassword(QString s) { password = s; }

                QString getHost() { return host; }
                void setHost(QString s) { host = s; }

                QString getWagoHost() { return wago_host; }
                void setWagoHost(QString s) { wago_host = s; }

                bool useCalaosFr() { return use_calaosfr; }
                void setUseCalaosFr(bool s) { use_calaosfr = s; }

                bool useCalaosServer() { return use_wago_calaosd; }
                void setUseCalaosServer(bool s) { use_wago_calaosd = s; }

        private:
                ConfigOptions();

                QString username;
                QString password;
                QString host;
                QString wago_host;
                bool use_calaosfr;
                bool use_wago_calaosd;

};

#endif // CONFIGOPTIONS_H

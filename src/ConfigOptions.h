#ifndef CONFIGOPTIONS_H
#define CONFIGOPTIONS_H

#include <QtWidgets>

class ConfigOptions
{
        public:
                static ConfigOptions &Instance()
                {
                        static ConfigOptions c;
                        return c;
                }

                QString getUsername() { return username; }
                void setUsername(QString s) { username = s; settings.setValue("calaos/cn_user", username); }

                QString getPassword() { return password; }
                void setPassword(QString s) { password = s; settings.setValue("calaos/cn_pass", password); }

                QString getHost() { return host; }
                void setHost(QString s) { host = s; settings.setValue("calaos/calaos_host", host); }

                QString getWagoHost() { return wago_host; }
                void setWagoHost(QString s) { wago_host = s; settings.setValue("calaos/wago_host", wago_host); }

                bool useCalaosFr() { return use_calaosfr; }
                void setUseCalaosFr(bool s) { use_calaosfr = s; settings.setValue("calaos/use_calaos_fr", use_calaosfr); }

                bool useCalaosServer() { return use_wago_calaosd; }
                void setUseCalaosServer(bool s) { use_wago_calaosd = s; settings.setValue("calaos/use_wago_calaosd", use_wago_calaosd); }

                void saveConfig() { settings.sync(); }

                void setOption(const QString &key, const QVariant &value);
                QVariant getOption(const QString &key);
                bool optionExists(const QString &key) { return settings.contains(key); }

        private:
                ConfigOptions();

                QString username;
                QString password;
                QString host;
                QString wago_host;
                bool use_calaosfr;
                bool use_wago_calaosd;

                QSettings settings;

};

#endif // CONFIGOPTIONS_H

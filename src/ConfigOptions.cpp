#include "ConfigOptions.h"

ConfigOptions::ConfigOptions()
{
    wago_host = settings.value("calaos/wago_host", "10.0.0.123").toString();
    host = settings.value("calaos/calaos_host", "192.168.1.100").toString();

    username = settings.value("calaos/cn_user").toString();
    password = settings.value("calaos/cn_pass").toString();

    use_calaosfr = settings.value("calaos/use_calaos_fr").toBool();
    use_wago_calaosd = settings.value("calaos/use_wago_calaosd").toBool();
}

void ConfigOptions::setOption(const QString &key, const QVariant &value)
{
    settings.setValue(key, value);
}

QVariant ConfigOptions::getOption(const QString &key)
{
    return settings.value(key);
}

#include "ConfigOptions.h"
#include <QRandomGenerator>

ConfigOptions::ConfigOptions()
{
    wago_host = settings.value("calaos/wago_host", "10.0.0.123").toString();
    quint64 cryptKey = settings.value("calaos/config_saved", 0).toLongLong();

    if (cryptKey == 0)
    {
        cryptKey = QRandomGenerator::global()->generate64();
        settings.setValue("calaos/config_saved", cryptKey);
    }

    crypt.setKey(cryptKey);
    crypt.setIntegrityProtectionMode(SimpleCrypt::ProtectionHash);

    if (settings.contains("calaos/calaos_host"))
    {
        CalaosAuth old;
        old.username = settings.value("calaos/cn_user").toString();
        old.password = settings.value("calaos/cn_pass").toString();
        old.host = settings.value("calaos/calaos_host").toString();
        addAuth(old);

        settings.remove("calaos/calaos_host");
        settings.remove("calaos/cn_user");
        settings.remove("calaos/cn_pass");
    }
}

void ConfigOptions::setOption(const QString &key, const QVariant &value)
{
    settings.setValue(key, value);
}

QVariant ConfigOptions::getOption(const QString &key)
{
    return settings.value(key);
}

QVector<CalaosAuth> ConfigOptions::getAuthList()
{
    QVector<CalaosAuth> authList;

    int size = settings.beginReadArray("authList");
    for (int i = 0; i < size; ++i)
    {
        settings.setArrayIndex(i);
        CalaosAuth auth;
        auth.username = settings.value("username").toString();
        auth.password = crypt.decryptToString(settings.value("password").toString());
        auth.host = settings.value("host").toString();
        authList.append(auth);
    }
    settings.endArray();

    return authList;
}

void ConfigOptions::addAuth(const CalaosAuth &auth)
{
    QVector<CalaosAuth> authList = getAuthList();
    bool exists = false;

    for (int i = 0; i < authList.size(); ++i)
    {
        if (authList[i].host == auth.host)
        {
            authList[i].username = auth.username;
            authList[i].password = auth.password;
            exists = true;
            break;
        }
    }

    if (!exists)
        authList.append(auth);

    settings.remove("authList");

    settings.beginWriteArray("authList", authList.size());
    for (int i = 0; i < authList.size(); ++i)
    {
        settings.setArrayIndex(i);
        settings.setValue("username", authList[i].username);
        settings.setValue("password", crypt.encryptToString(authList[i].password));
        settings.setValue("host", authList[i].host);
    }
    settings.endArray();
}

void ConfigOptions::deleteAuth(const QString &host)
{
    QVector<CalaosAuth> authList = getAuthList();

    for (int i = 0; i < authList.size(); ++i)
    {
        if (authList[i].host == host)
        {
            authList.removeAt(i);
            break;
        }
    }

    settings.remove("authList");

    settings.beginWriteArray("authList", authList.size());
    for (int i = 0; i < authList.size(); ++i)
    {
        settings.setArrayIndex(i);
        settings.setValue("username", authList[i].username);
        settings.setValue("password", crypt.encryptToString(authList[i].password));
        settings.setValue("host", authList[i].host);
    }
    settings.endArray();
}

#ifndef ADMINCACHE_H
#define ADMINCACHE_H

#include <QHash>
#include <QString>

// Admin password and admin session of each box, for the life of the application. Memory only:
// nothing here may reach QSettings, a file or a log.
class AdminCache
{
public:
    struct Entry
    {
        QString user;
        QString adminPass;
        QString session;
    };

    static AdminCache &instance();
    ~AdminCache() { clear(); }

    // The entry of this host when it was unlocked as this user; empty adminPass otherwise.
    Entry get(const QString &host, const QString &user) const;
    bool has(const QString &host, const QString &user) const;

    void store(const QString &host, const QString &user, const QString &adminPass, const QString &session);
    void dropSession(const QString &host);
    // Forgets the password and the session of one host (all hosts for clear()).
    void invalidate(const QString &host);
    void clear();

    // Overwrites then releases. Copies made elsewhere (QLineEdit, request bodies) are out of
    // reach; store() keeps a private deep copy so this hits the buffer the cache owns.
    static void wipe(QString &s);

private:
    static void wipe(Entry &e);

    QHash<QString, Entry> m_entries;
};

#endif // ADMINCACHE_H

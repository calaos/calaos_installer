#include "AdminCache.h"

AdminCache &AdminCache::instance()
{
    static AdminCache cache;
    return cache;
}

AdminCache::Entry AdminCache::get(const QString &host, const QString &user) const
{
    const auto it = m_entries.constFind(host);
    if (it == m_entries.constEnd() || it->user != user)
        return Entry();
    return *it;
}

bool AdminCache::has(const QString &host, const QString &user) const
{
    return !get(host, user).adminPass.isEmpty();
}

void AdminCache::store(const QString &host, const QString &user, const QString &adminPass, const QString &session)
{
    Entry &e = m_entries[host];
    if (e.user != user || e.adminPass != adminPass)
        wipe(e.adminPass);
    wipe(e.session);
    e.user = user;
    e.adminPass = QString(adminPass.constData(), adminPass.size());
    e.session = QString(session.constData(), session.size());
}

void AdminCache::dropSession(const QString &host)
{
    const auto it = m_entries.find(host);
    if (it != m_entries.end())
        wipe(it->session);
}

void AdminCache::invalidate(const QString &host)
{
    const auto it = m_entries.find(host);
    if (it == m_entries.end())
        return;
    wipe(*it);
    m_entries.erase(it);
}

void AdminCache::clear()
{
    for (Entry &e : m_entries)
        wipe(e);
    m_entries.clear();
}

void AdminCache::wipe(QString &s)
{
    s.fill(QChar(0));
    s.clear();
}

void AdminCache::wipe(Entry &e)
{
    wipe(e.adminPass);
    wipe(e.session);
}

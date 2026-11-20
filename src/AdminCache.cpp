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
        wipe(e);
    e.user = user;
    e.adminPass = adminPass;
    e.session = session;
}

void AdminCache::dropSession(const QString &host)
{
    const auto it = m_entries.find(host);
    if (it != m_entries.end())
        it->session.clear();
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

void AdminCache::wipe(Entry &e)
{
    e.adminPass.fill(QChar(0));
    e.session.fill(QChar(0));
    e.adminPass.clear();
    e.session.clear();
}

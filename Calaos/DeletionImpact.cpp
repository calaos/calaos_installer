#include <DeletionImpact.h>
#include <Condition.h>
#include <ListeRoom.h>
#include <ListeRule.h>
#include <algorithm>
#include <set>

using namespace Calaos;

namespace
{

bool contains(const std::vector<IOBase *> &ios, IOBase *io)
{
    return std::find(ios.begin(), ios.end(), io) != ios.end();
}

//A condition that makes the rule run by itself when something changes. One
//saved with trigger="false" only takes part in the evaluation.
bool providesTrigger(Condition *cond, const std::vector<IOBase *> &gone)
{
    if (!cond->isTrigger())
        return false;

    switch (cond->getType())
    {
    case COND_SCRIPT:
        for (int k = 0;k < cond->getScriptInputSize();k++)
            if (!contains(gone, cond->getScriptInput(k)))
                return true;
        return false;
    case COND_START:
        return true;
    default:
        //Standard and output conditions on a deleted IO remove the rule instead.
        return cond->get_size() > 0 || cond->getOutput();
    }
}

bool becomesInert(Rule *rule, const std::vector<IOBase *> &gone)
{
    bool lostTrigger = false;
    for (int j = 0;j < rule->get_size_conds();j++)
    {
        Condition *cond = rule->get_condition(j);
        if (providesTrigger(cond, gone))
            return false;

        if (cond->getType() == COND_SCRIPT)
            for (int k = 0;k < cond->getScriptInputSize();k++)
                lostTrigger |= contains(gone, cond->getScriptInput(k));
    }

    return lostTrigger;
}

}

DeletionImpact Calaos::deletionImpact(IOBase *io)
{
    return deletionImpact(ListeRoom::iosDeletedWith(io));
}

DeletionImpact Calaos::deletionImpactOfRoom(Room *room)
{
    std::vector<IOBase *> ios;
    for (int i = 0;i < room->get_size_in();i++)
        ios.push_back(room->get_input(i));
    for (int i = 0;i < room->get_size_out();i++)
        ios.push_back(room->get_output(i));

    return deletionImpact(ios);
}

DeletionImpact Calaos::deletionImpact(const std::vector<IOBase *> &requested)
{
    DeletionImpact res;
    for (IOBase *io: requested)
        for (IOBase *gone: ListeRoom::iosDeletedWith(io))
            if (!contains(res.ios, gone))
                res.ios.push_back(gone);

    //Same id/iid/oid names as ListeRule::RemoveRulesUsing reserves.
    std::set<std::string> names;
    for (IOBase *io: res.ios)
        for (const char *k: { "id", "iid", "oid" })
        {
            const std::string v = io->get_param(k);
            if (!v.empty())
                names.insert(v);
        }

    ListeRule &rules = ListeRule::Instance();
    for (int i = 0;i < rules.size();i++)
    {
        Rule *rule = rules.get_rule(i);

        bool removed = false;
        for (IOBase *io: res.ios)
            removed |= ListeRule::ruleHolds(rule, io);

        if (removed)
        {
            res.removedRules.push_back(rule);
            continue;
        }

        if (becomesInert(rule, res.ios))
            res.inertRules.push_back(rule);

        for (const std::string &id: ListeRule::variablesNaming(rule, names))
            res.orphanVariables.push_back({ rule, id, false });
        for (const std::string &id: ListeRule::attachmentsNaming(rule, names))
            res.orphanVariables.push_back({ rule, id, true });
    }

    //Links name IOs by id. A device that goes with them keeps nothing dangling.
    std::set<std::string> goneIds;
    for (IOBase *io: res.ios)
        if (!io->get_param("id").empty())
            goneIds.insert(io->get_param("id"));

    for (int r = 0;r < ListeRoom::Instance().size();r++)
    {
        for (Device *dev: ListeRoom::Instance().get_room(r)->get_devices())
        {
            if (contains(res.ios, dev))
                continue;

            for (const DeviceLink &link: dev->links())
            {
                if (goneIds.count(link.io))
                    res.brokenLinks.push_back({ dev, link.name, link.io });
                if (!link.commandIo.empty() && goneIds.count(link.commandIo))
                    res.brokenLinks.push_back({ dev, link.name, link.commandIo });
            }
        }
    }

    return res;
}

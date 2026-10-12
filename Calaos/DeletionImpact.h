#ifndef DELETIONIMPACT_H
#define DELETIONIMPACT_H

#include <IOBase.h>
#include <Device.h>
#include <Room.h>
#include <Rule.h>
#include <string>
#include <vector>

namespace Calaos
{

struct OrphanVariable
{
    Rule *rule;
    std::string id; //still written in the rule, naming nothing once the IO is gone
};

struct BrokenLink
{
    Device *device; //survives the deletion
    std::string link;
    std::string io; //id the link keeps pointing at
};

// What deleting some IOs does to the rest of the project, computed from the
// model as it stands: Rule and Device pointers are valid until the deletion.
struct DeletionImpact
{
    //Everything destroyed: the IO first, then a device's owned IOs.
    std::vector<IOBase *> ios;
    std::vector<Rule *> removedRules;
    //Script rules left with no trigger: the server never runs them.
    std::vector<Rule *> inertRules;
    std::vector<OrphanVariable> orphanVariables;
    std::vector<BrokenLink> brokenLinks;

    bool affectsRules() const
    {
        return !removedRules.empty() || !inertRules.empty() || !orphanVariables.empty();
    }
    bool isEmpty() const { return !affectsRules() && brokenLinks.empty(); }
};

DeletionImpact deletionImpact(IOBase *io);
DeletionImpact deletionImpact(const std::vector<IOBase *> &ios);
//Every IO of the room: what removing the room destroys.
DeletionImpact deletionImpactOfRoom(Room *room);

}

#endif // DELETIONIMPACT_H

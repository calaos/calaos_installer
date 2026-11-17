#ifndef CLIMATEPROGRAM_H
#define CLIMATEPROGRAM_H

#include <Device.h>
#include <IOBase.h>
#include <QString>
#include <string>
#include <vector>

namespace Calaos
{

// Time program of a climate (docs/17_device_templates.md of calaos_base): the
// server creates its IOs from the device params and links each under a role
// of the same name. Their ids are never rebuilt from the device id: a taken id
// gets a suffix, so an IO is always found through its link.
namespace ClimateProgram
{

enum class Set { Heat, Cool };

constexpr int kMaxLevels = 6;

//Climate impls that run the program.
bool supports(Device *dev);

bool isProgramParam(const std::string &name);
//The device param, or `def` when absent.
std::string param(Device *dev, const std::string &name, const std::string &def);
void setParam(Device *dev, const std::string &name, const std::string &value);

//Level count of a set as the server reads it: above 6 is 6, anything but a
//non-negative whole number is 0.
int levels(Device *dev, Set set);
void setLevels(Device *dev, Set set, int count);

//Whether the climate offers the mode of a set, from what DeviceCommand::offer
//reads of the published list_rule (modes param, default of a linked output).
//Unlinked outputs do not count; a device without documentation offers all.
bool offers(Device *dev, Set set);

std::string levelRole(Set set, int level);
std::string setpointRole(Set set, int level);
std::string defaultRole(Set set);

//Owned IO linked under role, or nullptr.
IOBase *ioForRole(Device *dev, const std::string &role);
bool isProgramRole(const std::string &role);
//Program role of an owned IO, empty for any other IO.
std::string roleOf(IOBase *io);
bool isProgramIO(IOBase *io);
//The program IOs of a device, in file order.
std::vector<IOBase *> programIos(Device *dev);

//Program IOs are the server's: they go with their device, never alone.
bool canDeleteAlone(IOBase *io);

//"Heating level 1", "Program enabled"...
QString roleLabel(const std::string &role);
//How rules name an IO: "<climate> › <role>" for a program IO, its name otherwise.
QString ruleLabel(IOBase *io);

//Level IOs (range and setpoint) above these counts, which applying them deletes,
//plus the defaults of a set left empty and program_enabled when both are.
std::vector<IOBase *> iosRemovedBy(Device *dev, int heat, int cool);
//Writes the counts and deletes what iosRemovedBy lists with their links and
//the rules using them, as the server does when it reloads the file.
//Returns what was deleted; the pointers are only good for comparison.
std::vector<IOBase *> applyLevels(Device *dev, int heat, int cool);

//Decimal value of an InternalInt as written in io.xml, `def` when unset.
double ioValue(IOBase *io, double def);
void setIoValue(IOBase *io, double value);
bool ioBool(IOBase *io, bool def);
void setIoBool(IOBase *io, bool value);

}

}

#endif // CLIMATEPROGRAM_H

#ifndef DEVICE_H
#define DEVICE_H

#include <IOBase.h>
#include <string>
#include <utility>
#include <vector>

namespace Calaos
{

struct DeviceLink
{
    std::string name;
    std::string io;
    std::string commandIo; //empty when the role has no separate command IO
};

// <calaos:device>. It is an IO itself, listed in its room as both input and
// output like the server's, so that rules can target it. Its owned IOs are
// ordinary IOs of the room tagged with the device id: the device holds no
// pointer to them, so deleting or moving one through the existing IO paths
// cannot leave anything dangling here.
class Device: public IOBase
{
public:
    explicit Device(Params &p);

    std::string family() { return get_param("type"); }
    std::string impl() { return get_param("impl"); }

    //Renaming the device carries its owned IOs along.
    void set_param(std::string opt, std::string val) override;

    //In file order. Empty for a second device reusing the id of a first one,
    //which keeps the tagged IOs: they would otherwise be written twice.
    std::vector<IOBase *> ownedIos();
    bool adoptIO(IOBase *io);

    std::vector<DeviceLink> &links() { return m_links; }
    std::vector<std::pair<std::string, std::string>> &deviceParams() { return m_deviceParams; }

private:
    std::vector<DeviceLink> m_links;
    std::vector<std::pair<std::string, std::string>> m_deviceParams;
    std::vector<std::string> m_ioOrder;
};

}

#endif // DEVICE_H

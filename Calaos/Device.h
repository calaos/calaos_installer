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
// ordinary IOs of the room; ownership is by object, not by id, so an empty or
// duplicated device id cannot hand them to another device.
class Device: public IOBase
{
public:
    explicit Device(Params &p);
    ~Device() override;

    //Used by the tree and the wizard to pick widgets and templates.
    std::string family() { return get_param("type"); }
    std::string impl() { return get_param("impl"); }

    //In file order.
    const std::vector<IOBase *> &ownedIos() const { return m_ownedIos; }
    //Takes io from its previous device, if any. Devices do not nest.
    bool adoptIO(IOBase *io);
    void releaseIO(IOBase *io);

    std::vector<DeviceLink> &links() { return m_links; }
    std::vector<std::pair<std::string, std::string>> &deviceParams() { return m_deviceParams; }

private:
    std::vector<IOBase *> m_ownedIos;
    std::vector<DeviceLink> m_links;
    std::vector<std::pair<std::string, std::string>> m_deviceParams;
};

}

#endif // DEVICE_H

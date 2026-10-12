#include <Device.h>
#include <ListeRoom.h>
#include <algorithm>

using namespace Calaos;

Device::Device(Params &p):
    IOBase(p, p["type"], TSTRING, IOBase::IO_BOTH)
{
}

void Device::set_param(std::string opt, std::string val)
{
    std::vector<IOBase *> owned;
    if (opt == "id")
        owned = ownedIos();

    IOBase::set_param(opt, val);

    for (IOBase *io: owned)
        io->setOwnerDevice(val);
}

std::vector<IOBase *> Device::ownedIos()
{
    std::vector<IOBase *> res;
    const std::string id = get_param("id");

    if (ListeRoom::Instance().get_device(id) != this)
        return res;

    ListeRoom &lr = ListeRoom::Instance();
    for (int r = 0;r < lr.size();r++)
    {
        Room *room = lr.get_room(r);
        auto collect = [&res, &id](IOBase *io)
        {
            if (io->getOwnerDevice() == id && std::find(res.begin(), res.end(), io) == res.end())
                res.push_back(io);
        };
        for (int i = 0;i < room->get_size_in();i++)
            collect(room->get_input(i));
        for (int i = 0;i < room->get_size_out();i++)
            collect(room->get_output(i));
    }

    //Rooms list inputs and outputs apart; the file interleaves them.
    auto rank = [this](IOBase *io)
    {
        auto it = std::find(m_ioOrder.begin(), m_ioOrder.end(), io->get_param("id"));
        return it - m_ioOrder.begin();
    };
    std::stable_sort(res.begin(), res.end(), [&rank](IOBase *a, IOBase *b)
    {
        return rank(a) < rank(b);
    });

    return res;
}

bool Device::adoptIO(IOBase *io)
{
    //Devices do not nest (spec §3.2).
    if (!io || dynamic_cast<Device *>(io))
        return false;

    io->setOwnerDevice(get_param("id"));
    if (std::find(m_ioOrder.begin(), m_ioOrder.end(), io->get_param("id")) == m_ioOrder.end())
        m_ioOrder.push_back(io->get_param("id"));

    return true;
}

#include <Device.h>
#include <algorithm>

using namespace Calaos;

Device::Device(Params &p):
    IOBase(p, p["type"], TSTRING, IOBase::IO_BOTH)
{
}

Device::~Device()
{
    for (IOBase *io: m_ownedIos)
        io->ownerDevice = nullptr;
}

bool Device::adoptIO(IOBase *io)
{
    if (!io || dynamic_cast<Device *>(io))
        return false;

    if (io->ownerDevice == this)
        return true;
    if (io->ownerDevice)
        io->ownerDevice->releaseIO(io);

    io->ownerDevice = this;
    m_ownedIos.push_back(io);

    return true;
}

void Device::releaseIO(IOBase *io)
{
    auto it = std::find(m_ownedIos.begin(), m_ownedIos.end(), io);
    if (it == m_ownedIos.end())
        return;

    m_ownedIos.erase(it);
    io->ownerDevice = nullptr;
}

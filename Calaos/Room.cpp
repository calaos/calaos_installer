/******************************************************************************
**  Copyright (c) 2007-2008, Calaos. All Rights Reserved.
**
**  This file is part of Calaos Home.
**
**  Calaos Home is free software; you can redistribute it and/or modify
**  it under the terms of the GNU General Public License as published by
**  the Free Software Foundation; either version 3 of the License, or
**  (at your option) any later version.
**
**  Calaos Home is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU General Public License for more details.
**
**  You should have received a copy of the GNU General Public License
**  along with Foobar; if not, write to the Free Software
**  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
**
******************************************************************************/
//-----------------------------------------------------------------------------
#include <Room.h>
#include <Device.h>
#include <ListeRule.h>
#include <algorithm>
//-----------------------------------------------------------------------------
using namespace std;
using namespace Calaos;
//-----------------------------------------------------------------------------
Room::~Room()
{
    //Self-contained on purpose: a room destroyed outside ListeRoom (or while
    //ListeRoom is itself being destroyed) must not need to be found there.
    //An IO listed as both input and output is owned once.
    std::vector<IOBase *> owned;
    for (IOBase *io: inputs)
        owned.push_back(io);
    for (IOBase *io: outputs)
        if (std::find(owned.begin(), owned.end(), io) == owned.end())
            owned.push_back(io);

    inputs.clear();
    outputs.clear();

    //Rules point at the IOs without owning them: they go first.
    for (IOBase *io: owned)
        ListeRule::Instance().RemoveRulesUsing(io);

    for (IOBase *io: owned)
        delete io;
}
//-----------------------------------------------------------------------------
std::vector<Device *> Room::get_devices()
{
    std::vector<Device *> res;
    auto collect = [&res](IOBase *io)
    {
        Device *dev = dynamic_cast<Device *>(io);
        if (dev && std::find(res.begin(), res.end(), dev) == res.end())
            res.push_back(dev);
    };
    for (IOBase *io: inputs)
        collect(io);
    for (IOBase *io: outputs)
        collect(io);
    return res;
}
//-----------------------------------------------------------------------------
bool Room::hasInput(IOBase *io) const
{
    return std::find(inputs.begin(), inputs.end(), io) != inputs.end();
}
//-----------------------------------------------------------------------------
bool Room::hasOutput(IOBase *io) const
{
    return std::find(outputs.begin(), outputs.end(), io) != outputs.end();
}
//-----------------------------------------------------------------------------
void Room::detach(IOBase *io)
{
    inputs.erase(std::remove(inputs.begin(), inputs.end(), io), inputs.end());
    outputs.erase(std::remove(outputs.begin(), outputs.end(), io), outputs.end());
}
//-----------------------------------------------------------------------------
void Room::AddInput(IOBase *in)
{
    inputs.push_back(in);
}
//-----------------------------------------------------------------------------
void Room::AddOutput(IOBase *out)
{
    outputs.push_back(out);
}
//-----------------------------------------------------------------------------
void Room::RemoveInput(int pos, bool del)
{
    vector<IOBase *>::iterator iter = inputs.begin();
    for (int i = 0;i < pos;iter++, i++) ;
    if (del) delete inputs[pos];
    inputs.erase(iter);
}
//-----------------------------------------------------------------------------
void Room::RemoveOutput(int pos, bool del)
{
    vector<IOBase *>::iterator iter = outputs.begin();
    for (int i = 0;i < pos;iter++, i++) ;
    if (del) delete outputs[pos];
    outputs.erase(iter);
}
//-----------------------------------------------------------------------------

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
#include <ListeRoom.h>
//-----------------------------------------------------------------------------
using namespace std;
using namespace Calaos;
//-----------------------------------------------------------------------------
Room::~Room()
{
        while (inputs.size() > 0)
                ListeRoom::Instance().deleteIO(inputs[0]);

        while (outputs.size() > 0)
                ListeRoom::Instance().deleteIO(outputs[0]);

        inputs.clear();
        outputs.clear();
}
//-----------------------------------------------------------------------------
void Room::AddInput(Input *in)
{
        inputs.push_back(in);
}
//-----------------------------------------------------------------------------
void Room::AddOutput(Output *out)
{
        outputs.push_back(out);
}
//-----------------------------------------------------------------------------
void Room::RemoveInput(int pos, bool del)
{
        vector<Input *>::iterator iter = inputs.begin();
        for (int i = 0;i < pos;iter++, i++) ;
        if (del) delete inputs[pos];
        inputs.erase(iter);
}
//-----------------------------------------------------------------------------
void Room::RemoveInput(Input *input)
{
        inputs.erase(std::remove( inputs.begin(), inputs.end(), input) , inputs.end());
}
//-----------------------------------------------------------------------------
void Room::RemoveOutput(int pos, bool del)
{
        vector<Output *>::iterator iter = outputs.begin();
        for (int i = 0;i < pos;iter++, i++) ;
        if (del) delete outputs[pos];
        outputs.erase(iter);
}
//-----------------------------------------------------------------------------
void Room::RemoveOutput(Output *output)
{
        outputs.erase(std::remove( outputs.begin(), outputs.end(), output) , outputs.end());
}
//-----------------------------------------------------------------------------

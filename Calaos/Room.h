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
#ifndef S_ROOM_H
#define S_ROOM_H
//-----------------------------------------------------------------------------
#include <Utils.h>
#include <iostream>
#include <IOBase.h>
//-----------------------------------------------------------------------------
using namespace std;
//-----------------------------------------------------------------------------
namespace Calaos
{

class Room
{
        protected:
                std::string name;
                std::string type;
                int hits;

                std::vector<IOBase *> inputs;
                std::vector<IOBase *> outputs;

        public:
                Room(std::string _name, std::string _type, int _hits = 0)
                                : name(_name), type(_type), hits(_hits)
                        { }
                ~Room();

                std::string &get_name() { return name; }
                void set_name(std::string &s) { name = s; }

                std::string &get_type() { return type; }
                void set_type(std::string &s) { type = s; }

                int get_hits() { return hits; }
                void set_hits(int h) { hits = h; }

                void AddInput(IOBase *p);
                void RemoveInput(int i, bool del = true);
                void RemoveInput(IOBase *input);
                void AddOutput(IOBase *p);
                void RemoveOutput(int i, bool del = true);
                void RemoveOutput(IOBase *output);

                IOBase *get_input(int i) { return inputs[i]; }
                IOBase *get_output(int i) { return outputs[i]; }

                int get_size_in() { return inputs.size(); }
                int get_size_out() { return outputs.size(); }
};
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
}
#endif

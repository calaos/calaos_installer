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
#include <ListeRoom.h>
#include <QString>
//-----------------------------------------------------------------------------
using namespace std;
using namespace Calaos;
//-----------------------------------------------------------------------------
ListeRoom &ListeRoom::Instance()
{
    static ListeRoom inst;

    return inst;
}
//-----------------------------------------------------------------------------
ListeRoom::~ListeRoom()
{
    clear();
}
//-----------------------------------------------------------------------------
void ListeRoom::clear()
{
    for (uint i = 0;i < rooms.size();i++)
        delete rooms[i];

    rooms.clear();
}
//-----------------------------------------------------------------------------
void ListeRoom::Add(Room *p)
{
    rooms.push_back(p);
}
//-----------------------------------------------------------------------------
void ListeRoom::Remove(int pos)
{
    vector<Room *>::iterator iter = rooms.begin();
    for (int i = 0;i < pos;iter++, i++) ;
    delete rooms[pos];
    rooms.erase(iter);
}
//-----------------------------------------------------------------------------
void ListeRoom::Remove(Room *room)
{
    delete room;
    rooms.erase( std::remove( rooms.begin(), rooms.end(), room) , rooms.end());
}
//-----------------------------------------------------------------------------
Room *ListeRoom::operator[] (int i) const
{
    return rooms[i];
}
//-----------------------------------------------------------------------------
Room *ListeRoom::get_room(int i)
{
    return rooms[i];
}
//-----------------------------------------------------------------------------
bool ListeRoom::roomExists(Room *room)
{
    for (uint j = 0;j < rooms.size();j++)
    {
        if (rooms[j] == room)
            return true;
    }

    return false;
}
//-----------------------------------------------------------------------------
int ListeRoom::searchIO(IOBase *io)
{
    for (uint j = 0;j < rooms.size();j++)
    {
        for (int m = 0;m < rooms[j]->get_size_in();m++)
        {
            IOBase *in = rooms[j]->get_input(m);
            if (in == dynamic_cast<IOBase *>(io))
            {
                return j;
            }
        }

        for (int m = 0;m < rooms[j]->get_size_out();m++)
        {
            IOBase *out = rooms[j]->get_output(m);
            if (out == dynamic_cast<IOBase *>(io))
            {
                return j;
            }
        }
    }

    return -1;
}
//-----------------------------------------------------------------------------
IOBase *ListeRoom::get_input(std::string i)
{
    for (uint j = 0;j < rooms.size();j++)
    {
        for (int m = 0;m < rooms[j]->get_size_in();m++)
        {
            IOBase *in = rooms[j]->get_input(m);
            if (in->get_param("id") == i || in->get_param("iid") == i)
            {
                return in;
            }
        }
    }

    return NULL;
}
//-----------------------------------------------------------------------------
IOBase *ListeRoom::get_output(std::string i)
{
    for (uint j = 0;j < rooms.size();j++)
    {
        for (int m = 0;m < rooms[j]->get_size_out();m++)
        {
            IOBase *out = rooms[j]->get_output(m);
            if (out->get_param("id") == i || out->get_param("oid") == i)
            {
                return out;
            }
        }
    }

    return NULL;
}
//-----------------------------------------------------------------------------
IOBase *ListeRoom::get_input(int i)
{
    int cpt = 0;

    for (uint j = 0;j < rooms.size();j++)
    {
        for (int m = 0;m < rooms[j]->get_size_in();m++)
        {
            IOBase *in = rooms[j]->get_input(m);
            if (cpt == i)
            {
                return in;
            }

            cpt++;
        }
    }

    return NULL;
}
//-----------------------------------------------------------------------------
IOBase *ListeRoom::get_output(int i)
{
    int cpt = 0;

    for (uint j = 0;j < rooms.size();j++)
    {
        for (int m = 0;m < rooms[j]->get_size_out();m++)
        {
            IOBase *out = rooms[j]->get_output(m);
            if (cpt == i)
            {
                return out;
            }

            cpt++;
        }
    }

    return NULL;
}
//-----------------------------------------------------------------------------
bool ListeRoom::delete_input(IOBase *input, bool del)
{
    bool done = false;
    for (uint j = 0;!done && j < rooms.size();j++)
    {
        for (int m = 0;!done && m < get_room(j)->get_size_in();m++)
        {
            IOBase *in = get_room(j)->get_input(m);
            if (in == input)
            {
                get_room(j)->RemoveInput(m, del);
                done = true;
            }
        }
    }

    return done;
}
//-----------------------------------------------------------------------------
bool ListeRoom::delete_output(IOBase *output, bool del)
{
    bool done = false;
    for (uint j = 0;!done && j < rooms.size();j++)
    {
        for (int m = 0;!done && m < get_room(j)->get_size_out();m++)
        {
            IOBase *out = get_room(j)->get_output(m);
            if (out == output)
            {
                get_room(j)->RemoveOutput(m, del);
                done = true;
            }
        }
    }

    return done;
}
//-----------------------------------------------------------------------------
int ListeRoom::get_nb_input()
{
    int cpt = 0;

    for (uint j = 0;j < rooms.size();j++)
    {
        for (int m = 0;m < rooms[j]->get_size_in();m++)
        {
            cpt++;
        }
    }

    return cpt;
}
//-----------------------------------------------------------------------------
int ListeRoom::get_nb_output()
{
    int cpt = 0;

    for (uint j = 0;j < rooms.size();j++)
    {
        for (int m = 0;m < rooms[j]->get_size_out();m++)
        {
            cpt++;
        }
    }

    return cpt;
}
//-----------------------------------------------------------------------------
IOBase *ListeRoom::get_chauffage_var(std::string chauff_id, ChauffType type)
{
    for (uint j = 0;j < rooms.size();j++)
    {
        for (int m = 0;m < rooms[j]->get_size_in();m++)
        {
            IOBase *in = rooms[j]->get_input(m);
            if (in->get_param("chauffage_id") == chauff_id)
            {
                switch (type)
                {
                case PLAGE_HORAIRE: if (in->get_gui_type() == "time_range") return in; break;
                case CONSIGNE: if (in->get_gui_type() == "var_int") return in; break;
                case ACTIVE: if (in->get_gui_type() == "var_bool") return in; break;
                }
            }
        }
    }

    return NULL;
}
//-----------------------------------------------------------------------------
Room *ListeRoom::searchRoomByName(string name, string type)
{
    Room *r = NULL;
    vector<Room *>::iterator itRoom;

    for(itRoom = rooms.begin(); itRoom != rooms.end() && !r; itRoom++)
        if( (*itRoom)->get_name() == name && (*itRoom)->get_type() == type)
            r = *itRoom;

    return r;
}

Room *ListeRoom::searchRoomByInput(IOBase *input)
{
    for (uint j = 0;j < rooms.size();j++)
    {
        for (int m = 0;m < rooms[j]->get_size_in();m++)
        {
            IOBase *in = rooms[j]->get_input(m);
            if (in == input)
            {
                return rooms[j];
            }
        }
    }

    return NULL;
}

Room *ListeRoom::searchRoomByOutput(IOBase *output)
{
    for (uint j = 0;j < rooms.size();j++)
    {
        for (int m = 0;m < rooms[j]->get_size_out();m++)
        {
            IOBase *out = rooms[j]->get_output(m);
            if (out == output)
            {
                return rooms[j];
            }
        }
    }

    return NULL;
}

string ListeRoom::getRoomType(int type)
{
    switch (type)
    {
    case ROOM_SALON: return "salon";
    case ROOM_CUISINE: return "cuisine";
    case ROOM_CHAMBRE: return "chambre";
    case ROOM_BUREAU: return "bureau";
    case ROOM_SAM: return "sam";
    case ROOM_CAVE: return "cave";
    case ROOM_GARAGE: return "garage";
    case ROOM_EXT: return "exterieur";
    case ROOM_WC: return "sdb";
    case ROOM_HALL: return "hall";
    default:
    case ROOM_MISC: return "misc";
    }
}
//-----------------------------------------------------------------------------
bool ListeRoom::deleteIOInput(IOBase *input)
{
    //first delete all rules using "input"
    ListeRule::Instance().RemoveRuleInput(input);

    if (input->is_inout())
        ListeRoom::Instance().delete_output(input, false);

    return ListeRoom::Instance().delete_input(input);
}


bool ListeRoom::deleteIOOutput(IOBase *output)
{
    //first delete all rules using "output"
    ListeRule::Instance().RemoveRuleOutput(output);

    if (output->is_inout())
        ListeRoom::Instance().delete_input(output, false);

    return ListeRoom::Instance().delete_output(output);
}

IOBase* ListeRoom::createInput(Params param, Room *room)
{
    IOBase *input = NULL;

    if (!param.Exists("name")) param.Add("name", "Input");
    if (!param.Exists("type")) param.Add("type", "WIDigitalBP");
    if (param["type"].substr(0, 2) == "WI")
    {
        if (!param.Exists("var")) param.Add("var", "0");
        if (!param.Exists("host")) param.Add("host", WAGO_HOST);
        if (!param.Exists("port")) param.Add("port", "502");
    }

    if (param["type"] == "InternalBool" || param["type"] == "InternalInt" || param["type"] == "InternalString")
    {
        if (!param.Exists("id")) param.Add("id", ListeRoom::get_new_id("intern_"));
    }
    else
    {
        if (!param.Exists("id")) param.Add("id", ListeRoom::get_new_id("input_"));
    }

    if (param["type"] == "InputTime" || param["type"] == "InputTimeDate" ||
        param["type"] == "InputTimer")
    {
        if (!param.Exists("hour")) param.Add("hour", "0");
        if (!param.Exists("min")) param.Add("min", "0");
        if (!param.Exists("sec")) param.Add("sec", "0");
    }
    if (param["type"] == "InputTimeDate")
    {
        if (!param.Exists("year")) param.Add("year", "0");
        if (!param.Exists("month")) param.Add("month", "0");
        if (!param.Exists("day")) param.Add("day", "0");
        param.Add("type", "InputTime");
    }

    if (param["type"] == "InputTimer")
    {
        if (!param.Exists("msec")) param.Add("msec", "0");
        std::string type = param["type"];
        input = IOFactory::Instance().CreateIO(type, param);
        if (input) room->AddInput(input);

        //also add the it as an output
        if (input) room->AddOutput(input);
    }
    else if (param["type"] == "scenario")
    {
        std::string type = param["type"];
        input = IOFactory::Instance().CreateIO(type, param);
        if (input) room->AddInput(input);

        //also add it as an output
        if (input) room->AddOutput(input);
    }
    else if (param["type"] == "InternalBool" || param["type"] == "InternalInt" || param["type"] == "InternalString")
    {
        if (!param.Exists("name")) param.Add("name", "Value");

        std::string type = param["type"];
        input = IOFactory::Instance().CreateIO(type, param);
        if (input) room->AddInput(input);

        //also add it as an output
        if (input) room->AddOutput(input);
    }
    else
    {
        std::string type = param["type"];
        input = IOFactory::Instance().CreateIO(type, param, IOBase::IO_INPUT);
        if (input) room->AddInput(input);
    }

    return input;
}

IOBase* ListeRoom::createOutput(Params param, Room *room)
{
    IOBase *output = NULL;

    if (!param.Exists("name")) param.Add("name", "Output");
    if (!param.Exists("type")) param.Add("type", "WODigital");

    // HOST and port has only to be set in case of wago hw type
    if (param["type"].substr(0, 2) == "WO")
    {

        if (!param.Exists("host")) param.Add("host", WAGO_HOST);
        if (!param.Exists("port")) param.Add("port", "502");
        if (param["type"] == "WOVolet")
        {
            if (!param.Exists("var_up")) param.Add("var_up", "0");
            if (!param.Exists("var_down")) param.Add("var_down", "0");
            if (!param.Exists("time")) param.Add("time", "30");
        }
        if (param["type"] == "WOVoletSmart")
        {
            if (!param.Exists("var_up")) param.Add("var_up", "0");
            if (!param.Exists("var_down")) param.Add("var_down", "0");
            if (!param.Exists("time_up")) param.Add("time_up", "30");
            if (!param.Exists("time_down")) param.Add("time_down", "28");

            //Set var_save automatically here.
            if (!param.Exists("var_save")) param.Add("var_save", ListeRoom::get_new_varsave());
        }
        else if (param["type"] == "WONeon")
        {
            if (!param.Exists("var")) param.Add("var", "0");
            if (!param.Exists("var_relay")) param.Add("var_relay", "0");
        }
    }

    if (!param.Exists("id")) param.Add("id", ListeRoom::get_new_id("output_"));

    std::string type = param["type"];
    output = IOFactory::Instance().CreateIO(type, param, IOBase::IO_OUTPUT);
    if (output) room->AddOutput(output);

    return output;
}

IOBase* ListeRoom::createCamera(Params param, Room *room)
{
    if (!param.Exists("name")) param.Add("name", "Camera");
    if (!param.Exists("type")) param.Add("type", "axis");
    if (!param.Exists("host")) param.Add("host", "192.168.1.10");
    if (!param.Exists("port")) param.Add("port", "80");
    if (!param.Exists("oid")) param.Add("oid", ListeRoom::get_new_id("output_"));
    if (!param.Exists("iid")) param.Add("iid", ListeRoom::get_new_id("input_"));
    if (!param.Exists("id")) param.Add("id", param["iid"] + "_" + param["oid"]);
    if (!param.Exists("model")) param.Add("model", "");

    IOBase *output = IOFactory::Instance().CreateIO("Camera", param, IOBase::IO_BOTH);
    if (output) room->AddOutput(output);
    if (output) room->AddInput(output);

    return output;
}

IOBase *ListeRoom::createAudio(Params param, Room *room)
{
    if (!param.Exists("name")) param.Add("name", "Lecteur Audio");
    if (!param.Exists("type")) param.Add("type", "slim");
    if (!param.Exists("host")) param.Add("host", "192.168.1.10");
    if (!param.Exists("port")) param.Add("port", "9090");
    if (!param.Exists("oid")) param.Add("oid", ListeRoom::get_new_id("output_"));
    if (!param.Exists("iid")) param.Add("iid", ListeRoom::get_new_id("input_"));

    IOBase *output = IOFactory::Instance().CreateIO("Audio", param, IOBase::IO_BOTH);
    if (output) room->AddOutput(output);
    if (output) room->AddInput(output);

    return output;
}

std::string ListeRoom::get_new_id(std::string prefix)
{
    int cpt = 0;
    bool found = false;
    while (!found)
    {
        IOBase *in = ListeRoom::Instance().get_input(prefix + Utils::to_string(cpt));
        IOBase *out = ListeRoom::Instance().get_output(prefix + Utils::to_string(cpt));

        if (!in && !out)
            found = true;
        else
            cpt++;
    }

    std::string ret = prefix + Utils::to_string(cpt);
    return ret;
}

std::string ListeRoom::get_new_varsave()
{
    map<string, IOBase *> volets;

    for (int j = 0;j < ListeRoom::Instance().size();j++)
    {
        Room *room = ListeRoom::Instance().get_room(j);
        for (int m = 0;m < room->get_size_out();m++)
        {
            IOBase *o = room->get_output(m);
            if (o->get_param("type") == "WOVoletSmart" &&
                o->get_params().Exists("var_save"))
            {
                volets[o->get_param("var_save")] = o;
            }
        }
    }

    int cpt = 0;
    bool found = false;
    while (!found)
    {
        string s = to_string(cpt);

        if (volets.find(s) == volets.end())
            found = true;
        else
            cpt++;
    }

    return to_string(cpt);
}

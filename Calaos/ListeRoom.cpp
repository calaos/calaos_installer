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
                        Input *in = rooms[j]->get_input(m);
                        if (in == io)
                        {
                                return j;
                        }
                }

                for (int m = 0;m < rooms[j]->get_size_out();m++)
                {
                        Output *out = rooms[j]->get_output(m);
                        if (out == io)
                        {
                                return j;
                        }
                }
        }

        return -1;
}
//-----------------------------------------------------------------------------
Input *ListeRoom::get_input(std::string i)
{
        for (uint j = 0;j < rooms.size();j++)
        {
                for (int m = 0;m < rooms[j]->get_size_in();m++)
                {
                        Input *in = rooms[j]->get_input(m);
                        if (in->get_param("id") == i || in->get_param("iid") == i)
                        {
                                return in;
                        }
                }
        }

        return NULL;
}
//-----------------------------------------------------------------------------
Output *ListeRoom::get_output(std::string i)
{
        for (uint j = 0;j < rooms.size();j++)
        {
                for (int m = 0;m < rooms[j]->get_size_out();m++)
                {
                        Output *out = rooms[j]->get_output(m);
                        if (out->get_param("id") == i || out->get_param("oid") == i)
                        {
                                return out;
                        }
                }
        }

        return NULL;
}
//-----------------------------------------------------------------------------
Input *ListeRoom::get_input(int i)
{
        int cpt = 0;

        for (uint j = 0;j < rooms.size();j++)
        {
                for (int m = 0;m < rooms[j]->get_size_in();m++)
                {
                        Input *in = rooms[j]->get_input(m);
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
Output *ListeRoom::get_output(int i)
{
        int cpt = 0;

        for (uint j = 0;j < rooms.size();j++)
        {
                for (int m = 0;m < rooms[j]->get_size_out();m++)
                {
                        Output *out = rooms[j]->get_output(m);
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
bool ListeRoom::delete_input(Input *input, bool del)
{
        bool done = false;
        for (uint j = 0;!done && j < rooms.size();j++)
        {
                for (int m = 0;!done && m < get_room(j)->get_size_in();m++)
                {
                        Input *in = get_room(j)->get_input(m);
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
bool ListeRoom::delete_output(Output *output, bool del)
{
        bool done = false;
        for (uint j = 0;!done && j < rooms.size();j++)
        {
                for (int m = 0;!done && m < get_room(j)->get_size_out();m++)
                {
                        Output *out = get_room(j)->get_output(m);
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
Input *ListeRoom::get_chauffage_var(std::string &chauff_id, ChauffType type)
{
        for (uint j = 0;j < rooms.size();j++)
        {
                for (int m = 0;m < rooms[j]->get_size_in();m++)
                {
                        Input *in = rooms[j]->get_input(m);
                        if (in->get_param("chauffage_id") == chauff_id)
                        {
                                switch (type)
                                {
                                  case PLAGE_HORAIRE: if (in->get_param("type") == "InPlageHoraire") return in; break;
                                  case CONSIGNE: if (in->get_param("type") == "InternalIntInput") return in; break;
                                  case ACTIVE: if (in->get_param("type") == "InternalBoolInput") return in; break;
                                }
                        }
                }
        }

        return NULL;
}
//-----------------------------------------------------------------------------
Room * ListeRoom::searchRoomByName(string name, string type)
{
        Room *r = NULL;
        vector<Room *>::iterator itRoom;

        for(itRoom = rooms.begin(); itRoom != rooms.end() && !r; itRoom++)
                if( (*itRoom)->get_name() == name && (*itRoom)->get_type() == type)
                        r = *itRoom;

        return r;
}
//-----------------------------------------------------------------------------
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
bool ListeRoom::deleteIO(Input *input)
{
        //first delete all rules using "input"
        ListeRule::Instance().RemoveRule(input);

        bool done = false;
        if (IOBase::isAudioType(input->get_param("type")) ||
            IOBase::isCameraType(input->get_param("type")))
        {
                Camera *cam = dynamic_cast<Camera *>(input);
                if (cam)
                {
                        Output *o = dynamic_cast<Output *> (cam);
                        ListeRule::Instance().RemoveRule(o);
                        if (o)
                                ListeRoom::Instance().delete_output(o, false);
                }

                Audio *audio = dynamic_cast<Audio *>(input);
                if (audio)
                {
                        Output *o = dynamic_cast<Output *> (audio);
                        ListeRule::Instance().RemoveRule(o);
                        if (o)
                                ListeRoom::Instance().delete_output(o, false);
                }
        }

        if (input->get_param("type") == "InputTimer")
        {
                //also delete the output
                InputTimer *tm = dynamic_cast<InputTimer *> (input);
                Output *o = dynamic_cast<Output *> (tm);
                ListeRule::Instance().RemoveRule(o);
                if (o) ListeRoom::Instance().delete_output(o, false);
        }

        if (input->get_param("type") == "scenario" || input->get_param("type") == "Scenario")
        {
                //also delete the output
                Scenario *sc = dynamic_cast<Scenario *> (input);
                if (sc)
                {
                        Output *o = dynamic_cast<Output *> (sc);
                        ListeRule::Instance().RemoveRule(o);
                        if (o)
                                ListeRoom::Instance().delete_output(o, false);
                }
        }

        if (input->get_param("type") == "InternalBool" ||
            input->get_param("type") == "InternalInt" ||
            input->get_param("type") == "InternalString")
        {
                //also delete the output
                Internal *internal = dynamic_cast<Internal *> (input);

                if (internal)
                {
                        Output *o = dynamic_cast<Output *> (internal);
                        ListeRule::Instance().RemoveRule(o);
                        if (o)
                                ListeRoom::Instance().delete_output(o, false);
                }
        }

        done = ListeRoom::Instance().delete_input(input);

        return done;
}


bool ListeRoom::deleteIO(Output *output)
{
        //first delete all rules using "output"
        ListeRule::Instance().RemoveRule(output);

        bool done = false;
        if (IOBase::isAudioType(output->get_param("type")) ||
            IOBase::isCameraType(output->get_param("type")))
        {
                Camera *cam = dynamic_cast<Camera *>(output);
                if (cam)
                {
                        Input *o = dynamic_cast<Input *> (cam);
                        ListeRule::Instance().RemoveRule(o);
                        if (o)
                                ListeRoom::Instance().delete_input(o, false);
                }

                Audio *audio = dynamic_cast<Audio *>(output);
                if (audio)
                {
                        Input *o = dynamic_cast<Input *> (audio);
                        ListeRule::Instance().RemoveRule(o);
                        if (o)
                                ListeRoom::Instance().delete_input(o, false);
                }
        }


        if (output->get_param("type") != "OutTouchscreen")
        {
                if (output->get_param("type") == "InputTimer")
                {
                        //also delete the input
                        InputTimer *tm = dynamic_cast<InputTimer *> (output);
                        Input *in = dynamic_cast<Input *> (tm);
                        ListeRule::Instance().RemoveRule(in);
                        if (in) ListeRoom::Instance().delete_input(in, false);
                }

                if (output->get_param("type") == "scenario"
                                || output->get_param("type") == "Scenario")
                {
                        //also delete the output
                        Scenario *sc = dynamic_cast<Scenario *> (output);
                        if (sc)
                        {
                                Input *o = dynamic_cast<Input *> (sc);
                                ListeRule::Instance().RemoveRule(o);
                                if (o) ListeRoom::Instance().delete_input(o, false);
                        }
                }

                if (output->get_param("type") == "InternalBool" ||
                    output->get_param("type") == "InternalInt" ||
                    output->get_param("type") == "InternalString")
                {
                        //also delete the output
                        Internal *internal = dynamic_cast<Internal *> (output);
                        if (internal)
                        {
                                Input *o = dynamic_cast<Input *> (internal);
                                if (o)
                                {
                                        ListeRule::Instance().RemoveRule(o);
                                        ListeRoom::Instance().delete_input(o, false);
                                }
                        }
                }

                done = ListeRoom::Instance().delete_output(output);
        }

        return done;
}

Input* ListeRoom::createInput(Params param, Room *room)
{
        Input *input = NULL;

        if (!param.Exists("name")) param.Add("name", "Input");
        if (!param.Exists("type")) param.Add("type", "WIDigitalBP");
        if (param["type"] != "scenario" && param["type"] != "InputTimeDate" &&
            param["type"] != "InputTime" && param["type"] != "X10Output" &&
            param["type"] != "InternalInt" && param["type"] != "InternalBool" &&
            param["type"] != "InternalString")
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
                input = IOFactory::CreateInput(type, param);
                if (input) room->AddInput(input);

                //also add the it as an output
                InputTimer *o = dynamic_cast<InputTimer *> (input);
                if (o) room->AddOutput(o);
        }
        else if (param["type"] == "scenario")
        {
                std::string type = param["type"];
                input = IOFactory::CreateInput(type, param);
                if (input) room->AddInput(input);

                //also add it as an output
                Scenario *o = dynamic_cast<Scenario *> (input);
                if (o) room->AddOutput(o);
        }
        else if (param["type"] == "InternalBool" || param["type"] == "InternalInt" || param["type"] == "InternalString")
        {
                if (!param.Exists("name")) param.Add("name", "Value");

                std::string type = param["type"];
                input = IOFactory::CreateInput(type, param);
                if (input) room->AddInput(input);

                //also add it as an output
                Internal *o = dynamic_cast<Internal *> (input);
                if (o) room->AddOutput(o);
        }
        else
        {
                std::string type = param["type"];
                input = IOFactory::CreateInput(type, param);
                if (input) room->AddInput(input);
        }

        return input;
}

Output* ListeRoom::createOutput(Params param, Room *room)
{
        Output *output = NULL;

        if (!param.Exists("name")) param.Add("name", "Output");
        if (!param.Exists("type")) param.Add("type", "WODigital");
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
        }
        else if (param["type"] == "WONeon")
        {
                if (!param.Exists("var")) param.Add("var", "0");
                if (!param.Exists("var_relay")) param.Add("var_relay", "0");
        }

        if (!param.Exists("id")) param.Add("id", ListeRoom::get_new_id("output_"));

        std::string type = param["type"];
        output = IOFactory::CreateOutput(type, param);
        if (output) room->AddOutput(output);

        return output;
}

Camera* ListeRoom::createCamera(Params param, Room *room)
{
        Camera *camera = NULL;

        if (!param.Exists("name")) param.Add("name", "Camera");
        if (!param.Exists("type")) param.Add("type", "axis");
        if (!param.Exists("host")) param.Add("host", "192.168.1.10");
        if (!param.Exists("port")) param.Add("port", "80");
        if (!param.Exists("oid")) param.Add("oid", ListeRoom::get_new_id("output_"));
        if (!param.Exists("iid")) param.Add("iid", ListeRoom::get_new_id("input_"));
        if (!param.Exists("id")) param.Add("id", param["iid"] + "_" + param["oid"]);
        if (!param.Exists("model")) param.Add("model", "");

        Output *output = IOFactory::CreateOutput("Camera", param);
        if (output) room->AddOutput(output);

        camera = dynamic_cast<Camera *> (output);
        Input *input = dynamic_cast<Input *> (output);
        if (input) room->AddInput(input);

        return camera;
}

Audio* ListeRoom::createAudio(Params param, Room *room)
{
        Audio *audio = NULL;

        if (!param.Exists("name")) param.Add("name", "Lecteur Audio");
        if (!param.Exists("type")) param.Add("type", "slim");
        if (!param.Exists("host")) param.Add("host", "192.168.1.10");
        if (!param.Exists("port")) param.Add("port", "9090");
        if (!param.Exists("oid")) param.Add("oid", ListeRoom::get_new_id("output_"));
        if (!param.Exists("iid")) param.Add("iid", ListeRoom::get_new_id("input_"));

        Output *output = IOFactory::CreateOutput("Audio", param);
        if (output) room->AddOutput(output);

        audio = dynamic_cast<Audio *> (output);
        Input *input = dynamic_cast<Input *> (output);
        if (input) room->AddInput(input);

        return audio;
}

std::string ListeRoom::get_new_id(std::string prefix)
{
        int cpt = 0;
        bool found = false;
        while (!found)
        {
                Input *in = ListeRoom::Instance().get_input(prefix + Utils::to_string(cpt));
                Output *out = ListeRoom::Instance().get_output(prefix + Utils::to_string(cpt));

                if (!in && !out)
                        found = true;
                else
                        cpt++;
        }

        std::string ret = prefix + Utils::to_string(cpt);
        return ret;
}

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
#ifndef S_ListeRoom_H
#define S_ListeRoom_H
//-----------------------------------------------------------------------------

#include <Utils.h>
#include <iostream>
#include <Room.h>
#include <ListeRule.h>
#include <IOBase.h>
#include <IOFactory.h>

//-----------------------------------------------------------------------------
using namespace std;
//-----------------------------------------------------------------------------
namespace Calaos
{
//-----------------------------------------------------------------------------
enum { ROOM_SALON = 0, ROOM_CUISINE, ROOM_CHAMBRE, ROOM_BUREAU, ROOM_SAM, ROOM_CAVE, ROOM_MISC,
       ROOM_GARAGE, ROOM_EXT, ROOM_WC, ROOM_HALL };
//-----------------------------------------------------------------------------
typedef enum { PLAGE_HORAIRE, CONSIGNE, ACTIVE } ChauffType;
//-----------------------------------------------------------------------------
class ListeRoom
{
        protected:
                std::vector<Room *> rooms;

                ListeRoom()
                        { }

        public:
                //singleton
                static ListeRoom &Instance();

                ~ListeRoom();

                void Add(Room *p);
                void Remove(int i);
                void Remove(Room *room);
                Room *get_room(int i);
                Room *operator[] (int i) const;

                bool roomExists(Room *room);

                Input *get_input(std::string i);
                Output *get_output(std::string i);

                //return -1 if not found, else return room id
                int searchIO(IOBase *io);

                Input *get_input(int i);
                Output *get_output(int i);
                bool delete_input(Input *in, bool del = true);
                bool delete_output(Output *out, bool del = true);

                int get_nb_input();
                int get_nb_output();

                Input *get_chauffage_var(std::string chauff_id, ChauffType type);

                int size() { return rooms.size(); }

                Room *searchRoomByName(string name, string type);

                Room *searchRoomByInput(Input *);
                Room *searchRoomByOutput(Output *);

                bool deleteIO(Input *input);
                bool deleteIO(Output *output);

                Input* createInput(Params param, Room *room);
                Output* createOutput(Params param, Room *room);
                Camera* createCamera(Params param, Room *room);
                Audio* createAudio(Params param, Room *room);

                static string get_new_id(string prefix);
                static string get_new_varsave();

                static string getRoomType(int type);

                void clear();
};

}
#endif

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
#include <Device.h>

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

    QStringList unknownXml;

    //format attribute of <home> in the file loaded last, empty when absent
    QString loadedFormat;

    ListeRoom();

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

    IOBase *get_input(std::string i);
    IOBase *get_output(std::string i);

    //return -1 if not found, else return room id
    int searchIO(IOBase *io);

    IOBase *get_input(int i);
    IOBase *get_output(int i);
    bool delete_input(IOBase *in, bool del = true);
    bool delete_output(IOBase *out, bool del = true);

    int get_nb_input();
    int get_nb_output();

    IOBase *get_chauffage_var(std::string chauff_id, ChauffType type);

    int size() { return rooms.size(); }

    Room *searchRoomByName(string name, string type);

    Room *searchRoomByInput(IOBase *);
    Room *searchRoomByOutput(IOBase *);

    //Both delete a device's owned IOs along with it.
    bool deleteIOInput(IOBase *input);
    bool deleteIOOutput(IOBase *output);
    //What deleteIOInput/deleteIOOutput(io) destroys, so that views can drop
    //their pointers to it.
    static std::vector<IOBase *> iosDeletedWith(IOBase *io);

    IOBase *createInput(Params param, Room *room);
    IOBase *createOutput(Params param, Room *room);
    IOBase *createCamera(Params param, Room *room);
    IOBase *createAudio(Params param, Room *room);
    IOBase *createAVR(Params param, Room *room);
    Device *createDevice(Params param, Room *room);

    //First device holding that id, nullptr for an empty id
    Device *get_device(const std::string &id);
    bool hasDevices();

    //Newest io.xml format this installer reads and writes in full.
    static const int IO_FORMAT_SUPPORTED = 2;

    //A file in a newer format may hold data this model cannot represent:
    //it is opened read-only rather than silently stripped on save.
    static bool isFormatSupported(const QString &format);
    void setLoadedFormat(const QString &format) { loadedFormat = format; }
    QString getLoadedFormat() const { return loadedFormat; }
    bool isReadOnly() const { return !isFormatSupported(loadedFormat); }

    static string get_new_id(string prefix);

    /* Ids that rules.xml references but io.xml does not define. They are kept
     * alive by placeholder IOs (see ProjectManager) which live outside any
     * Room, so the room scan of get_new_id() cannot see them. Reserving them
     * is what stops a brand new IO from being handed a still-referenced id and
     * silently inheriting the rules of the IO that disappeared.
     * Rebuilt from scratch at every rules.xml load, so the reservation lasts
     * exactly as long as the dangling reference itself. */
    static void reservePendingIOId(const string &id);
    static void clearPendingIOIds();
    static bool isPendingIOId(const string &id);
    static string get_new_varsave();

    static string getRoomType(int type);

    //Children of <home> the reader does not know, written back as they were read
    const QStringList &getUnknownXml() const { return unknownXml; }
    void addUnknownXml(const QString &xml) { unknownXml.append(xml); }

    void clear();
};

}
#endif

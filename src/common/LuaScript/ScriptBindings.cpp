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
#include <ScriptBindings.h>
#include <ListeRoom.h>
#include <ScriptManager.h>

#ifdef CALAOS_INSTALLER
#include <QTime>
#include <DialogScriptEditor.h>
#endif

using namespace Calaos;

//Debug
void Calaos::Lua_stackDump(lua_State *L)
{
    int i;
    int top = lua_gettop(L);
    for (i = 1; i <= top; i++)
    {  /* repeat for each level */
        int t = lua_type(L, i);
        switch (t)
        {
        case LUA_TSTRING:  /* strings */
            printf("`%s'", lua_tostring(L, i));
            break;

        case LUA_TBOOLEAN:  /* booleans */
            printf(lua_toboolean(L, i) ? "true" : "false");
            break;

        case LUA_TNUMBER:  /* numbers */
            printf("%g", lua_tonumber(L, i));
            break;

        default:  /* other values */
            printf("%s", lua_typename(L, t));
            break;
        }
        printf("  ");  /* put a separator */
    }
    printf("\n");  /* end the listing */
}

/* This print version will replace the one in base lib. It prints out everything
 * through the shared logging facility instead of stdout
 */
int Calaos::Lua_print(lua_State *L)
{
    int n = lua_gettop(L);
    string msg;

    for (int i = 1;i <= n;i++)
    {
        if (i > 1)
            msg += " ";
        if (lua_isstring(L, i))
            msg += lua_tostring(L, i);
        else if (lua_isnil(L, i))
            msg += "nil";
        else if (lua_isboolean(L, i))
            msg += lua_toboolean(L, i)? "true" : "false";
        else
            msg += to_string(luaL_typename(L, i)) + to_string(":") + to_string(lua_topointer(L, i));
    }

    LuaPrinter::Instance().Print(QString::fromUtf8(msg.c_str()));

    return 0;
}

void Calaos::Lua_DebugHook(lua_State *L, lua_Debug *ar)
{
    double time;

    QTime t = QTime::currentTime();
    time = (double)t.second() + (((double) t.msec()) / 1000);

    if (time - ScriptManager::start_time > SCRIPT_MAX_EXEC_TIME)
    {
        string err = "Aborting script, takes too much time to execute (";
        err += to_string(time - ScriptManager::start_time) + " sec.)";

        ScriptManager::abortTimeout = true;

        lua_pushstring(L, err.c_str());
        lua_error(L);
    }
}

Lunar<Lua_Calaos>::RegType Lua_Calaos::methods[] =
{
    { "getOutputValue", &Lua_Calaos::getIOValue },
    { "setOutputValue", &Lua_Calaos::setIOValue },
    { "getInputValue", &Lua_Calaos::getIOValue },
    { "getIOValue", &Lua_Calaos::getIOValue },
    { "setIOValue", &Lua_Calaos::setIOValue },
    { "getIOParam", &Lua_Calaos::getIOParam },
    { "setIOParam", &Lua_Calaos::setIOParam },
    { "waitForIO", &Lua_Calaos::waitForIO },
    { "requestUrl", &Lua_Calaos::requestUrl },
    { 0, 0 }
};

const char Lua_Calaos::className[] = "Calaos";

Lua_Calaos::Lua_Calaos()
{
}

Lua_Calaos::Lua_Calaos(lua_State *L)
{
    string err = "Calaos(): Don't create a new object, juste use the existing one \"calaos:...\"";
    lua_pushstring(L, err.c_str());
    lua_error(L);
}

Lua_Calaos::~Lua_Calaos()
{
}

int Lua_Calaos::getIOValue(lua_State *L)
{
    int nb = lua_gettop(L);

    if (nb == 1 && lua_isstring(L, 1))
    {
        string id = lua_tostring(L, 1);
        IOBase *io = ListeRoom::Instance().get_input(id);
        if (!io)
            io = ListeRoom::Instance().get_output(id);
        if (!io)
        {
            string err = "getIOValue(): invalid io ID (" + id + "). Requires an existing Input or Output ID.";
            lua_pushstring(L, err.c_str());
            lua_error(L);
        }
        else
        {
            switch (io->get_type())
            {
            case TBOOL:
                lua_pushboolean(L, true);
                break;
            case TINT:
                lua_pushnumber(L, 42);
                break;
            case TSTRING:
                lua_pushstring(L, "18.42");
                break;
            case TUNKNOWN :
                lua_pushstring(L, "unknown");
                break;
            default:
                break;
            }
        }
    }
    else
    {
        string err = "getIOValue(): invalid argument. Requires an Input ID.";
        lua_pushstring(L, err.c_str());
        lua_error(L);
    }

    return 1;
}

int Lua_Calaos::setIOValue(lua_State *L)
{
    int nb = lua_gettop(L);

    if (nb == 2 && lua_isstring(L, 1))
    {
        //Should be correct if here
    }
    else
    {
        string err = "getOutputValue(): invalid argument. Requires an Output ID.";
        lua_pushstring(L, err.c_str());
        lua_error(L);
    }

    return 0;
}

int Lua_Calaos::getIOParam(lua_State *L)
{
    int nb = lua_gettop(L);

    if (nb == 2 && lua_isstring(L, 1) && lua_isstring(L, 2))
    {
        lua_pushstring(L, "test");
    }
    else
    {
        string err = "getIOParam(): invalid argument. Requires an IO id.";
        lua_pushstring(L, err.c_str());
        lua_error(L);
    }

    return 1;
}

int Lua_Calaos::setIOParam(lua_State *L)
{
    int nb = lua_gettop(L);

    if (nb == 3 && lua_isstring(L, 1) && lua_isstring(L, 2))
    {
        //Should be correct if here
    }
    else
    {
        string err = "setIOParam(): invalid argument. Requires an IO id.";
        lua_pushstring(L, err.c_str());
        lua_error(L);
    }

    return 1;
}

int Lua_Calaos::waitForIO(lua_State *L)
{
    int nb = lua_gettop(L);

    if (nb == 1 && lua_isstring(L, 1))
    {
        //Should be correct if here
    }
    else
    {
        string err = "waitForIO(): invalid argument. Requires an IO id.";
        lua_pushstring(L, err.c_str());
        lua_error(L);
    }

    return 1;
}

int Lua_Calaos::requestUrl(lua_State *L)
{
    int nb = lua_gettop(L);

    if (nb == 1 && lua_isstring(L, 1))
    {
        string url = lua_tostring(L, 1);
    }
    else if (nb == 2 && lua_isstring(L, 1) && lua_isstring(L, 2))
    {
        string url = lua_tostring(L, 1);
        string post_data = lua_tostring(L, 2);
    }
    else
    {
        string err = "requestUrl(): invalid argument. Requires a string (URL to call).";
        lua_pushstring(L, err.c_str());
        lua_error(L);
    }

    return 0;
}

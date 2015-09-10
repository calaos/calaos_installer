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
#include <ScriptManager.h>
#include <setjmp.h>

#ifdef CALAOS_INSTALLER
#include <QTime>
#endif

using namespace Calaos;

double ScriptManager::start_time = 0.0;
bool ScriptManager::abortTimeout = false;
static jmp_buf panic_jmp;

ScriptManager::ScriptManager()
{
}

ScriptManager::~ScriptManager()
{
}

bool ScriptManager::ExecuteScript(string script)
{
    bool ret = true;
    errorScript = true;

    const luaL_Reg *l;
    lua_State *L = lua_open();

    luaL_openlibs(L);

    /* disable some calls from base lib */
    lua_pushnil(L);
    lua_setglobal(L, "print"); /* we provide another version */

    /* register new functions */
    lua_register(L, "print", Lua_print);

    Lunar<Lua_Calaos>::Register(L);
    Lua_Calaos lc;
    Lunar<Lua_Calaos>::push(L, &lc);
    lua_setglobal(L, "calaos");

    //Call setlocale to change for C locale (and avoid problems with double value<>string conversion)
    setlocale(LC_ALL, "C");

    //Set a hook to kill script in case of a wrong use (infinite loop, ...)
    lua_sethook(L, Lua_DebugHook, LUA_MASKLINE | LUA_MASKCOUNT, 1);

    QTime t = QTime::currentTime();
    start_time = (double)t.second() + (((double) t.msec()) / 1000);
    abortTimeout = false;

    int err = luaL_loadbuffer(L, script.c_str(), script.length(), "CalaosScript");
    if (err)
    {
        ret = false;
        if (err == LUA_ERRSYNTAX)
        {
            string msg = lua_tostring(L, -1);
            errorMsg = "Syntax Error:\n" + msg;
        }
        else if (err == LUA_ERRMEM)
        {
            string msg = lua_tostring(L, -1);
            errorMsg = "Fatal Error:\nLUA memory allocation error:\n" + msg;
        }
    }
    else
    {
        if (setjmp(panic_jmp) == 1)
        {
            ret = false;
            errorMsg = "Fatal Error:\nScript panic !";
        }

        if ((err = lua_pcall(L, 0, 1, 0)))
        {
            ret = false;

            string errcode;
            if (err == LUA_ERRRUN) errcode = "Runtime error";
            else if (err == LUA_ERRSYNTAX) errcode = "Syntax error";
            else if (err == LUA_ERRMEM) errcode = "Memory allocation error";
            else if (err == LUA_ERRERR) errcode = "Error";
            else errcode = "Unknown error";

            string msg = lua_tostring(L, -1);
            errorMsg = "Error " + errcode + " :\n\t" + msg;

            if (abortTimeout)
            {
                //script aborted due to a long run. We don't check more here and say it's ok
                errorScript = false;
                ret = true;
            }
        }
        else
        {
            if (!lua_isboolean(L, -1))
            {
                ret = false;
                errorMsg = "Error:\nScript must return either \"true\" or \"false\"";
            }
            else
            {
                errorScript = false;
                ret = lua_toboolean(L, -1);
            }
        }
    }

    lua_close(L);

    return ret;
}

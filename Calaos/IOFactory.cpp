/******************************************************************************
*  Copyright (c) 2007-2014, Calaos. All Rights Reserved.
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

#include <IOFactory.h>
#include <QtCore>

#include <set>
#include <algorithm>

using namespace Calaos;

Registrar::Registrar(string type, function<IOBase *(Params &)> classFunc, bool legacy)
{
    std::transform(type.begin(), type.end(), type.begin(), Utils::to_lower());
    IOFactory::Instance().RegisterClass(type, classFunc, legacy);
}

bool IOFactory::isLegacyType(string type) const
{
    std::transform(type.begin(), type.end(), type.begin(), Utils::to_lower());
    return legacyTypes.count(type) > 0;
}

std::vector<string> IOFactory::creatableTypes() const
{
    std::vector<string> types;
    for (const auto &entry: ioFunctionRegistry)
    {
        if (!legacyTypes.count(entry.first))
            types.push_back(entry.first);
    }
    std::sort(types.begin(), types.end());
    return types;
}

// Same set as the server's dropLegacyBounds(). Not InputTime/InputTimer: their `min` is minutes.
static bool hasLegacyBounds(const std::string &lowerType)
{
    static const std::set<std::string> types = {
        "wianalog", "wagoinputanalog", "witemp", "wagoinputtemp", "woanalog", "wagooutputanalog",
        "webinputanalog", "webinputtemp", "weboutputanalog",
        "mqttinputanalog", "mqttinputtemp", "mqttoutputanalog",
        "knxinputanalog", "knxinputtemp", "knxoutputanalog",
        "owtemp", "internalint"
    };
    return types.count(lowerType) > 0;
}

IOBase *IOFactory::CreateIO(std::string type, Params &params, int io_type, string scriptDemoValue)
{
    IOBase *obj = nullptr;

    std::transform(type.begin(), type.end(), type.begin(), Utils::to_lower());

    if (hasLegacyBounds(type))
    {
        params.Delete("min");
        params.Delete("max");
    }

    auto it = ioFunctionRegistry.find(type);
    if (it != ioFunctionRegistry.end())
        obj = it->second(params);

    if (!obj)
    {
        qWarning() << type.c_str() << ": Unknown type! Creating a default IO";
        obj = new IOBase(params, "unknown", TSTRING, io_type, scriptDemoValue);
    }

    return obj;
}

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

using namespace Calaos;

Registrar::Registrar(string type, function<Input *(Params &)> classFunc)
{
    std::transform(type.begin(), type.end(), type.begin(), Utils::to_lower());
    IOFactory::Instance().RegisterClass(type, classFunc);
}

Registrar::Registrar(string type, function<Output *(Params &)> classFunc)
{
    std::transform(type.begin(), type.end(), type.begin(), Utils::to_lower());
    IOFactory::Instance().RegisterClass(type, classFunc);
}

Input *IOFactory::CreateInput(std::string type, Params &params)
{
    Input *obj = nullptr;

    std::transform(type.begin(), type.end(), type.begin(), Utils::to_lower());

    auto it = inputFunctionRegistry.find(type);
    if (it != inputFunctionRegistry.end())
        obj = it->second(params);

    if (!obj)
        qWarning() << type.c_str() << ": Unknown Input type !";

    return obj;
}

Output *IOFactory::CreateOutput(std::string type, Params &params)
{
    Output *obj = nullptr;

    std::transform(type.begin(), type.end(), type.begin(), Utils::to_lower());

    auto it = outputFunctionRegistry.find(type);
    if (it != outputFunctionRegistry.end())
        obj = it->second(params);

    if (!obj)
        qWarning() << type.c_str() << ": Unknown Output type !";

    return obj;
}

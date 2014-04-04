/******************************************************************************
**  Copyright (c) 2007-2014, Calaos. All Rights Reserved.
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
#ifndef S_IOFactory_H
#define S_IOFactory_H

#include <unordered_map>
#include <memory>
#include <functional>
#include <IOBase.h>
#include <stdlib.h>
#include <Params.h>

namespace Calaos
{

class Registrar
{
public:
    Registrar(string type, function<Input *(Params &)> classFunc);
    Registrar(string type, function<Output *(Params &)> classFunc);
};

#define REGISTER_FACTORY(NAME, TYPE, RETURNCLASS) static Registrar NAME##_reg_(#NAME, [](Params &_p) -> RETURNCLASS * { return new TYPE(_p); });

#define REGISTER_INPUT_USERTYPE(NAME, TYPE) REGISTER_FACTORY(NAME, TYPE, Input)
#define REGISTER_INPUT(TYPE) REGISTER_INPUT_USERTYPE(TYPE, TYPE)

#define REGISTER_OUTPUT_USERTYPE(NAME, TYPE) REGISTER_FACTORY(NAME, TYPE, Output)
#define REGISTER_OUTPUT(TYPE) REGISTER_OUTPUT_USERTYPE(TYPE, TYPE)

class IOFactory
{
private:
    IOFactory() {}

    unordered_map<string, function<Input *(Params &)>> inputFunctionRegistry;
    unordered_map<string, function<Output *(Params &)>> outputFunctionRegistry;

public:

    Input *CreateInput(string type, Params &params);
    Output *CreateOutput(string type, Params &params);

    void RegisterClass(string type, function<Input *(Params &)> classFunc)
    {
        inputFunctionRegistry[type] = classFunc;
    }

    void RegisterClass(string type, function<Output *(Params &)> classFunc)
    {
        outputFunctionRegistry[type] = classFunc;
    }

    static IOFactory &Instance()
    {
        static IOFactory inst;
        return inst;
    }
};

}
#endif

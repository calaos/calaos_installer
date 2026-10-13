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
#include <set>
#include <vector>
#include <IOBase.h>
#include <stdlib.h>
#include <Params.h>

namespace Calaos
{

class Registrar
{
public:
    Registrar(string type, function<IOBase *(Params &)> classFunc, bool legacy = false);
};

#define REGISTER_FACTORY(NAME, GTYPE, DTYPE, ITYPE, SVALUE) \
    static Registrar NAME##_reg_(#NAME, [](Params &_p) -> IOBase * { return new IOBase(_p, GTYPE, DTYPE, ITYPE, SVALUE); });

// Types the server no longer knows. Registered so that existing files are still read and
// written back unchanged; they must never be offered for creation.
#define REGISTER_LEGACY_FACTORY(NAME, GTYPE, DTYPE, ITYPE, SVALUE) \
    static Registrar NAME##_reg_(#NAME, [](Params &_p) -> IOBase * { return new IOBase(_p, GTYPE, DTYPE, ITYPE, SVALUE); }, true);

#define REGISTER_LEGACY_INPUT(NAME, GTYPE, DTYPE, SVALUE) REGISTER_LEGACY_FACTORY(NAME, GTYPE, DTYPE, IOBase::IO_INPUT, SVALUE)
#define REGISTER_LEGACY_OUTPUT(NAME, GTYPE, DTYPE, SVALUE) REGISTER_LEGACY_FACTORY(NAME, GTYPE, DTYPE, IOBase::IO_OUTPUT, SVALUE)
#define REGISTER_LEGACY_INOUT(NAME, GTYPE, DTYPE, SVALUE) REGISTER_LEGACY_FACTORY(NAME, GTYPE, DTYPE, IOBase::IO_BOTH, SVALUE)

#define REGISTER_INPUT(NAME, GTYPE, DTYPE, SVALUE) REGISTER_FACTORY(NAME, GTYPE, DTYPE, IOBase::IO_INPUT, SVALUE)
#define REGISTER_OUTPUT(NAME, GTYPE, DTYPE, SVALUE) REGISTER_FACTORY(NAME, GTYPE, DTYPE, IOBase::IO_OUTPUT, SVALUE)
#define REGISTER_INOUT(NAME, GTYPE, DTYPE, SVALUE) REGISTER_FACTORY(NAME, GTYPE, DTYPE, IOBase::IO_BOTH, SVALUE)

class IOFactory
{
private:
    IOFactory() {}

    unordered_map<string, function<IOBase *(Params &)>> ioFunctionRegistry;
    std::set<string> legacyTypes;

public:

    IOBase *CreateIO(string type, Params &params, int io_type = IOBase::IO_NONE, string scriptDemoValue = string());

    void RegisterClass(string type, function<IOBase *(Params &)> classFunc, bool legacy = false)
    {
        ioFunctionRegistry[type] = classFunc;
        if (legacy)
            legacyTypes.insert(type);
    }

    //Case-insensitive, like the server's own lookup.
    bool isLegacyType(string type) const;

    //Lower-cased registered types the installer may offer for creation.
    std::vector<string> creatableTypes() const;

    static IOFactory &Instance()
    {
        static IOFactory inst;
        return inst;
    }
};

}
#endif

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
#ifndef S_CONDITION_H
#define S_CONDITION_H
//-----------------------------------------------------------------------------
#include <Utils.h>
#include <iostream>
#include <IOBase.h>
#include <Params.h>
#include <stdlib.h>
//-----------------------------------------------------------------------------
using namespace std;
//-----------------------------------------------------------------------------
namespace Calaos
{

class Condition
{
        protected:
                std::vector<Input *> inputs;
                Params params;
                Params ops;

                //this is used to do the condition test
                //based on another input
                Params params_var;

        public:
                Condition()
                        { }
                ~Condition();

                void Add(Input *p);
                bool Evaluate();
                void Remove(int i);

                Input *get_input(int i) { return inputs[i]; }
                Params &get_params() { return params; }
                Params &get_operator() { return ops; }
                Params &get_params_var() { return params_var; }
                void set_param(Params &p) { params = p; }
                void set_operator(Params &p) { ops = p; }
                void set_param_var(Params &p) { params_var = p; }

                int get_size() { return inputs.size(); }
};
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
}
#endif

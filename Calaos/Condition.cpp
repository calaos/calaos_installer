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
#include <Condition.h>

using namespace Calaos;

Condition::~Condition()
{
}

void Condition::Add(IOBase *in)
{
    inputs.push_back(in);
}

void Condition::Remove(int pos)
{
    vector<IOBase *>::iterator iter = inputs.begin();
    for (int i = 0;i < pos;iter++, i++) ;
    inputs.erase(iter);
}

void Condition::removeScriptInput(int pos)
{
    vector<IOBase *>::iterator iter = in_event.begin();
    for (int i = 0;i < pos;iter++, i++) ;
    in_event.erase(iter);
}

void Condition::removeScriptInput(IOBase *in)
{
    vector<IOBase *>::iterator iter = in_event.begin();
    for (int i = 0;i < (int)in_event.size() && in != in_event[i];iter++, i++) ;
    in_event.erase(iter);
}

Condition *Condition::duplicate()
{
    Condition *cond = new Condition(getType());

    cond->condition_trigger = condition_trigger;
    cond->inputs = inputs;
    cond->params = params;
    cond->ops = ops;
    cond->params_var = params_var;
    cond->script = script;
    cond->in_event = in_event;
    cond->output = output;
    cond->output_param = output_param;
    cond->output_param_var = output_param_var;
    cond->output_oper = output_oper;

    return cond;
}

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
#ifndef S_CONDITION_H
#define S_CONDITION_H

#include <Utils.h>
#include "IOBase.h"

namespace Calaos
{

enum { COND_UNKONWN = 0, COND_STD, COND_START, COND_SCRIPT, COND_OUTPUT };

class Condition
{
        protected:
                int condition_type;

                std::vector<IOBase *> inputs;
                Params params;
                Params ops;

                //this is used to do the condition test
                //based on another input
                Params params_var;

                string script;
                //These are declared inputs that will trigger the rule execution
                //Most generaly, inputs are those used in the script
                vector<IOBase *> in_event;

                //Output condition
                IOBase *output;
                string output_param, output_param_var, output_oper;

        public:
                Condition(int ctype): condition_type(ctype)
                        { }
                ~Condition();

                int getType() { return condition_type; }

                /*-- Standard Condition --*/
                void Add(IOBase *p);
                bool Evaluate();
                void Remove(int i);

                IOBase *get_input(int i) { return inputs[i]; }
                Params &get_params() { return params; }
                Params &get_operator() { return ops; }
                Params &get_params_var() { return params_var; }
                void set_param(Params &p) { params = p; }
                void set_operator(Params &p) { ops = p; }
                void set_param_var(Params &p) { params_var = p; }

                int get_size() { return inputs.size(); }
                /*-- Standard Condition --*/

                /*-- Script Condition --*/
                int getScriptInputSize() { return in_event.size(); }
                IOBase *getScriptInput(int i) { return in_event[i]; }
                void addScriptInput(IOBase *in) { in_event.push_back(in); }
                void clearScriptInput() { in_event.clear(); }
                void removeScriptInput(int i);
                void removeScriptInput(IOBase *in);
                string getScript() { return script; }
                void setScript(string s) { script = s; }
                /*-- Script Condition --*/

                /*-- Output Condition --*/
                IOBase *getOutput() { return output; }
                void setOutput(IOBase *o) { output = o; }
                string getOutputParam() { return output_param; }
                void setOutputParam(string param) { output_param = param; }
                string getOutputOper() { return output_oper; }
                void setOutputOper(string p) { output_oper = p; }
                string getOutputParamVar() { return output_param_var; }
                void setOutputParamVar(string param) { output_param_var = param; }
                /*-- Output Condition --*/

};

}
#endif

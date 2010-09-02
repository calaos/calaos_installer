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
#include <Rule.h>

using namespace Calaos;

Rule::~Rule()
{
        for (uint i = 0;i < conds.size();i++)
                delete conds[i];

        for (uint i = 0;i < actions.size();i++)
                delete actions[i];
}

void Rule::AddCondition(Condition *cond)
{
        conds.push_back(cond);
}

void Rule::AddAction(Action *act)
{
        actions.push_back(act);
}

void Rule::RemoveCondition(int pos)
{
        vector<Condition *>::iterator iter = conds.begin();
        for (int i = 0;i < pos;iter++, i++) ;
        conds.erase(iter);
}

void Rule::RemoveAction(int pos)
{
        vector<Action *>::iterator iter = actions.begin();
        for (int i = 0;i < pos;iter++, i++) ;
        actions.erase(iter);
}

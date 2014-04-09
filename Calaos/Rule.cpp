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

void Rule::Print()
{
    cout << "Content of rule \"" << name << " - " << type << "\"" << endl;
    cout << "Conditions (" << conds.size() << ") :" << endl;
    for (uint i = 0;i < conds.size();i++)
    {
        Condition *c = conds[i];
        if (c->getType() == COND_STD)
        {
            cout << "  * Condition Standard : ";
            for (int j = 0;j < c->get_size();j++)
            {
                cout << "\"" << c->get_input(j)->get_param("name") << "\" ";
            }
            cout << endl;
        }
        else if (c->getType() == COND_SCRIPT)
        {
            cout << "  * Condition Script" << endl;
        }
        else if (c->getType() == COND_START)
        {
            cout << "  * Condition Start" << endl;
        }
        else
        {
            cout << "  * Condition UNKNOWN !" << endl;
        }
    }

    cout << "Actions (" << actions.size() << ") :" << endl;
    for (uint i = 0;i < actions.size();i++)
    {
        Action *a = actions[i];
        if (a->getType() == ACTION_STD)
        {
            cout << "  * Action Standard : ";
            for (int j = 0;j < a->get_size();j++)
            {
                cout << "\"" << a->get_output(j)->get_param("name") << "\" ";
            }
            cout << endl;
        }
        else if (a->getType() == ACTION_MAIL)
        {
            cout << "  * Action Mail" << endl;
        }
        else if (a->getType() == ACTION_SCRIPT)
        {
            cout << "  * Action Script" << endl;
        }
        else if (a->getType() == ACTION_TOUCHSCREEN)
        {
            cout << "  * Action Touchscreen" << endl;
        }
        else
        {
            cout << "  * Action UNKNOWN !" << endl;
        }
    }
    cout << endl;
}

void Rule::MoveConditionUp(int pos)
{
    if (pos <= 0 || conds.size() < 2) return;

    int pos1 = pos - 1;
    int pos2 = pos;

    std::swap(conds[pos1], conds[pos2]);
}

void Rule::MoveConditionDown(int pos)
{
    MoveConditionUp(pos + 1);
}

void Rule::MoveActionUp(int pos)
{
    if (pos <= 0 || actions.size() < 2) return;

    int pos1 = pos - 1;
    int pos2 = pos;

    std::swap(actions[pos1], actions[pos2]);
}

void Rule::MoveActionDown(int pos)
{
    MoveActionUp(pos + 1);
}

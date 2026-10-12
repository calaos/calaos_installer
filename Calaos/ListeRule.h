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
#ifndef S_LISTERULE_H
#define S_LISTERULE_H
//-----------------------------------------------------------------------------
#include <Utils.h>
#include <iostream>
#include <Rule.h>
#include <ListeRoom.h>
#include <Room.h>
#include <unistd.h>
#include <set>
#include <string>
#include <vector>
//-----------------------------------------------------------------------------
using namespace std;
//-----------------------------------------------------------------------------
namespace Calaos
{

class ListeRule
{
protected:
    std::vector<Rule *> rules;

    ListeRule()
    { }

public:
    //singleton
    static ListeRule &Instance();

    ~ListeRule();

    void Add(Rule *p);
    void Remove(int i);
    void Remove(Rule *obj);
    //Rules keep raw pointers: this must run before io is destroyed.
    //Script conditions only lose their trigger on io; variable ids naming io
    //stay in the rules and are reserved.
    void RemoveRulesUsing(IOBase *io);
    Rule *searchRuleUsing(IOBase *io);

    //Same matching as RemoveRulesUsing, for reading what a deletion would do.
    //ruleHolds leaves out script triggers: losing one never removes the rule.
    static bool ruleHolds(Rule *rule, IOBase *io);
    static bool scriptTriggeredBy(Rule *rule, IOBase *io);
    //Variable ids of the rule (sorted, unique) that are one of names.
    static std::vector<std::string> variablesNaming(Rule *rule, const std::set<std::string> &names);

    Rule *searchRule(string type, string name);

    Rule *get_rule(int i);
    Rule *operator[] (int i) const;

    int size() { return rules.size(); }

    void clear();
};

}
#endif

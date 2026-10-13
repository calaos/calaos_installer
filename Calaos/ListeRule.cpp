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
#include <ListeRule.h>
#include <algorithm>
#include <set>
//-----------------------------------------------------------------------------
using namespace Calaos;
//-----------------------------------------------------------------------------
ListeRule &ListeRule::Instance()
{
    static ListeRule inst;

    return inst;
}
//-----------------------------------------------------------------------------
ListeRule::~ListeRule()
{
    clear();
}
//-----------------------------------------------------------------------------
void ListeRule::Add(Rule *p)
{
    rules.push_back(p);
}
//-----------------------------------------------------------------------------
void ListeRule::Remove(int pos)
{
    vector<Rule *>::iterator iter = rules.begin();
    for (int i = 0;i < pos;iter++, i++) ;
    delete rules[pos];
    rules.erase(iter);
}
//-----------------------------------------------------------------------------
Rule *ListeRule::operator[] (int i) const
{
    return rules[i];
}
//-----------------------------------------------------------------------------
void ListeRule::Remove(Rule *obj)
{
    rules.erase( std::remove( rules.begin(), rules.end(), obj) , rules.end());
    delete obj;
}
//-----------------------------------------------------------------------------
Rule *ListeRule::get_rule(int i)
{
    return rules[i];
}
//-----------------------------------------------------------------------------
namespace
{

//Pointer only: it is what dangles, and what a rule holds. Several IOs may share
//an id (or a camera answer to iid/oid), and those must keep their rules.
//Script triggers are not rule members: they are detached, not matched.
bool ruleUses(Rule *rule, IOBase *io)
{
    for (int j = 0;j < rule->get_size_conds();j++)
    {
        Condition *cond = rule->get_condition(j);

        for (int k = 0;k < cond->get_size();k++)
            if (cond->get_input(k) == io)
                return true;
        if (cond->getOutput() == io)
            return true;
    }

    for (int j = 0;j < rule->get_size_actions();j++)
    {
        Action *action = rule->get_action(j);
        for (int k = 0;k < action->get_size();k++)
            if (action->get_output(k) == io)
                return true;
    }

    return false;
}

bool scriptUses(Rule *rule, IOBase *io)
{
    for (int j = 0;j < rule->get_size_conds();j++)
    {
        Condition *cond = rule->get_condition(j);
        for (int k = 0;k < cond->getScriptInputSize();k++)
            if (cond->getScriptInput(k) == io)
                return true;
    }

    return false;
}

//Variables are referenced by id, possibly a legacy iid/oid.
template<class F>
void forEachVariable(Rule *rule, F fn)
{
    auto all = [&](Params &vars)
    {
        for (int k = 0;k < vars.size();k++)
        {
            string key, value;
            vars.get_item(k, key, value);
            fn(value);
        }
    };

    for (int j = 0;j < rule->get_size_conds();j++)
    {
        Condition *cond = rule->get_condition(j);
        all(cond->get_params_var());
        fn(cond->getOutputParamVar());
    }

    for (int j = 0;j < rule->get_size_actions();j++)
        all(rule->get_action(j)->get_params_var());
}

void reserveVariablesOf(Rule *rule, const std::set<string> &names)
{
    forEachVariable(rule, [&](const string &var)
    {
        if (names.count(var))
            ListeRoom::reservePendingIOId(var);
    });
}

}
//-----------------------------------------------------------------------------
bool ListeRule::ruleHolds(Rule *rule, IOBase *io)
{
    return ruleUses(rule, io);
}
//-----------------------------------------------------------------------------
std::vector<std::string> ListeRule::variablesNaming(Rule *rule, const std::set<std::string> &names)
{
    std::set<string> found;
    forEachVariable(rule, [&](const string &var)
    {
        if (names.count(var))
            found.insert(var);
    });

    return std::vector<string>(found.begin(), found.end());
}
//-----------------------------------------------------------------------------
Rule *ListeRule::searchRuleUsing(IOBase *io)
{
    for (Rule *rule: rules)
        if (ruleUses(rule, io) || scriptUses(rule, io))
            return rule;

    return NULL;
}
//-----------------------------------------------------------------------------
void ListeRule::RemoveRulesUsing(IOBase *io)
{
    for (Rule *rule: rules)
        for (int j = 0;j < rule->get_size_conds();j++)
            rule->get_condition(j)->removeScriptInput(io);

    for (;;)
    {
        auto it = std::find_if(rules.begin(), rules.end(), [&](Rule *r) { return ruleUses(r, io); });
        if (it == rules.end())
            break;
        Remove(*it);
    }

    //A variable reference outlives its IO (the user decides what to do with it),
    //so its id must not be given to a new IO.
    std::set<string> names;
    for (const char *k: { "id", "iid", "oid" })
    {
        const string v = io->get_param(k);
        if (!v.empty())
            names.insert(v);
    }
    for (Rule *rule: rules)
        reserveVariablesOf(rule, names);
}
//-----------------------------------------------------------------------------
Rule *ListeRule::searchRule(string type, string name)
{
    for (uint i = 0;i < rules.size();i++)
    {
        if (rules[i]->get_name() == name &&
            rules[i]->get_type() == type)
        {
            return rules[i];
        }
    }

    return NULL;
}
//-----------------------------------------------------------------------------
void ListeRule::clear()
{
    for (uint i = 0;i < rules.size();i++)
        delete rules[i];

    rules.clear();
}
//-----------------------------------------------------------------------------

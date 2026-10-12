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

//A condition or action may hold an IO other than the one in the room list
//(legacy iid/oid references), so ids are compared as well as pointers.
//An empty id never identifies anything.
bool sameIO(IOBase *ref, IOBase *io)
{
    if (!ref)
        return false;
    if (ref == io)
        return true;

    static const char *keys[] = { "id", "iid", "oid" };
    for (const char *k: keys)
    {
        const string a = io->get_param(k);
        if (a.empty())
            continue;
        for (const char *m: keys)
            if (a == ref->get_param(m))
                return true;
    }

    return false;
}

bool ruleUses(Rule *rule, IOBase *io)
{
    for (int j = 0;j < rule->get_size_conds();j++)
    {
        Condition *cond = rule->get_condition(j);

        for (int k = 0;k < cond->get_size();k++)
            if (sameIO(cond->get_input(k), io))
                return true;
        for (int k = 0;k < cond->getScriptInputSize();k++)
            if (sameIO(cond->getScriptInput(k), io))
                return true;
        if (sameIO(cond->getOutput(), io))
            return true;
    }

    for (int j = 0;j < rule->get_size_actions();j++)
    {
        Action *action = rule->get_action(j);
        for (int k = 0;k < action->get_size();k++)
            if (sameIO(action->get_output(k), io))
                return true;
    }

    return false;
}

}
//-----------------------------------------------------------------------------
Rule *ListeRule::searchRuleUsing(IOBase *io)
{
    for (Rule *rule: rules)
        if (ruleUses(rule, io))
            return rule;

    return NULL;
}
//-----------------------------------------------------------------------------
void ListeRule::RemoveRulesUsing(IOBase *io)
{
    while (Rule *rule = searchRuleUsing(io))
        Remove(rule);
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
Rule *ListeRule::searchRuleInput(IOBase *input)
{
    for (uint i = 0;i < rules.size();i++)
    {
        Rule *rule = rules[i];
        for (int j = 0;j < rule->get_size_conds();j++)
        {
            Condition *cond = rule->get_condition(j);
            for (int k = 0;k < cond->get_size();k++)
            {
                if (cond->get_input(k) == input)
                {
                    return rules[i];
                }
            }
        }
    }

    return NULL;
}
//-----------------------------------------------------------------------------
Rule *ListeRule::searchRuleOutput(IOBase *output)
{
    for (uint i = 0;i < rules.size();i++)
    {
        Rule *rule = rules[i];
        for (int j = 0;j < rule->get_size_actions();j++)
        {
            Action *action = rule->get_action(j);
            for (int k = 0;k < action->get_size();k++)
            {
                if (action->get_output(k) == output)
                {
                    return rules[i];
                }
            }
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

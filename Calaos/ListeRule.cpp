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
void ListeRule::RemoveRule(Input *obj)
{
        //delete all rules using "output"
        for (uint i = 0;i < rules.size();i++)
        {
                Rule *rule = get_rule(i);
                Rule *rule_to_del = NULL;
                for (int j = 0;j < rule->get_size_conds();j++)
                {
                        Condition *cond = rule->get_condition(j);
                        for (int k = 0;k < cond->get_size();k++)
                        {
                                if (obj->get_param("id") == cond->get_input(k)->get_param("id"))
                                        rule_to_del = rule;
                                if (obj->get_param("iid") == cond->get_input(k)->get_param("id") && obj->get_param("iid") != "")
                                        rule_to_del = rule;
                                if (obj->get_param("id") == cond->get_input(k)->get_param("iid") && cond->get_input(k)->get_param("iid") != "")
                                        rule_to_del = rule;
                                if (obj->get_param("iid") == cond->get_input(k)->get_param("iid") && obj->get_param("iid") != "")
                                        rule_to_del = rule;
                        }
                }
                if (rule_to_del)
                {
                        Remove(rule_to_del);
                        i--;
                }
        }
}
//-----------------------------------------------------------------------------
void ListeRule::RemoveRule(Output *obj)
{
        //delete all rules using "output"
        for (uint i = 0;i < rules.size();i++)
        {
                Rule *rule = get_rule(i);
                Rule *rule_to_del = NULL;
                for (int j = 0;j < rule->get_size_actions();j++)
                {
                        Action *action = rule->get_action(j);
                        for (int k = 0;k < action->get_size();k++)
                        {
                                if (obj->get_param("id") == action->get_output(k)->get_param("id"))
                                        rule_to_del = rule;
                                if (obj->get_param("oid") == action->get_output(k)->get_param("id") && obj->get_param("oid") != "")
                                        rule_to_del = rule;
                                if (obj->get_param("id") == action->get_output(k)->get_param("oid") && action->get_output(k)->get_param("oid") != "")
                                        rule_to_del = rule;
                                if (obj->get_param("oid") == action->get_output(k)->get_param("oid") && obj->get_param("oid") != "")
                                        rule_to_del = rule;
                        }
                }
                if (rule_to_del)
                {
                        Remove(rule_to_del);
                        i--;
                }
        }
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
Rule *ListeRule::searchRule(Input *input)
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
Rule *ListeRule::searchRule(Output *output)
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
void ListeRule::RemoveSpecialRules(string specialType)
{
        for (uint i = 0;i < rules.size();i++)
        {
                Rule *rule = get_rule(i);

                if(rule->get_specialType() == specialType)
                {
                        Remove(rule);
                        i=0;
                }
        }
}
//-----------------------------------------------------------------------------
void ListeRule::clear()
{
        for (uint i = 0;i < rules.size();i++)
                delete rules[i];

        rules.clear();
}
//-----------------------------------------------------------------------------

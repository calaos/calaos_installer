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
#ifndef S_ACTION_H
#define S_ACTION_H

#include <Utils.h>
#include "IOBase.h"

namespace Calaos
{
enum { ACTION_UNKONWN = 0, ACTION_STD, ACTION_MAIL, ACTION_SCRIPT, ACTION_TOUCHSCREEN };

class Action
{
protected:
    int action_type;

    std::vector<IOBase *> outputs;
    Params params;

    //this is used to do the action
    //based on another output state
    Params params_var;

    string mail_sender;
    string mail_recipients;
    string mail_subject;
    string mail_attachment;
    string mail_message;

    string script;

    string action_touchscreen, action_touchscreen_cam;

public:
    Action(int atype): action_type(atype)
    { }
    ~Action();

    int getType() { return action_type; }

    /*-- Standard Action --*/
    void Add(IOBase *p);
    void Remove(int i);

    IOBase *get_output(int i) { return outputs[i]; }
    Params &get_params() { return params; }
    void set_param(Params &p) { params = p; }
    Params &get_params_var() { return params_var; }
    void set_param_var(Params &p) { params_var = p; }

    int get_size() { return outputs.size(); }
    /*-- Standard Action --*/


    /*-- Mail Action --*/
    string getMailSender() { return mail_sender; }
    string getMailRecipients() { return mail_recipients; }
    string getMailSubject() { return mail_subject; }
    string getMailAttachment() { return mail_attachment; }
    string getMailMessage() { return mail_message; }

    void setMailSender(string s) { mail_sender = s; }
    void setMailRecipients(string s) { mail_recipients = s; }
    void setMailSubject(string s) { mail_subject = s; }
    void setMailAttachment(string s) { mail_attachment = s; }
    void setMailMessage(string s) { mail_message = s; }
    /*-- Mail Action --*/

    /*-- Script Action --*/
    string getScript() { return script; }
    void setScript(string s) { script = s; }
    /*-- Script Action --*/

    /*-- Touchscreen Action --*/
    string getTouchscreenAction() { return action_touchscreen; }
    void setTouchscreenAction(string s) { action_touchscreen = s; }
    string getTouchscreenCamera() { return action_touchscreen_cam; }
    void setTouchscreenCamera(string s) { action_touchscreen_cam = s; }
    /*-- Touchscreen Action --*/

    Action *duplicate();

};

}
#endif

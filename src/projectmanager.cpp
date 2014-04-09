#include "projectmanager.h"

map<string, bool> ProjectManager::wagoTypeCache;

IOXmlWriter::IOXmlWriter()
{
    setAutoFormatting(true);
}

bool IOXmlWriter::writeFile(QIODevice *device)
{
    setDevice(device);

    writeStartDocument("1.0");
    writeNamespace("http://www.calaos.fr", "calaos");
    writeStartElement("http://www.calaos.fr", "ioconfig");
    writeStartElement("http://www.calaos.fr", "home");

    for (int i = 0;i < ListeRoom::Instance().size();i++)
    {
        Room *room = ListeRoom::Instance().get_room(i);

        writeRoom(room);
    }

    writeEndDocument();

    return true;
}

void IOXmlWriter::writeRoom(Room *room)
{
    writeStartElement("http://www.calaos.fr", "room");

    QXmlStreamAttributes attr;
    attr.append("name", QString::fromUtf8(room->get_name().c_str()));
    attr.append("type", QString::fromUtf8(room->get_type().c_str()));
    attr.append("hits", QString::fromUtf8(Utils::to_string(room->get_hits()).c_str()));
    writeAttributes(attr);

    for (int i = 0;i < room->get_size_in();i++)
    {
        writeInput(room->get_input(i));
    }

    for (int i = 0;i < room->get_size_out();i++)
    {
        writeOutput(room->get_output(i));
    }

    writeEndElement();
}

void IOXmlWriter::writeInput(IOBase *io)
{
    string type = io->get_param("type");

    if (io->get_gui_type() == "audio" || io->get_gui_type() == "camera")
        return;

    if (type == "InternalBool" ||
        type == "InternalInt" ||
        type == "InternalString")
    {
        writeStartElement("http://www.calaos.fr", "internal");
    }
    else
    {
        writeStartElement("http://www.calaos.fr", "input");
    }

    QXmlStreamAttributes attr;

    for (int i = 0;i < io->get_params().size();i++)
    {
        string key, value;
        io->get_params().get_item(i, key, value);

        attr.append(QString::fromUtf8(key.c_str()),
                    QString::fromUtf8(value.c_str()));
    }
    writeAttributes(attr);

    IOBase *in = io;

    //special case: InPlageHoraire
    if (type == "InPlageHoraire" && in)
    {
        writeStartElement("http://www.calaos.fr", "lundi");
        writePlages(in->plg_lundi);
        writeEndElement();
        writeStartElement("http://www.calaos.fr", "mardi");
        writePlages(in->plg_mardi);
        writeEndElement();
        writeStartElement("http://www.calaos.fr", "mercredi");
        writePlages(in->plg_mercredi);
        writeEndElement();
        writeStartElement("http://www.calaos.fr", "jeudi");
        writePlages(in->plg_jeudi);
        writeEndElement();
        writeStartElement("http://www.calaos.fr", "vendredi");
        writePlages(in->plg_vendredi);
        writeEndElement();
        writeStartElement("http://www.calaos.fr", "samedi");
        writePlages(in->plg_samedi);
        writeEndElement();
        writeStartElement("http://www.calaos.fr", "dimanche");
        writePlages(in->plg_dimanche);
        writeEndElement();
    }

    writeEndElement();
}

void IOXmlWriter::writePlages(vector<Horaire> &day)
{
    for (uint i = 0;i < day.size();i++)
    {
        Horaire &h = day[i];

        writeStartElement("http://www.calaos.fr", "plage");
        QXmlStreamAttributes attr;
        attr.append(QString::fromUtf8("start_hour"),
                    QString::fromUtf8(h.shour.c_str()));
        attr.append(QString::fromUtf8("start_min"),
                    QString::fromUtf8(h.smin.c_str()));
        attr.append(QString::fromUtf8("start_sec"),
                    QString::fromUtf8(h.ssec.c_str()));
        attr.append(QString::fromUtf8("end_hour"),
                    QString::fromUtf8(h.ehour.c_str()));
        attr.append(QString::fromUtf8("end_min"),
                    QString::fromUtf8(h.emin.c_str()));
        attr.append(QString::fromUtf8("end_sec"),
                    QString::fromUtf8(h.esec.c_str()));
        writeAttributes(attr);
        writeEndElement();
    }
}

void IOXmlWriter::writeOutput(IOBase *io)
{
    string type = io->get_param("type");

    if (type == "Scenario" || type == "scenario" ||
        type == "InputTimer" || type == "InternalBool" ||
        type == "InternalInt" || type == "InternalString")
        return;

    if (io->get_gui_type() == "camera")
    {
        writeStartElement("http://www.calaos.fr", "camera");
    }
    else if (io->get_gui_type() == "audio")
    {
        writeStartElement("http://www.calaos.fr", "audio");
    }
    else
    {
        writeStartElement("http://www.calaos.fr", "output");
    }

    QXmlStreamAttributes attr;

    for (int i = 0;i < io->get_params().size();i++)
    {
        string key, value;
        io->get_params().get_item(i, key, value);

        attr.append(QString::fromUtf8(key.c_str()),
                    QString::fromUtf8(value.c_str()));
    }
    writeAttributes(attr);

    writeEndElement();
}

RuleXmlWriter::RuleXmlWriter()
{
    setAutoFormatting(true);
}

bool RuleXmlWriter::writeFile(QIODevice *device)
{
    setDevice(device);

    writeStartDocument("1.0");
    writeNamespace("http://www.calaos.fr", "calaos");

    writeStartElement("http://www.calaos.fr", "rules");

    for (int i = 0;i < ListeRule::Instance().size();i++)
    {
        Rule *rule = ListeRule::Instance().get_rule(i);

        writeRule(rule);
    }

    writeEndDocument();

    return true;
}

void RuleXmlWriter::writeRule(Rule *rule)
{
    writeStartElement("http://www.calaos.fr", "rule");

    QXmlStreamAttributes attr;
    attr.append("name", QString::fromUtf8(rule->get_name().c_str()));
    attr.append("type", QString::fromUtf8(rule->get_type().c_str()));
    attr.append("specialType", QString::fromUtf8(rule->get_specialType().c_str()));
    writeAttributes(attr);

    writeCondition(rule);
    writeAction(rule);

    writeEndElement();
}

void RuleXmlWriter::writeCondition(Rule *rule)
{
    for (int i = 0;i < rule->get_size_conds();i++)
    {
        Condition *cond = rule->get_condition(i);

        switch (cond->getType())
        {
        case COND_STD:
        {
            for (int j = 0;j < cond->get_size();j++)
            {
                writeStartElement("http://www.calaos.fr", "condition");
                QXmlStreamAttributes attr;
                attr.append("type", "standard");
                writeAttributes(attr);

                writeStartElement("http://www.calaos.fr", "input");

                Params &oper = cond->get_operator();
                Params &params = cond->get_params();
                Params &params_var = cond->get_params_var();

                string id = cond->get_input(j)->get_param("id");
                if (cond->get_input(j)->get_gui_type() == "audio" ||
                    cond->get_input(j)->get_gui_type() == "camera")
                    id = cond->get_input(j)->get_param("iid");

                attr.clear();
                attr.append("id", QString::fromUtf8(id.c_str()));
                attr.append("oper", QString::fromUtf8(oper[id].c_str()));
                attr.append("val", QString::fromUtf8(params[id].c_str()));
                if (params_var[id] != "")
                    attr.append("val_var", QString::fromUtf8(params_var[id].c_str()));
                writeAttributes(attr);

                writeEndElement();
                writeEndElement();
            }
            break;
        }
        case COND_OUTPUT:
        {
            writeStartElement("http://www.calaos.fr", "condition");
            QXmlStreamAttributes attr;
            attr.append("type", "output");
            writeAttributes(attr);

            writeStartElement("http://www.calaos.fr", "output");

            string id = cond->getOutput()->get_param("id");
            if (cond->getOutput()->get_gui_type() == "audio" ||
                cond->getOutput()->get_gui_type() == "camera")
                id = cond->getOutput()->get_param("oid");

            attr.clear();
            attr.append("id", QString::fromUtf8(id.c_str()));
            attr.append("oper", QString::fromUtf8(cond->getOutputOper().c_str()));
            attr.append("val", QString::fromUtf8(cond->getOutputParam().c_str()));
            if (cond->getOutputParamVar() != "")
                attr.append("val_var", QString::fromUtf8(cond->getOutputParamVar().c_str()));
            writeAttributes(attr);

            writeEndElement();

            writeEndElement();

            break;
        }
        case COND_START:
        {
            writeStartElement("http://www.calaos.fr", "condition");
            QXmlStreamAttributes attr;
            attr.append("type", "start");
            writeAttributes(attr);
            writeEndElement();
            break;
        }
        case COND_SCRIPT:
        {
            writeStartElement("http://www.calaos.fr", "condition");
            QXmlStreamAttributes attr;
            attr.append("type", "script");
            writeAttributes(attr);

            for (int i = 0;i < cond->getScriptInputSize();i++)
            {
                writeStartElement("http://www.calaos.fr", "input");
                QXmlStreamAttributes attr;

                string id = cond->getScriptInput(i)->get_param("id");
                if (cond->getScriptInput(i)->get_gui_type() == "audio" ||
                    cond->getScriptInput(i)->get_gui_type() == "camera")
                    id = cond->getScriptInput(i)->get_param("iid");

                attr.append("id", QString::fromUtf8(id.c_str()));
                writeAttributes(attr);
                writeEndElement();
            }

            writeStartElement("http://www.calaos.fr", "script");
            attr.clear();
            attr.append("type", "lua");
            writeAttributes(attr);

            writeCDATA(QString::fromUtf8(cond->getScript().c_str()));

            writeEndElement();
            writeEndElement();
            break;
        }
        }

    }
}

void RuleXmlWriter::writeAction(Rule *rule)
{
    for (int i = 0;i < rule->get_size_actions();i++)
    {
        Action *action = rule->get_action(i);

        switch (action->getType())
        {
        case ACTION_STD:
        {
            for (int j = 0;j < action->get_size();j++)
            {
                writeStartElement("http://www.calaos.fr", "action");
                QXmlStreamAttributes attr;
                attr.append("type", "standard");
                writeAttributes(attr);
                writeStartElement("http://www.calaos.fr", "output");

                Params &params = action->get_params();
                Params &params_var = action->get_params_var();

                string id = action->get_output(j)->get_param("id");
                if (action->get_output(j)->get_gui_type() == "audio" ||
                    action->get_output(j)->get_gui_type() == "camera")
                    id = action->get_output(j)->get_param("oid");

                attr.clear();
                attr.append("id", QString::fromUtf8(id.c_str()));
                attr.append("val", QString::fromUtf8(params[id].c_str()));
                if (params_var[id] != "")
                    attr.append("val_var", QString::fromUtf8(params_var[id].c_str()));
                writeAttributes(attr);

                writeEndElement();
                writeEndElement();
            }
            break;
        }
        case ACTION_MAIL:
        {
            writeStartElement("http://www.calaos.fr", "action");
            QXmlStreamAttributes attr;
            attr.append("type", "mail");
            writeAttributes(attr);

            writeStartElement("http://www.calaos.fr", "mail");
            attr.clear();
            attr.append("sender", QString::fromUtf8(action->getMailSender().c_str()));
            attr.append("recipients", QString::fromUtf8(action->getMailRecipients().c_str()));
            attr.append("subject", QString::fromUtf8(action->getMailSubject().c_str()));
            attr.append("attachment", QString::fromUtf8(action->getMailAttachment().c_str()));
            writeAttributes(attr);

            writeCDATA(QString::fromUtf8(action->getMailMessage().c_str()));

            writeEndElement();
            writeEndElement();
            break;
        }
        case ACTION_SCRIPT:
        {
            writeStartElement("http://www.calaos.fr", "action");
            QXmlStreamAttributes attr;
            attr.append("type", "script");
            writeAttributes(attr);

            writeStartElement("http://www.calaos.fr", "script");
            attr.clear();
            attr.append("type", "lua");
            writeAttributes(attr);

            writeCDATA(QString::fromUtf8(action->getScript().c_str()));

            writeEndElement();
            writeEndElement();
            break;
        }
        case ACTION_TOUCHSCREEN:
        {
            writeStartElement("http://www.calaos.fr", "action");
            QXmlStreamAttributes attr;
            attr.append("type", "touchscreen");
            attr.append("action", QString::fromUtf8(action->getTouchscreenAction().c_str()));
            writeAttributes(attr);
            writeEndElement();
            break;
        }
        }

    }
}

IOXmlReader::IOXmlReader()
{
}

bool IOXmlReader::readFile(QIODevice *device)
{
    setDevice(device);

    bool file_ok = false;

    while (!atEnd())
    {
        readNext();

        if (isStartElement())
        {
            if (name() == "ioconfig")
            {
                file_ok = true;
                continue;
            }
            else if (name() == "home" && file_ok)
            {
                readHome();
            }
            else
                raiseError(QString::fromUtf8("Ce fichier n'est pas un fichier IO Calaos."));
        }
    }

    return !error();
}

void IOXmlReader::readHome()
{
    while (!atEnd())
    {
        readNext();

        if (isEndElement())
            break;

        if (isStartElement())
        {
            if (name() == "room")
            {
                readRoom();
            }
        }
    }
}

void IOXmlReader::readRoom()
{
    string _name = attributes().value("name").toString().toUtf8().data();
    string _type = attributes().value("type").toString().toUtf8().data();
    string _hits = attributes().value("hits").toString().toUtf8().data();
    int hits;
    from_string(_hits, hits);

    Room *room = new Room(_name, _type, hits);
    ListeRoom::Instance().Add(room);

    while (!atEnd())
    {
        readNext();

        if (isEndElement())
            break;

        if (isStartElement())
        {
            if (name() == "input")
                readInput(room);
            else if (name() == "output")
                readOutput(room);
            else if (name() == "camera")
                readCamera(room);
            else if (name() == "internal")
                readInput(room);
            else if (name() == "audio")
                readAudio(room);
        }
    }
}

void IOXmlReader::readInput(Room *room)
{
    Params p;

    for (int i = 0;i < attributes().size();i++)
    {
        QXmlStreamAttribute attr = attributes().at(i);
        p.Add(attr.name().toString().toUtf8().data(),
              attr.value().toString().toUtf8().data());
    }

    if (ListeRoom::Instance().get_input(p["id"]))
    {
        QMessageBox::critical(NULL, "Calaos Installer", QString::fromUtf8("L'entrée avec l'id \"%1\" existe déjà !").arg(QString(p["id"].c_str())));
    }

    IOBase *in = ListeRoom::Instance().createInput(p, room);

    //Do cache of 849/842 only with WIDigitalBP/WIDIgitialTriple
    if (in->get_param("type") == "WIDigitalBP" ||
        in->get_param("type") == "WIDigitalTriple" ||
        in->get_param("type") == "WIDigitalLong")
    {
        if (in->get_param("wago_841") != "true")
            ProjectManager::wagoTypeCache[in->get_param("host")] = false;
        else
            ProjectManager::wagoTypeCache[in->get_param("host")] = true;
    }

    while (!atEnd())
    {
        readNext();

        if (isEndElement())
            break;

        if (isStartElement())
        {
            if (p["type"] == "InPlageHoraire" && name() == "lundi")
                readPlageDay(in->plg_lundi);
            else if (p["type"] == "InPlageHoraire" && name() == "mardi")
                readPlageDay(in->plg_mardi);
            else if (p["type"] == "InPlageHoraire" && name() == "mercredi")
                readPlageDay(in->plg_mercredi);
            else if (p["type"] == "InPlageHoraire" && name() == "jeudi")
                readPlageDay(in->plg_jeudi);
            else if (p["type"] == "InPlageHoraire" && name() == "vendredi")
                readPlageDay(in->plg_vendredi);
            else if (p["type"] == "InPlageHoraire" && name() == "samedi")
                readPlageDay(in->plg_samedi);
            else if (p["type"] == "InPlageHoraire" && name() == "dimanche")
                readPlageDay(in->plg_dimanche);
        }
    }
}

void IOXmlReader::readPlageDay(vector<Horaire> &day)
{
    while (!atEnd())
    {
        readNext();

        if (isEndElement())
            break;

        if (isStartElement())
        {
            if (name() == "plage")
            {
                Horaire h;
                readPlage(h);
                day.push_back(h);
            }
        }
    }
}

void IOXmlReader::readPlage(Horaire &horaire)
{
    for (int i = 0;i < attributes().size();i++)
    {
        QXmlStreamAttribute attr = attributes().at(i);
        if (attr.name() == "start_hour")
            horaire.shour = attr.value().toString().toUtf8().data();
        if (attr.name() == "start_min")
            horaire.smin = attr.value().toString().toUtf8().data();
        if (attr.name() == "start_sec")
            horaire.ssec = attr.value().toString().toUtf8().data();
        if (attr.name() == "end_hour")
            horaire.ehour = attr.value().toString().toUtf8().data();
        if (attr.name() == "end_min")
            horaire.emin = attr.value().toString().toUtf8().data();
        if (attr.name() == "end_sec")
            horaire.esec = attr.value().toString().toUtf8().data();
    }

    while (!atEnd())
    {
        readNext();

        if (isEndElement())
            break;
    }
}

void IOXmlReader::readOutput(Room *room)
{
    Params p;

    for (int i = 0;i < attributes().size();i++)
    {
        QXmlStreamAttribute attr = attributes().at(i);
        p.Add(attr.name().toString().toUtf8().data(),
              attr.value().toString().toUtf8().data());
    }

    if (ListeRoom::Instance().get_output(p["id"]))
    {
        QMessageBox::critical(NULL, "Calaos Installer", QString::fromUtf8("La sortie avec l'id \"%1\" existe déjà !").arg(QString(p["id"].c_str())));
    }

    IOBase *output = ListeRoom::Instance().createOutput(p, room);

    if (!output)
        return;

    //Do cache of 849/842 only with WODigital/WOVolet*
    if (output->get_param("type") == "WODigital" ||
        output->get_param("type") == "WOVolet" ||
        output->get_param("type") == "WOVoletSmart")
    {
        if (output->get_param("wago_841") != "true")
            ProjectManager::wagoTypeCache[output->get_param("host")] = false;
        else
            ProjectManager::wagoTypeCache[output->get_param("host")] = true;
    }

    while (!atEnd())
    {
        readNext();

        if (isEndElement())
            break;
    }
}

void IOXmlReader::readCamera(Room *room)
{
    Params p;

    for (int i = 0;i < attributes().size();i++)
    {
        QXmlStreamAttribute attr = attributes().at(i);
        p.Add(attr.name().toString().toUtf8().data(),
              attr.value().toString().toUtf8().data());
    }

    ListeRoom::Instance().createCamera(p, room);

    while (!atEnd())
    {
        readNext();

        if (isEndElement())
            break;
    }
}

void IOXmlReader::readAudio(Room *room)
{
    Params p;

    for (int i = 0;i < attributes().size();i++)
    {
        QXmlStreamAttribute attr = attributes().at(i);
        p.Add(attr.name().toString().toUtf8().data(),
              attr.value().toString().toUtf8().data());
    }

    ListeRoom::Instance().createAudio(p, room);

    while (!atEnd())
    {
        readNext();

        if (isEndElement())
            break;
    }
}

bool ProjectManager::saveIOsToFile(QString &file)
{
    QFile t(file);
    if (!t.open(QIODevice::WriteOnly | QIODevice::Text))
        return false;

    IOXmlWriter xmlfile;
    xmlfile.writeFile(&t);

    t.close();

    return true;
}

bool ProjectManager::saveRulesToFile(QString &file)
{
    QFile t(file);
    if (!t.open(QIODevice::WriteOnly | QIODevice::Text))
        return false;

    RuleXmlWriter xmlfile;
    xmlfile.writeFile(&t);

    t.close();

    return true;
}

bool ProjectManager::loadIOsFromFile(QString &file)
{
    QFile t(file);
    if (!t.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;

    IOXmlReader xmlfile;
    xmlfile.readFile(&t);

    t.close();

    return true;
}

bool ProjectManager::loadRulesFromFile(QString &file)
{
    QFile t(file);
    if (!t.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;

    QDomDocument doc("rules");
    if (!doc.setContent(&t))
    {
        t.close();
        return false;
    }
    t.close();

    QDomElement elem = doc.documentElement();

    if (elem.isNull())
        return false;

    QDomElement node_rule = elem.firstChildElement("calaos:rule");
    while (!node_rule.isNull())
    {
        string name = node_rule.attribute("name").toUtf8().data();
        string type = node_rule.attribute("type").toUtf8().data();
        string stype = node_rule.attribute("specialType").toUtf8().data();

        Rule *rule = new Rule(type, name, stype);
        ListeRule::Instance().Add(rule);

        //Read conditions
        QDomElement node_cond = node_rule.firstChildElement("calaos:condition");
        while (!node_cond.isNull())
        {
            Condition *cond = NULL;
            string cond_type = node_cond.attribute("type").toUtf8().data();

            if (cond_type == "standard" || cond_type == "")
            {
                cond = new Condition(COND_STD);

                QDomElement node_in = node_cond.firstChildElement("calaos:input");
                while(!node_in.isNull())
                {
                    string id = node_in.attribute("id").toUtf8().data();
                    string oper = node_in.attribute("oper").toUtf8().data();
                    string val = node_in.attribute("val").toUtf8().data();
                    string val_var = node_in.attribute("val_var").toUtf8().data();

                    IOBase *input = ListeRoom::Instance().get_input(id);

                    if (input)
                    {
                        cond->Add(input);
                        cond->get_params().Add(id, val);
                        cond->get_operator().Add(id, oper);
                        if (val_var != "")
                            cond->get_params_var().Add(id, val_var);
                    }

                    node_in = node_in.nextSiblingElement("calaos:input");
                }
            }
            else if (cond_type == "output")
            {
                cond = new Condition(COND_OUTPUT);

                QDomElement node_in = node_cond.firstChildElement("calaos:output");
                while(!node_in.isNull())
                {
                    string id = node_in.attribute("id").toUtf8().data();
                    string oper = node_in.attribute("oper").toUtf8().data();
                    string val = node_in.attribute("val").toUtf8().data();
                    string val_var = node_in.attribute("val_var").toUtf8().data();

                    IOBase *output = ListeRoom::Instance().get_output(id);

                    if (output)
                    {
                        cond->setOutput(output);
                        cond->setOutputOper(oper);
                        cond->setOutputParam(val);
                        if (val_var != "")
                            cond->setOutputParamVar(val_var);
                    }

                    node_in = node_in.nextSiblingElement("calaos:output");
                }
            }
            else if (cond_type == "start")
            {
                cond = new Condition(COND_START);
            }
            else if (cond_type == "script")
            {
                cond = new Condition(COND_SCRIPT);

                QDomElement node_in = node_cond.firstChildElement();
                while (!node_in.isNull())
                {
                    if (node_in.tagName() == "calaos:input")
                    {
                        string id = node_in.attribute("id").toUtf8().data();
                        IOBase *input = ListeRoom::Instance().get_input(id);
                        if (input)
                            cond->addScriptInput(input);
                    }
                    else if (node_in.tagName() == "calaos:script")
                    {
                        QDomCDATASection data = node_in.toCDATASection();
                        cond->setScript(data.data().toUtf8().data());
                    }

                    node_in = node_in.nextSiblingElement();
                }
            }

            if (cond)
            {
                rule->AddCondition(cond);
            }

            node_cond = node_cond.nextSiblingElement("calaos:condition");
        }

        //Read actions
        QDomElement node_action = node_rule.firstChildElement("calaos:action");
        while (!node_action.isNull())
        {
            Action *action = NULL;
            string action_type = node_action.attribute("type").toUtf8().data();
            if (action_type == "standard" || action_type == "")
            {
                action = new Action(ACTION_STD);

                QDomElement node_out = node_action.firstChildElement("calaos:output");
                while(!node_out.isNull())
                {
                    string id = node_out.attribute("id").toUtf8().data();
                    string val = node_out.attribute("val").toUtf8().data();
                    string val_var = node_out.attribute("val_var").toUtf8().data();

                    IOBase *output = ListeRoom::Instance().get_output(id);

                    if (output)
                    {
                        action->Add(output);
                        action->get_params().Add(id, val);
                        if (val_var != "")
                            action->get_params_var().Add(id, val_var);
                    }

                    node_out = node_out.nextSiblingElement("calaos:output");
                }
            }
            else if (action_type == "mail")
            {
                action = new Action(ACTION_MAIL);

                QDomElement node_mail = node_action.firstChildElement("calaos:mail");
                string mail_sender = node_mail.attribute("sender").toUtf8().data();
                string mail_recipients = node_mail.attribute("recipients").toUtf8().data();
                string mail_subject = node_mail.attribute("subject").toUtf8().data();
                string mail_attachment = node_mail.attribute("attachment").toUtf8().data();

                action->setMailSender(mail_sender);
                action->setMailRecipients(mail_recipients);
                action->setMailSubject(mail_subject);
                action->setMailAttachment(mail_attachment);

                QDomCDATASection data = node_mail.firstChild().toCDATASection();
                action->setMailMessage(data.data().toUtf8().data());
            }
            else if (action_type == "script")
            {
                action = new Action(ACTION_SCRIPT);

                QDomElement node_script = node_action.firstChildElement();
                if (node_script.attribute("type") == "lua")
                {
                    QDomCDATASection data = node_script.firstChild().toCDATASection();
                    action->setScript(data.data().toUtf8().data());
                }
            }
            else if (action_type == "touchscreen")
            {
                action = new Action(ACTION_TOUCHSCREEN);

                string tact = node_action.attribute("action").toUtf8().data();
                action->setTouchscreenAction(tact);
            }

            if (action)
            {
                rule->AddAction(action);
            }

            node_action = node_action.nextSiblingElement("calaos:action");
        }

        node_rule = node_rule.nextSiblingElement("calaos:rule");
    }

    return true;
}

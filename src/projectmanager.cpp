#include "projectmanager.h"

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

void IOXmlWriter::writeInput(Input *io)
{
        string type = io->get_param("type");

        if (IOBase::isAudioType(type) || IOBase::isCameraType(type))
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

        InPlageHoraire *in = dynamic_cast<InPlageHoraire *>(io);

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

void IOXmlWriter::writeOutput(Output *io)
{
        string type = io->get_param("type");

        if (type == "Scenario" || type == "scenario" ||
            type == "InputTimer" || type == "InternalBool" ||
            type == "InternalInt" || type == "InternalString")
                return;

        if (IOBase::isCameraType(type))
        {
                writeStartElement("http://www.calaos.fr", "camera");
        }
        else if (IOBase::isAudioType(type))
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
                for (int j = 0;j < cond->get_size();j++)
                {
                        writeStartElement("http://www.calaos.fr", "condition");
                        writeStartElement("http://www.calaos.fr", "input");

                        Params &oper = cond->get_operator();
                        Params &params = cond->get_params();
                        Params &params_var = cond->get_params_var();

                        string id = cond->get_input(j)->get_param("id");

                        QXmlStreamAttributes attr;
                        attr.append("id", QString::fromUtf8(id.c_str()));
                        attr.append("oper", QString::fromUtf8(oper[id].c_str()));
                        attr.append("val", QString::fromUtf8(params[id].c_str()));
                        if (params_var[id] != "")
                                attr.append("val_var", QString::fromUtf8(params_var[id].c_str()));
                        writeAttributes(attr);

                        writeEndElement();
                        writeEndElement();
                }
        }
}

void RuleXmlWriter::writeAction(Rule *rule)
{
        for (int i = 0;i < rule->get_size_actions();i++)
        {
                Action *action = rule->get_action(i);
                for (int j = 0;j < action->get_size();j++)
                {
                        writeStartElement("http://www.calaos.fr", "action");
                        writeStartElement("http://www.calaos.fr", "output");

                        Params &params = action->get_params();
                        Params &params_var = action->get_params_var();

                        string id = action->get_output(j)->get_param("id");

                        QXmlStreamAttributes attr;
                        attr.append("id", QString::fromUtf8(id.c_str()));
                        attr.append("val", QString::fromUtf8(params[id].c_str()));
                        if (params_var[id] != "")
                                attr.append("val_var", QString::fromUtf8(params_var[id].c_str()));
                        writeAttributes(attr);

                        writeEndElement();
                        writeEndElement();
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
        string _name = attributes().value("name").toString().toLocal8Bit().data();
        string _type = attributes().value("type").toString().toLocal8Bit().data();
        string _hits = attributes().value("hits").toString().toLocal8Bit().data();
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
                p.Add(attr.name().toString().toLocal8Bit().data(),
                      attr.value().toString().toLocal8Bit().data());
        }

        Input *in = ListeRoom::Instance().createInput(p, room);
        InPlageHoraire *input = dynamic_cast<InPlageHoraire *>(in);

        while (!atEnd())
        {
                readNext();

                if (isEndElement())
                        break;

                if (isStartElement())
                {
                        if (p["type"] == "InPlageHoraire" && name() == "lundi")
                                readPlageDay(input->plg_lundi);
                        else if (p["type"] == "InPlageHoraire" && name() == "mardi")
                                readPlageDay(input->plg_mardi);
                        else if (p["type"] == "InPlageHoraire" && name() == "mercredi")
                                readPlageDay(input->plg_mercredi);
                        else if (p["type"] == "InPlageHoraire" && name() == "jeudi")
                                readPlageDay(input->plg_jeudi);
                        else if (p["type"] == "InPlageHoraire" && name() == "vendredi")
                                readPlageDay(input->plg_vendredi);
                        else if (p["type"] == "InPlageHoraire" && name() == "samedi")
                                readPlageDay(input->plg_samedi);
                        else if (p["type"] == "InPlageHoraire" && name() == "dimanche")
                                readPlageDay(input->plg_dimanche);
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
                        horaire.shour = attr.value().toString().toLocal8Bit().data();
                if (attr.name() == "start_min")
                        horaire.smin = attr.value().toString().toLocal8Bit().data();
                if (attr.name() == "start_sec")
                        horaire.ssec = attr.value().toString().toLocal8Bit().data();
                if (attr.name() == "end_hour")
                        horaire.ehour = attr.value().toString().toLocal8Bit().data();
                if (attr.name() == "end_min")
                        horaire.emin = attr.value().toString().toLocal8Bit().data();
                if (attr.name() == "end_sec")
                        horaire.esec = attr.value().toString().toLocal8Bit().data();
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
                p.Add(attr.name().toString().toLocal8Bit().data(),
                      attr.value().toString().toLocal8Bit().data());
        }

        ListeRoom::Instance().createOutput(p, room);

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
                p.Add(attr.name().toString().toLocal8Bit().data(),
                      attr.value().toString().toLocal8Bit().data());
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
                p.Add(attr.name().toString().toLocal8Bit().data(),
                      attr.value().toString().toLocal8Bit().data());
        }

        ListeRoom::Instance().createAudio(p, room);

        while (!atEnd())
        {
                readNext();

                if (isEndElement())
                        break;
        }
}

RuleXmlReader::RuleXmlReader()
{
}

bool RuleXmlReader::readFile(QIODevice *device)
{
        setDevice(device);

        bool file_ok = false;

        while (!atEnd())
        {
                readNext();

                if (isStartElement())
                {
                        if (name() == "rules")
                        {
                                file_ok = true;
                                continue;
                        }
                        else if (name() == "rule" && file_ok)
                        {
                                readRule();
                        }
                        else
                                raiseError(QString::fromUtf8("Ce fichier n'est pas un fichier Rules Calaos."));
                }
        }

        return !error();
}

void RuleXmlReader::readRule()
{
        string _name = attributes().value("name").toString().toLocal8Bit().data();
        string _type = attributes().value("type").toString().toLocal8Bit().data();

        Rule *rule = new Rule(_type, _name);
        ListeRule::Instance().Add(rule);

        while (!atEnd())
        {
                readNext();

                if (isEndElement())
                        break;

                if (isStartElement())
                {
                        if (name() == "condition")
                                readCondition(rule);
                        else if (name() == "action")
                                readAction(rule);
                }
        }
}

void RuleXmlReader::readCondition(Rule *rule)
{
        Condition *cond = new Condition();
        rule->AddCondition(cond);

        while (!atEnd())
        {
                readNext();

                if (isEndElement())
                        break;

                if (isStartElement())
                {
                        if (name() == "input")
                                readInput(cond);
                }
        }
}

void RuleXmlReader::readInput(Condition *cond)
{
        string id = attributes().value("id").toString().toLocal8Bit().data();
        string oper = attributes().value("oper").toString().toLocal8Bit().data();
        string val = attributes().value("val").toString().toLocal8Bit().data();
        string val_var = "";
        if (attributes().hasAttribute("val_var"))
                val_var = attributes().value("val_var").toString().toLocal8Bit().data();

        Input *input = ListeRoom::Instance().get_input(id);

        if (input)
        {
                cond->Add(input);
                cond->get_params().Add(id, val);
                cond->get_operator().Add(id, oper);
                if (val_var != "")
                        cond->get_params_var().Add(id, val_var);
        }

        while (!atEnd())
        {
                readNext();

                if (isEndElement())
                        break;
        }
}

void RuleXmlReader::readAction(Rule *rule)
{
        Action *action = new Action();
        rule->AddAction(action);

        while (!atEnd())
        {
                readNext();

                if (isEndElement())
                        break;

                if (isStartElement())
                {
                        if (name() == "output")
                                readOutput(action);
                }
        }
}

void RuleXmlReader::readOutput(Action *action)
{
        string id = attributes().value("id").toString().toLocal8Bit().data();
        string val = attributes().value("val").toString().toLocal8Bit().data();
        string val_var = "";
        if (attributes().hasAttribute("val_var"))
                val_var = attributes().value("val_var").toString().toLocal8Bit().data();

        Output *output = ListeRoom::Instance().get_output(id);

        if (output)
        {
                action->Add(output);
                action->get_params().Add(id, val);
                if (val_var != "")
                        action->get_params_var().Add(id, val_var);
        }

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

        RuleXmlReader xmlfile;
        xmlfile.readFile(&t);

        t.close();

        return true;
}

#include "projectmanager.h"
#include <QDebug>

map<string, bool> ProjectManager::wagoTypeCache;

static const QString CALAOS_NS = QStringLiteral("http://www.calaos.fr");

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

    //Same rule as the server: format 1 readers silently drop devices, so the
    //marker is only there when there is something to lose.
    if (ListeRoom::Instance().hasDevices())
        writeAttribute("format", QString::number(ListeRoom::IO_FORMAT_SUPPORTED));

    for (int i = 0;i < ListeRoom::Instance().size();i++)
    {
        Room *room = ListeRoom::Instance().get_room(i);

        writeRoom(room);
    }

    for (const QString &xml: ListeRoom::Instance().getUnknownXml())
        writeRawXml(xml);

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

    //A device and the IOs it owns are written nested, by writeDevice().
    auto nested = [](IOBase *io)
    {
        return dynamic_cast<Device *>(io) ||
               ListeRoom::Instance().get_device(io->getOwnerDevice());
    };

    for (int i = 0;i < room->get_size_in();i++)
    {
        if (!nested(room->get_input(i)))
            writeInput(room->get_input(i));
    }

    for (int i = 0;i < room->get_size_out();i++)
    {
        if (!nested(room->get_output(i)))
            writeOutput(room->get_output(i));
    }

    for (Device *dev: room->get_devices())
        writeDevice(dev);

    for (const QString &xml: room->getUnknownXml())
        writeRawXml(xml);

    writeEndElement();
}

void IOXmlWriter::writeInput(IOBase *io)
{
    string type = io->get_param("type");

    if (io->get_gui_type() == "audio" || io->get_gui_type() == "camera" || io->get_gui_type() == "avreceiver")
        return;

    if (type == "InternalBool" ||
        type == "InternalInt" ||
        type == "InternalString")
    {
        writeStartElement("http://www.calaos.fr", "internal");
    }
    else
    {
        if (io->is_inout() &&
            io->get_gui_type() != "scenario" &&
            io->get_gui_type() != "timer")
            return; //We don't want to write the input when IO is IO_BOTH, except for scenario and timer

        writeStartElement("http://www.calaos.fr", "input");
    }

    QXmlStreamAttributes attr;

    if (io->get_gui_type() == "time_range")
    {
        //Save months
        stringstream ssmonth;
        ssmonth << io->range_months;
        string str = ssmonth.str();
        std::reverse(str.begin(), str.end());
        attr.append(QString("months"),
                    QString::fromUtf8(str.c_str()));
    }

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
        writePlages(in->range_monday);
        writeEndElement();
        writeStartElement("http://www.calaos.fr", "mardi");
        writePlages(in->range_tuesday);
        writeEndElement();
        writeStartElement("http://www.calaos.fr", "mercredi");
        writePlages(in->range_wednesday);
        writeEndElement();
        writeStartElement("http://www.calaos.fr", "jeudi");
        writePlages(in->range_thursday);
        writeEndElement();
        writeStartElement("http://www.calaos.fr", "vendredi");
        writePlages(in->range_friday);
        writeEndElement();
        writeStartElement("http://www.calaos.fr", "samedi");
        writePlages(in->range_saturday);
        writeEndElement();
        writeStartElement("http://www.calaos.fr", "dimanche");
        writePlages(in->range_sunday);
        writeEndElement();
    }

    for (const QString &xml: io->getUnknownXml())
        writeRawXml(xml);

    writeEndElement();
}

void IOXmlWriter::writePlages(vector<TimeRange> &day)
{
    for (uint i = 0;i < day.size();i++)
    {
        TimeRange &h = day[i];

        writeStartElement("http://www.calaos.fr", "plage");
        QXmlStreamAttributes attr;
        attr.append(QString::fromUtf8("start_hour"),
                    QString::fromUtf8(h.shour.c_str()));
        attr.append(QString::fromUtf8("start_min"),
                    QString::fromUtf8(h.smin.c_str()));
        attr.append(QString::fromUtf8("start_sec"),
                    QString::fromUtf8(h.ssec.c_str()));
        attr.append(QString::fromUtf8("start_type"),
                    QString("%1").arg(h.start_type));
        attr.append(QString::fromUtf8("start_offset"),
                    QString("%1").arg(h.start_offset));

        attr.append(QString::fromUtf8("end_hour"),
                    QString::fromUtf8(h.ehour.c_str()));
        attr.append(QString::fromUtf8("end_min"),
                    QString::fromUtf8(h.emin.c_str()));
        attr.append(QString::fromUtf8("end_sec"),
                    QString::fromUtf8(h.esec.c_str()));
        attr.append(QString::fromUtf8("end_type"),
                    QString("%1").arg(h.end_type));
        attr.append(QString::fromUtf8("end_offset"),
                    QString("%1").arg(h.end_offset));
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
    else if (io->get_gui_type() == "avreceiver")
    {
        writeStartElement("http://www.calaos.fr", "avr");
    }
    else if (io->get_gui_type() == "remote_ui")
    {
        writeStartElement("http://www.calaos.fr", "remote_ui");
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

    // Special case: RemoteUI - write the pages XML content
    if (type == "RemoteUI")
    {
        QString pagesXml = io->getRemoteUIPagesXml();
        if (!pagesXml.isEmpty())
            writeRawXml(pagesXml);
    }

    for (const QString &xml: io->getUnknownXml())
        writeRawXml(xml);

    writeEndElement();
}

void IOXmlWriter::writeDevice(Device *dev)
{
    writeStartElement("http://www.calaos.fr", "device");

    QXmlStreamAttributes attr;
    for (int i = 0;i < dev->get_params().size();i++)
    {
        string key, value;
        dev->get_params().get_item(i, key, value);
        attr.append(QString::fromUtf8(key.c_str()), QString::fromUtf8(value.c_str()));
    }
    writeAttributes(attr);

    //Each kind is written by exactly one of the two, as in writeRoom().
    for (IOBase *io: dev->ownedIos())
    {
        if (io->is_input())
            writeInput(io);
        if (io->is_output())
            writeOutput(io);
    }

    for (const DeviceLink &l: dev->links())
    {
        writeStartElement("http://www.calaos.fr", "link");
        writeAttribute("name", QString::fromUtf8(l.name.c_str()));
        writeAttribute("io", QString::fromUtf8(l.io.c_str()));
        if (!l.commandIo.empty())
            writeAttribute("command_io", QString::fromUtf8(l.commandIo.c_str()));
        writeEndElement();
    }

    for (const auto &p: dev->deviceParams())
    {
        writeStartElement("http://www.calaos.fr", "param");
        writeAttribute("name", QString::fromUtf8(p.first.c_str()));
        writeAttribute("value", QString::fromUtf8(p.second.c_str()));
        writeEndElement();
    }

    for (const QString &xml: dev->getUnknownXml())
        writeRawXml(xml);

    writeEndElement();
}

void IOXmlWriter::writeRawXml(const QString &xml)
{
    QXmlStreamReader reader(xml);
    while (!reader.atEnd())
    {
        reader.readNext();

        if (reader.isStartElement())
        {
            //Before the element, so that its own prefixes are used instead of
            //generated ones; calaos is already declared on the root.
            for (const QXmlStreamNamespaceDeclaration &ns: reader.namespaceDeclarations())
            {
                if (ns.namespaceUri() == CALAOS_NS)
                    continue;
                if (ns.prefix().isEmpty())
                    writeDefaultNamespace(ns.namespaceUri().toString());
                else
                    writeNamespace(ns.namespaceUri().toString(), ns.prefix().toString());
            }
            writeStartElement(reader.namespaceUri().toString(), reader.name().toString());
            writeAttributes(reader.attributes());
        }
        else if (reader.isEndElement())
            writeEndElement();
        else if (reader.isCDATA())
            writeCDATA(reader.text().toString());
        else if (reader.isCharacters() && !reader.isWhitespace())
            writeCharacters(reader.text().toString());
    }
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
    for (int i = 0;i < rule->ruleParams.size();i++)
    {
        string key, value;
        rule->ruleParams.get_item(i, key, value);

        attr.append(QString::fromUtf8(key.c_str()),
                    QString::fromUtf8(value.c_str()));
    }
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
                attr.append("trigger", cond->isTrigger()?"true":"false");
                writeAttributes(attr);

                writeStartElement("http://www.calaos.fr", "input");

                Params &oper = cond->get_operator();
                Params &params = cond->get_params();
                Params &params_var = cond->get_params_var();

                string id = cond->get_input(j)->get_param("id");
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
            if (!cond->getOutput())
            {
                //Nothing to write, and dereferencing it would crash. This can
                //only happen for a condition that was built without an output.
                qWarning() << "Skipping an output condition without any output";
                break;
            }

            writeStartElement("http://www.calaos.fr", "condition");
            QXmlStreamAttributes attr;
            attr.append("type", "output");
            attr.append("trigger", cond->isTrigger()?"true":"false");
            writeAttributes(attr);

            writeStartElement("http://www.calaos.fr", "output");

            attr.clear();
            attr.append("id", QString::fromStdString(cond->getOutput()->get_param("id")));
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

                attr.append("id", QString::fromStdString(cond->getScriptInput(i)->get_param("id")));
                writeAttributes(attr);
                writeEndElement();
            }

            writeStartElement("http://www.calaos.fr", "script");
            attr.clear();
            attr.append("type", "lua");
            writeAttributes(attr);

            writeCDATA(QString::fromUtf8(cond->getScript().c_str()).replace('\r', ""));

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

            writeCDATA(QString::fromUtf8(action->getMailMessage().c_str()).replace('\r', ""));

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

            writeCDATA(QString::fromUtf8(action->getScript().c_str()).replace('\r', ""));

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
            if (action->getTouchscreenAction() == "view_camera")
                attr.append("camera", QString::fromUtf8(action->getTouchscreenCamera().c_str()));
            writeAttributes(attr);
            writeEndElement();
            break;
        }
        case ACTION_PUSH:
        {
            writeStartElement("http://www.calaos.fr", "action");
            QXmlStreamAttributes attr;
            attr.append("type", "push");
            writeAttributes(attr);

            writeStartElement("http://www.calaos.fr", "push");
            attr.clear();
            attr.append("attachment", QString::fromUtf8(action->getMailAttachment().c_str()));
            writeAttributes(attr);

            writeCDATA(QString::fromUtf8(action->getMailMessage().c_str()).replace('\r', ""));

            writeEndElement();
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
            if (name() == QStringLiteral("ioconfig"))
            {
                file_ok = true;
                continue;
            }
            else if (name() == QStringLiteral("home") && file_ok)
            {
                readHome();
            }
            else
                raiseError(QObject::tr("This file is not correctly formated!"));
        }
    }

    return !error();
}

void IOXmlReader::readHome()
{
    ListeRoom::Instance().setLoadedFormat(attributes().value("format").toString());

    while (!atEnd())
    {
        readNext();

        if (isEndElement())
            break;

        if (isStartElement())
        {
            if (name() == QStringLiteral("room"))
                readRoom();
            else
                ListeRoom::Instance().addUnknownXml(readRawElement());
        }
    }
}

// Copies the element under the cursor with its whole subtree and leaves the
// cursor on its end tag, where the caller's loop expects it.
QString IOXmlReader::readRawElement()
{
    QString xml;
    QXmlStreamWriter writer(&xml);
    writer.writeNamespace(CALAOS_NS, "calaos");

    int depth = 0;
    while (true)
    {
        if (isStartElement())
        {
            for (const QXmlStreamNamespaceDeclaration &ns: namespaceDeclarations())
            {
                if (ns.namespaceUri() == CALAOS_NS)
                    continue;
                if (ns.prefix().isEmpty())
                    writer.writeDefaultNamespace(ns.namespaceUri().toString());
                else
                    writer.writeNamespace(ns.namespaceUri().toString(), ns.prefix().toString());
            }
            writer.writeStartElement(namespaceUri().toString(), name().toString());
            writer.writeAttributes(attributes());
            depth++;
        }
        else if (isEndElement())
        {
            writer.writeEndElement();
            if (--depth == 0)
                break;
        }
        else if (isCDATA())
            writer.writeCDATA(text().toString());
        else if (isCharacters() && !isWhitespace())
            writer.writeCharacters(text().toString());

        if (atEnd())
            break;
        readNext();
    }

    return xml;
}

void IOXmlReader::readUnknownChildren(IOBase *io)
{
    if (!io)
    {
        skipCurrentElement();
        return;
    }

    while (!atEnd())
    {
        readNext();

        if (isEndElement())
            break;

        if (isStartElement())
            io->addUnknownXml(readRawElement());
    }
}

void IOXmlReader::readRoom()
{
    string _name = attributes().value("name").toString().toUtf8().data();
    string _type = attributes().value("type").toString().toUtf8().data();
    string _hits = attributes().value("hits").toString().toUtf8().data();
    // from_string() leaves the target untouched on an empty value; 0 is what
    // the server uses for an absent or unparsable attribute.
    int hits = 0;
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
            if (name() == QStringLiteral("input"))
                readInput(room);
            else if (name() == QStringLiteral("output"))
                readOutput(room);
            else if (name() == QStringLiteral("camera"))
                readCamera(room);
            else if (name() == QStringLiteral("internal"))
                readInput(room);
            else if (name() == QStringLiteral("audio"))
                readAudio(room);
            else if (name() == QStringLiteral("avr"))
                readAVR(room);
            else if (name() == QStringLiteral("remote_ui"))
                readOutput(room); // RemoteUI is treated as an output
            else if (name() == QStringLiteral("device"))
                readDevice(room);
            else
                room->addUnknownXml(readRawElement());
        }
    }
}

void IOXmlReader::readDevice(Room *room)
{
    Params p;

    for (int i = 0;i < attributes().size();i++)
    {
        QXmlStreamAttribute attr = attributes().at(i);
        p.Add(attr.name().toString().toUtf8().data(),
              attr.value().toString().toUtf8().data());
    }

    if (ListeRoom::Instance().get_input(p["id"]) || ListeRoom::Instance().get_output(p["id"]))
    {
        QMessageBox::critical(nullptr, "Calaos Installer", QObject::tr("Device with id \"%1\" already exists!").arg(QString(p["id"].c_str())));
    }

    Device *dev = ListeRoom::Instance().createDevice(p, room);

    while (!atEnd())
    {
        readNext();

        if (isEndElement())
            break;

        if (!isStartElement())
            continue;

        if (name() == QStringLiteral("input") || name() == QStringLiteral("internal"))
            dev->adoptIO(readInput(room));
        else if (name() == QStringLiteral("output"))
            dev->adoptIO(readOutput(room));
        else if (name() == QStringLiteral("link"))
        {
            dev->links().push_back({ attributes().value("name").toString().toStdString(),
                                     attributes().value("io").toString().toStdString(),
                                     attributes().value("command_io").toString().toStdString() });
            skipCurrentElement();
        }
        else if (name() == QStringLiteral("param"))
        {
            dev->deviceParams().emplace_back(attributes().value("name").toString().toStdString(),
                                             attributes().value("value").toString().toStdString());
            skipCurrentElement();
        }
        else
            dev->addUnknownXml(readRawElement());
    }
}

IOBase *IOXmlReader::readInput(Room *room)
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
        QMessageBox::critical(nullptr, "Calaos Installer", QObject::tr("Input with id \"%1\" already exists!").arg(QString(p["id"].c_str())));
    }

    IOBase *in = ListeRoom::Instance().createInput(p, room);

    if (in->get_gui_type() == "time_range")
    {
        // bitset<12>("") is all zeroes: an absent attribute must not
        // disable every month, the server reads it as all months active.
        bool hasMonths = p.Exists("months");
        string m = p["months"];
        in->get_params().Delete("months");
        std::reverse(m.begin(), m.end());
        try
        {
            in->range_months = hasMonths ? bitset<12>(m) : bitset<12>().set();
        }
        catch(...)
        {
            in->range_months.set(); //set all months by default
        }
    }

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
            if (p["type"] == "InPlageHoraire" && name() == QStringLiteral("lundi"))
                readPlageDay(in->range_monday);
            else if (p["type"] == "InPlageHoraire" && name() == QStringLiteral("mardi"))
                readPlageDay(in->range_tuesday);
            else if (p["type"] == "InPlageHoraire" && name() == QStringLiteral("mercredi"))
                readPlageDay(in->range_wednesday);
            else if (p["type"] == "InPlageHoraire" && name() == QStringLiteral("jeudi"))
                readPlageDay(in->range_thursday);
            else if (p["type"] == "InPlageHoraire" && name() == QStringLiteral("vendredi"))
                readPlageDay(in->range_friday);
            else if (p["type"] == "InPlageHoraire" && name() == QStringLiteral("samedi"))
                readPlageDay(in->range_saturday);
            else if (p["type"] == "InPlageHoraire" && name() == QStringLiteral("dimanche"))
                readPlageDay(in->range_sunday);
            else
                in->addUnknownXml(readRawElement());
        }
    }

    return in;
}

void IOXmlReader::readPlageDay(vector<TimeRange> &day)
{
    while (!atEnd())
    {
        readNext();

        if (isEndElement())
            break;

        if (isStartElement())
        {
            if (name() == QStringLiteral("plage"))
            {
                TimeRange h;
                readPlage(h);
                day.push_back(h);
            }
            else
                skipCurrentElement();
        }
    }
}

void IOXmlReader::readPlage(TimeRange &horaire)
{
    for (int i = 0;i < attributes().size();i++)
    {
        QXmlStreamAttribute attr = attributes().at(i);
        if (attr.name() == QStringLiteral("start_hour"))
            horaire.shour = attr.value().toString().toUtf8().data();
        if (attr.name() == QStringLiteral("start_min"))
            horaire.smin = attr.value().toString().toUtf8().data();
        if (attr.name() == QStringLiteral("start_sec"))
            horaire.ssec = attr.value().toString().toUtf8().data();
        if (attr.name() == QStringLiteral("start_type"))
            horaire.start_type = attr.value().toString().toInt();
        if (attr.name() == QStringLiteral("start_offset"))
            horaire.start_offset = attr.value().toString().toInt();

        if (attr.name() == QStringLiteral("end_hour"))
            horaire.ehour = attr.value().toString().toUtf8().data();
        if (attr.name() == QStringLiteral("end_min"))
            horaire.emin = attr.value().toString().toUtf8().data();
        if (attr.name() == QStringLiteral("end_sec"))
            horaire.esec = attr.value().toString().toUtf8().data();
        if (attr.name() == QStringLiteral("end_type"))
            horaire.end_type = attr.value().toString().toInt();
        if (attr.name() == QStringLiteral("end_offset"))
            horaire.end_offset = attr.value().toString().toInt();
    }

    skipCurrentElement();
}

IOBase *IOXmlReader::readOutput(Room *room)
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
        QMessageBox::critical(nullptr, "Calaos Installer", QObject::tr("Output with id \"%1\" already exists!").arg(QString(p["id"].c_str())));
    }

    IOBase *output = ListeRoom::Instance().createOutput(p, room);

    if (!output)
    {
        skipCurrentElement();
        return nullptr;
    }

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

    if (p["type"] != "RemoteUI")
    {
        readUnknownChildren(output);
        return output;
    }

    QString pagesXml;
    while (!atEnd())
    {
        readNext();

        if (isEndElement())
            break;

        if (isStartElement())
        {
            if (name() == QStringLiteral("pages"))
                pagesXml = readRemoteUIPagesElement();
            else
                output->addUnknownXml(readRawElement());
        }
    }

    output->setRemoteUIPagesXml(pagesXml);
    return output;
}

QString IOXmlReader::readRemoteUIPagesElement()
{
    // Reads the content of <calaos:pages> element and returns it as a raw XML string
    QString xml;
    QXmlStreamWriter writer(&xml);
    writer.setAutoFormatting(true);

    writer.writeStartElement("http://www.calaos.fr", "pages");

    int depth = 1;
    while (!atEnd() && depth > 0)
    {
        readNext();

        if (isStartElement())
        {
            depth++;
            writer.writeStartElement(namespaceUri().toString(), name().toString());

            // Write all attributes
            for (const QXmlStreamAttribute &attr : attributes())
            {
                writer.writeAttribute(attr);
            }
        }
        else if (isEndElement())
        {
            depth--;
            if (depth > 0)
            {
                writer.writeEndElement();
            }
        }
        else if (isCharacters() && !isWhitespace())
        {
            writer.writeCharacters(text().toString());
        }
    }

    writer.writeEndElement(); // close pages

    return xml;
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

    readUnknownChildren(ListeRoom::Instance().createCamera(p, room));
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

    readUnknownChildren(ListeRoom::Instance().createAudio(p, room));
}

void IOXmlReader::readAVR(Room *room)
{
    Params p;

    for (int i = 0;i < attributes().size();i++)
    {
        QXmlStreamAttribute attr = attributes().at(i);
        p.Add(attr.name().toString().toUtf8().data(),
              attr.value().toString().toUtf8().data());
    }

    readUnknownChildren(ListeRoom::Instance().createAVR(p, room));
}

bool ProjectManager::saveIOsToFile(QString &file)
{
    if (ListeRoom::Instance().isReadOnly())
        return false;

    QFile t(file);
    if (!t.open(QIODevice::WriteOnly))
        return false;

    IOXmlWriter xmlfile;
    xmlfile.writeFile(&t);

    t.close();

    return true;
}

bool ProjectManager::saveRulesToFile(QString &file)
{
    if (ListeRoom::Instance().isReadOnly())
        return false;

    QFile t(file);
    if (!t.open(QIODevice::WriteOnly))
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
    bool ok = xmlfile.readFile(&t);

    t.close();

    return ok;
}

/*
 * Handling of IOs that rules.xml references but io.xml does not define.
 *
 * Historically such a reference was simply dropped while reading rules.xml.
 * Because the installer always rewrites rules.xml in full from its in-memory
 * model, merely opening a project whose io.xml and rules.xml are out of sync
 * and saving it destroyed, without a word, every condition and every action
 * that used the unresolved id.
 *
 * Instead we now build a placeholder IO carrying the id. It is deliberately
 * NOT registered in any Room, so it never reaches io.xml and never shows up in
 * the IO tree; it only keeps the reference alive so that rules.xml round-trips
 * unchanged. The user is told about it once, at the end of the load.
 */
namespace
{

map<string, IOBase *> missingIOCache;
//id -> names of the rules referencing it
map<string, QStringList> missingIOUsage;

void resetMissingIOs()
{
    //The placeholders are intentionally not deleted: rules loaded previously
    //may still point at them, and they are a handful of small objects.
    missingIOCache.clear();
    missingIOUsage.clear();
    ListeRoom::clearPendingIOIds();
}

IOBase *getMissingIO(const string &id, int ioType, const string &ruleName)
{
    if (id.empty())
        return nullptr;

    string key = (ioType == IOBase::IO_INPUT? "in:": "out:") + id;

    QStringList &usage = missingIOUsage[key];
    QString rname = QString::fromUtf8(ruleName.c_str());
    if (!rname.isEmpty() && !usage.contains(rname))
        usage.append(rname);

    auto it = missingIOCache.find(key);
    if (it != missingIOCache.end())
        return it->second;

    Params p;
    p.Add("id", id);
    p.Add("type", "unknown");
    p.Add("name", QObject::tr("Missing IO (%1)").arg(QString::fromUtf8(id.c_str())).toUtf8().data());

    IOBase *io = new IOBase(p, "unknown", TSTRING, ioType);
    missingIOCache[key] = io;

    //Do not hand this id out again to a brand new IO while a rule still uses it.
    ListeRoom::reservePendingIOId(id);

    qWarning() << "rules.xml references an unknown"
               << (ioType == IOBase::IO_INPUT? "input": "output") << "id" << id.c_str()
               << "- keeping the reference instead of dropping it";

    return io;
}

}

/* Reported by the caller once the project is fully loaded, so that the warning
 * does not fight with the modal progress dialog shown during the load. */
QStringList ProjectManager::missingIOReport()
{
    QStringList lines;
    int shown = 0;
    for (const auto &it: missingIOUsage)
    {
        if (shown >= 20)
        {
            lines.append(QObject::tr("... and %1 more (the full list is written to the "
                                     "application log)").arg((int)missingIOUsage.size() - shown));
            break;
        }
        //Keys are "in:<id>" / "out:<id>": say which side the id is missing from,
        //an input and an output may legitimately share the same id.
        QString key = QString::fromUtf8(it.first.c_str());
        QString side = key.startsWith("in:")? QObject::tr("input"): QObject::tr("output");
        lines.append(QString("• %1 %2 (%3)")
                     .arg(side)
                     .arg(key.section(':', 1))
                     .arg(it.second.join(", ")));
        shown++;
    }

    return lines;
}

QString ProjectManager::readOnlyReason()
{
    if (!ListeRoom::Instance().isReadOnly())
        return QString();

    return QObject::tr("This project uses io.xml format \"%1\", and this version of Calaos "
                       "Installer only knows up to format %2. It is opened read-only: saving "
                       "it would drop what this version cannot represent. Update Calaos "
                       "Installer to modify this project.")
            .arg(ListeRoom::Instance().getLoadedFormat())
            .arg(ListeRoom::IO_FORMAT_SUPPORTED);
}

bool ProjectManager::loadRulesFromFile(QString &file)
{
    resetMissingIOs();

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
        Params p;
        for (int i = 0;i < node_rule.attributes().size();i++)
        {
            QDomNode attr = node_rule.attributes().item(i);
            QDomAttr dattr = attr.toAttr();
            p.Add(dattr.name().toUtf8().data(),
                  dattr.value().toUtf8().data());
        }

        Rule *rule = new Rule(p);
        ListeRule::Instance().Add(rule);

        //Read conditions
        QDomElement node_cond = node_rule.firstChildElement("calaos:condition");
        while (!node_cond.isNull())
        {
            Condition *cond = nullptr;
            string cond_type = node_cond.attribute("type").toUtf8().data();

            if (cond_type == "standard" || cond_type == "")
            {
                cond = new Condition(COND_STD);
                string cond_trig = node_cond.attribute("trigger").toUtf8().data();

                if (cond_trig == "false")
                    cond->setTrigger(false);

                QDomElement node_in = node_cond.firstChildElement("calaos:input");
                while(!node_in.isNull())
                {
                    string id = node_in.attribute("id").toUtf8().data();
                    string oper = node_in.attribute("oper").toUtf8().data();
                    string val = node_in.attribute("val").toUtf8().data();
                    string val_var = node_in.attribute("val_var").toUtf8().data();

                    IOBase *input = ListeRoom::Instance().get_input(id);
                    if (!input)
                        input = getMissingIO(id, IOBase::IO_INPUT, rule->get_name());

                    if (input)
                    {
                        id = input->get_param("id"); //set id back to "id" parameter if old iid/oid where used
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
            string cond_trig = node_cond.attribute("trigger").toUtf8().data();

                if (cond_trig == "false")
                    cond->setTrigger(false);

                QDomElement node_in = node_cond.firstChildElement("calaos:output");
                while(!node_in.isNull())
                {
                    string id = node_in.attribute("id").toUtf8().data();
                    string oper = node_in.attribute("oper").toUtf8().data();
                    string val = node_in.attribute("val").toUtf8().data();
                    string val_var = node_in.attribute("val_var").toUtf8().data();

                    IOBase *output = ListeRoom::Instance().get_output(id);
                    if (!output)
                        output = getMissingIO(id, IOBase::IO_OUTPUT, rule->get_name());

                    if (output)
                    {
                        id = output->get_param("id"); //set id back to "id" parameter if old iid/oid where used
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
                        if (!input)
                            input = getMissingIO(id, IOBase::IO_INPUT, rule->get_name());
                        if (input)
                        {
                            id = input->get_param("id"); //set id back to "id" parameter if old iid/oid where used
                            cond->addScriptInput(input);
                        }
                    }
                    else if (node_in.tagName() == "calaos:script")
                    {
                        QDomCDATASection data = node_in.firstChild().toCDATASection();
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
            Action *action = nullptr;
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
                    if (!output)
                        output = getMissingIO(id, IOBase::IO_OUTPUT, rule->get_name());

                    if (output)
                    {
                        id = output->get_param("id"); //set id back to "id" parameter if old iid/oid where used
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

                if (action->getTouchscreenAction() == "view_camera")
                    action->setTouchscreenCamera(node_action.attribute("camera").toUtf8().data());
            }
            else if (action_type == "push")
            {
                action = new Action(ACTION_PUSH);

                QDomElement node_mail = node_action.firstChildElement("calaos:push");
                string mail_attachment = node_mail.attribute("attachment").toUtf8().data();

                action->setMailAttachment(mail_attachment);

                QDomCDATASection data = node_mail.firstChild().toCDATASection();
                action->setMailMessage(data.data().toUtf8().data());
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

#include "DialogScriptEditor.h"
#include "ui_DialogScriptEditor.h"
#include "ListeRoom.h"
#include "dialogioproperties.h"

using namespace Calaos;

DialogScriptEditor::DialogScriptEditor(QString script, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogScriptEditor)
{
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    ui->setupUi(this);

    codeEditor = new CodeEditor(this);
    codeEditor->setPlainText(script);

    ui->logEdit->setStyleSheet("font: 10pt \"Courier New\";");

    ui->contentVLayout->insertWidget(0, codeEditor);
    ui->splitter->setStretchFactor(0, 2);
    ui->splitter->setStretchFactor(1, 1);

    connect(&LuaPrinter::Instance(), SIGNAL(print(QString)), this, SLOT(print_message(QString)));

    QStringList headers;
    headers << tr("Home") << "id";
    ui->homeTree->setHeaderLabels(headers);
    ui->homeTree->setUpdatesEnabled(false);

    for (int i = 0;i < ListeRoom::Instance().size();i++)
    {
        Room *room = ListeRoom::Instance().get_room(i);
        bool selected = false;
        if (i == 0) selected = true;
        QTreeWidgetItemRoom *iroom = addItemRoom(room, selected);

        for (int j = 0;j < room->get_size_in();j++)
        {
            IOBase *in = room->get_input(j);

            if (in->get_gui_type() == "switch" ||
                in->get_gui_type() == "switch3" ||
                in->get_gui_type() == "switch_long" ||
                in->get_gui_type() == "time" ||
                in->get_gui_type() == "time_range" ||
                in->get_gui_type() == "temp" ||
                in->get_gui_type() == "analog_in" ||
                in->get_gui_type() == "string_in")
            {
                addItemInput(in, iroom, false);
            }

            QCoreApplication::processEvents();
        }

        for (int j = 0;j < room->get_size_out();j++)
        {
            IOBase *out = room->get_output(j);
            addItemOutput(out, iroom, false);
        }
    }

    ui->homeTree->expandAll();

    ui->homeTree->setUpdatesEnabled(true);
    ui->homeTree->resizeColumnToContents(0);
}

DialogScriptEditor::~DialogScriptEditor()
{
    delete ui;
}

QString DialogScriptEditor::getIconFromRoom(Room *room)
{
    QString icon;

    if (room->get_type() == "salon" || room->get_type() == "lounge")
        icon = ":/img/rooms/lounge_small.png";
    else if (room->get_type() == "sdb" || room->get_type() == "bathroom")
        icon = ":/img/rooms/bathroom_small.png";
    else if (room->get_type() == "chambre" || room->get_type() == "bedroom")
        icon = ":/img/rooms/bedroom_small.png";
    else if (room->get_type() == "cave" || room->get_type() == "cellar")
        icon = ":/img/rooms/cellar_small.png";
    else if (room->get_type() == "couloir" || room->get_type() == "hall" || room->get_type() == "corridor")
        icon = ":/img/rooms/corridor_small.png";
    else if (room->get_type() == "sam" || room->get_type() == "diningroom")
        icon = ":/img/rooms/diningroom_small.png";
    else if (room->get_type() == "garage")
        icon = ":/img/rooms/garage_small.png";
    else if (room->get_type() == "cuisine" || room->get_type() == "kitchen")
        icon = ":/img/rooms/kitchen_small.png";
    else if (room->get_type() == "bureau" || room->get_type() == "office")
        icon = ":/img/rooms/office_small.png";
    else if (room->get_type() == "exterieur" || room->get_type() == "outside")
        icon = ":/img/rooms/outside_small.png";
    else if (room->get_type() == "misc" || room->get_type() == "divers" || room->get_type() == "various")
        icon = ":/img/rooms/various_small.png";
    else
        icon = ":/img/rooms/various_small.png";

    return icon;
}

QTreeWidgetItemRoom *DialogScriptEditor::addItemRoom(Room *room, bool selected)
{
    QTreeWidgetItemRoom *item = new QTreeWidgetItemRoom(room, ui->homeTree);
    item->setSizeHint(0, QSize(24, 24));

    if (selected) ui->homeTree->setCurrentItem(item);

    item->setData(0, Qt::DisplayRole, QString::fromUtf8(room->get_name().c_str()));
    item->setData(0, Qt::DecorationRole, QIcon(getIconFromRoom(room)));

    QString s = QString::fromUtf8(room->get_name().c_str());
    s += " (" + QString::fromUtf8(room->get_type().c_str()) + ")";
    item->setData(0, Qt::ToolTipRole, s);
    item->setData(0, Qt::StatusTipRole, s);

    return item;
}

QTreeWidgetItemInput *DialogScriptEditor::addItemInput(IOBase *in, QTreeWidgetItemRoom *parent, bool selected)
{
    QTreeWidgetItemInput *item = new QTreeWidgetItemInput(in, parent);

    if (selected) ui->homeTree->setCurrentItem(item);
    item->setData(0, Qt::DisplayRole, QString::fromUtf8(in->get_param("name").c_str()));
    string type = in->get_gui_type();
    qDebug() << "Type: " << type.c_str();
    if (type == "switch" ||
        type == "switch3" ||
        type == "switch_long")
        item->setData(0, Qt::DecorationRole, QIcon(":/img/icon_inter.png"));
    else if (type == "temp")
        item->setData(0, Qt::DecorationRole, QIcon(":/img/temp.png"));
    else if (type == "time_range" || type == "time")
        item->setData(0, Qt::DecorationRole, QIcon(":/img/icon_clock.png"));
    else if (type == "analog_in")
        item->setData(0, Qt::DecorationRole, QIcon(":/img/icon_analog.png"));
    else if (type == "string_in")
        item->setData(0, Qt::DecorationRole, QIcon(":/img/text.png"));
    else
        item->setData(0, Qt::DecorationRole, QIcon(":/img/icon_unknown.png"));

    QString s = QString::fromUtf8(in->get_param("name").c_str());
    s += " (" + QString::fromUtf8(in->get_param("type").c_str()) + ")";
    if (type == "WIDigitalBP" || type == "WIDigitalTriple" || type == "WIDigitalLong" ||
        type == "WITemp" || type == "WIAnalog")
        s += " #" + QString::fromUtf8(in->get_param("var").c_str());

    item->setData(0, Qt::ToolTipRole, s);
    item->setData(0, Qt::StatusTipRole, s);

    item->setData(1, Qt::DisplayRole, QString::fromUtf8(in->get_param("id").c_str()));

    return item;
}

QTreeWidgetItemOutput *DialogScriptEditor::addItemOutput(IOBase *out, QTreeWidgetItemRoom *parent, bool selected)
{
    QTreeWidgetItemOutput *item = new QTreeWidgetItemOutput(out, parent);

    if (selected) ui->homeTree->setCurrentItem(item);
    item->setData(0, Qt::DisplayRole, QString::fromUtf8(out->get_param("name").c_str()));

    string type = out->get_gui_type();
    if (type == "light" || type == "light_dimmer" || type == "light_rgb")
        item->setData(0, Qt::DecorationRole, QIcon(":/img/icon_light_on.png"));
    else if (type == "shutter" || type == "shutter_smart")
        item->setData(0, Qt::DecorationRole, QIcon(":/img/icon_shutter.png"));
    else if (type == "scenario")
        item->setData(0, Qt::DecorationRole, QIcon(":/img/icon_scenario.png"));
    else if (type == "timer")
        item->setData(0, Qt::DecorationRole, QIcon(":/img/icon_clock.png"));
    else if (type == "var_bool")
        item->setData(0, Qt::DecorationRole, QIcon(":/img/icon_bool_on.png"));
    else if (type == "var_int")
        item->setData(0, Qt::DecorationRole, QIcon(":/img/icon_int.png"));
    else if (type == "var_string" || type == "string_out" || type == "string_in")
        item->setData(0, Qt::DecorationRole, QIcon(":/img/text.png"));
    else if (type == "audio" || type == "avreceiver")
        item->setData(0, Qt::DecorationRole, QIcon(":/img/icon_sound.png"));
    else if (type == "camera")
        item->setData(0, Qt::DecorationRole, QIcon(":/img/icon_camera_on.png"));
    else if (type == "analog_out")
        item->setData(0, Qt::DecorationRole, QIcon(":/img/icon_analog.png"));
    else
        item->setData(0, Qt::DecorationRole, QIcon(":/img/icon_unknown.png"));

    QString s = QString::fromUtf8(out->get_param("name").c_str());
    s += " (" + QString::fromUtf8(out->get_param("type").c_str()) + ")";
    if (type == "WODigital" || type == "WOAnalog")
        s += " #" + QString::fromUtf8(out->get_param("var").c_str());
    if (type == "WOVolet" || type == "WOVoletSmart")
        s += " #" + QString::fromUtf8(out->get_param("var_up").c_str()) +
             " #" + QString::fromUtf8(out->get_param("var_down").c_str());

    item->setData(0, Qt::ToolTipRole, s);
    item->setData(0, Qt::StatusTipRole, s);

    item->setData(1, Qt::DisplayRole, QString::fromUtf8(out->get_param("id").c_str()));

    return item;
}

void DialogScriptEditor::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

QString DialogScriptEditor::getScript()
{
    return codeEditor->toPlainText();
}

void DialogScriptEditor::on_pushButton_Valid_clicked()
{
    scriptError = false;
    if (Execute())
    {
        QTextCharFormat format = ui->logEdit->currentCharFormat();
        format.setForeground(QBrush(Qt::darkGreen));
        ui->logEdit->setCurrentCharFormat(format);

        QString t(tr("The script was successfully executed."));
        ui->logEdit->appendPlainText(t);
    }
    else
    {
        QTextCharFormat format = ui->logEdit->currentCharFormat();
        format.setForeground(QBrush(Qt::red));
        ui->logEdit->setCurrentCharFormat(format);

        scriptError = true;
        QString t(QString::fromUtf8(ScriptManager::Instance().getErrorMsg().c_str()));
        ui->logEdit->appendPlainText(t);
    }
}

void DialogScriptEditor::on_pushButton_Ok_clicked()
{
    on_pushButton_Valid_clicked();

    if (!scriptError)
        accept();
}

bool DialogScriptEditor::Execute()
{
    ui->logEdit->clear();

    bool v = ScriptManager::Instance().ExecuteScript(codeEditor->toPlainText().toUtf8().data());

    if (!ScriptManager::Instance().hasError())
    {
        QTextCharFormat format = ui->logEdit->currentCharFormat();
        format.setForeground(QBrush(Qt::darkYellow));
        ui->logEdit->setCurrentCharFormat(format);

        if (v)
            ui->logEdit->appendPlainText(tr("The script returns \"true\""));
        else
            ui->logEdit->appendPlainText(tr("The script returns \"false\""));
    }

    return !ScriptManager::Instance().hasError();
}

void DialogScriptEditor::print_message(QString msg)
{
    QTextCharFormat format = ui->logEdit->currentCharFormat();
    format.setForeground(QBrush(Qt::darkCyan));
    ui->logEdit->setCurrentCharFormat(format);

    ui->logEdit->appendPlainText(msg);
}

void DialogScriptEditor::on_homeTree_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    Q_UNUSED(column);
    Params *p = NULL;
    IOBase *io = nullptr;

    QTreeWidgetItemInput *itinput = dynamic_cast<QTreeWidgetItemInput *>(item);
    if (itinput)
    {
        io = itinput->getInput();
        p = &io->get_params();
    }

    QTreeWidgetItemOutput *itoutput = dynamic_cast<QTreeWidgetItemOutput *>(item);
    if (itoutput)
    {
        io = itoutput->getOutput();
        p = &io->get_params();
    }

    if (!p)
        return;

    DialogIOProperties dialog(io, *p, this);
    dialog.exec();
}

void DialogScriptEditor::on_homeTree_customContextMenuRequested(const QPoint &pos)
{
    QTreeWidgetItem *item = ui->homeTree->itemAt(pos);
    qDebug() << "custom context menu";
    if (!item)
        return;

    QTreeWidgetItemOutput *itoutput = dynamic_cast<QTreeWidgetItemOutput *>(item);
    QTreeWidgetItemInput *itinput = dynamic_cast<QTreeWidgetItemInput *>(item);

    if (!itoutput && !itinput)
        return;

    QMenu item_menu(ui->homeTree);
    QAction *action = NULL;

    action = item_menu.addAction(tr("Properties"));
    action->setIcon(QIcon(":/img/document-properties.png"));
    connect(action, &QAction::triggered, [=]()
    {
        on_homeTree_itemDoubleClicked(item, 0);
    });

    item_menu.exec(QCursor::pos());
}

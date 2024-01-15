#include "dialognewroom.h"
#include "ui_DialogNewRoom.h"

DialogNewRoom::DialogNewRoom(QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::DialogNewRoom)
{
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    m_ui->setupUi(this);

    //hide error labels by default.
    m_ui->label_error_exists->hide();
    m_ui->label_error_empty->hide();

    m_ui->edit_name->setFocus();
}

DialogNewRoom::~DialogNewRoom()
{
    delete m_ui;
}

void DialogNewRoom::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type())
    {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void DialogNewRoom::on_buttonBox_accepted()
{
    if (m_ui->edit_name->text().isEmpty())
    {
        m_ui->label_error_empty->show();
        return;
    }

    string name, type;

    name = m_ui->edit_name->text().toUtf8().data();
    type = ListeRoom::getRoomType(m_ui->list_type->currentRow());

    if (ListeRoom::Instance().searchRoomByName(name, type) != NULL)
    {
        m_ui->label_error_empty->hide();
        m_ui->label_error_exists->show();
        return;
    }

    ListeRoom::Instance().Add(new Room(name, type));

    accept();
}

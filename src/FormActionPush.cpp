#include "FormActionPush.h"
#include "ui_FormActionPush.h"

#include "formrules.h"
#include "DialogMailMessage.h"

FormActionPush::FormActionPush(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormActionPush)
{
    ui->setupUi(this);

    setWindowFlags(Qt::Popup);
    setFocusPolicy(Qt::NoFocus);
    setFocusProxy(parent);
    setMouseTracking(true);
}

FormActionPush::~FormActionPush()
{
    delete ui;
}

void FormActionPush::setAction(QTreeWidgetItem *item, Rule *_rule, Action *_action)
{
    rule = _rule;
    action = _action;
    qitem = item;

    ui->labelRuleName->setText(QString::fromUtf8(rule->get_name().c_str()));
    ui->labelRuleType->setText(QString::fromUtf8(rule->get_type().c_str()));

    setDone = false;
    ui->comboCamera->clear();
    ui->comboCamera->addItem(tr("<None>"), QString::fromUtf8(""));
    ui->comboCamera->setCurrentIndex(0);
    for (int i = 0;i < ListeRoom::Instance().get_nb_output();i++)
    {
        IOBase *camera = ListeRoom::Instance().get_output(i);
        if (camera->get_gui_type() == "camera")
        {
            ui->comboCamera->addItem(QString::fromUtf8(camera->get_param("name").c_str()),
                                     QString::fromUtf8(camera->get_param("id").c_str()));

            if (camera->get_param("id") == action->getMailAttachment())
                ui->comboCamera->setCurrentIndex(ui->comboCamera->count() - 1);
        }
    }
    setDone = true;
}

void FormActionPush::on_comboCamera_currentIndexChanged(int index)
{
    if (!setDone) return;

    action->setMailAttachment(ui->comboCamera->itemData(index).toString().toUtf8().data());
}

void FormActionPush::on_btEditMsg_clicked()
{
    DialogMailMessage dm(QString::fromUtf8(action->getMailMessage().c_str()));

    if (dm.exec() == QDialog::Accepted)
    {
        action->setMailMessage(dm.getMessage().toUtf8().data());
    }
}

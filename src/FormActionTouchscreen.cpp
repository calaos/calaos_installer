#include "FormActionTouchscreen.h"
#include "ui_FormActionTouchscreen.h"

#include "formrules.h"
#include "DialogMailMessage.h"

FormActionTouchscreen::FormActionTouchscreen(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormActionTouchscreen),
    rule(NULL), action(NULL)
{
    ui->setupUi(this);

    setWindowFlags(Qt::Popup);
    setFocusPolicy(Qt::NoFocus);
    setFocusProxy(parent);
    setMouseTracking(true);
}

FormActionTouchscreen::~FormActionTouchscreen()
{
    delete ui;
}

void FormActionTouchscreen::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type())
    {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void FormActionTouchscreen::setAction(QTreeWidgetItem *item, Rule *_rule, Action *_action)
{
    rule = _rule;
    action = _action;
    qitem = item;

    setDone = false;

    ui->labelRuleName->setText(QString::fromUtf8(rule->get_name().c_str()));
    ui->labelRuleType->setText(QString::fromUtf8(rule->get_type().c_str()));

    //ui->editAction->setText(QString::fromUtf8(action->getTouchscreenAction().c_str()));

    ui->comboBoxAction->clear();
    ui->comboBoxCamera->clear();

    ui->comboBoxAction->addItem(tr("Display Camera"), QString("view_camera"));
    ui->comboBoxAction->setCurrentIndex(0);

    ui->comboBoxCamera->addItem(tr("<None>"), QString::fromUtf8(""));
    ui->comboBoxCamera->setCurrentIndex(0);
    for (int i = 0;i < ListeRoom::Instance().get_nb_output();i++)
    {
        IOBase *camera = ListeRoom::Instance().get_output(i);
        if (camera->get_gui_type() == "camera")
        {
            ui->comboBoxCamera->addItem(QString::fromUtf8(camera->get_param("name").c_str()),
                                        QString::fromUtf8(camera->get_param("id").c_str()));

            if (camera->get_param("id") == action->getTouchscreenCamera())
                ui->comboBoxCamera->setCurrentIndex(ui->comboBoxCamera->count() - 1);
        }
    }

    setDone = true;
}

void FormActionTouchscreen::on_comboBoxAction_currentIndexChanged(int)
{
    updateAction();
}

void FormActionTouchscreen::on_comboBoxCamera_currentIndexChanged(int)
{
    updateAction();
}

void FormActionTouchscreen::updateAction()
{
    if (!setDone) return;

    QString textAct = ui->comboBoxAction->itemData(ui->comboBoxAction->currentIndex()).toString();
    QString textCam = ui->comboBoxCamera->itemData(ui->comboBoxCamera->currentIndex()).toString();

    action->setTouchscreenAction(textAct.toUtf8().data());
    action->setTouchscreenCamera(textCam.toUtf8().data());

    FormRules::updateItemAction(qitem, action);
}

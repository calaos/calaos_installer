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

        ui->editAction->setText(QString::fromUtf8(action->getTouchscreenAction().c_str()));

        setDone = true;
}

void FormActionTouchscreen::on_editAction_textChanged(QString text)
{
        if (!setDone) return;

        action->setTouchscreenAction(text.toUtf8().data());
}

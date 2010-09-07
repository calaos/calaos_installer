#include "FormConditionScript.h"
#include "ui_FormConditionScript.h"

#include "DialogScriptEditor.h"
#include "DialogInputEventScript.h"
#include "formrules.h"

FormConditionScript::FormConditionScript(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::FormConditionScript),
        rule(NULL), condition(NULL)
{
        ui->setupUi(this);

        setWindowFlags(Qt::Popup);
        setFocusPolicy(Qt::NoFocus);
        setFocusProxy(parent);
        setMouseTracking(true);
}

FormConditionScript::~FormConditionScript()
{
        delete ui;
}

void FormConditionScript::changeEvent(QEvent *e)
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

void FormConditionScript::setCondition(QTreeWidgetItem *item, Rule *_rule, Condition *_cond)
{
        rule = _rule;
        condition = _cond;
        qitem = item;

        ui->labelRuleName->setText(QString::fromUtf8(rule->get_name().c_str()));
        ui->labelRuleType->setText(QString::fromUtf8(rule->get_type().c_str()));
}

void FormConditionScript::on_btEditScript_clicked()
{
        DialogScriptEditor ds(QString::fromUtf8(condition->getScript().c_str()));

        if (ds.exec() == QDialog::Accepted)
        {
                condition->setScript(ds.getScript().toUtf8().data());
        }
}

void FormConditionScript::on_btEditInput_clicked()
{
        if (!condition)
                return;

        DialogInputEventScript d(condition);

        d.exec();
}

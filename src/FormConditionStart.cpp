#include "FormConditionStart.h"
#include "ui_FormConditionStart.h"

#include "formrules.h"

FormConditionStart::FormConditionStart(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::FormConditionStart),
        rule(NULL), condition(NULL)
{
        ui->setupUi(this);

        setWindowFlags(Qt::Popup);
        setFocusPolicy(Qt::NoFocus);
        setFocusProxy(parent);
        setMouseTracking(true);
}

FormConditionStart::~FormConditionStart()
{
        delete ui;
}

void FormConditionStart::changeEvent(QEvent *e)
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

void FormConditionStart::setCondition(QTreeWidgetItem *item, Rule *_rule, Condition *_condition)
{
        rule = _rule;
        condition = _condition;
        qitem = item;

        ui->labelRuleName->setText(QString::fromUtf8(rule->get_name().c_str()));
        ui->labelRuleType->setText(QString::fromUtf8(rule->get_type().c_str()));
}

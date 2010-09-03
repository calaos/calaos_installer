#include "FormActionScript.h"
#include "ui_FormActionScript.h"

#include "DialogScriptEditor.h"
#include "formrules.h"

FormActionScript::FormActionScript(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::FormActionScript),
        rule(NULL), action(NULL)
{
        ui->setupUi(this);

        setWindowFlags(Qt::Popup);
        setFocusPolicy(Qt::NoFocus);
        setFocusProxy(parent);
        setMouseTracking(true);
}

FormActionScript::~FormActionScript()
{
        delete ui;
}

void FormActionScript::changeEvent(QEvent *e)
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

void FormActionScript::setAction(QTreeWidgetItem *item, Rule *_rule, Action *_action)
{
        rule = _rule;
        action = _action;
        qitem = item;
}

void FormActionScript::on_btEditScript_clicked()
{
        DialogScriptEditor ds(QString::fromUtf8(action->getScript().c_str()));

        if (ds.exec() == QDialog::Accepted)
        {
                action->setScript(ds.getScript().toUtf8().data());
        }
}

#include "DialogScriptEditor.h"
#include "ui_DialogScriptEditor.h"

using namespace Calaos;

DialogScriptEditor::DialogScriptEditor(QString script, QWidget *parent) :
                QDialog(parent),
                ui(new Ui::DialogScriptEditor)
{
        ui->setupUi(this);

        codeEditor = new CodeEditor(this);
        codeEditor->setPlainText(script);

        ui->contentVLayout->insertWidget(0, codeEditor);
}

DialogScriptEditor::~DialogScriptEditor()
{
        delete ui;
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
                QString t(QString::fromUtf8("Le script s'est executé avec succès."));
                ui->logEdit->appendPlainText(t);
        }
        else
        {
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
                if (v)
                        ui->logEdit->appendPlainText(QString::fromUtf8("Le script a renvoyé \"true\""));
                else
                        ui->logEdit->appendPlainText(QString::fromUtf8("Le script a renvoyé \"false\""));
        }

        return !ScriptManager::Instance().hasError();
}

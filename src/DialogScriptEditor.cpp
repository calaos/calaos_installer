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

        connect(&LuaPrinter::Instance(), SIGNAL(print(QString)), this, SLOT(on_print_message(QString)));
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

void DialogScriptEditor::on_print_message(QString msg)
{
        QTextCharFormat format = ui->logEdit->currentCharFormat();
        format.setForeground(QBrush(Qt::darkCyan));
        ui->logEdit->setCurrentCharFormat(format);

        ui->logEdit->appendPlainText(QString("Lua print: ") + msg);
}

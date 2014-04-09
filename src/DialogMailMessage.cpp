#include "DialogMailMessage.h"
#include "ui_DialogMailMessage.h"

DialogMailMessage::DialogMailMessage(QString msg, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogMailMessage)
{
    ui->setupUi(this);

    ui->textEdit->setPlainText(msg);
}

DialogMailMessage::~DialogMailMessage()
{
    delete ui;
}

void DialogMailMessage::changeEvent(QEvent *e)
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

QString DialogMailMessage::getMessage()
{
    return ui->textEdit->toPlainText();
}

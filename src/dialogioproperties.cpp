#include "dialogioproperties.h"
#include "ui_DialogIoProperties.h"

DialogIOProperties::DialogIOProperties(const Params &p, bool editable, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogIOProperties)
{
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    ui->setupUi(this);

    ioWidget = new WidgetIOProperties(p, editable, this);
    ui->verticalLayout->insertWidget(0, ioWidget);

    if (!editable)
        ui->buttonBox->setStandardButtons(QDialogButtonBox::Close);
}

DialogIOProperties::DialogIOProperties(IOBase *io, bool editable, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogIOProperties)
{
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    ui->setupUi(this);

    ioWidget = new WidgetIOProperties(io->get_params(), editable, this);
    ui->verticalLayout->insertWidget(0, ioWidget);

    if (!editable)
        ui->buttonBox->setStandardButtons(QDialogButtonBox::Close);
}

DialogIOProperties::~DialogIOProperties()
{
    delete ui;
}

void DialogIOProperties::on_buttonBox_accepted()
{
    accept();
}

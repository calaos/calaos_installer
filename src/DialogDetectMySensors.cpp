#include "DialogDetectMySensors.h"
#include "ui_DialogDetectMySensors.h"
#include "MySensors.h"
#include <algorithm>
#include <QMessageBox>

#define SEARCH_TIMEOUT    500

using namespace std;

DialogDetectMySensors::DialogDetectMySensors(const Params &parentParams, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogDetectMySensors),
    parentParams(parentParams)
{
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    ui->setupUi(this);
}

DialogDetectMySensors::~DialogDetectMySensors()
{
    if(tcpSocket!=nullptr) delete tcpSocket;
    delete ui;
}

void DialogDetectMySensors::changeEvent(QEvent *e)
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

void DialogDetectMySensors::on_buttonDiscover_clicked()
{
    if (ui->lineEditPort->text().size() == 0)
    {
        QMessageBox::warning(
            this,
            tr("Missing port field"),
            tr("Port must be specified (/dev/ttyxxx for Serial or a number (eg 12345) for TCP).") );
        return;
    }
    if (ui->comboBoxGateway->currentText() == "TCP" &&
            ui->lineEditHost->text().size() == 0)
    {
        QMessageBox::warning(
            this,
            tr("Missing host field"),
            tr("Host must be specified when using TCP.") );
        return;
    }

    if (searchSensors() != 0)
    {
        QMessageBox::warning(
            this,
            tr("Failed to connect to server"),
            tr("Error while connecting to host") );
        return;
    }
}

Params& DialogDetectMySensors::getParams()
{
    QList<QTreeWidgetItem *> qList = ui->treeMySensors->selectedItems();
    QString sPort = ui->lineEditPort->text();
    QString sHost = ui->lineEditHost->text();
    QString sGateway = ui->comboBoxGateway->currentText();

    if(qList.count() > 0)
    {
        params.Add("node_id", qList[0]->text(0).toStdString());
        params.Add("sensor_id", qList[0]->text(1).toStdString());
        params.Add("name", qList[0]->text(3).toStdString());
        params.Add("gateway", sGateway.toStdString());
        params.Add("port", sPort.toStdString());
        if (sGateway == "TCP")
            params.Add("host", sHost.toStdString());
    }

    return params;
}

int DialogDetectMySensors::searchSensors()
{
    if (timer)
        return 1;

    if (ui->comboBoxGateway->currentText() == "TCP")
    {
        tcpSocket = new QTcpSocket(this);
        if (tcpSocket == nullptr)
            return 1;
    }

    ui->treeMySensors->setEnabled(false);
    ui->buttonBox->setEnabled(false);
    ui->buttonDiscover->setEnabled(false);
    ui->lineEditHost->setEnabled(false);
    ui->lineEditPort->setEnabled(false);
    ui->comboBoxGateway->setEnabled(false);

    ui->treeMySensors->clear();

    if (spinnerLayout) delete spinnerLayout;
    if (spinnerLayoutWidget) delete spinnerLayoutWidget;

    spinnerLayoutWidget = new QWidget(this);
    spinnerLayoutWidget->setGeometry(ui->treeMySensors->geometry());
    spinnerLayout = new QHBoxLayout(spinnerLayoutWidget);
    spinnerLayout->setContentsMargins(0, 0, 0, 0);
    spinnerLayoutWidget->show();

    if (spinner) delete spinner;
    spinner = new QAnimationLabel(":/img/loader.gif", this);
    spinnerLayout->addWidget(spinner, 0, Qt::AlignCenter);
    spinner->start();

    if (ui->comboBoxGateway->currentText() == "TCP")
    {
        tcpSocket->connectToHost(ui->lineEditHost->text(), ui->lineEditPort->text().toInt());
        connect(tcpSocket, &QTcpSocket::readyRead, this, &DialogDetectMySensors::socketReadyRead);
        connect(tcpSocket, &QTcpSocket::connected, this, &DialogDetectMySensors::socketConnected);
        connect(tcpSocket, &QTcpSocket::errorOccurred, this, &DialogDetectMySensors::socketdisplayError);
    }

    return 0;
}

void DialogDetectMySensors::socketdisplayError(QAbstractSocket::SocketError socketError)
{
    switch (socketError) {
    case QAbstractSocket::RemoteHostClosedError:
        break;
    case QAbstractSocket::HostNotFoundError:
        QMessageBox::warning(this, tr("MySensors detection"),
                                 tr("The host was not found. Please check the "
                                    "host name and port settings."));
        break;
    case QAbstractSocket::ConnectionRefusedError:
        QMessageBox::warning(this, tr("MySensors detection"),
                                 tr("The connection was refused by the node."));
        break;
    default:
        QMessageBox::warning(this, tr("MySensors detection"),
                                 tr("The following error occurred: %1.")
                                 .arg(tcpSocket->errorString()));
    }
    enableUI();
}


void DialogDetectMySensors::socketConnected()
{
    int nodeId;

    /* Send presentation request frame */
    for (nodeId = 0; nodeId < 255; nodeId++) {
        QString requestPres = QString::number(nodeId) + ";0;3;0;19;0\n";
        tcpSocket->write(requestPres.toLocal8Bit());
    }

    /* Let start a timer to end the discovery as soon as we are connected */
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &DialogDetectMySensors::timerDone);
    timer->start(SEARCH_TIMEOUT);
}


void DialogDetectMySensors::addSensorToList(QStringList fields)
{
    bool found;

    /* Check invalid messages */
    if (fields[4].toInt() == MySensors::S_ARDUINO_NODE ||
        fields[4].toInt() == MySensors::S_ARDUINO_REPEATER_NODE ||
        fields[0].toInt() == 255 ||
        fields[1].toInt() == 255) {
        return;
    }

    found = (std::find(listSensor.begin(), listSensor.end(), fields) != listSensor.end());
    if (!found)
        listSensor.push_back(fields);
}

void DialogDetectMySensors::socketReadyRead()
{
    QByteArray array = tcpSocket->readAll();
    QString sData = array;
    QStringList nodeAnswers = sData.split("\n");

    foreach (const QString &nodeAnswer, nodeAnswers) {
        QStringList fields = nodeAnswer.split(";");
        /* MySensors messages should always have 6 fields */
        if (fields.count() != 6)
            continue;

        /* This is a presentation */
        if (fields[2].toInt() == (int) MySensors::C_PRESENTATION)
            addSensorToList(fields);
    }
}

void DialogDetectMySensors::enableUI()
{
    ui->treeMySensors->setEnabled(true);
    ui->buttonBox->setEnabled(true);
    ui->buttonDiscover->setEnabled(true);
    ui->lineEditPort->setEnabled(true);
    ui->comboBoxGateway->setEnabled(true);
    if (ui->comboBoxGateway->currentText() == "TCP") {
        ui->lineEditHost->setEnabled(1);
    }
}

void DialogDetectMySensors::timerDone()
{
    delete timer;
    delete spinner;
    delete spinnerLayout;
    delete spinnerLayoutWidget;
    delete tcpSocket;
    std::list<QStringList>::iterator it;

    /* Copy everything from the list to the QTree */
    for (it = listSensor.begin(); it != listSensor.end(); ++it){
        QTreeWidgetItem *item = new QTreeWidgetItem(ui->treeMySensors);
        QStringList sl = *it;
        QString type = QString::fromStdString(MySensors::getSensorString(sl[4].toInt()));
        item->setData(0, Qt::DisplayRole, sl[0]);
        item->setData(1, Qt::DisplayRole, sl[1]);
        item->setData(2, Qt::DisplayRole, type);
        item->setData(3, Qt::DisplayRole, sl[5]);
    }

    listSensor.empty();

    enableUI();

    ui->treeMySensors->resizeColumnToContents(0);
    ui->treeMySensors->resizeColumnToContents(1);
}

void DialogDetectMySensors::on_comboBoxGateway_activated(const QString &arg1)
{
    if (arg1 == "TCP") {
        ui->lineEditHost->setEnabled(1);
        ui->lineEditPort->setText("5003");
    }
}

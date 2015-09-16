#include <QComboBox>

#include "wizardhue.h"
#include "ui_wizardhue.h"

#include "Params.h"
#include "qtreewidget_addition.h"

#define DISCOVER_TIMEOUT 5000
#define RETRY_COUNT 12

WizardHue::WizardHue(Room *room, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WizardHue),
    bridgeFound(false),
    apiKeyFound(false),
    networkAccessManager(new QNetworkAccessManager(this)),
    retry(0)
{
    ui->setupUi(this);
    udpSocket = new QUdpSocket(this);
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(timeout()));
    hueBridgeDiscover();
    timer->start(DISCOVER_TIMEOUT);
    connect(udpSocket, SIGNAL(readyRead()),
               this, SLOT(readPendingDatagrams()));
    ui->stackedWidget->setCurrentWidget(ui->bridgeDetectionPage);

    QHostAddress h1;
    h1.setAddress("192.168.0.1");


    qDebug() << h1.toString();
}

WizardHue::~WizardHue()
{
    delete timer;
    delete udpSocket;
    delete ui;
}

void WizardHue::timeout()
{
    retry++;
    if (retry == RETRY_COUNT)
    {
        timer->stop();
        retry = 0;
        if (!bridgeFound)
            qDebug() << "Bridge not found";
        else if (!apiKeyFound)
            qDebug() << "Api Key not received";
        ui->stackedWidget->setCurrentWidget(ui->errorPage);
        return;
    }
    if (!bridgeFound)
        hueBridgeDiscover();
    if (!apiKeyFound)
        askForApiKey();
}

void WizardHue::hueBridgeDiscover()
{
    QByteArray datagram = "M-SEARCH * HTTP/1.1\r\n"
                          "HOST: 239.255.255.250:1900\r\n"
                          "MAN: \"ssdp:discover\"\r\n"
                          "MX: %1\r\n"
                          "ST: libhue:idl\r\n";
    QHostAddress broadcastAddress = QHostAddress("239.255.255.250");
    udpSocket->writeDatagram(datagram.data(),datagram.size(), broadcastAddress , 1900);
}

void WizardHue::readPendingDatagrams()
{
    while (udpSocket->hasPendingDatagrams())
    {
        QByteArray datagram;
        QHostAddress sender;
        quint16 senderPort;

        datagram.resize(udpSocket->pendingDatagramSize());
        udpSocket->readDatagram(datagram.data(), datagram.size(),
                                &sender, &senderPort);
        if (!bridgeFound)
        {
            QHostAddress ipv4 = QHostAddress(sender.toIPv4Address());
            qDebug() << "found bridge :" <<  ipv4.toString();
            bridgeFound = true;
            bridgeAddress = ipv4;
            askForApiKey();
        }

    }
}

void WizardHue::askForApiKey()
{
    if (!bridgeFound)
        return;

    QNetworkRequest request;
    QVariantMap params;
    params.insert("devicetype", "calaos");
    QJsonDocument jsonDoc = QJsonDocument::fromVariant(params);
    request.setUrl(QUrl("http://" + bridgeAddress.toString() + "/api"));
    QNetworkReply *reply = networkAccessManager->post(request, jsonDoc.toJson());
    connect(reply, SIGNAL(finished()), this, SLOT(apiKeyReceived()));
}

void WizardHue::apiKeyReceived()
{
    QNetworkReply *reply = static_cast<QNetworkReply*>(sender());
    QByteArray response = reply->readAll();
    qDebug() << "create user finished" << response;

    QJsonParseError error;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(response, &error);

    if (error.error != QJsonParseError::NoError) {
        qWarning() << "cannot parse response:" << error.errorString() << response;
        return;
    }
    QVariant rsp = jsonDoc.toVariant();

    QVariantMap map = rsp.toList().first().toMap();
    if (map.contains("error")) {
        QString errorMessage = map.value("error").toMap().value("description").toString();
        qWarning() << "error creating user" << errorMessage;
        return;
    }

    if (!map.contains("success")) {
        qWarning() << "Huh? Neither error nor success contained in response...";
        return;
    }

    apiKey = map.value("success").toMap().value("username").toString();
    qDebug() << apiKey;
    ui->stackedWidget->setCurrentWidget(ui->hueSelectionPage);
    timer->stop();
    askForConfig();
}


void WizardHue::askForConfig()
{
    if (!bridgeFound && !apiKeyFound)
        return;

    QNetworkRequest request;
    request.setUrl(QUrl("http://" + bridgeAddress.toString() + "/api/" + apiKey));
    QNetworkReply *reply = networkAccessManager->get(request);
    connect(reply, SIGNAL(finished()), this, SLOT(configReceived()));

}

void WizardHue::configReceived()
{
    QNetworkReply *reply = static_cast<QNetworkReply*>(sender());
    QByteArray response = reply->readAll();
    QJsonParseError error;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(response, &error);
    QVariantMap var = jsonDoc.toVariant().toMap();

    QVariantMap config = var["lights"].toMap();

    for(auto e : config.keys())
    {
        QVariantMap light = config[e].toMap();
        QString name = light["name"].toString();

        Params param;

        param.Add("hue_id", e.toStdString());
        param.Add("name", name.toStdString());
        param.Add("type", "HueOutputLightYUV");
        IOBase *output = IOFactory::Instance().CreateIO("HueOutputLightYUV", param);

        QTreeWidgetItemOutput *item = new QTreeWidgetItemOutput(output, ui->treeAllHue);
        item->setIcon(0, QIcon(":/img/light_on.png"));
        item->setData(1, Qt::DisplayRole, name);

    }

    for (int i = 0;i < ListeRoom::Instance().size();i++)
    {
        Room *room = ListeRoom::Instance().get_room(i);

        for (int j = 0;j < room->get_size_out();j++)
        {
            IOBase *out = room->get_output(j);

            if (out->get_param("type") != "HueOutputLightYUV")
                continue;
            QTreeWidgetItemOutput *item = new QTreeWidgetItemOutput(out, ui->treeSelectedHue);
            item->setIcon(0, QIcon(":/img/light_on.png"));
            item->setData(1, Qt::DisplayRole, out->get_param("name").c_str());
            QComboBox *comboBox = new QComboBox(ui->treeSelectedHue);
            for (int i = 0;i < ListeRoom::Instance().size();i++)
                comboBox->addItem(ListeRoom::Instance().get_room(i)->get_name().c_str());

            ui->treeSelectedHue->setItemWidget(item, 2, comboBox);
        }
    }


}

void WizardHue::on_buttonAdd_clicked()
{
    QTreeWidgetItemOutput *oldItem = reinterpret_cast<QTreeWidgetItemOutput *>(ui->treeAllHue->currentItem());
    if (oldItem)
    {
        IOBase *out = oldItem->getOutput();
        QTreeWidgetItemOutput *item = new QTreeWidgetItemOutput(oldItem->getOutput(), ui->treeSelectedHue);

        item->setIcon(0, QIcon(":/img/light_on.png"));
        item->setData(1, Qt::DisplayRole, out->get_param("name").c_str());
    }
}

void WizardHue::on_buttonDel_clicked()
{

}

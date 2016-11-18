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
    Q_UNUSED(room);
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

    ui->buttonBox->setVisible(false);
    qDebug() << h1.toString();

    connect(this, SIGNAL(accepted()), this, SLOT(on_accepted()));
    connect(ui->treeSelectedHue, SIGNAL(itemDoubleClicked(QTreeWidgetItem *, int)), this, SLOT(on_treeWidgetItemDoubleClicked(QTreeWidgetItem *, int)));
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
    hueConfig = jsonDoc.object();
    QJsonObject jLights = hueConfig["lights"].toObject();

    int i = 0;
    for (auto j : jLights)
    {
        QJsonObject jObj = j.toObject();
        QTreeWidgetItem *item = new QTreeWidgetItem(ui->treeAllHue);

        qDebug() << jObj.keys();
        item->setData(0, Qt::DisplayRole, jLights.keys().at(i));
        item->setData(1, Qt::DisplayRole, jObj["name"].toString());
        i++;
    }


    for (int i = 0;i < ListeRoom::Instance().size();i++)
    {
        Room *room = ListeRoom::Instance().get_room(i);

        for (int j = 0;j < room->get_size_out();j++)
        {
            IOBase *out = room->get_output(j);

            if (out->get_param("type") != "HueOutputLightRGB")
                continue;
            QTreeWidgetItem *item = new QTreeWidgetItem(ui->treeSelectedHue);

            item->setData(0, Qt::DisplayRole, out->get_param("name").c_str());
            QComboBox *comboBox = new QComboBox(ui->treeSelectedHue);
            for (int i = 0;i < ListeRoom::Instance().size();i++)
                comboBox->addItem(ListeRoom::Instance().get_room(i)->get_name().c_str());

            ui->treeSelectedHue->setItemWidget(item, 1, comboBox);
        }
    }

    ui->buttonBox->setVisible(true);
}

void WizardHue::on_buttonAdd_clicked()
{       
    QTreeWidgetItem *it = reinterpret_cast<QTreeWidgetItem *>(ui->treeAllHue->currentItem());
    if (it)
    {
        QString id = it->data(0, Qt::DisplayRole).toString();
        QString name = it->data(1, Qt::DisplayRole).toString();
        QJsonObject jLights = hueConfig["lights"].toObject();
        QJsonObject jObj;

        for (auto j : jLights)
        {
            if ( j.toObject()["name"] == name)
            {
                jObj = j.toObject();
            }
        }


        //QJsonObject jLight = hueConfig["lights"] .at(idx).toObject();

        QTreeWidgetItem *item = new QTreeWidgetItem(ui->treeSelectedHue);
        item->setData(0, Qt::DisplayRole, id);
        item->setData(1, Qt::DisplayRole, jObj["name"].toString());
        item->setData(2, Qt::EditRole, jObj["name"].toString());
        item->setFlags(item->flags() | Qt::ItemIsEditable);

        QComboBox *comboBox = new QComboBox(ui->treeSelectedHue);

        for (int i = 0;i < ListeRoom::Instance().size();i++)
        {
            QString str = ListeRoom::Instance().get_room(i)->get_name().c_str();
            comboBox->addItem(str);
            qDebug() << str;

        }
        ui->treeSelectedHue->setItemWidget(item, 3, comboBox);

    }
}

void WizardHue::on_buttonDel_clicked()
{
    QTreeWidgetItem *it = reinterpret_cast<QTreeWidgetItem *>(ui->treeSelectedHue->currentItem());
    delete it;

}

void WizardHue::on_accepted()
{

    QTreeWidgetItemIterator it(ui->treeSelectedHue);

    while (*it)
    {
        Params param;

        param.Add("id_hue", (*it)->data(0, Qt::DisplayRole).toString().toStdString());
        param.Add("name", (*it)->data(1, Qt::DisplayRole).toString().toStdString());
        param.Add("api", apiKey.toStdString());
        param.Add("host", bridgeAddress.toString().toStdString());
        param.Add("type", "HueOutputLightRGB");
        param.Add("io_type", "output");
        params << param;
        ++it;
    }

}

void WizardHue::on_treeWidgetItemDoubleClicked(QTreeWidgetItem * item, int column)
{
    if (column == 1) {
        ui->treeSelectedHue->editItem(item, column);
    }
}

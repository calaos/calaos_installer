#include "dialogioproperties.h"
#include "ui_DialogIoProperties.h"

DialogIOProperties::DialogIOProperties(IOBase *_io, const Params &p, int t, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogIOProperties),
    io(_io),
    params(p),
    changedParams(p),
    type(t)
{
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    ui->setupUi(this);

    ui->toolBox->setCurrentIndex(0);
    createIOProperties();
}

DialogIOProperties::~DialogIOProperties()
{
    delete ui;
}

void DialogIOProperties::updateChangedParam(const string &prop, const QString value, const QString pvalue, QLabel *title, QPushButton *revert)
{
    bool isChanged = value != pvalue;

    if (isChanged)
        hider.unhide(revert);
    else
        hider.hide(revert);

    QFont f = title->font();
    f.setBold(isChanged);
    title->setFont(f);

    if (isChanged)
        changedParams.Add(prop, value.toUtf8().constData());
    else
    {
        if (params.Exists(prop))
            changedParams.Add(prop, params[prop]);
        else
            changedParams.Delete(prop);
    }
}

void DialogIOProperties::createIOProperties()
{
    ui->mainLayout->setColumnMinimumWidth(0, 150);
    ui->optionLayout->setColumnMinimumWidth(0, 150);

    QString lang = "en"; //TODO change language here
    QString rsc = QString(":/doc/%1/io_doc.json").arg(lang);

    QFile f(rsc);
    if (!f.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::warning(this, tr("Error"), tr("Failed to load IO documentation from %1").arg(rsc));
        return;
    }
    QJsonParseError jerr;
    QJsonDocument jdoc = QJsonDocument::fromJson(f.readAll(), &jerr);
    if (jerr.error != QJsonParseError::NoError ||
        !jdoc.isObject())
    {
        QMessageBox::warning(this, tr("Error"), tr("Failed to parse JSON IO documentation from %1").arg(rsc));
        return;
    }

    QString iotype = QString::fromUtf8(io->get_param("type").c_str());
    QJsonObject jobj = jdoc.object();
    for (auto it = jobj.begin();it != jobj.end();it++)
        jobj.insert(it.key().toLower(), it.value());

    QJsonObject jobjAlias;
    if (!jobj.contains(iotype))
    {
        //Search in aliases
        bool aliasfound = false;
        for (auto it = jobj.constBegin();it != jobj.constEnd();it++)
        {
            QJsonObject o = it.value().toObject();
            QJsonArray jalias = o["alias"].toArray();
            for (int i = 0;i < jalias.size();i++)
            {
                if (jalias.at(i).toString() == iotype)
                {
                    aliasfound = true;
                    jobjAlias = o;
                }
            }
        }

        if (!aliasfound)
        {
            QMessageBox::warning(this, tr("Error"), tr("IO type %1 is not found in %2").arg(iotype).arg(rsc));
            return;
        }
    }

    QJsonObject jioobj;
    if (jobjAlias.isEmpty())
        jioobj = jobj[iotype].toObject();
    else
        jioobj = jobjAlias;
    ui->labelTitle->setText(iotype);
    ui->labelDesc->setText(jioobj["description"].toString());

    int rowMain = 0, rowOption = 0;

    QJsonArray jparams = jioobj["parameters"].toArray();
    for (int i = 0;i < jparams.size();i++)
    {
        QJsonObject jparam = jparams[i].toObject();

        QGridLayout *layout = jparam["mandatory"].toString() == "true"?ui->mainLayout:ui->optionLayout;
        int row = jparam["mandatory"].toBool()?rowMain:rowOption;

        QLabel *title = new QLabel(jparam["name"].toString());
        layout->addWidget(title, row, 0);

        QPushButton *revert = new QPushButton();
        revert->setIcon(QIcon(":/img/document-revert.png"));
        revert->setToolTip(tr("Revert modification"));
        layout->addWidget(revert, row, 1);
        hider.hide(revert);

        QString pvalue;
        string prop = jparam["name"].toString().toUtf8().constData();
        if (io->get_params().Exists(prop))
            pvalue = QString::fromUtf8(io->get_param(prop).c_str());
        else
            pvalue = jparam["default"].toString();

        if (jparam["type"].toString() == "string")
        {
            QLineEdit *w = new QLineEdit();
            w->setEnabled(jparam["readonly"].toString() != "true");
            w->setText(pvalue);
            layout->addWidget(w, row, 2);

            connect(w, &QLineEdit::textChanged, [=]()
            {
                updateChangedParam(prop, w->text(), pvalue, title, revert);
            });

            connect(revert, &QPushButton::clicked, [=]()
            {
                w->setText(pvalue);
            });
        }
        else if (jparam["type"].toString() == "bool")
        {
            QCheckBox *w = new QCheckBox();
            w->setEnabled(jparam["readonly"].toString() != "true");
            w->setChecked(pvalue == "true");
            layout->addWidget(w, row, 2);

            connect(w, &QCheckBox::stateChanged, [=]()
            {
                updateChangedParam(prop, w->isChecked()?"true":"false", pvalue, title, revert);
            });

            connect(revert, &QPushButton::clicked, [=]()
            {
                w->setChecked(pvalue == "true");
            });
        }
        else if (jparam["type"].toString() == "int")
        {
            QSpinBox *w = new QSpinBox();
            w->setEnabled(jparam["readonly"].toString() != "true");
            if (!jparam["min"].toString().isEmpty())
                w->setMinimum(jparam["min"].toString().toInt());
            else
                w->setMinimum(-999999999);
            if (!jparam["max"].toString().isEmpty())
                w->setMaximum(jparam["max"].toString().toInt());
            else
                w->setMaximum(999999999);
            w->setValue(pvalue.toInt());
            layout->addWidget(w, row, 2);

            connect(w, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), [=]()
            {
                updateChangedParam(prop, QString("%1").arg(w->value()), pvalue.isEmpty()?"0":pvalue, title, revert);
            });

            connect(revert, &QPushButton::clicked, [=]()
            {
                w->setValue(pvalue.toInt());
            });
        }
        else if (jparam["type"].toString() == "float")
        {
            QDoubleSpinBox *w = new QDoubleSpinBox();
            w->setEnabled(jparam["readonly"].toString() != "true");
            if (!jparam["min"].toString().isEmpty())
                w->setMinimum(jparam["min"].toString().toDouble());
            else
                w->setMinimum(-999999999.0);
            if (!jparam["max"].toString().isEmpty())
                w->setMaximum(jparam["max"].toString().toDouble());
            else
                w->setMaximum(999999999.0);
            w->setValue(pvalue.toDouble());
            layout->addWidget(w, row, 2);
            w->setDecimals(3);

            connect(w, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), [=]()
            {
                updateChangedParam(prop, QString("%1").arg(w->value()), pvalue.isEmpty()?"0":pvalue, title, revert);
            });

            connect(revert, &QPushButton::clicked, [=]()
            {
                w->setValue(pvalue.toDouble());
            });
        }

        QPushButton *help = new QPushButton();
        help->setIcon(QIcon(":/img/icon_unkown.png"));
        help->setFlat(true);
        layout->addWidget(help, row, 3);

        //avoid copy the QJsonObject in the lambda
        QString helpInfo = jparam["description"].toString();

        connect(help, &QPushButton::clicked, [=]()
        {
            if (balloonTip)
                delete balloonTip;

            balloonTip = new BalloonTip(QPixmap(":/img/icon_unkown.png"), title->text(), helpInfo, 800, help);
            balloonTip->setArrowPosition(BalloonTip::TopLeft);
            balloonTip->move(QCursor::pos());
            balloonTip->show();
        });

        if (jparam["mandatory"].toBool())
            rowMain++;
        else
            rowOption++;
    }
}

void DialogIOProperties::on_buttonBox_accepted()
{
    accept();
}

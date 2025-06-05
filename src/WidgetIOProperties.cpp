#include "WidgetIOProperties.h"
#include "ui_WidgetIOProperties.h"
#include <QCheckBox>

WidgetIOProperties::WidgetIOProperties(const Params &p, bool _editable, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetIOProperties),
    params(p),
    changedParams(p),
    editable(_editable)
{
    ui->setupUi(this);

    ui->toolBox->setCurrentIndex(0);
    entryHelper = EntryHelpers::getEntryHelper(p, this);
    createIOProperties();
}

WidgetIOProperties::~WidgetIOProperties()
{
    if(entryHelper!=nullptr) delete entryHelper;
    delete ui;
}

void WidgetIOProperties::updateChangedParam(const string &prop, const QString value, const QString pvalue, QLabel *title, QPushButton *revert)
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

void WidgetIOProperties::createIOProperties()
{
    ui->mainLayout->setColumnMinimumWidth(0, 150);
    ui->optionLayout->setColumnMinimumWidth(0, 150);

    QString lang = Utils::GetLocale();
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

    QString iotype = QString::fromUtf8(params["type"].c_str());
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

    //convert to vector for sorting
    QVector<QJsonValue> vec;
    for (const QJsonValue &value : jparams)
        vec.append(value);

    std::sort(vec.begin(), vec.end(), [](const QJsonValue &a, const QJsonValue &b)
    {
        QString keyA = a.toObject().value("name").toString();
        QString keyB = b.toObject().value("name").toString();
        return keyA < keyB; // Case-sensitive comparison
    });

    // Convert back to QJsonArray
    QJsonArray jparamsSorted;
    for (const QJsonValue &value : vec)
        jparamsSorted.append(value);

    for (int i = 0;i < jparamsSorted.size();i++)
    {
        QJsonObject jparam = jparamsSorted[i].toObject();

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
        if (params.Exists(prop))
            pvalue = QString::fromUtf8(params[prop].c_str());
        else
            pvalue = jparam["default"].toString();

        if (jparam["type"].toString() == "string")
        {
            QLineEdit *w = new QLineEdit();
            w->setEnabled(jparam["readonly"].toString() != "true" && editable);
            w->setText(pvalue);
            layout->addWidget(w, row, 2);

            UiObject uiObject;
            uiObject.type = UiObjectType::LineEdit;
            uiObject.lineEdit = w;
            uiObjectMap[prop] = uiObject;

            connect(w, &QLineEdit::textChanged, this, [=]()
            {
                updateChangedParam(prop, w->text(), pvalue, title, revert);
            });

            connect(revert, &QPushButton::clicked, this, [=]()
            {
                w->setText(pvalue);
            });
        }
        else if (jparam["type"].toString() == "bool")
        {
            QCheckBox *w = new QCheckBox();
            w->setEnabled(jparam["readonly"].toString() != "true" && editable);
            w->setChecked(pvalue == "true");
            layout->addWidget(w, row, 2);

            UiObject uiObject;
            uiObject.type = UiObjectType::CheckBox;
            uiObject.checkBox = w;
            uiObjectMap[prop] = uiObject;

            connect(w, &QCheckBox::stateChanged, this, [=]()
            {
                updateChangedParam(prop, w->isChecked()?"true":"false", pvalue, title, revert);
            });

            connect(revert, &QPushButton::clicked, this, [=]()
            {
                w->setChecked(pvalue == "true");
            });
        }
        else if (jparam["type"].toString() == "int")
        {
            QSpinBox *w = new QSpinBox();
            w->setEnabled(jparam["readonly"].toString() != "true" && editable);
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

            UiObject uiObject;
            uiObject.type = UiObjectType::SpinBox;
            uiObject.spinBox = w;
            uiObjectMap[prop] = uiObject;

            connect(w, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, [=]()
            {
                updateChangedParam(prop, QString("%1").arg(w->value()), pvalue.isEmpty()?"0":pvalue, title, revert);
            });

            connect(revert, &QPushButton::clicked, this, [=]()
            {
                w->setValue(pvalue.toInt());
            });
        }
        else if (jparam["type"].toString() == "float")
        {
            QDoubleSpinBox *w = new QDoubleSpinBox();
            w->setEnabled(jparam["readonly"].toString() != "true" && editable);
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

            UiObject uiObject;
            uiObject.type = UiObjectType::DoubleSpinBox;
            uiObject.doubleSpinBox = w;
            uiObjectMap[prop] = uiObject;

            connect(w, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, [=]()
            {
                updateChangedParam(prop, QString("%1").arg(w->value()), pvalue.isEmpty()?"0":pvalue, title, revert);
            });

            connect(revert, &QPushButton::clicked, this, [=]()
            {
                w->setValue(pvalue.toDouble());
            });
        }
        else if (jparam["type"].toString() == "list")
        {
            QComboBox *w = new QComboBox();
            int defIndex = 0;
            w->setEnabled(jparam["readonly"].toString() != "true" && editable);

            //fill combobox with values, if no value, set editable to true
            QJsonObject jvalues = jparam["list_value"].toObject();
            if (jvalues.empty())
                w->setEditable(true);
            else
            {
                w->setEditable(false);
                for (auto it = jvalues.begin();it != jvalues.end();it++)
                {
                    w->addItem(it.value().toString(), it.key());
                    if (it.key() == pvalue)
                    {
                        defIndex = w->count() - 1;
                        w->setCurrentIndex(defIndex);
                    }
                }
            }

            layout->addWidget(w, row, 2);

            UiObject uiObject;
            uiObject.type = UiObjectType::ComboBox;
            uiObject.comboBox = w;
            uiObjectMap[prop] = uiObject;

            if (w->isEditable())
            {
                connect(w, &QComboBox::currentTextChanged, this, [=]()
                {
                    updateChangedParam(prop, w->currentText(), pvalue, title, revert);
                });
            }
            else
            {
                connect(w, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, [=]()
                {
                    updateChangedParam(prop, w->currentData().toString(), pvalue, title, revert);
                });
            }

            connect(revert, &QPushButton::clicked, this, [=]()
            {
                if (w->isEditable())
                    w->setEditText(pvalue);
                else
                    w->setCurrentIndex(defIndex);
            });
        }

        QPushButton *help = new QPushButton();
        help->setIcon(QIcon(":/img/icon_unkown.png"));
        help->setFlat(true);
        layout->addWidget(help, row, 3);

        if((i==0)&&(entryHelper != nullptr))
        {
            QPushButton *entryHelperButton = new QPushButton();
            entryHelperButton->setIcon(QIcon(":/img/icon_entry_helper.png"));
            entryHelperButton->setFlat(true);
            layout->addWidget(entryHelperButton, row, 4);
            connect(entryHelperButton, &QPushButton::clicked, this, [=]()
            {
                if (entryHelper->exec() == QDialog::Accepted)
                    setValues(entryHelper->getParams());
            });
        }

        //avoid copy the QJsonObject in the lambda
        QString helpInfo = jparam["description"].toString();

        connect(help, &QPushButton::clicked, this, [=]()
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

void WidgetIOProperties::setValues(Params& params)
{
    int size = params.size();
    string key;
    string value;
    QString qvalue;
    map<string, UiObject>::iterator it;

    for(int i=0; i < size; i++)
    {
        params.get_item(i, key, value);
        it = uiObjectMap.find(key);
        if(it==uiObjectMap.end()) continue;

        qvalue = QString::fromUtf8(value.c_str());
        switch(it->second.type)
        {
            case UiObjectType::LineEdit :
                it->second.lineEdit->setText(qvalue);
                break;
            case UiObjectType::CheckBox :
                it->second.checkBox->setChecked(qvalue == "true");
                break;
            case UiObjectType::SpinBox :
                it->second.spinBox->setValue(qvalue.toInt());
                break;
            case UiObjectType::DoubleSpinBox :
                it->second.spinBox->setValue(qvalue.toDouble());
                break;
            case UiObjectType::ComboBox :
                if (it->second.comboBox->isEditable())
                    it->second.comboBox->setEditText(qvalue);
                else
                {
                    int defIndex = it->second.comboBox->findText(qvalue);
                    if(defIndex!=-1) it->second.comboBox->setCurrentIndex(defIndex);
                }
                break;
        }
    }
}

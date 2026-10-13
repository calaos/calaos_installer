#include "WidgetIOProperties.h"
#include "ui_WidgetIOProperties.h"
#include "IODoc.h"
#include <QCheckBox>
#include <QDoubleValidator>
#include <QLocale>

namespace
{

// A stored value outside the schema range must stay editable and revertible as is:
// QAbstractSpinBox clamps silently on setValue(), and the revert button would then
// write the clamped bound over the stored value.
template<class T>
bool widenToStoredValue(T &minV, T &maxV, T stored)
{
    if (stored >= minV && stored <= maxV)
        return false;
    minV = qMin(minV, stored);
    maxV = qMax(maxV, stored);
    return true;
}

// Missing min/max fall back to a sentinel that must not be shown as a documented bound.
QString documentedRange(const QJsonObject &jparam, const QString &min, const QString &max)
{
    const bool hasMin = !jparam["min"].toString().isEmpty();
    const bool hasMax = !jparam["max"].toString().isEmpty();
    if (hasMin && hasMax)
        return QObject::tr("%1 to %2").arg(min, max);
    if (hasMin)
        return QObject::tr("at least %1").arg(min);
    if (hasMax)
        return QObject::tr("at most %1").arg(max);
    return QObject::tr("no documented bound");
}

const char *s_outOfRangeStyle = "QAbstractSpinBox { background-color: #ffe8b3; color: black; }";

}

WidgetIOProperties::WidgetIOProperties(const Params &p, bool _editable, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetIOProperties),
    params(p),
    changedParams(p),
    editable(_editable)
{
    ui->setupUi(this);

    ui->toolBox->setCurrentIndex(0);
    createIOProperties();
}

WidgetIOProperties::~WidgetIOProperties()
{
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

void WidgetIOProperties::flagOutOfRange(QAbstractSpinBox *w, QLabel *title, const QString &value, const QString &range)
{
    QString tip = tr("The value %1 is outside the documented range (%2). "
                     "It is kept as is unless you change it.").arg(value, range);
    w->setToolTip(tip);
    title->setToolTip(tip);
    w->setStyleSheet(s_outOfRangeStyle);
}

void WidgetIOProperties::createIOProperties()
{
    ui->mainLayout->setColumnMinimumWidth(0, 150);
    ui->optionLayout->setColumnMinimumWidth(0, 150);

    QString iotype = QString::fromUtf8(params["type"].c_str());
    QJsonObject jioobj = IODoc::getIOObject(iotype);

    if (jioobj.isEmpty())
    {
        QMessageBox::warning(this, tr("Error"), tr("IO type %1 is not found in IO documentation").arg(iotype));
        return;
    }

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
            int minV = jparam["min"].toString().isEmpty() ? -999999999 : jparam["min"].toString().toInt();
            int maxV = jparam["max"].toString().isEmpty() ? 999999999 : jparam["max"].toString().toInt();
            const QString docRange = documentedRange(jparam, QString::number(minV), QString::number(maxV));
            bool parsed = false;
            int storedV = pvalue.toInt(&parsed);
            // Only a value read from io.xml is kept; a schema default is still clamped.
            if (params.Exists(prop) && parsed && widenToStoredValue(minV, maxV, storedV))
                flagOutOfRange(w, title, pvalue, docRange);
            w->setMinimum(minV);
            w->setMaximum(maxV);
            w->setValue(pvalue.toInt());
            layout->addWidget(w, row, 2);

            UiObject uiObject;
            uiObject.type = UiObjectType::SpinBox;
            uiObject.spinBox = w;
            uiObject.title = title;
            uiObject.docRange = docRange;
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
        else if (jparam["type"].toString() == "float" && jparam["mandatory"].toString() != "true" &&
                 jparam["default"].toString().isEmpty())
        {
            // A spin box always holds a number: an optional bound (value_min/value_max, ...)
            // needs an empty state meaning "param absent".
            QLineEdit *w = new QLineEdit();
            w->setEnabled(jparam["readonly"].toString() != "true" && editable);
            w->setPlaceholderText(tr("unbounded"));
            QDoubleValidator *v = new QDoubleValidator(w);
            QLocale c = QLocale::c();
            c.setNumberOptions(QLocale::RejectGroupSeparator);
            v->setLocale(c);
            v->setNotation(QDoubleValidator::StandardNotation);
            w->setValidator(v);
            w->setText(pvalue);
            layout->addWidget(w, row, 2);

            UiObject uiObject;
            uiObject.type = UiObjectType::LineEdit;
            uiObject.lineEdit = w;
            uiObjectMap[prop] = uiObject;
            optionalNumbers.push_back(prop);

            connect(w, &QLineEdit::textChanged, this, [=]()
            {
                const QString text = w->text().trimmed();
                updateChangedParam(prop, text, pvalue, title, revert);
                if (text.isEmpty())
                    changedParams.Delete(prop);
            });

            connect(revert, &QPushButton::clicked, this, [=]()
            {
                w->setText(pvalue);
            });
        }
        else if (jparam["type"].toString() == "float")
        {
            QDoubleSpinBox *w = new QDoubleSpinBox();
            w->setEnabled(jparam["readonly"].toString() != "true" && editable);
            double minV = jparam["min"].toString().isEmpty() ? -999999999.0 : jparam["min"].toString().toDouble();
            double maxV = jparam["max"].toString().isEmpty() ? 999999999.0 : jparam["max"].toString().toDouble();
            const QString docRange = documentedRange(jparam, QString::number(minV), QString::number(maxV));
            bool parsed = false;
            double storedV = pvalue.toDouble(&parsed);
            // "nan" and "inf" parse fine but would make the widened bounds meaningless.
            if (params.Exists(prop) && parsed && qIsFinite(storedV) && widenToStoredValue(minV, maxV, storedV))
                flagOutOfRange(w, title, pvalue, docRange);
            // Decimals first: the bounds and the value are rounded to them when set.
            w->setDecimals(3);
            w->setMinimum(minV);
            w->setMaximum(maxV);
            w->setValue(pvalue.toDouble());
            layout->addWidget(w, row, 2);

            UiObject uiObject;
            uiObject.type = UiObjectType::DoubleSpinBox;
            uiObject.doubleSpinBox = w;
            uiObject.title = title;
            uiObject.docRange = docRange;
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

QString WidgetIOProperties::validationError()
{
    for (const string &prop: optionalNumbers)
    {
        QLineEdit *e = uiObjectMap[prop].lineEdit;
        if (e->isEnabled() && !e->text().trimmed().isEmpty() && !e->hasAcceptableInput())
            return tr("%1 must be a plain number.").arg(QString::fromStdString(prop));
    }

    auto lo = uiObjectMap.find("value_min"), hi = uiObjectMap.find("value_max");
    if (lo != uiObjectMap.end() && hi != uiObjectMap.end() &&
        lo->second.lineEdit && hi->second.lineEdit &&
        !lo->second.lineEdit->text().isEmpty() && !hi->second.lineEdit->text().isEmpty() &&
        lo->second.lineEdit->text().toDouble() > hi->second.lineEdit->text().toDouble())
        return tr("The minimum value cannot be greater than the maximum value.");

    return QString();
}

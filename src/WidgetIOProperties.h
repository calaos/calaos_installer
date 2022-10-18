#ifndef WIDGETIOPROPERTIES_H
#define WIDGETIOPROPERTIES_H

#include <QDialog>
#include <QInputDialog>
#include <QMessageBox>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QComboBox>

#include <Params.h>
#include <ListeRoom.h>
#include "BalloonTip.h"
#include "EntryHelpers.h"

using namespace Calaos;

namespace Ui {
class WidgetIOProperties;
}

enum { OBJ_NONE = 0, OBJ_INPUT, OBJ_OUTPUT, OBJ_ROOM, OBJ_RULE };

class Hider : public QObject
{
    Q_OBJECT
public:
    Hider(QObject * parent = 0) : QObject(parent) {}
    bool eventFilter(QObject *, QEvent * ev)
    {
        return ev->type() == QEvent::Paint;
    }
    void hide(QWidget * w)
    {
        w->installEventFilter(this);
        w->update();
    }
    void unhide(QWidget * w)
    {
        w->removeEventFilter(this);
        w->update();
    }
    Q_SLOT void hideWidget()
    {
        QObject * s = sender();
        if (s->isWidgetType()) { hide(qobject_cast<QWidget*>(s)); }
    }
};

enum UiObjectType { LineEdit, CheckBox, SpinBox, DoubleSpinBox, ComboBox };

struct UiObject
{
    UiObjectType type;
    QLineEdit* lineEdit;
    QCheckBox* checkBox;
    QSpinBox* spinBox;
    QDoubleSpinBox* doubleSpinBox;
    QComboBox* comboBox;
};

class WidgetIOProperties : public QWidget
{
    Q_OBJECT
    Q_DISABLE_COPY(WidgetIOProperties)
public:
    explicit WidgetIOProperties(const Params &p, bool editable = true, QWidget *parent = 0);
    ~WidgetIOProperties();

    Params &getParams() { return changedParams; }

private:
    Ui::WidgetIOProperties *ui;

    Params params;
    Params changedParams;
    bool editable;

    Hider hider;

    BalloonTip *balloonTip = nullptr;

    std::map<string, UiObject> uiObjectMap;
    IEntryHelper* entryHelper;

    void createIOProperties();
    void updateChangedParam(const string &prop, const QString value, const QString pvalue, QLabel *title, QPushButton *revert);
    void setValues(Params& params);
};

#endif // WIDGETIOPROPERTIES_H

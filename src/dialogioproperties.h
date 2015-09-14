#ifndef DIALOGIOPROPERTIES_H
#define DIALOGIOPROPERTIES_H

#include <QtWidgets>
#include "WidgetIOProperties.h"

namespace Ui
{
class DialogIOProperties;
}

class DialogIOProperties : public QDialog
{
    Q_OBJECT
    Q_DISABLE_COPY(DialogIOProperties)
public:
    DialogIOProperties(const Params &p, bool editable = true, QWidget *parent = 0);
    DialogIOProperties(IOBase *io, bool editable = true, QWidget *parent = 0);
    virtual ~DialogIOProperties();

    Params &getParams() { return ioWidget->getParams(); }

private:
    Ui::DialogIOProperties *ui;

    WidgetIOProperties *ioWidget;

private slots:
    void on_buttonBox_accepted();
};

#endif // DIALOGIOPROPERTIES_H

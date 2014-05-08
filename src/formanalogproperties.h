#ifndef FORMANALOGPROPERTIES_H
#define FORMANALOGPROPERTIES_H

#include <QWidget>

namespace Ui {
class FormAnalogProperties;
}

class FormAnalogProperties : public QWidget
{
    Q_OBJECT

public:
    explicit FormAnalogProperties(QWidget *parent = 0);
    ~FormAnalogProperties();

private:
    Ui::FormAnalogProperties *ui;
};

#endif // FORMANALOGPROPERTIES_H

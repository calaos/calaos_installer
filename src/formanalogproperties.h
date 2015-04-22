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
    explicit FormAnalogProperties(QWidget *parent = 0, bool isOutput = true);
    ~FormAnalogProperties();
    QString getUnit();
    double getStep();
    double getCoeff();
    double getOffset();
    double getMin();
    double getMax();

    void updateUI(bool isOutput);

private:
    Ui::FormAnalogProperties *ui;
};

#endif // FORMANALOGPROPERTIES_H

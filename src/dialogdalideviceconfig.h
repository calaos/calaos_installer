#ifndef DIALOGDALIDEVICECONFIG_H
#define DIALOGDALIDEVICECONFIG_H

#include <QDialog>

namespace Ui
{
        class DialogDaliDeviceConfig;
}

class DialogDaliDeviceConfig : public QDialog
{
        Q_OBJECT

        public:
                explicit DialogDaliDeviceConfig(QWidget *parent = 0);
                ~DialogDaliDeviceConfig();

                void setFadeRate(int value);
                int getFadeRate();
                void setFadeTime(int value);
                int getFadeTime();
                void setMaxValue(int value);
                int getMaxValue();
                void setMinValue(int value);
                int getMinValue();
                void setSystemFailureLevel(int value);
                int getSystemFailureLevel();
                void setPowerOnLevel(int value);
                int getPowerOnLevel();

        protected:
                void changeEvent(QEvent *e);

        private:
                Ui::DialogDaliDeviceConfig *ui;
};

#endif // DIALOGDALIDEVICECONFIG_H

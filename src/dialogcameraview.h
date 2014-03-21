#ifndef DIALOGCAMERAVIEW_H
#define DIALOGCAMERAVIEW_H

#include <QBuffer>
#include <QtWidgets>
#include <iostream>
#include <IOBase.h>
#include <QDialog>

#include <ListeRoom.h>
using namespace std;
using namespace Calaos;

namespace Ui
{
        class DialogCameraView;
}

class DialogCameraView : public QDialog
{
                Q_OBJECT
                Q_DISABLE_COPY(DialogCameraView)
        public:
                explicit DialogCameraView(Camera *camera, QWidget *parent = 0);
                virtual ~DialogCameraView();

                void DownloadPicture();

        protected:
                virtual void changeEvent(QEvent *e);

        private:
                Ui::DialogCameraView *ui;

                Camera *camera;

};

#endif // DIALOGCAMERAVIEW_H

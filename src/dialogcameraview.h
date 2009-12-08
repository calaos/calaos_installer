#ifndef DIALOGCAMERAVIEW_H
#define DIALOGCAMERAVIEW_H

#include <QBuffer>
#include <QHttp>
#include <QtGui/QDialog>
#include <iostream>
#include <IOBase.h>

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

                QHttp *http;
                QBuffer *buffer;
                QByteArray bytes;
                int request;
                Camera *camera;

        public slots:
                void downloadFinished (int requestId, bool error);

};

#endif // DIALOGCAMERAVIEW_H

#ifndef DIALOGCAMERAVIEW_H
#define DIALOGCAMERAVIEW_H

#include <QBuffer>
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
    explicit DialogCameraView(IOBase *camera, QWidget *parent = 0);
    virtual ~DialogCameraView();

    void DownloadPicture();

protected:
    virtual void changeEvent(QEvent *e);

private:
    Ui::DialogCameraView *ui;

    IOBase *camera;

};

#endif // DIALOGCAMERAVIEW_H

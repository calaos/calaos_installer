#ifndef DIALOGAUTODETECT_H
#define DIALOGAUTODETECT_H

#include <QDialog>
#include <QTimer>
#include <QUdpSocket>
#include <QPushButton>

namespace Ui {
class DialogAutoDetect;
}

class DialogAutoDetect : public QDialog
{
    Q_OBJECT

public:
    explicit DialogAutoDetect(QWidget *parent = 0);
    ~DialogAutoDetect();
    QString getHost();
    QUdpSocket *udpSocket;
    QTimer *timer;

private:
    Ui::DialogAutoDetect *ui;
    void calaosDiscover();

private slots:
    void readPendingDatagrams(void);
    void timeout(void);
    void on_list_doubleClicked(const QModelIndex &index);
};

#endif // DIALOGAUTODETECT_H

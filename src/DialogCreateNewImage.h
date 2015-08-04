#ifndef DIALOGCREATENEWIMAGE_H
#define DIALOGCREATENEWIMAGE_H

#include <QDialog>
#include <QMap>
#include <QList>

class CalaosImage
{
public:
    QString url;
    QString version;
    QString type;
};

namespace Ui {
class DialogCreateNewImage;
}

class DialogCreateNewImage : public QDialog
{
    Q_OBJECT

    QMap<QString, QList<CalaosImage*>> images;
    bool stableOnly;

public:
    explicit DialogCreateNewImage(QWidget *parent = 0);
    ~DialogCreateNewImage();

    void updateVersions(const QString &arg1);
private slots:
    void on_machineCombo_currentIndexChanged(const QString &arg1);

    void on_radioButton_2_clicked();

    void on_radioButton_clicked();

    void on_downloadButton_clicked();

    void on_selectImageButton_clicked();

    void on_writeButton_clicked();

private:
    Ui::DialogCreateNewImage *ui;
    QStringList getUserFriendlyNames(const QStringList &devices) const;
};

#endif // DIALOGCREATENEWIMAGE_H

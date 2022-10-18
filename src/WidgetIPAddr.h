#ifndef WIDGETIPADDR_H
#define WIDGETIPADDR_H

#include <QFrame>
#include <QLineEdit>
#include <QIntValidator>

class WidgetIPAddr : public QFrame
{
    typedef QFrame baseClass;

    Q_OBJECT

public:
    WidgetIPAddr(QWidget *parent);
    ~WidgetIPAddr();

#define QTUTL_IP_SIZE 4

    virtual bool eventFilter( QObject *obj, QEvent *event );

    QString getIPAddress();

public slots:
    void slotTextChanged( QLineEdit* pEdit );

signals:
    void signalTextChanged( QLineEdit* pEdit );

private:
    QLineEdit *m_pLineEdit[QTUTL_IP_SIZE];

    static QString getIPItemStr( unsigned char item );
};

class IPItemValidator : public QIntValidator
{
public:
    IPItemValidator( QObject* parent ) : QIntValidator( parent )
    {
        setRange( 0, UCHAR_MAX );
    }
    ~IPItemValidator() {}

    virtual void fixup( QString & input ) const
    {
        if ( input.isEmpty() )
            input = "0";
    }
};

#endif // WIDGETIPADDR_H

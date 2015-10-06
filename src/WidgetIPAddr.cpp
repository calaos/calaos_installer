#include "WidgetIPAddr.h"

WidgetIPAddr::WidgetIPAddr(QWidget *parent) : baseClass(parent)
{
    setFrameShape( QFrame::StyledPanel );
    setFrameShadow( QFrame::Sunken );

    QHBoxLayout* pLayout = new QHBoxLayout( this );
    setLayout( pLayout );
    pLayout->setContentsMargins( 0, 0, 0, 0 );
    pLayout->setSpacing( 0 );

    for ( int i = 0; i != QTUTL_IP_SIZE; ++i )
    {
        if ( i != 0 )
        {
            QLabel* pDot = new QLabel( ".", this );
            pDot->setStyleSheet( "background: white" );
            pLayout->addWidget( pDot );
            pLayout->setStretch( pLayout->count(), 0 );
        }

        m_pLineEdit[i] = new QLineEdit( this );
        QLineEdit* pEdit = m_pLineEdit[i];
        pEdit->installEventFilter( this );

        pLayout->addWidget( pEdit );
        pLayout->setStretch( pLayout->count(), 1 );

        pEdit->setFrame( false );
        pEdit->setAlignment( Qt::AlignCenter );

        QFont font = pEdit->font();
        font.setStyleHint( QFont::Monospace );
        font.setFixedPitch( true );
        pEdit->setFont( font );

        pEdit->setValidator( new IPItemValidator( pEdit ) );

        pEdit->setText("0");
    }

    setMaximumWidth( 30 * QTUTL_IP_SIZE );

    connect( this, SIGNAL(signalTextChanged(QLineEdit*)),
             this, SLOT(slotTextChanged(QLineEdit*)),
             Qt::QueuedConnection );
}

WidgetIPAddr::~WidgetIPAddr()
{
}

QString WidgetIPAddr::getIPAddress()
{
    return QString("%1.%2.%3.%4")
            .arg(m_pLineEdit[0]->text())
            .arg(m_pLineEdit[1]->text())
            .arg(m_pLineEdit[2]->text())
            .arg(m_pLineEdit[3]->text());
}

QString WidgetIPAddr::getIPItemStr( unsigned char item )
{
    return QString("%1").arg(item);
}

void WidgetIPAddr::slotTextChanged( QLineEdit* pEdit )
{
    for ( unsigned int i = 0; i != QTUTL_IP_SIZE; ++i )
    {
        if ( pEdit == m_pLineEdit[i] )
        {
            if ( pEdit->text().size() == getIPItemStr( UCHAR_MAX ).length() &&
                 pEdit->text().size() == pEdit->cursorPosition() )
            {
                // auto-move to next item
                if ( i+1 != QTUTL_IP_SIZE )
                {
                    m_pLineEdit[i+1]->setFocus();
                    m_pLineEdit[i+1]->selectAll();
                }
            }
        }
    }
}

bool WidgetIPAddr::eventFilter(QObject *obj, QEvent *event)
{
    bool bRes = baseClass::eventFilter(obj, event);

    if ( event->type() == QEvent::KeyPress )
    {
        QKeyEvent* pEvent = dynamic_cast<QKeyEvent*>( event );
        if ( pEvent )
        {
            for ( unsigned int i = 0; i != QTUTL_IP_SIZE; ++i )
            {
                QLineEdit* pEdit = m_pLineEdit[i];
                if ( pEdit == obj )
                {
                    switch ( pEvent->key() )
                    {
                    case Qt::Key_Left:
                    {
                        if ( pEdit->cursorPosition() == 0 )
                        {
                            // user wants to move to previous item
                            if ( i != 0 )
                            {
                                m_pLineEdit[i-1]->setFocus();
                                m_pLineEdit[i-1]->setCursorPosition( m_pLineEdit[i-1]->text().size() );
                            }
                        }
                        break;
                    }
                    case Qt::Key_Right:
                    {
                        if ( pEdit->text().isEmpty() ||
                             (pEdit->text().size() == pEdit->cursorPosition()) )
                        {
                            // user wants to move to next item
                            if ( i+1 != QTUTL_IP_SIZE )
                            {
                                m_pLineEdit[i+1]->setFocus();
                                m_pLineEdit[i+1]->setCursorPosition( 0 );
                            }
                        }
                        break;
                    }
                    default:
                    {
                        emit signalTextChanged( pEdit );
                    }
                    }

                    break;
                }
            }
        }
    }

    return bRes;
}

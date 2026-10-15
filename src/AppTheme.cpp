#include "AppTheme.h"

#include <QGuiApplication>
#include <QStyleHints>
#include <QtGlobal>

namespace AppTheme
{

void prepareEnvironment()
{
#ifdef Q_OS_WIN
    // Keeps the native title bar, menus and dialogs light; an explicit user
    // choice of platform options is left alone.
    if (!qEnvironmentVariableIsSet("QT_QPA_PLATFORM"))
        qputenv("QT_QPA_PLATFORM", "windows:darkmode=0");
#endif
}

QPalette lightPalette()
{
    QPalette p(QColor(239, 239, 239));
    p.setColor(QPalette::WindowText, QColor(0, 0, 0));
    p.setColor(QPalette::Base, QColor(255, 255, 255));
    p.setColor(QPalette::AlternateBase, QColor(247, 247, 247));
    p.setColor(QPalette::ToolTipBase, QColor(255, 255, 220));
    p.setColor(QPalette::ToolTipText, QColor(0, 0, 0));
    p.setColor(QPalette::Text, QColor(0, 0, 0));
    p.setColor(QPalette::Button, QColor(239, 239, 239));
    p.setColor(QPalette::ButtonText, QColor(0, 0, 0));
    p.setColor(QPalette::BrightText, QColor(255, 255, 255));
    p.setColor(QPalette::Link, QColor(0, 0, 255));
    p.setColor(QPalette::Highlight, QColor(48, 140, 198));
    p.setColor(QPalette::HighlightedText, QColor(255, 255, 255));
    p.setColor(QPalette::PlaceholderText, QColor(0, 0, 0, 128));
    const QColor disabled(120, 120, 120);
    p.setColor(QPalette::Disabled, QPalette::WindowText, disabled);
    p.setColor(QPalette::Disabled, QPalette::Text, disabled);
    p.setColor(QPalette::Disabled, QPalette::ButtonText, disabled);
    return p;
}

void forceLight()
{
#if QT_VERSION >= QT_VERSION_CHECK(6, 8, 0)
    QGuiApplication::styleHints()->setColorScheme(Qt::ColorScheme::Light);
#else
    // No colour scheme override before 6.8: pin the palette instead.
    QGuiApplication::setPalette(lightPalette());
#endif
}

}

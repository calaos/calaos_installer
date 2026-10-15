#ifndef APPTHEME_H
#define APPTHEME_H

#include <QPalette>

// The installer is designed on a light background (stylesheets and QML hard-code
// light and dark areas); a dark system palette makes half of the Remote UI editor unreadable.
namespace AppTheme
{

// Before QApplication exists: Qt reads the Windows dark mode option while creating the platform.
void prepareEnvironment();

// After QApplication exists, before any window.
void forceLight();

QPalette lightPalette();

}

#endif // APPTHEME_H

/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the demonstration applications of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** No Commercial Usage
** This file contains pre-release code and may not be distributed.
** You may use this file in accordance with the terms and conditions
** contained in the Technology Preview License Agreement accompanying
** this package.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights.  These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** If you have questions regarding the use of this file, please contact
** Nokia at qt-info@nokia.com.
**
**
**
**
**
**
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "textedit.h"

#include <QAction>
#include <QApplication>
#include <QClipboard>
#include <QColorDialog>
#include <QComboBox>
#include <QFontComboBox>
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QFontDatabase>
#include <QMenu>
#include <QMenuBar>
#include <QPrintDialog>
#include <QPrinter>
#include <QTextCodec>
#include <QTextEdit>
#include <QToolBar>
#include <QTextCursor>
#include <QTextDocumentWriter>
#include <QTextList>
#include <QtDebug>
#include <QCloseEvent>
#include <QMessageBox>
#include <QPrintPreviewDialog>
#include <QActionGroup>
#include <QCoreApplication>
#include <QHash>
#include <QMap>
#include <QSet>
#include <QVector>
#include <algorithm>

using namespace Calaos;

#ifdef Q_WS_MAC
const QString rsrcPath = ":/images_textedit/mac";
#else
const QString rsrcPath = ":/images_textedit/win";
#endif

TextEdit::TextEdit(QWidget *parent)
    : QMainWindow(parent)
{
    setToolButtonStyle(Qt::ToolButtonFollowStyle);
    setupFileActions();
    setupEditActions();
    setupTextActions();

    textEdit = new QTextEdit(this);
    connect(textEdit, &QTextEdit::currentCharFormatChanged,
            this, &TextEdit::currentCharFormatChanged);
    connect(textEdit, &QTextEdit::cursorPositionChanged,
            this, &TextEdit::cursorPositionChanged);

    setCentralWidget(textEdit);
    textEdit->setFocus();
    setCurrentFileName(QString());

    fontChanged(textEdit->font());
    colorChanged(textEdit->textColor());
    alignmentChanged(textEdit->alignment());

    connect(textEdit->document(), &QTextDocument::modificationChanged,
            this, &TextEdit::setWindowModified);
    connect(textEdit->document(), &QTextDocument::undoAvailable,
            actionUndo, &QAction::setEnabled);
    connect(textEdit->document(), &QTextDocument::redoAvailable,
            actionRedo, &QAction::setEnabled);

    setWindowModified(textEdit->document()->isModified());
    actionUndo->setEnabled(textEdit->document()->isUndoAvailable());
    actionRedo->setEnabled(textEdit->document()->isRedoAvailable());

    connect(actionUndo, &QAction::triggered, textEdit, &QTextEdit::undo);
    connect(actionRedo, &QAction::triggered, textEdit, &QTextEdit::redo);

    actionCut->setEnabled(false);
    actionCopy->setEnabled(false);

    connect(actionCut, &QAction::triggered, textEdit, &QTextEdit::cut);
    connect(actionCopy, &QAction::triggered, textEdit, &QTextEdit::copy);
    connect(actionPaste, &QAction::triggered, textEdit, &QTextEdit::paste);

    connect(textEdit, &QTextEdit::copyAvailable, actionCut, &QAction::setEnabled);
    connect(textEdit, &QTextEdit::copyAvailable, actionCopy, &QAction::setEnabled);

#ifndef QT_NO_CLIPBOARD
    connect(QApplication::clipboard(), &QClipboard::dataChanged, this, &TextEdit::clipboardDataChanged);
#endif
}

void TextEdit::setupFileActions()
{
    QToolBar *tb = new QToolBar(this);
    tb->setWindowTitle(tr("File"));
    addToolBar(tb);

    QMenu *menu = new QMenu(tr("&File"), this);
    menuBar()->addMenu(menu);

    QAction *a;

    a = new QAction(tr("Save as..."), this);
    a->setPriority(QAction::LowPriority);
    connect(a, &QAction::triggered, this, &TextEdit::fileSaveAs);
    menu->addAction(a);
    menu->addSeparator();

#ifndef QT_NO_PRINTER
    a = new QAction(QIcon::fromTheme("document-print", QIcon(rsrcPath + "/fileprint.png")),
                    tr("&Print..."), this);
    a->setPriority(QAction::LowPriority);
    a->setShortcut(QKeySequence::Print);
    connect(a, &QAction::triggered, this, &TextEdit::filePrint);
    tb->addAction(a);
    menu->addAction(a);

    a = new QAction(QIcon::fromTheme("fileprint", QIcon(rsrcPath + "/fileprint.png")),
                    tr("Print preview..."), this);
    connect(a, &QAction::triggered, this, &TextEdit::filePrintPreview);
    menu->addAction(a);

    a = new QAction(QIcon::fromTheme("exportpdf", QIcon(rsrcPath + "/exportpdf.png")),
                    tr("&Export in PDF..."), this);
    a->setPriority(QAction::LowPriority);
    a->setShortcut(Qt::CTRL | Qt::Key_D);
    connect(a, &QAction::triggered, this, &TextEdit::filePrintPdf);
    tb->addAction(a);
    menu->addAction(a);

    menu->addSeparator();
#endif

    a = new QAction(tr("&Close"), this);
    a->setShortcut(Qt::CTRL | Qt::Key_Q);
    connect(a, &QAction::triggered, this, &TextEdit::close);
    menu->addAction(a);
}

void TextEdit::setupEditActions()
{
    QToolBar *tb = new QToolBar(this);
    tb->setWindowTitle(tr("Edit"));
    addToolBar(tb);
    QMenu *menu = new QMenu(tr("&Edit"), this);
    menuBar()->addMenu(menu);

    QAction *a;
    a = actionUndo = new QAction(QIcon::fromTheme("edit-undo", QIcon(rsrcPath + "/editundo.png")),
                                 tr("&Undo"), this);
    a->setShortcut(QKeySequence::Undo);
    tb->addAction(a);
    menu->addAction(a);
    a = actionRedo = new QAction(QIcon::fromTheme("edit-redo", QIcon(rsrcPath + "/editredo.png")),
                                 tr("&Redo"), this);
    a->setPriority(QAction::LowPriority);
    a->setShortcut(QKeySequence::Redo);
    tb->addAction(a);
    menu->addAction(a);
    menu->addSeparator();
    a = actionCut = new QAction(QIcon::fromTheme("edit-cut", QIcon(rsrcPath + "/editcut.png")),
                                tr("Cut"), this);
    a->setPriority(QAction::LowPriority);
    a->setShortcut(QKeySequence::Cut);
    tb->addAction(a);
    menu->addAction(a);
    a = actionCopy = new QAction(QIcon::fromTheme("edit-copy", QIcon(rsrcPath + "/editcopy.png")),
                                 tr("Copy"), this);
    a->setPriority(QAction::LowPriority);
    a->setShortcut(QKeySequence::Copy);
    tb->addAction(a);
    menu->addAction(a);
    a = actionPaste = new QAction(QIcon::fromTheme("edit-paste", QIcon(rsrcPath + "/editpaste.png")),
                                  tr("Paste"), this);
    a->setPriority(QAction::LowPriority);
    a->setShortcut(QKeySequence::Paste);
    tb->addAction(a);
    menu->addAction(a);
#ifndef QT_NO_CLIPBOARD
    actionPaste->setEnabled(!QApplication::clipboard()->text().isEmpty());
#endif
}

void TextEdit::setupTextActions()
{
    QToolBar *tb = new QToolBar(this);
    tb->setWindowTitle(tr("Format"));
    addToolBar(tb);

    QMenu *menu = new QMenu(tr("F&ormat"), this);
    menuBar()->addMenu(menu);

    actionTextBold = new QAction(QIcon::fromTheme("format-text-bold", QIcon(rsrcPath + "/textbold.png")),
                                 tr("&Bold"), this);
    actionTextBold->setShortcut(Qt::CTRL | Qt::Key_B);
    actionTextBold->setPriority(QAction::LowPriority);
    QFont bold;
    bold.setBold(true);
    actionTextBold->setFont(bold);
    connect(actionTextBold, &QAction::triggered, this, &TextEdit::textBold);
    tb->addAction(actionTextBold);
    menu->addAction(actionTextBold);
    actionTextBold->setCheckable(true);

    actionTextItalic = new QAction(QIcon::fromTheme("format-text-italic", QIcon(rsrcPath + "/textitalic.png")),
                                   tr("&Italic"), this);
    actionTextItalic->setPriority(QAction::LowPriority);
    actionTextItalic->setShortcut(Qt::CTRL | Qt::Key_I);
    QFont italic;
    italic.setItalic(true);
    actionTextItalic->setFont(italic);
    connect(actionTextItalic, &QAction::triggered, this, &TextEdit::textItalic);
    tb->addAction(actionTextItalic);
    menu->addAction(actionTextItalic);
    actionTextItalic->setCheckable(true);

    actionTextUnderline = new QAction(QIcon::fromTheme("format-text-underline", QIcon(rsrcPath + "/textunder.png")),
                                      tr("&Underline"), this);
    actionTextUnderline->setShortcut(Qt::CTRL | Qt::Key_U);
    actionTextUnderline->setPriority(QAction::LowPriority);
    QFont underline;
    underline.setUnderline(true);
    actionTextUnderline->setFont(underline);
    connect(actionTextUnderline, &QAction::triggered, this, &TextEdit::textUnderline);
    tb->addAction(actionTextUnderline);
    menu->addAction(actionTextUnderline);
    actionTextUnderline->setCheckable(true);

    menu->addSeparator();

    QActionGroup *grp = new QActionGroup(this);
    connect(grp, &QActionGroup::triggered, this, &TextEdit::textAlign);

    // Make sure the alignLeft  is always left of the alignRight
    if (QApplication::isLeftToRight()) {
        actionAlignLeft = new QAction(QIcon::fromTheme("format-justify-left", QIcon(rsrcPath + "/textleft.png")),
                                      tr("left"), grp);
        actionAlignCenter = new QAction(QIcon::fromTheme("format-justify-center", QIcon(rsrcPath + "/textcenter.png")), tr("Centered"), grp);
        actionAlignRight = new QAction(QIcon::fromTheme("format-justify-right", QIcon(rsrcPath + "/textright.png")), tr("Right"), grp);
    } else {
        actionAlignRight = new QAction(QIcon::fromTheme("format-justify-right", QIcon(rsrcPath + "/textright.png")), tr("Right"), grp);
        actionAlignCenter = new QAction(QIcon::fromTheme("format-justify-center", QIcon(rsrcPath + "/textcenter.png")), tr("Centered"), grp);
        actionAlignLeft = new QAction(QIcon::fromTheme("format-justify-left", QIcon(rsrcPath + "/textleft.png")), tr("Left"), grp);
    }
    actionAlignJustify = new QAction(QIcon::fromTheme("format-justify-fill", QIcon(rsrcPath + "/textjustify.png")), tr("Justify"), grp);

    actionAlignLeft->setShortcut(Qt::CTRL | Qt::Key_L);
    actionAlignLeft->setCheckable(true);
    actionAlignLeft->setPriority(QAction::LowPriority);
    actionAlignCenter->setShortcut(Qt::CTRL | Qt::Key_E);
    actionAlignCenter->setCheckable(true);
    actionAlignCenter->setPriority(QAction::LowPriority);
    actionAlignRight->setShortcut(Qt::CTRL | Qt::Key_R);
    actionAlignRight->setCheckable(true);
    actionAlignRight->setPriority(QAction::LowPriority);
    actionAlignJustify->setShortcut(Qt::CTRL | Qt::Key_J);
    actionAlignJustify->setCheckable(true);
    actionAlignJustify->setPriority(QAction::LowPriority);

    tb->addActions(grp->actions());
    menu->addActions(grp->actions());

    menu->addSeparator();

    QPixmap pix(16, 16);
    pix.fill(Qt::black);
    actionTextColor = new QAction(pix, tr("Color..."), this);
    connect(actionTextColor, &QAction::triggered, this, &TextEdit::textColor);
    tb->addAction(actionTextColor);
    menu->addAction(actionTextColor);


    tb = new QToolBar(this);
    tb->setAllowedAreas(Qt::TopToolBarArea | Qt::BottomToolBarArea);
    tb->setWindowTitle(tr("Format"));
    addToolBarBreak(Qt::TopToolBarArea);
    addToolBar(tb);

    comboStyle = new QComboBox(tb);
    tb->addWidget(comboStyle);
    comboStyle->addItem("Standard");
    comboStyle->addItem("Bullet List (Disc)");
    comboStyle->addItem("Bullet List (Circle)");
    comboStyle->addItem("Bullet List (Square)");
    comboStyle->addItem("Ordered List (Decimal)");
    comboStyle->addItem("Ordered List (Alpha lower)");
    comboStyle->addItem("Ordered List (Alpha upper)");
    comboStyle->addItem("Ordered List (Roman lower)");
    comboStyle->addItem("Ordered List (Roman upper)");

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    connect(comboStyle, &QComboBox::activated,
            this, &TextEdit::textStyle);
#else
    connect(comboStyle, SIGNAL(activated(int)),
                this, SLOT(textStyle(int)));
#endif

    comboFont = new QFontComboBox(tb);
    tb->addWidget(comboFont);
    connect(comboFont, &QFontComboBox::textActivated, this, &TextEdit::textFamily);

    comboSize = new QComboBox(tb);
    comboSize->setObjectName("comboSize");
    tb->addWidget(comboSize);
    comboSize->setEditable(true);

    foreach(int size, QFontDatabase::standardSizes())
        comboSize->addItem(QString::number(size));

    connect(comboSize, &QComboBox::textActivated, this, &TextEdit::textSize);
    comboSize->setCurrentIndex(comboSize->findText(QString::number(QApplication::font()
                                                                   .pointSize())));
}

void TextEdit::loadText(const QString &str)
{
    if (Qt::mightBeRichText(str))
        textEdit->setHtml(str);
    else
        textEdit->setPlainText(str);

    setCurrentFileName("");
}

bool TextEdit::load(const QString &f)
{
    if (!QFile::exists(f))
        return false;
    QFile file(f);
    if (!file.open(QFile::ReadOnly))
        return false;

    QByteArray data = file.readAll();
    QTextCodec *codec = Qt::codecForHtml(data);
    QString str = codec->toUnicode(data);
    if (Qt::mightBeRichText(str)) {
        textEdit->setHtml(str);
    } else {
        str = QString::fromLocal8Bit(data);
        textEdit->setPlainText(str);
    }

    setCurrentFileName(f);
    return true;
}

void TextEdit::setCurrentFileName(const QString &fileName)
{
    this->fileName = fileName;
    textEdit->document()->setModified(false);

    QString shownName;
    if (fileName.isEmpty())
        shownName = "untitled.txt";
    else
        shownName = QFileInfo(fileName).fileName();

    setWindowTitle(tr("Calaos Installer - Listing"));
    setWindowModified(false);
}

bool TextEdit::fileSave()
{
    if (fileName.isEmpty())
        return fileSaveAs();

    QTextDocumentWriter writer(fileName);
    bool success = writer.write(textEdit->document());
    if (success)
        textEdit->document()->setModified(false);
    return success;
}

bool TextEdit::fileSaveAs()
{
    QString fn = QFileDialog::getSaveFileName(this, tr("Save as..."),
                                              QString(), tr("ODF Files (*.odt);;HTML Files (*.htm *.html);;All files (*)"));
    if (fn.isEmpty())
        return false;
    if (! (fn.endsWith(".odt", Qt::CaseInsensitive) || fn.endsWith(".htm", Qt::CaseInsensitive) || fn.endsWith(".html", Qt::CaseInsensitive)) )
        fn += ".odt"; // default
    setCurrentFileName(fn);
    return fileSave();
}

void TextEdit::filePrint()
{
#ifndef QT_NO_PRINTER
    QPrinter printer(QPrinter::HighResolution);
    QPrintDialog *dlg = new QPrintDialog(&printer, this);
    if (textEdit->textCursor().hasSelection())
        dlg->setOption(QAbstractPrintDialog::PrintSelection, true);
    dlg->setWindowTitle(tr("Print the document"));
    if (dlg->exec() == QDialog::Accepted) {
        textEdit->print(&printer);
    }
    delete dlg;
#endif
}

void TextEdit::filePrintPreview()
{
#ifndef QT_NO_PRINTER
    QPrinter printer(QPrinter::HighResolution);
    QPrintPreviewDialog preview(&printer, this);
    connect(&preview, &QPrintPreviewDialog::paintRequested, this, &TextEdit::printPreview);
    preview.exec();
#endif
}

void TextEdit::printPreview(QPrinter *printer)
{
#ifdef QT_NO_PRINTER
    Q_UNUSED(printer);
#else
    textEdit->print(printer);
#endif
}


void TextEdit::filePrintPdf()
{
#ifndef QT_NO_PRINTER
    //! [0]
    QString fileName = QFileDialog::getSaveFileName(this, tr("Export in PDF"),
                                                    QString(), "*.pdf");
    if (!fileName.isEmpty()) {
        if (QFileInfo(fileName).suffix().isEmpty())
            fileName.append(".pdf");
        QPrinter printer(QPrinter::HighResolution);
        printer.setOutputFormat(QPrinter::PdfFormat);
        printer.setOutputFileName(fileName);
        textEdit->document()->print(&printer);
    }
    //! [0]
#endif
}

void TextEdit::textBold()
{
    QTextCharFormat fmt;
    fmt.setFontWeight(actionTextBold->isChecked() ? QFont::Bold : QFont::Normal);
    mergeFormatOnWordOrSelection(fmt);
}

void TextEdit::textUnderline()
{
    QTextCharFormat fmt;
    fmt.setFontUnderline(actionTextUnderline->isChecked());
    mergeFormatOnWordOrSelection(fmt);
}

void TextEdit::textItalic()
{
    QTextCharFormat fmt;
    fmt.setFontItalic(actionTextItalic->isChecked());
    mergeFormatOnWordOrSelection(fmt);
}

void TextEdit::textFamily(const QString &f)
{
    QTextCharFormat fmt;
    fmt.setFontFamily(f);
    mergeFormatOnWordOrSelection(fmt);
}

void TextEdit::textSize(const QString &p)
{
    qreal pointSize = p.toFloat();
    if (p.toFloat() > 0) {
        QTextCharFormat fmt;
        fmt.setFontPointSize(pointSize);
        mergeFormatOnWordOrSelection(fmt);
    }
}

void TextEdit::textStyle(int styleIndex)
{
    QTextCursor cursor = textEdit->textCursor();

    if (styleIndex != 0) {
        QTextListFormat::Style style = QTextListFormat::ListDisc;

        switch (styleIndex) {
        default:
        case 1:
            style = QTextListFormat::ListDisc;
            break;
        case 2:
            style = QTextListFormat::ListCircle;
            break;
        case 3:
            style = QTextListFormat::ListSquare;
            break;
        case 4:
            style = QTextListFormat::ListDecimal;
            break;
        case 5:
            style = QTextListFormat::ListLowerAlpha;
            break;
        case 6:
            style = QTextListFormat::ListUpperAlpha;
            break;
        case 7:
            style = QTextListFormat::ListLowerRoman;
            break;
        case 8:
            style = QTextListFormat::ListUpperRoman;
            break;
        }

        cursor.beginEditBlock();

        QTextBlockFormat blockFmt = cursor.blockFormat();

        QTextListFormat listFmt;

        if (cursor.currentList()) {
            listFmt = cursor.currentList()->format();
        } else {
            listFmt.setIndent(blockFmt.indent() + 1);
            blockFmt.setIndent(0);
            cursor.setBlockFormat(blockFmt);
        }

        listFmt.setStyle(style);

        cursor.createList(listFmt);

        cursor.endEditBlock();
    } else {
        // ####
        QTextBlockFormat bfmt;
        bfmt.setObjectIndex(-1);
        cursor.mergeBlockFormat(bfmt);
    }
}

void TextEdit::textColor()
{
    QColor col = QColorDialog::getColor(textEdit->textColor(), this);
    if (!col.isValid())
        return;
    QTextCharFormat fmt;
    fmt.setForeground(col);
    mergeFormatOnWordOrSelection(fmt);
    colorChanged(col);
}

void TextEdit::textAlign(QAction *a)
{
    if (a == actionAlignLeft)
        textEdit->setAlignment(Qt::AlignLeft | Qt::AlignAbsolute);
    else if (a == actionAlignCenter)
        textEdit->setAlignment(Qt::AlignHCenter);
    else if (a == actionAlignRight)
        textEdit->setAlignment(Qt::AlignRight | Qt::AlignAbsolute);
    else if (a == actionAlignJustify)
        textEdit->setAlignment(Qt::AlignJustify);
}

void TextEdit::currentCharFormatChanged(const QTextCharFormat &format)
{
    fontChanged(format.font());
    colorChanged(format.foreground().color());
}

void TextEdit::cursorPositionChanged()
{
    alignmentChanged(textEdit->alignment());
}

void TextEdit::clipboardDataChanged()
{
#ifndef QT_NO_CLIPBOARD
    actionPaste->setEnabled(!QApplication::clipboard()->text().isEmpty());
#endif
}

void TextEdit::mergeFormatOnWordOrSelection(const QTextCharFormat &format)
{
    QTextCursor cursor = textEdit->textCursor();
    if (!cursor.hasSelection())
        cursor.select(QTextCursor::WordUnderCursor);
    cursor.mergeCharFormat(format);
    textEdit->mergeCurrentCharFormat(format);
}

void TextEdit::fontChanged(const QFont &f)
{
    comboFont->setCurrentIndex(comboFont->findText(QFontInfo(f).family()));
    comboSize->setCurrentIndex(comboSize->findText(QString::number(f.pointSize())));
    actionTextBold->setChecked(f.bold());
    actionTextItalic->setChecked(f.italic());
    actionTextUnderline->setChecked(f.underline());
}

void TextEdit::colorChanged(const QColor &c)
{
    QPixmap pix(16, 16);
    pix.fill(c);
    actionTextColor->setIcon(pix);
}

void TextEdit::alignmentChanged(Qt::Alignment a)
{
    if (a & Qt::AlignLeft) {
        actionAlignLeft->setChecked(true);
    } else if (a & Qt::AlignHCenter) {
        actionAlignCenter->setChecked(true);
    } else if (a & Qt::AlignRight) {
        actionAlignRight->setChecked(true);
    } else if (a & Qt::AlignJustify) {
        actionAlignJustify->setChecked(true);
    }
}

QString TextEdit::readFile(QString f)
{
    QFile file(f);
    if (!file.open(QFile::ReadOnly))
        return QString();

    QByteArray data = file.readAll();
    QTextCodec *codec = Qt::codecForHtml(data);
    return (codec->toUnicode(data));
}

/* Report generation.
 *
 * Every IO of the project is turned into one or more report lines by
 * describeIo(). A line knows which bus it belongs to (a Wago PLC is identified
 * by its host, so two PLCs never end up mixed in the same table) and which
 * category it falls into. Types unknown to the classification tables are not
 * dropped, they land in a fallback group so the report always accounts for the
 * whole project.
 */
namespace IoReport
{
struct Tr { Q_DECLARE_TR_FUNCTIONS(IoReport) };

enum Bus
{
    BUS_WAGO = 10,
    BUS_KNX = 20,
    BUS_MYSENSORS = 30,
    BUS_MQTT = 40,
    BUS_WEB = 50,
    BUS_GPIO = 60,
    BUS_HUE = 70,
    BUS_OLA = 80,
    BUS_XPL = 90,
    BUS_ZIBASE = 95,
    BUS_1WIRE = 96,
    BUS_OTHER = 99
};

enum Category
{
    CAT_IN_DIGITAL = 100,
    CAT_IN_DIGITAL_KNX = 110,
    CAT_IN_ANALOG = 120,
    CAT_IN_TEMP = 130,
    CAT_IN_STRING = 140,
    CAT_IN_MISC = 190,
    CAT_OUT_DIGITAL = 200,
    CAT_OUT_DIGITAL_KNX = 210,
    CAT_OUT_DIMMER = 220,
    CAT_OUT_RGB = 230,
    CAT_OUT_SHUTTER = 240,
    CAT_OUT_ANALOG = 250,
    CAT_OUT_STRING = 260,
    CAT_OUT_MISC = 290,
    CAT_DALI = 300
};

struct Line
{
    QString icon;
    QString address;
    QString name;
    QString room;
    QString detail;
    QString flags;
    int sortKey = 0;
};

struct Entry
{
    int bus = BUS_OTHER;
    QString groupKey;
    QString groupTitle;
    QString catKey;
    QString catTitle;
    bool misc = false;
    Line line;
};

struct Table
{
    QString title;
    QVector<Line> lines;
};

struct Group
{
    QString title;
    QMap<QString, Table> tables;
    int count = 0;
};

static const QString ICON_SWITCH = QStringLiteral("<img src=\":/img/icon_inter.png\" />");
static const QString ICON_LIGHT = QStringLiteral("<img src=\":/img/icon_light_on.png\" />");
static const QString ICON_TOR = QStringLiteral("<img src=\":/img/icon_tor_on.png\" />");
static const QString ICON_SHUTTER = QStringLiteral("<img src=\":/img/icon_shutter.png\" />");
static const QString ICON_ANALOG = QStringLiteral("<img src=\":/img/icon_analog.png\" />");
static const QString ICON_TEMP = QStringLiteral("<img src=\":/img/temp.png\" />");
static const QString ICON_STRING = QStringLiteral("<img src=\":/img/text.png\" />");
static const QString ICON_INT = QStringLiteral("<img src=\":/img/icon_int.png\" />");
static const QString ICON_BOOL = QStringLiteral("<img src=\":/img/icon_bool_on.png\" />");
static const QString ICON_SCENARIO = QStringLiteral("<img src=\":/img/icon_scenario.png\" />");
static const QString ICON_CLOCK = QStringLiteral("<img src=\":/img/icon_clock.png\" />");
static const QString ICON_CAMERA = QStringLiteral("<img src=\":/img/icon_camera_on.png\" />");
static const QString ICON_SOUND = QStringLiteral("<img src=\":/img/icon_sound.png\" />");

QString esc(const std::string &s)
{
    return QString::fromUtf8(s.c_str()).toHtmlEscaped();
}

QString esc(const QString &s)
{
    return s.toHtmlEscaped();
}

QString param(IOBase *io, const char *key)
{
    return QString::fromUtf8(io->get_param(key).c_str());
}

/* Alias names registered in IOBase.cpp all describe the same hardware. The
 * report must not care which spelling the config file uses. */
QString canonicalType(const QString &type)
{
    static QHash<QString, QString> aliases;
    if (aliases.isEmpty())
    {
        aliases.insert(QStringLiteral("WIDigital"), QStringLiteral("WIDigitalBP"));
        aliases.insert(QStringLiteral("WagoInputSwitch"), QStringLiteral("WIDigitalBP"));
        aliases.insert(QStringLiteral("WagoInputSwitchTriple"), QStringLiteral("WIDigitalTriple"));
        aliases.insert(QStringLiteral("WagoInputSwitchLongPress"), QStringLiteral("WIDigitalLong"));
        aliases.insert(QStringLiteral("WagoInputTemp"), QStringLiteral("WITemp"));
        aliases.insert(QStringLiteral("WagoInputAnalog"), QStringLiteral("WIAnalog"));
        aliases.insert(QStringLiteral("WagoOutputLight"), QStringLiteral("WODigital"));
        aliases.insert(QStringLiteral("WagoOutputShutter"), QStringLiteral("WOVolet"));
        aliases.insert(QStringLiteral("WagoOutputShutterSmart"), QStringLiteral("WOVoletSmart"));
        aliases.insert(QStringLiteral("WagoOutputDimmer"), QStringLiteral("WODali"));
        aliases.insert(QStringLiteral("WagoOutputDimmerRGB"), QStringLiteral("WODaliRVB"));
        aliases.insert(QStringLiteral("WagoOutputAnalog"), QStringLiteral("WOAnalog"));
    }

    return aliases.value(type, type);
}

int busForType(const QString &type)
{
    static QHash<QString, int> buses;
    if (buses.isEmpty())
    {
        const char *wago[] = { "WIDigitalBP", "WIDigitalTriple", "WIDigitalLong", "WITemp",
                               "WIAnalog", "WODigital", "WOVolet", "WOVoletSmart", "WODali",
                               "WODaliRVB", "WOAnalog", "WONeon", 0 };
        const char *knx[] = { "KNXInputSwitch", "KNXInputSwitchTriple", "KNXInputSwitchLongPress",
                              "KNXInputAnalog", "KNXInputTemp", "KNXOutputLight",
                              "KNXOutputLightDimmer", "KNXOutputLightRGB", "KNXOutputAnalog",
                              "KNXOutputShutter", "KNXOutputShutterSmart", 0 };
        const char *mysensors[] = { "MySensorsInputSwitch", "MySensorsInputSwitchTriple",
                                    "MySensorsInputSwitchLongPress", "MySensorsInputAnalog",
                                    "MySensorsInputTemp", "MySensorsInputString",
                                    "MySensorsOutputLight", "MySensorsOutputDimmer",
                                    "MySensorsOutputLightRGB", "MySensorsOutputAnalog",
                                    "MySensorsOutputShutter", "MySensorsOutputShutterSmart",
                                    "MySensorsOutputString", 0 };
        const char *mqtt[] = { "MqttInputSwitch", "MqttInputAnalog", "MqttInputTemp",
                               "MqttInputString", "MqttOutputLight", "MqttOutputLightDimmer",
                               "MqttOutputLightRGB", "MqttOutputAnalog", "MqttOutputShutter", 0 };
        const char *web[] = { "WebInputSwitch", "WebInputAnalog", "WebInputTemp", "WebInputString",
                              "WebOutputLight", "WebOutputLightRGB", "WebOutputAnalog",
                              "WebOutputString", 0 };
        const char *gpio[] = { "GpioInputSwitch", "GpioInputSwitchTriple",
                               "GpioInputSwitchLongPress", "GpioOutputSwitch",
                               "GpioOutputShutter", "GpioOutputShutterSmart", 0 };
        const char *hue[] = { "HueOutputLightRGB", 0 };
        const char *ola[] = { "OLAOutputLightDimmer", "OLAOutputLightRGB", 0 };
        const char *xpl[] = { "xPLInputSwitch", "xPLInputAnalog", "xPLInputTemp", "xPLInputString",
                              "xPLOutputSwitch", "xPLOutputAnalog", "xPLOutputString", 0 };
        const char *zibase[] = { "ZibaseDigitalIn", "ZibaseAnalogIn", "ZibaseTemp",
                                 "ZibaseDigitalOut", 0 };
        const char *onewire[] = { "OWTemp", 0 };

        struct { const char **list; int bus; } all[] = {
            { wago, BUS_WAGO }, { knx, BUS_KNX }, { mysensors, BUS_MYSENSORS },
            { mqtt, BUS_MQTT }, { web, BUS_WEB }, { gpio, BUS_GPIO }, { hue, BUS_HUE },
            { ola, BUS_OLA }, { xpl, BUS_XPL }, { zibase, BUS_ZIBASE }, { onewire, BUS_1WIRE },
        };

        for (unsigned int i = 0;i < sizeof(all) / sizeof(all[0]);i++)
        {
            for (int j = 0;all[i].list[j];j++)
                buses.insert(QString::fromLatin1(all[i].list[j]), all[i].bus);
        }
    }

    return buses.value(type, BUS_OTHER);
}

QString busName(int bus)
{
    switch (bus)
    {
    case BUS_WAGO: return QStringLiteral("Wago");
    case BUS_KNX: return QStringLiteral("KNX");
    case BUS_MYSENSORS: return QStringLiteral("MySensors");
    case BUS_MQTT: return QStringLiteral("MQTT");
    case BUS_WEB: return QStringLiteral("Web");
    case BUS_GPIO: return QStringLiteral("GPIO");
    case BUS_HUE: return QStringLiteral("Philips Hue");
    case BUS_OLA: return QStringLiteral("OLA / DMX");
    case BUS_XPL: return QStringLiteral("xPL");
    case BUS_ZIBASE: return QStringLiteral("Zibase");
    case BUS_1WIRE: return QStringLiteral("1-Wire");
    default: return Tr::tr("Others / unclassified");
    }
}

/* Sortable key for a host: dotted IPv4 addresses sort numerically and come
 * first, anything else sorts alphabetically after them. */
QString hostSortKey(const QString &host)
{
    const QStringList parts = host.split(QLatin1Char('.'));
    if (parts.size() == 4)
    {
        QString key = QStringLiteral("0");
        for (int i = 0;i < parts.size();i++)
        {
            bool ok = false;
            const int v = parts[i].toInt(&ok);
            if (!ok || v < 0 || v > 255)
                break;
            key += QString("%1").arg(v, 3, 10, QLatin1Char('0'));
        }
        if (key.size() == 13)
            return key;
    }

    return QStringLiteral("1") + host.toLower();
}

QString catKeyOf(int order, const QString &discriminator = QString())
{
    return QString("%1|%2").arg(QString("%1").arg(order, 4, 10, QLatin1Char('0')), discriminator);
}

int daliLine(IOBase *io, const char *lineParam)
{
    QString l = param(io, lineParam);
    if (l.isEmpty())
        l = param(io, "line");

    const int v = l.toInt();
    return v > 0 ? v : 1;
}

QString guiTypeLabel(const QString &guiType, bool isInput)
{
    if (guiType == QLatin1String("switch")) return Tr::tr("Digital inputs");
    if (guiType == QLatin1String("switch3")) return Tr::tr("Digital inputs");
    if (guiType == QLatin1String("switch_long")) return Tr::tr("Digital inputs");
    if (guiType == QLatin1String("analog_in")) return Tr::tr("Analog inputs");
    if (guiType == QLatin1String("temp")) return Tr::tr("Temperature inputs");
    if (guiType == QLatin1String("string_in")) return Tr::tr("String inputs");
    if (guiType == QLatin1String("light")) return Tr::tr("Digital outputs");
    if (guiType == QLatin1String("light_dimmer")) return Tr::tr("Dimmers");
    if (guiType == QLatin1String("light_rgb")) return Tr::tr("RGB lights");
    if (guiType == QLatin1String("shutter")) return Tr::tr("Shutters");
    if (guiType == QLatin1String("shutter_smart")) return Tr::tr("Shutters");
    if (guiType == QLatin1String("analog_out")) return Tr::tr("Analog outputs");
    if (guiType == QLatin1String("string_out")) return Tr::tr("String outputs");
    if (guiType == QLatin1String("var_bool")) return Tr::tr("Internal booleans");
    if (guiType == QLatin1String("var_int")) return Tr::tr("Internal integers");
    if (guiType == QLatin1String("var_string")) return Tr::tr("Internal strings");
    if (guiType == QLatin1String("scenario")) return Tr::tr("Scenarios");
    if (guiType == QLatin1String("timer")) return Tr::tr("Timers");
    if (guiType == QLatin1String("time")) return Tr::tr("Time events");
    if (guiType == QLatin1String("time_range")) return Tr::tr("Time ranges");
    if (guiType == QLatin1String("camera")) return Tr::tr("Cameras");
    if (guiType == QLatin1String("audio")) return Tr::tr("Audio zones");
    if (guiType == QLatin1String("avreceiver")) return Tr::tr("AV receivers");
    if (guiType == QLatin1String("remote_ui")) return Tr::tr("Remote UI");
    if (guiType.isEmpty()) return isInput ? Tr::tr("Other inputs") : Tr::tr("Other outputs");

    return Tr::tr("Other (%1)").arg(guiType);
}

int guiTypeCategory(const QString &guiType, bool isInput)
{
    if (guiType == QLatin1String("switch") ||
        guiType == QLatin1String("switch3") ||
        guiType == QLatin1String("switch_long")) return CAT_IN_DIGITAL;
    if (guiType == QLatin1String("analog_in")) return CAT_IN_ANALOG;
    if (guiType == QLatin1String("temp")) return CAT_IN_TEMP;
    if (guiType == QLatin1String("string_in")) return CAT_IN_STRING;
    if (guiType == QLatin1String("light")) return CAT_OUT_DIGITAL;
    if (guiType == QLatin1String("light_dimmer")) return CAT_OUT_DIMMER;
    if (guiType == QLatin1String("light_rgb")) return CAT_OUT_RGB;
    if (guiType == QLatin1String("shutter") ||
        guiType == QLatin1String("shutter_smart")) return CAT_OUT_SHUTTER;
    if (guiType == QLatin1String("analog_out")) return CAT_OUT_ANALOG;
    if (guiType == QLatin1String("string_out")) return CAT_OUT_STRING;

    return isInput ? CAT_IN_MISC : CAT_OUT_MISC;
}

QString guiTypeIcon(const QString &guiType)
{
    if (guiType == QLatin1String("switch") ||
        guiType == QLatin1String("switch3") ||
        guiType == QLatin1String("switch_long")) return ICON_SWITCH;
    if (guiType == QLatin1String("analog_in") ||
        guiType == QLatin1String("analog_out")) return ICON_ANALOG;
    if (guiType == QLatin1String("temp")) return ICON_TEMP;
    if (guiType == QLatin1String("string_in") ||
        guiType == QLatin1String("string_out") ||
        guiType == QLatin1String("var_string")) return ICON_STRING;
    if (guiType == QLatin1String("light") ||
        guiType == QLatin1String("light_dimmer") ||
        guiType == QLatin1String("light_rgb")) return ICON_LIGHT;
    if (guiType == QLatin1String("shutter") ||
        guiType == QLatin1String("shutter_smart")) return ICON_SHUTTER;
    if (guiType == QLatin1String("var_int")) return ICON_INT;
    if (guiType == QLatin1String("var_bool")) return ICON_BOOL;
    if (guiType == QLatin1String("scenario")) return ICON_SCENARIO;
    if (guiType == QLatin1String("timer") ||
        guiType == QLatin1String("time") ||
        guiType == QLatin1String("time_range")) return ICON_CLOCK;
    if (guiType == QLatin1String("camera")) return ICON_CAMERA;
    if (guiType == QLatin1String("audio") ||
        guiType == QLatin1String("avreceiver")) return ICON_SOUND;

    return ICON_TOR;
}

/* Best effort address for a bus that is not the Wago one. */
QString genericAddress(IOBase *io, int bus)
{
    switch (bus)
    {
    case BUS_KNX:
        return param(io, "knx_group");
    case BUS_MYSENSORS:
    {
        const QString node = param(io, "node_id");
        const QString sensor = param(io, "sensor_id");
        if (node.isEmpty() && sensor.isEmpty())
            return QString();
        return QString("%1/%2").arg(node, sensor);
    }
    case BUS_MQTT:
    {
        QString topic = param(io, "topic_sub");
        if (topic.isEmpty()) topic = param(io, "topic_pub");
        if (topic.isEmpty()) topic = param(io, "path");
        return topic;
    }
    case BUS_WEB:
        return param(io, "url");
    case BUS_GPIO:
        return param(io, "gpio");
    case BUS_HUE:
        return param(io, "id_hue");
    case BUS_1WIRE:
        return param(io, "ow_id");
    default:
        break;
    }

    static const char *fallback[] = { "var", "address", "url", "gpio", 0 };
    for (int i = 0;fallback[i];i++)
    {
        const QString v = param(io, fallback[i]);
        if (!v.isEmpty())
            return v;
    }

    return QString();
}

QVector<Entry> describeIo(IOBase *io, bool isInput)
{
    QVector<Entry> entries;

    const QString type = param(io, "type");
    const QString canon = canonicalType(type);
    const QString guiType = QString::fromUtf8(io->get_gui_type().c_str());
    const int bus = busForType(canon);
    const QString host = param(io, "host");
    const QString port = param(io, "port");
    const bool knx = param(io, "knx") == QLatin1String("true");

    QString groupTitle;
    if (bus == BUS_WAGO)
    {
        groupTitle = QString("Wago %1").arg(host.isEmpty() ? Tr::tr("(host not set)") : esc(host));
        if (!port.isEmpty() && port != QLatin1String("502"))
            groupTitle += QLatin1Char(':') + esc(port);
    }
    else if (bus == BUS_OTHER || host.isEmpty())
        groupTitle = busName(bus);
    else
        groupTitle = QString("%1 (%2)").arg(busName(bus), esc(host));

    /* The fallback group is a single bucket, it must not be split per host. */
    const QString busKey = QString("%1").arg(bus, 3, 10, QLatin1Char('0'));
    const QString groupKey = bus == BUS_OTHER ? busKey
                             : QString("%1|%2|%3").arg(busKey, hostSortKey(host), port);

    QString flags;
    if (param(io, "enabled") == QLatin1String("false"))
        flags = Tr::tr("disabled");
    if (knx)
        flags = flags.isEmpty() ? QStringLiteral("KNX") : flags + QStringLiteral(", KNX");

    const QString baseName = esc(io->get_param("name"));

    auto add = [&](const QString &catKey, const QString &catTitle, const QString &icon,
                   const QString &address, const QString &nameSuffix, const QString &detail,
                   bool misc)
    {
        Entry e;
        e.bus = bus;
        e.groupKey = groupKey;
        e.groupTitle = groupTitle;
        e.catKey = catKey;
        e.catTitle = catTitle;
        e.misc = misc;
        e.line.icon = icon;
        e.line.address = esc(address);
        e.line.name = baseName + nameSuffix;
        e.line.detail = detail;
        e.line.flags = flags;
        e.line.sortKey = address.toInt();
        entries.append(e);
    };

    if (bus != BUS_WAGO)
    {
        const int cat = guiTypeCategory(guiType, isInput);
        const bool misc = (cat == CAT_IN_MISC || cat == CAT_OUT_MISC);
        add(catKeyOf(cat, misc ? guiType : QString()), guiTypeLabel(guiType, isInput),
            guiTypeIcon(guiType), genericAddress(io, bus), QString(), QString(), misc);
        return entries;
    }

    const int digitalIn = knx ? CAT_IN_DIGITAL_KNX : CAT_IN_DIGITAL;
    const QString digitalInTitle = knx ? Tr::tr("KNX digital inputs") : Tr::tr("Digital inputs");
    const int digitalOut = knx ? CAT_OUT_DIGITAL_KNX : CAT_OUT_DIGITAL;
    const QString digitalOutTitle = knx ? Tr::tr("KNX digital outputs") : Tr::tr("Digital outputs");

    if (canon == QLatin1String("WIDigitalBP") ||
        canon == QLatin1String("WIDigitalTriple") ||
        canon == QLatin1String("WIDigitalLong"))
    {
        QString detail = Tr::tr("Push button");
        if (canon == QLatin1String("WIDigitalTriple")) detail = Tr::tr("Triple click");
        else if (canon == QLatin1String("WIDigitalLong")) detail = Tr::tr("Long press");

        add(catKeyOf(digitalIn), digitalInTitle, ICON_SWITCH, param(io, "var"),
            QString(), detail, false);
    }
    else if (canon == QLatin1String("WITemp"))
    {
        add(catKeyOf(CAT_IN_TEMP), Tr::tr("Temperature inputs"), ICON_TEMP, param(io, "var"),
            QString(), QString(), false);
    }
    else if (canon == QLatin1String("WIAnalog"))
    {
        add(catKeyOf(CAT_IN_ANALOG), Tr::tr("Analog inputs"), ICON_ANALOG, param(io, "var"),
            QString(), QString(), false);
    }
    else if (canon == QLatin1String("WODigital"))
    {
        const QString icon = param(io, "gtype") == QLatin1String("light") ? ICON_LIGHT : ICON_TOR;
        add(catKeyOf(digitalOut), digitalOutTitle, icon, param(io, "var"),
            QString(), QString(), false);
    }
    else if (canon == QLatin1String("WOVolet") || canon == QLatin1String("WOVoletSmart"))
    {
        add(catKeyOf(digitalOut), digitalOutTitle, ICON_SHUTTER, param(io, "var_up"),
            QString(), Tr::tr("Shutter (up)"), false);
        add(catKeyOf(digitalOut), digitalOutTitle, ICON_SHUTTER, param(io, "var_down"),
            QString(), Tr::tr("Shutter (down)"), false);

        /* WOVoletSmart keeps the shutter position in a PLC word, it takes a real
         * address and must show up in the report. */
        if (io->get_params().Exists("var_save"))
            add(catKeyOf(digitalOut), digitalOutTitle, ICON_SHUTTER, param(io, "var_save"),
                QString(), Tr::tr("Shutter (position save)"), false);
    }
    else if (canon == QLatin1String("WODali"))
    {
        const bool group = param(io, "group") == QLatin1String("1");
        const int line = daliLine(io, "line");
        add(catKeyOf(CAT_DALI + line * 10 + (group ? 1 : 0),
                     QString::number(line) + (group ? QStringLiteral("g") : QString())),
            group ? Tr::tr("DALI line %1 (groups)").arg(line) : Tr::tr("DALI line %1").arg(line),
            ICON_LIGHT, param(io, "address"), QString(),
            group ? Tr::tr("Group") : QString(), false);
    }
    else if (canon == QLatin1String("WODaliRVB"))
    {
        struct Channel
        {
            const char *address;
            const char *line;
            const char *group;
            QString suffix;
        };

        const Channel channels[] = {
            { "raddress", "rline", "rgroup", Tr::tr(" (Red)") },
            { "gaddress", "gline", "ggroup", Tr::tr(" (Green)") },
            { "baddress", "bline", "bgroup", Tr::tr(" (Blue)") },
        };

        for (unsigned int i = 0;i < sizeof(channels) / sizeof(channels[0]);i++)
        {
            const bool group = param(io, channels[i].group) == QLatin1String("1");
            const int line = daliLine(io, channels[i].line);
            add(catKeyOf(CAT_DALI + line * 10 + (group ? 1 : 0),
                         QString::number(line) + (group ? QStringLiteral("g") : QString())),
                group ? Tr::tr("DALI line %1 (groups)").arg(line) : Tr::tr("DALI line %1").arg(line),
                ICON_LIGHT, param(io, channels[i].address), channels[i].suffix,
                group ? Tr::tr("Group") : QString(), false);
        }
    }
    else if (canon == QLatin1String("WONeon"))
    {
        add(catKeyOf(digitalOut), digitalOutTitle, ICON_LIGHT, param(io, "var_relay"),
            QString(), Tr::tr("Neon (relay output)"), false);
        add(catKeyOf(CAT_OUT_ANALOG), Tr::tr("Analog outputs"), ICON_ANALOG, param(io, "var"),
            QString(), Tr::tr("Neon (analog output)"), false);
    }
    else if (canon == QLatin1String("WOAnalog"))
    {
        add(catKeyOf(CAT_OUT_ANALOG), Tr::tr("Analog outputs"), ICON_ANALOG, param(io, "var"),
            QString(), QString(), false);
    }
    else
    {
        const int cat = guiTypeCategory(guiType, isInput);
        const bool misc = (cat == CAT_IN_MISC || cat == CAT_OUT_MISC);
        add(catKeyOf(cat, misc ? guiType : QString()), guiTypeLabel(guiType, isInput),
            guiTypeIcon(guiType), genericAddress(io, bus), QString(), QString(), misc);
    }

    return entries;
}

bool lineLessThan(const Line &l1, const Line &l2)
{
    if (l1.sortKey != l2.sortKey)
        return l1.sortKey < l2.sortKey;
    return l1.name.localeAwareCompare(l2.name) < 0;
}

QString roomLabel(Room *r)
{
    const QString name = esc(r->get_name());
    const QString type = esc(r->get_type());

    if (type.isEmpty() || type.compare(name, Qt::CaseInsensitive) == 0)
        return name;

    return QString("%1 (%2)").arg(name, type);
}

void collect(IOBase *io, bool isInput, const QString &room, QMap<QString, Group> &groups)
{
    const QVector<Entry> entries = describeIo(io, isInput);

    for (int i = 0;i < entries.size();i++)
    {
        const Entry &e = entries[i];
        Group &g = groups[e.groupKey];
        g.title = e.groupTitle;

        Table &t = g.tables[e.catKey];
        t.title = e.catTitle;

        Line l = e.line;
        l.room = room;
        t.lines.append(l);
        g.count++;
    }
}

}

void TextEdit::loadIOList(const QString &projectName)
{
    const QString header = readFile(":/home_header.html");
    const QString footer = readFile(":/home_footer.html");
    const QString groupTpl = readFile(":/home_group.html");
    const QString tableFooter = readFile(":/home_room_footer.html");
    const QString itemHeader = readFile(":/home_item_header.html");
    const QString item = readFile(":/home_item.html");

    QMap<QString, IoReport::Group> groups;

    for (int i = 0;i < ListeRoom::Instance().size();i++)
    {
        Room *r = ListeRoom::Instance().get_room(i);
        const QString room = IoReport::roomLabel(r);

        /* An inout IO is registered both as an input and as an output of its
         * room, it must only be reported once. */
        QSet<IOBase *> seen;

        for (int j = 0;j < r->get_size_in();j++)
        {
            IOBase *io = r->get_input(j);
            if (seen.contains(io))
                continue;
            seen.insert(io);
            IoReport::collect(io, true, room, groups);
        }

        for (int j = 0;j < r->get_size_out();j++)
        {
            IOBase *io = r->get_output(j);
            if (seen.contains(io))
                continue;
            seen.insert(io);
            IoReport::collect(io, false, room, groups);
        }
    }

    QString title = tr("I/O list");
    if (!projectName.isEmpty())
        title = tr("I/O list - %1").arg(projectName);

    QString html = header.arg(IoReport::esc(title));

    for (QMap<QString, IoReport::Group>::const_iterator git = groups.constBegin();
         git != groups.constEnd(); ++git)
    {
        const IoReport::Group &g = git.value();

        html += groupTpl.arg(g.title,
                             tr("%1 line(s) in %2 table(s)")
                             .arg(QString::number(g.count), QString::number(g.tables.size())));

        for (QMap<QString, IoReport::Table>::const_iterator tit = g.tables.constBegin();
             tit != g.tables.constEnd(); ++tit)
        {
            QVector<IoReport::Line> lines = tit.value().lines;
            std::stable_sort(lines.begin(), lines.end(), IoReport::lineLessThan);

            html += itemHeader.arg(QString(), tit.value().title, tr("Name"),
                                   tr("Room"), tr("Type"), tr("Flags"));

            for (int i = 0;i < lines.size();i++)
            {
                const IoReport::Line &l = lines[i];
                html += item.arg(l.icon, l.address, l.name, l.room, l.detail, l.flags);
            }

            html += tableFooter;
        }
    }

    html += footer;

    textEdit->setHtml(html);
}

void TextEdit::loadRooms(const QString &projectName)
{
    const QString header = readFile(":/home_header.html");
    const QString footer = readFile(":/home_footer.html");
    const QString roomTpl = readFile(":/home_room.html");
    const QString tableFooter = readFile(":/home_room_footer.html");
    const QString inputs = readFile(":/home_inputs.html");
    const QString outputs = readFile(":/home_outputs.html");
    const QString item = readFile(":/home_item.html");

    QString title = tr("By room");
    if (!projectName.isEmpty())
        title = tr("By room - %1").arg(projectName);

    QString html = header.arg(IoReport::esc(title));

    for (int i = 0;i < ListeRoom::Instance().size();i++)
    {
        Room *r = ListeRoom::Instance().get_room(i);

        if (r->get_type() == "Internal")
            continue;

        html += roomTpl.arg(tr("Room"), IoReport::esc(r->get_name()));

        QSet<IOBase *> seen;

        html += inputs.arg(QString(), tr("Input"), tr("Name"), tr("Type"),
                           tr("Detail"), tr("Flags"));

        for (int j = 0;j < r->get_size_in();j++)
        {
            IOBase *io = r->get_input(j);
            if (seen.contains(io))
                continue;
            seen.insert(io);

            const QVector<IoReport::Entry> entries = IoReport::describeIo(io, true);
            for (int k = 0;k < entries.size();k++)
            {
                if (entries[k].misc)
                    continue;

                const IoReport::Line &l = entries[k].line;
                html += item.arg(l.icon, l.address, l.name, entries[k].catTitle,
                                 l.detail, l.flags);
            }
        }

        html += outputs.arg(QString(), tr("Output"), tr("Name"), tr("Type"),
                            tr("Detail"), tr("Flags"));

        for (int j = 0;j < r->get_size_out();j++)
        {
            IOBase *io = r->get_output(j);
            if (seen.contains(io))
                continue;
            seen.insert(io);

            const QVector<IoReport::Entry> entries = IoReport::describeIo(io, false);
            for (int k = 0;k < entries.size();k++)
            {
                if (entries[k].misc)
                    continue;

                const IoReport::Line &l = entries[k].line;
                html += item.arg(l.icon, l.address, l.name, entries[k].catTitle,
                                 l.detail, l.flags);
            }
        }

        html += tableFooter;
    }

    html += footer;

    textEdit->setHtml(html);
}

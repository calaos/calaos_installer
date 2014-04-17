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
    connect(textEdit, SIGNAL(currentCharFormatChanged(QTextCharFormat)),
            this, SLOT(currentCharFormatChanged(QTextCharFormat)));
    connect(textEdit, SIGNAL(cursorPositionChanged()),
            this, SLOT(cursorPositionChanged()));

    setCentralWidget(textEdit);
    textEdit->setFocus();
    setCurrentFileName(QString());

    fontChanged(textEdit->font());
    colorChanged(textEdit->textColor());
    alignmentChanged(textEdit->alignment());

    connect(textEdit->document(), SIGNAL(modificationChanged(bool)),
            this, SLOT(setWindowModified(bool)));
    connect(textEdit->document(), SIGNAL(undoAvailable(bool)),
            actionUndo, SLOT(setEnabled(bool)));
    connect(textEdit->document(), SIGNAL(redoAvailable(bool)),
            actionRedo, SLOT(setEnabled(bool)));

    setWindowModified(textEdit->document()->isModified());
    actionUndo->setEnabled(textEdit->document()->isUndoAvailable());
    actionRedo->setEnabled(textEdit->document()->isRedoAvailable());

    connect(actionUndo, SIGNAL(triggered()), textEdit, SLOT(undo()));
    connect(actionRedo, SIGNAL(triggered()), textEdit, SLOT(redo()));

    actionCut->setEnabled(false);
    actionCopy->setEnabled(false);

    connect(actionCut, SIGNAL(triggered()), textEdit, SLOT(cut()));
    connect(actionCopy, SIGNAL(triggered()), textEdit, SLOT(copy()));
    connect(actionPaste, SIGNAL(triggered()), textEdit, SLOT(paste()));

    connect(textEdit, SIGNAL(copyAvailable(bool)), actionCut, SLOT(setEnabled(bool)));
    connect(textEdit, SIGNAL(copyAvailable(bool)), actionCopy, SLOT(setEnabled(bool)));

#ifndef QT_NO_CLIPBOARD
    connect(QApplication::clipboard(), SIGNAL(dataChanged()), this, SLOT(clipboardDataChanged()));
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
    connect(a, SIGNAL(triggered()), this, SLOT(fileSaveAs()));
    menu->addAction(a);
    menu->addSeparator();

#ifndef QT_NO_PRINTER
    a = new QAction(QIcon::fromTheme("document-print", QIcon(rsrcPath + "/fileprint.png")),
                    tr("&Print..."), this);
    a->setPriority(QAction::LowPriority);
    a->setShortcut(QKeySequence::Print);
    connect(a, SIGNAL(triggered()), this, SLOT(filePrint()));
    tb->addAction(a);
    menu->addAction(a);

    a = new QAction(QIcon::fromTheme("fileprint", QIcon(rsrcPath + "/fileprint.png")),
                    tr("Print preview..."), this);
    connect(a, SIGNAL(triggered()), this, SLOT(filePrintPreview()));
    menu->addAction(a);

    a = new QAction(QIcon::fromTheme("exportpdf", QIcon(rsrcPath + "/exportpdf.png")),
                    tr("&Export in PDF..."), this);
    a->setPriority(QAction::LowPriority);
    a->setShortcut(Qt::CTRL + Qt::Key_D);
    connect(a, SIGNAL(triggered()), this, SLOT(filePrintPdf()));
    tb->addAction(a);
    menu->addAction(a);

    menu->addSeparator();
#endif

    a = new QAction(tr("&Close"), this);
    a->setShortcut(Qt::CTRL + Qt::Key_Q);
    connect(a, SIGNAL(triggered()), this, SLOT(close()));
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
    actionTextBold->setShortcut(Qt::CTRL + Qt::Key_B);
    actionTextBold->setPriority(QAction::LowPriority);
    QFont bold;
    bold.setBold(true);
    actionTextBold->setFont(bold);
    connect(actionTextBold, SIGNAL(triggered()), this, SLOT(textBold()));
    tb->addAction(actionTextBold);
    menu->addAction(actionTextBold);
    actionTextBold->setCheckable(true);

    actionTextItalic = new QAction(QIcon::fromTheme("format-text-italic", QIcon(rsrcPath + "/textitalic.png")),
                                   tr("&Italic"), this);
    actionTextItalic->setPriority(QAction::LowPriority);
    actionTextItalic->setShortcut(Qt::CTRL + Qt::Key_I);
    QFont italic;
    italic.setItalic(true);
    actionTextItalic->setFont(italic);
    connect(actionTextItalic, SIGNAL(triggered()), this, SLOT(textItalic()));
    tb->addAction(actionTextItalic);
    menu->addAction(actionTextItalic);
    actionTextItalic->setCheckable(true);

    actionTextUnderline = new QAction(QIcon::fromTheme("format-text-underline", QIcon(rsrcPath + "/textunder.png")),
                                      tr("&Underline"), this);
    actionTextUnderline->setShortcut(Qt::CTRL + Qt::Key_U);
    actionTextUnderline->setPriority(QAction::LowPriority);
    QFont underline;
    underline.setUnderline(true);
    actionTextUnderline->setFont(underline);
    connect(actionTextUnderline, SIGNAL(triggered()), this, SLOT(textUnderline()));
    tb->addAction(actionTextUnderline);
    menu->addAction(actionTextUnderline);
    actionTextUnderline->setCheckable(true);

    menu->addSeparator();

    QActionGroup *grp = new QActionGroup(this);
    connect(grp, SIGNAL(triggered(QAction*)), this, SLOT(textAlign(QAction*)));

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

    actionAlignLeft->setShortcut(Qt::CTRL + Qt::Key_L);
    actionAlignLeft->setCheckable(true);
    actionAlignLeft->setPriority(QAction::LowPriority);
    actionAlignCenter->setShortcut(Qt::CTRL + Qt::Key_E);
    actionAlignCenter->setCheckable(true);
    actionAlignCenter->setPriority(QAction::LowPriority);
    actionAlignRight->setShortcut(Qt::CTRL + Qt::Key_R);
    actionAlignRight->setCheckable(true);
    actionAlignRight->setPriority(QAction::LowPriority);
    actionAlignJustify->setShortcut(Qt::CTRL + Qt::Key_J);
    actionAlignJustify->setCheckable(true);
    actionAlignJustify->setPriority(QAction::LowPriority);

    tb->addActions(grp->actions());
    menu->addActions(grp->actions());

    menu->addSeparator();

    QPixmap pix(16, 16);
    pix.fill(Qt::black);
    actionTextColor = new QAction(pix, tr("Color..."), this);
    connect(actionTextColor, SIGNAL(triggered()), this, SLOT(textColor()));
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
    connect(comboStyle, SIGNAL(activated(int)),
            this, SLOT(textStyle(int)));

    comboFont = new QFontComboBox(tb);
    tb->addWidget(comboFont);
    connect(comboFont, SIGNAL(activated(QString)),
            this, SLOT(textFamily(QString)));

    comboSize = new QComboBox(tb);
    comboSize->setObjectName("comboSize");
    tb->addWidget(comboSize);
    comboSize->setEditable(true);

    QFontDatabase db;
    foreach(int size, db.standardSizes())
        comboSize->addItem(QString::number(size));

    connect(comboSize, SIGNAL(activated(QString)),
            this, SLOT(textSize(QString)));
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
        dlg->addEnabledOption(QAbstractPrintDialog::PrintSelection);
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
    connect(&preview, SIGNAL(paintRequested(QPrinter*)), SLOT(printPreview(QPrinter*)));
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

bool _sort_input_by_var(IOBase *in1, IOBase *in2)
{
    int var1, var2;
    from_string(in1->get_param("var"), var1);
    from_string(in2->get_param("var"), var2);

    return (var1 < var2);
}

bool _sort_output_by_var(IOBase *out1, IOBase *out2)
{
    int var1, var2;
    from_string(out1->get_param("var"), var1);
    from_string(out2->get_param("var"), var2);

    return (var1 < var2);
}

bool _sort_output_dali(IOBase *out1, IOBase *out2)
{
    int var1, var2;
    from_string(out1->get_param("address"), var1);
    from_string(out2->get_param("address"), var2);

    return (var1 < var2);
}

void TextEdit::loadIOList()
{
    QString html;

    QString header = readFile(":/home_header.html").arg("Maison");
    QString footer = readFile(":/home_footer.html");
    QString room = readFile(":/home_room.html");
    QString room_footer = readFile(":/home_room_footer.html");
    QString inputs = readFile(":/home_inputs.html");
    QString outputs = readFile(":/home_outputs.html");
    QString item_header = readFile(":/home_item_header.html");
    QString item = readFile(":/home_item.html");

    html += header;

    vector<IOBase *> wago_inputs_digital;
    vector<IOBase *> wago_inputs_digital_knx;
    vector<IOBase *> wago_inputs_analog;
    vector<IOBase *> wago_inputs_temp;

    vector<IOBase *> wago_outputs_digital;
    vector<IOBase *> wago_outputs_digital_knx;
    vector<IOBase *> wago_outputs_dali;
    vector<IOBase *> wago_outputs_dali_group;
    vector<IOBase *> wago_outputs_analog;

    vector<IOBase *> temp_ios; //handle temporary IOs

    for (int i = 0;i < ListeRoom::Instance().size();i++)
    {
        Room *r = ListeRoom::Instance().get_room(i);

        for (int j = 0;j < r->get_size_in();j++)
        {
            IOBase *in = r->get_input(j);

            if (in->get_param("type") == "WIDigitalBP" ||
                in->get_param("type") == "WIDigitalTriple" ||
                in->get_param("type") == "WIDigitalLong")
            {
                IOBase *nin = new IOBase(*in);
                nin->set_param("room_name", r->get_name());
                nin->set_param("room_type", r->get_type());

                if (in->get_param("knx") == "true")
                    wago_inputs_digital_knx.push_back(nin);
                else
                    wago_inputs_digital.push_back(nin);

                temp_ios.push_back(nin);
            }
            else if (in->get_param("type") == "WITemp")
            {
                IOBase *nin = new IOBase(*in);
                nin->set_param("room_name", r->get_name());
                nin->set_param("room_type", r->get_type());

                wago_inputs_temp.push_back(nin);

                temp_ios.push_back(nin);
            }
            else if (in->get_param("type") == "WIAnalog")
            {
                IOBase *nin = new IOBase(*in);
                nin->set_param("room_name", r->get_name());
                nin->set_param("room_type", r->get_type());

                wago_inputs_analog.push_back(nin);

                temp_ios.push_back(nin);
            }
        }

        for (int j = 0;j < r->get_size_out();j++)
        {
            IOBase *out = r->get_output(j);

            if (out->get_param("type") == "WODigital")
            {
                IOBase *nout = new IOBase(*out);
                nout->set_param("room_name", r->get_name());
                nout->set_param("room_type", r->get_type());

                if (out->get_param("knx") == "true")
                    wago_outputs_digital_knx.push_back(nout);
                else
                    wago_outputs_digital.push_back(nout);

                temp_ios.push_back(nout);
            }
            else if (out->get_param("type") == "WOVolet" ||
                     out->get_param("type") == "WOVoletSmart")
            {
                IOBase *volet_up = new IOBase(*out);
                IOBase *volet_down = new IOBase(*out);

                volet_up->set_param("room_name", r->get_name());
                volet_up->set_param("room_type", r->get_type());

                volet_down->set_param("room_name", r->get_name());
                volet_down->set_param("room_type", r->get_type());

                volet_up->get_params().Add("name", out->get_param("name") + " (Montée)");
                volet_down->get_params().Add("name", out->get_param("name") + " (Descente)");

                volet_up->get_params().Add("var", out->get_param("var_up"));
                volet_down->get_params().Add("var", out->get_param("var_down"));

                if (out->get_param("knx") == "true")
                {
                    wago_outputs_digital_knx.push_back(volet_up);
                    wago_outputs_digital_knx.push_back(volet_down);
                }
                else
                {
                    wago_outputs_digital.push_back(volet_up);
                    wago_outputs_digital.push_back(volet_down);
                }

                temp_ios.push_back(volet_up);
                temp_ios.push_back(volet_down);
            }
            else if (out->get_param("type") == "WODali")
            {
                IOBase *nout = new IOBase(*out);
                nout->set_param("room_name", r->get_name());
                nout->set_param("room_type", r->get_type());

                if (out->get_param("group") == "1")
                    wago_outputs_dali_group.push_back(nout);
                else
                    wago_outputs_dali.push_back(nout);

                temp_ios.push_back(nout);
            }
            else if (out->get_param("type") == "WODaliRVB")
            {
                IOBase *dali_r = new IOBase(*out);
                IOBase *dali_g = new IOBase(*out);
                IOBase *dali_b = new IOBase(*out);

                dali_r->set_param("room_name", r->get_name());
                dali_r->set_param("room_type", r->get_type());
                dali_g->set_param("room_name", r->get_name());
                dali_g->set_param("room_type", r->get_type());
                dali_b->set_param("room_name", r->get_name());
                dali_b->set_param("room_type", r->get_type());

                dali_r->get_params().Add("name", out->get_param("name") + " (Rouge)");
                dali_g->get_params().Add("name", out->get_param("name") + " (Vert)");
                dali_b->get_params().Add("name", out->get_param("name") + " (Bleu)");

                dali_r->get_params().Add("address", out->get_param("raddress"));
                dali_g->get_params().Add("address", out->get_param("gaddress"));
                dali_b->get_params().Add("address", out->get_param("baddress"));

                if (out->get_param("rgroup") == "1")
                    wago_outputs_dali_group.push_back(dali_r);
                else
                    wago_outputs_dali.push_back(dali_r);

                if (out->get_param("ggroup") == "1")
                    wago_outputs_dali_group.push_back(dali_g);
                else
                    wago_outputs_dali.push_back(dali_g);

                if (out->get_param("bgroup") == "1")
                    wago_outputs_dali_group.push_back(dali_b);
                else
                    wago_outputs_dali.push_back(dali_b);

                temp_ios.push_back(dali_r);
                temp_ios.push_back(dali_g);
                temp_ios.push_back(dali_b);
            }
            else if (out->get_param("type") == "WOAnalog")
            {
                IOBase *nout = new IOBase(*out);
                nout->set_param("room_name", r->get_name());
                nout->set_param("room_type", r->get_type());

                wago_outputs_analog.push_back(nout);

                temp_ios.push_back(nout);
            }
        }
    }

    // Sort by var
    sort(wago_inputs_digital.begin(), wago_inputs_digital.end(), _sort_input_by_var);
    sort(wago_inputs_digital_knx.begin(), wago_inputs_digital_knx.end(), _sort_input_by_var);
    sort(wago_inputs_analog.begin(), wago_inputs_analog.end(), _sort_input_by_var);
    sort(wago_inputs_temp.begin(), wago_inputs_temp.end(), _sort_input_by_var);

    sort(wago_outputs_digital.begin(), wago_outputs_digital.end(), _sort_output_by_var);
    sort(wago_outputs_digital_knx.begin(), wago_outputs_digital_knx.end(), _sort_output_by_var);
    sort(wago_outputs_dali.begin(), wago_outputs_dali.end(), _sort_output_dali);
    sort(wago_outputs_dali_group.begin(), wago_outputs_dali_group.end(), _sort_output_dali);
    sort(wago_outputs_analog.begin(), wago_outputs_analog.end(), _sort_output_by_var);

    if (!wago_inputs_digital.empty())
    {
        html += item_header.arg("")
                .arg(tr("Wago Input"))
                .arg(tr("Name"))
                .arg("")
                .arg("");
        for (int i = 0;i < wago_inputs_digital.size();i++)
        {
            QString icon, var;
            IOBase *io = wago_inputs_digital[i];

            icon = "<img src=\":/img/icon_inter.png\" />";
            var = io->get_param("var").c_str();

            html += item.arg(icon)
                    .arg(var)
                    .arg(QString::fromUtf8(io->get_param("name").c_str()))
                    .arg(QString::fromUtf8(io->get_param("room_name").c_str()))
                    .arg(QString::fromUtf8(io->get_param("room_type").c_str()));
        }
        html += room_footer;
    }

    if (!wago_inputs_digital_knx.empty())
    {
        html += item_header.arg("")
                .arg(tr("KNX Input"))
                .arg(tr("Name"))
                .arg("")
                .arg("");
        for (int i = 0;i < wago_inputs_digital_knx.size();i++)
        {
            QString icon, var;
            IOBase *io = wago_inputs_digital_knx[i];

            icon = "<img src=\":/img/icon_inter.png\" />";
            var = io->get_param("var").c_str();

            html += item.arg(icon)
                    .arg(var)
                    .arg(QString::fromUtf8(io->get_param("name").c_str()))
                    .arg(QString::fromUtf8(io->get_param("room_name").c_str()))
                    .arg(QString::fromUtf8(io->get_param("room_type").c_str()));
        }
        html += room_footer;
    }

    if (!wago_inputs_analog.empty())
    {
        html += item_header.arg("")
                .arg(tr("Analog Wago input"))
                .arg(tr("Name"))
                .arg("")
                .arg("");
        for (int i = 0;i < wago_inputs_analog.size();i++)
        {
            QString icon, var;
            IOBase *io = wago_inputs_analog[i];

            icon = "<img src=\":/img/icon_analog.png\" />";
            var = io->get_param("var").c_str();

            html += item.arg(icon)
                    .arg(var)
                    .arg(QString::fromUtf8(io->get_param("name").c_str()))
                    .arg(QString::fromUtf8(io->get_param("room_name").c_str()))
                    .arg(QString::fromUtf8(io->get_param("room_type").c_str()));
        }
        html += room_footer;
    }

    if (!wago_inputs_temp.empty())
    {
        html += item_header.arg("")
                .arg(tr("Temperature input"))
                .arg(tr("Name"))
                .arg("")
                .arg("");
        for (int i = 0;i < wago_inputs_temp.size();i++)
        {
            QString icon, var;
            IOBase *io = wago_inputs_temp[i];

            icon = "<img src=\":/img/temp.png\" />";
            var = io->get_param("var").c_str();

            html += item.arg(icon)
                    .arg(var)
                    .arg(QString::fromUtf8(io->get_param("name").c_str()))
                    .arg(QString::fromUtf8(io->get_param("room_name").c_str()))
                    .arg(QString::fromUtf8(io->get_param("room_type").c_str()));
        }
        html += room_footer;
    }

    if (!wago_outputs_digital.empty())
    {
        html += item_header.arg("")
                .arg(tr("Wago output"))
                .arg(tr("Name"))
                .arg("")
                .arg("");
        for (int i = 0;i < wago_outputs_digital.size();i++)
        {
            QString icon, var;
            IOBase *io = wago_outputs_digital[i];

            if (io->get_param("type") == "WODigital")
                icon = "<img src=\":/img/icon_light_on.png\" />";
            else if (io->get_param("type") == "WOVolet" || io->get_param("type") == "WOVoletSmart")
                icon = "<img src=\":/img/icon_shutter.png\" />";

            var = io->get_param("var").c_str();

            html += item.arg(icon)
                    .arg(var)
                    .arg(QString::fromUtf8(io->get_param("name").c_str()))
                    .arg(QString::fromUtf8(io->get_param("room_name").c_str()))
                    .arg(QString::fromUtf8(io->get_param("room_type").c_str()));
        }
        html += room_footer;
    }

    if (!wago_outputs_digital_knx.empty())
    {
        html += item_header.arg("")
                .arg(tr("KNX Output"))
                .arg(tr("Name"))
                .arg("")
                .arg("");
        for (int i = 0;i < wago_outputs_digital_knx.size();i++)
        {
            QString icon, var;
            IOBase *io = wago_outputs_digital_knx[i];

            if (io->get_param("type") == "WODigital")
                icon = "<img src=\":/img/icon_light_on.png\" />";
            else if (io->get_param("type") == "WOVolet" || io->get_param("type") == "WOVoletSmart")
                icon = "<img src=\":/img/icon_shutter.png\" />";

            var = io->get_param("var").c_str();

            html += item.arg(icon)
                    .arg(var)
                    .arg(QString::fromUtf8(io->get_param("name").c_str()))
                    .arg(QString::fromUtf8(io->get_param("room_name").c_str()))
                    .arg(QString::fromUtf8(io->get_param("room_type").c_str()));
        }
        html += room_footer;
    }

    if (!wago_outputs_dali.empty())
    {
        html += item_header.arg("")
                .arg(tr("DALI Output"))
                .arg(tr("Name"))
                .arg("")
                .arg("");
        for (int i = 0;i < wago_outputs_dali.size();i++)
        {
            QString icon, var;
            IOBase *io = wago_outputs_dali[i];

            icon = "<img src=\":/img/icon_light_on.png\" />";

            var = io->get_param("address").c_str();

            html += item.arg(icon)
                    .arg(var)
                    .arg(QString::fromUtf8(io->get_param("name").c_str()))
                    .arg(QString::fromUtf8(io->get_param("room_name").c_str()))
                    .arg(QString::fromUtf8(io->get_param("room_type").c_str()));
        }
        html += room_footer;
    }

    if (!wago_outputs_dali_group.empty())
    {
        html += item_header.arg("")
                .arg(tr("DALI Output (Group)"))
                .arg(tr("Name"))
                .arg("")
                .arg("");
        for (int i = 0;i < wago_outputs_dali_group.size();i++)
        {
            QString icon, var;
            IOBase *io = wago_outputs_dali_group[i];

            icon = "<img src=\":/img/icon_light_on.png\" />";

            var = io->get_param("address").c_str();

            html += item.arg(icon)
                    .arg(var)
                    .arg(QString::fromUtf8(io->get_param("name").c_str()))
                    .arg(QString::fromUtf8(io->get_param("room_name").c_str()))
                    .arg(QString::fromUtf8(io->get_param("room_type").c_str()));
        }
        html += room_footer;
    }

    if (!wago_outputs_analog.empty())
    {
        html += item_header.arg("")
                .arg(tr("Analog Output"))
                .arg(tr("Name"))
                .arg("")
                .arg("");
        for (int i = 0;i < wago_outputs_analog.size();i++)
        {
            QString icon, var;
            IOBase *io = wago_outputs_analog[i];

            icon = "<img src=\":/img/icon_analog.png\" />";

            var = io->get_param("var").c_str();

            html += item.arg(icon)
                    .arg(var)
                    .arg(QString::fromUtf8(io->get_param("name").c_str()))
                    .arg(QString::fromUtf8(io->get_param("room_name").c_str()))
                    .arg(QString::fromUtf8(io->get_param("room_type").c_str()));
        }
        html += room_footer;
    }

    //Free temp memory
    for (int i = 0;i < temp_ios.size();i++)
    {
        delete temp_ios[i];
    }
    temp_ios.clear();

    html += footer;

    textEdit->setHtml(html);
}

void TextEdit::loadRooms()
{
    QString html;

    QString header = readFile(":/home_header.html").arg("Maison");
    QString footer = readFile(":/home_footer.html");
    QString room = readFile(":/home_room.html");
    QString room_footer = readFile(":/home_room_footer.html");
    QString inputs = readFile(":/home_inputs.html");
    QString outputs = readFile(":/home_outputs.html");
    QString item = readFile(":/home_item.html");

    html += header;

    for (int i = 0;i < ListeRoom::Instance().size();i++)
    {
        Room *r = ListeRoom::Instance().get_room(i);

        if (r->get_type() == "Internal")
            continue;

        html += room.arg(QString::fromUtf8(r->get_name().c_str()));

        html += inputs;
        for (int j = 0;j < r->get_size_in();j++)
        {
            IOBase *in = r->get_input(j);

            QString icon, var, knx;

            if (in->get_param("type") == "WIDigitalBP" ||
                in->get_param("type") == "WIDigitalTriple" ||
                in->get_param("type") == "WIDigitalLong")
            {
                icon = "<img src=\":/img/icon_inter.png\" />";
                var = in->get_param("var").c_str();
                if (in->get_param("knx") == "true")
                    knx = "<img src=\":/img/checkbox.png\" />";
            }
            else if (in->get_param("type") == "WITemp")
            {
                icon = "<img src=\":/img/temp.png\" />";
                var = in->get_param("var").c_str();
            }
            else if (in->get_param("type") == "OWTemp")
            {
                icon = "<img src=\":/img/temp.png\" />";
                var = in->get_param("ow_id").c_str();
            }
            else
                continue;

            html += item.arg(icon)
                    .arg(var)
                    .arg(QString::fromUtf8(in->get_param("name").c_str()))
                    .arg("")
                    .arg(knx);
        }

        html += outputs;
        for (int j = 0;j < r->get_size_out();j++)
        {
            IOBase *out = r->get_output(j);

            QString icon, name, var, dali, knx;

            name = QString::fromUtf8(out->get_param("name").c_str());

            if (out->get_param("type") == "WODigital")
            {
                if (out->get_param("gtype") == "light")
                    icon = "<img src=\":/img/icon_light_on.png\" />";
                else
                    icon = "<img src=\":/img/icon_tor_on.png\" />";
                var = out->get_param("var").c_str();
                if (out->get_param("knx") == "true")
                    knx = "<img src=\":/img/checkbox.png\" />";

            }
            else if (out->get_param("type") == "WODali")
            {
                icon = "<img src=\":/img/icon_light_on.png\" />";
                var = out->get_param("address").c_str();
                if (out->get_param("group") == "1")
                    var += tr(" (Group)");
                dali = "<img src=\":/img/checkbox.png\" />";
            }
            else if (out->get_param("type") == "WODaliRVB")
            {
                icon = "<img src=\":/img/icon_light_on.png\" />";
                dali = "<img src=\":/img/checkbox.png\" />";

                var = out->get_param("raddress").c_str();
                if (out->get_param("rgroup") == "1")
                    var += tr(" (Red Group)");
                else
                    var += tr(" (Red)");

                html += item.arg(icon)
                        .arg(var)
                        .arg(name)
                        .arg(dali)
                        .arg(knx);

                var = out->get_param("gaddress").c_str();
                if (out->get_param("ggroup") == "1")
                    var += tr(" (Green Group)");
                else
                    var += tr(" (Green)");

                html += item.arg(icon)
                        .arg(var)
                        .arg(name)
                        .arg(dali)
                        .arg(knx);

                var = out->get_param("baddress").c_str();
                if (out->get_param("bgroup") == "1")
                    var += tr(" (Blue Group)");
                else
                    var += tr(" (Blue)");
            }
            else if (out->get_param("type") == "WONeon")
            {
                icon = "<img src=\":/img/icon_light_on.png\" />";
                var = out->get_param("var_relay").c_str();
                var += tr(" (Relay output)");

                html += item.arg(icon)
                        .arg(var)
                        .arg(name)
                        .arg(dali)
                        .arg(knx);

                var = out->get_param("var_relay").c_str();
                var += tr(" (Analog output)");
            }
            else if (out->get_param("type") == "WOVolet" || out->get_param("type") == "WOVoletSmart")
            {
                icon = "<img src=\":/img/icon_shutter.png\" />";
                var = out->get_param("var_up").c_str();
                var += tr(" (Up)");

                html += item.arg(icon)
                        .arg(var)
                        .arg(name)
                        .arg(dali)
                        .arg(knx);

                var = out->get_param("var_down").c_str();
                var += tr(" (Down)");
            }
            else
                continue;

            html += item.arg(icon)
                    .arg(var)
                    .arg(name)
                    .arg(dali)
                    .arg(knx);
        }

        html += room_footer;
    }

    html += footer;

    textEdit->setHtml(html);
}

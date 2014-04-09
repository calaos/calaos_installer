#ifndef DIALOGSCRIPTEDITOR_H
#define DIALOGSCRIPTEDITOR_H

#include <QtWidgets>
#include <QDialog>
#include "CodeEditor.h"
#include <ScriptManager.h>

namespace Ui {
class DialogScriptEditor;
}

class DialogScriptEditor : public QDialog
{
    Q_OBJECT

public:
    explicit DialogScriptEditor(QString script, QWidget *parent = 0);
    ~DialogScriptEditor();

    QString getScript();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::DialogScriptEditor *ui;

    CodeEditor *codeEditor;
    bool scriptError;

    bool Execute();

private slots:
    void on_pushButton_Ok_clicked();
    void on_pushButton_Valid_clicked();
    void on_print_message(QString msg);
};

class LuaPrinter: public QObject
{
    Q_OBJECT

public:
    static LuaPrinter &Instance()
    {
        static LuaPrinter lp;
        return lp;
    }

    void Print(QString msg) { emit print(msg); }

signals:
    void print(QString msg);

private:
    LuaPrinter(): QObject() {}
};

#endif // DIALOGSCRIPTEDITOR_H

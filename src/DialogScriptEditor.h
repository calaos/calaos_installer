#ifndef DIALOGSCRIPTEDITOR_H
#define DIALOGSCRIPTEDITOR_H

#include <QtGui>
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
};

#endif // DIALOGSCRIPTEDITOR_H

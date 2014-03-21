#ifndef IOEDITORSELECTION_H
#define IOEDITORSELECTION_H

#include <QtWidgets>
#include <QHBoxLayout>
#include <IOBase.h>
#include <QFrame>

using namespace Calaos;

class IOEditorSelection : public QFrame
{
        public:
                IOEditorSelection(QWidget *parent = 0);

                QComboBox *getComboBox() { return combobox; }
                QPushButton *getButton() { return button; }

                Input *other_input;
                Output *other_output;

        private:
                QComboBox *combobox;
                QPushButton *button;

};

#endif // IOEDITORSELECTION_H

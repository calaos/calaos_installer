#ifndef IOEDITORSELECTION_H
#define IOEDITORSELECTION_H

#include <QtGui>
#include <QHBoxLayout>

class IOEditorSelection : public QFrame
{
        public:
                IOEditorSelection(QWidget *parent = 0);

                QComboBox *getComboBox() { return combobox; }
                QPushButton *getButton() { return button; }

        private:
                QComboBox *combobox;
                QPushButton *button;

};

#endif // IOEDITORSELECTION_H

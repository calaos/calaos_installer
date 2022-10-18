#ifndef IOEDITORSELECTION_H
#define IOEDITORSELECTION_H

#include <QHBoxLayout>
#include <IOBase.h>
#include <QFrame>
#include <QComboBox>
#include <QPushButton>

using namespace Calaos;

class IOEditorSelection : public QFrame
{
public:
    IOEditorSelection(QWidget *parent = 0);

    QComboBox *getComboBox() { return combobox; }
    QPushButton *getButton() { return button; }

    IOBase *other_input;
    IOBase *other_output;

private:
    QComboBox *combobox;
    QPushButton *button;

};

#endif // IOEDITORSELECTION_H

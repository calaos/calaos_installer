#include "ioeditorselection.h"

IOEditorSelection::IOEditorSelection(QWidget *parent): QFrame(parent)
{
        QHBoxLayout *box = new QHBoxLayout(this);
        combobox = new QComboBox(this);
        button = new QPushButton("...", this);

        box->setMargin(0);

//        QSizePolicy sizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
//        sizePolicy.setHorizontalStretch(0);
//        sizePolicy.setVerticalStretch(0);
//        sizePolicy.setHeightForWidth(combobox->sizePolicy().hasHeightForWidth());
//        combobox->setSizePolicy(sizePolicy);

        box->addWidget(combobox, 1);

//        QSizePolicy sizePolicy1(QSizePolicy::Minimum, QSizePolicy::Fixed);
//        sizePolicy1.setHorizontalStretch(0);
//        sizePolicy1.setVerticalStretch(0);
//        sizePolicy1.setHeightForWidth(button->sizePolicy().hasHeightForWidth());
//        button->setSizePolicy(sizePolicy1);

        box->addWidget(button, 0);
}

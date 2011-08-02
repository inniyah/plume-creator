#include "editmenubox.h"

EditMenuBox::EditMenuBox(QWidget *parent) :
    QFrame(parent)
{
    QGridLayout *baseGridLayout = new QGridLayout;
    QToolButton *button = new QToolButton;

    baseGridLayout->addWidget(button,0,0);

    setLayout(baseGridLayout);
}

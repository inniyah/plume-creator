#include "attendbox.h"
#include "ui_attendbox.h"

AttendBox::AttendBox(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::AttendBox)
{
    ui->setupUi(this);
}

AttendBox::~AttendBox()
{
    delete ui;
}

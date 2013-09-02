#include "findreplace.h"
#include "ui_findreplace.h"

FindReplace::FindReplace(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FindReplace)
{
    ui->setupUi(this);

}

void FindReplace::postConstructor()
{



    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Warning);
    msgBox.setText("The Find & Replace feature isn't implemented !<br>For now, it's only a mock-up GUI. Please wait for a future release !"
                      " Update : it will be available for 0.63");
    msgBox.exec();




}

FindReplace::~FindReplace()
{
    delete ui;
}












//-----------------------------------------------------------------------------------
//--------Tree-----------------------------------------------------------------
//-----------------------------------------------------------------------------------



void FindReplace::createTree()
{
    domDocument = hub->mainTreeDomDoc();
    root = domDocument.documentElement();

}

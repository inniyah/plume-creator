#include "attendsettings.h"
#include "ui_attendsettings.h"

AttendSettings::AttendSettings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AttendSettings)
{
    ui->setupUi(this);

    QList<QPushButton *> buttonList = this->findChildren<QPushButton *>();
    foreach(QPushButton *pb, buttonList) {
        pb->setDefault( false );
        pb->setAutoDefault( false );
    }

}

AttendSettings::~AttendSettings()
{
    delete ui;
}
void AttendSettings::postConstructor()
{




    QDomElement root = hub->attendTreeDomDoc().documentElement().toElement();



    ui->spinBox_1_labelEdit->setText(root.attribute("spinBox_1_label", tr("Age :")));


    QStringList box1List = root.attribute("box_1", tr("None")).split("--", QString::SkipEmptyParts);
    foreach(const QString &string, box1List)
        new QListWidgetItem(string, ui->listWidget_1st);

    QStringList box2List = root.attribute("box_2", tr("None")).split("--", QString::SkipEmptyParts);
    foreach(const QString &string, box2List)
        new QListWidgetItem(string, ui->listWidget_2nd);

    QStringList box3List = root.attribute("box_3", tr("None")).split("--", QString::SkipEmptyParts);
    foreach(const QString &string, box3List)
        new QListWidgetItem(string, ui->listWidget_3rd);


    connect(ui->listWidget_1st, SIGNAL(itemActivated(QListWidgetItem*)), this, SLOT(itemActivatedSlot_1(QListWidgetItem*)));
    connect(ui->listWidget_2nd, SIGNAL(itemActivated(QListWidgetItem*)), this, SLOT(itemActivatedSlot_2(QListWidgetItem*)));
    connect(ui->listWidget_3rd, SIGNAL(itemActivated(QListWidgetItem*)), this, SLOT(itemActivatedSlot_3(QListWidgetItem*)));
    connect(ui->listWidget_1st, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(itemActivatedSlot_1(QListWidgetItem*)));
    connect(ui->listWidget_2nd, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(itemActivatedSlot_2(QListWidgetItem*)));
    connect(ui->listWidget_3rd, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(itemActivatedSlot_3(QListWidgetItem*)));

    connect(ui->addButton_1st, SIGNAL(clicked()), this, SLOT(addItem_1()));
    connect(ui->addButton_2nd, SIGNAL(clicked()), this, SLOT(addItem_2()));
    connect(ui->addButton_3rd, SIGNAL(clicked()), this, SLOT(addItem_3()));

    connect(ui->removeButton_1st, SIGNAL(clicked()), this, SLOT(removeItem_1()));
    connect(ui->removeButton_2nd, SIGNAL(clicked()), this, SLOT(removeItem_2()));
    connect(ui->removeButton_3rd, SIGNAL(clicked()), this, SLOT(removeItem_3()));

    connect(ui->listWidget_1st, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(renameItem_1()));
    connect(ui->listWidget_2nd, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(renameItem_2()));
    connect(ui->listWidget_3rd, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(renameItem_3()));

    // select the first of each list :
    ui->listWidget_1st->setCurrentItem( ui->listWidget_1st->item(0));
    ui->listWidget_2nd->setCurrentItem( ui->listWidget_2nd->item(0));
    ui->listWidget_3rd->setCurrentItem( ui->listWidget_3rd->item(0));



}

//-------------------------------------------------------------------------------------------

void AttendSettings::itemActivatedSlot_1(QListWidgetItem *item)
{
    itemActivated_1 = item;
    itemActivated_1->setFlags(Qt::ItemIsEditable	| itemActivated_1->flags());
}
void AttendSettings::itemActivatedSlot_2(QListWidgetItem *item)
{
    itemActivated_2 = item;
    itemActivated_2->setFlags(Qt::ItemIsEditable	| itemActivated_2->flags());
}
void AttendSettings::itemActivatedSlot_3(QListWidgetItem *item)
{
    itemActivated_3 = item;
    itemActivated_3->setFlags(Qt::ItemIsEditable	| itemActivated_3->flags());
}

//-------------------------------------------------------------------------------------------

void AttendSettings::addItem_1()
{
    new QListWidgetItem(tr("new"), ui->listWidget_1st);
}
void AttendSettings::addItem_2()
{
    new QListWidgetItem(tr("new"), ui->listWidget_2nd);

}
void AttendSettings::addItem_3()
{
    new QListWidgetItem(tr("new"), ui->listWidget_3rd);

}

//-------------------------------------------------------------------------------------------

void AttendSettings::removeItem_1()
{
    ui->listWidget_1st->takeItem(ui->listWidget_1st->row(itemActivated_1));

}
void AttendSettings::removeItem_2()
{
    ui->listWidget_2nd->takeItem(ui->listWidget_2nd->row(itemActivated_2));

}
void AttendSettings::removeItem_3()
{
    ui->listWidget_3rd->takeItem(ui->listWidget_3rd->row(itemActivated_3));
}

//-------------------------------------------------------------------------------------------

void AttendSettings::renameItem_1()
{
ui->listWidget_1st->editItem(itemActivated_1);

}
void AttendSettings::renameItem_2()
{
    ui->listWidget_2nd->editItem(itemActivated_2);

}
void AttendSettings::renameItem_3()
{
    ui->listWidget_3rd->editItem(itemActivated_3);

}

void AttendSettings::accept()
{
    QDomElement root = hub->attendTreeDomDoc().documentElement().toElement();

    QStringList list_1;
    int i;
    for(i = 0; i < ui->listWidget_1st->count(); ++i)
        list_1.append(ui->listWidget_1st->item(i)->text());
QString stringBox_1;
    foreach(const QString &string, list_1)
        stringBox_1.append("--" + string);
        root.setAttribute("box_1", stringBox_1);

    QStringList list_2;

    for(i = 0; i < ui->listWidget_2nd->count(); ++i)
        list_2.append(ui->listWidget_2nd->item(i)->text());
    QString stringBox_2;
    foreach(const QString &string, list_2)
        stringBox_2.append("--" + string);
        root.setAttribute("box_2",stringBox_2 );

    QStringList list_3;

    for(i = 0; i < ui->listWidget_3rd->count(); ++i)
        list_3.append(ui->listWidget_3rd->item(i)->text());
    QString stringBox_3;
    foreach(const QString &string, list_3)
        stringBox_3.append("--" + string);
        root.setAttribute("box_3", stringBox_3);

        root.setAttribute("spinBox_1_label", ui->spinBox_1_labelEdit->text());


    hub->addToSaveQueue();

    QDialog::accept();
}

#include "structurepage.h"
#include "ui_structurepage.h"

StructurePage::StructurePage(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::StructurePage)
{
    ui->setupUi(this);

    this->registerField("bookCountSpinBox", ui->bookCountSpinBox);
    this->registerField("actCountSpinBox", ui->actCountSpinBox);
    this->registerField("chapterCountSpinBox", ui->chapterCountSpinBox);
    this->registerField("sceneCountSpinBox", ui->sceneCountSpinBox);


}

void StructurePage::initializePage()
{
    m_bookCount = 1; //minimum and default
    resetPreviewTree();
    fillTemplateList();
}

StructurePage::~StructurePage()
{
    delete ui;
}

void StructurePage::on_bookCountSpinBox_valueChanged(int value)
{
    m_bookCount = value;

    resetPreviewTree();
}

void StructurePage::on_actCountSpinBox_valueChanged(int value)
{
    m_actCount = value;

    if(value == 0){
        ui->chapterCountSpinBox->setValue(0);
        ui->sceneCountSpinBox->setValue(0);
    }

    resetPreviewTree();
}

void StructurePage::on_chapterCountSpinBox_valueChanged(int value)
{
    m_chapterCount = value;

    if(value == 0)
        ui->sceneCountSpinBox->setValue(0);

    resetPreviewTree();

}

void StructurePage::on_sceneCountSpinBox_valueChanged(int value)
{
    m_sceneCount = value;
    resetPreviewTree();
}

void StructurePage::resetPreviewTree()
{
    ui->bookPreviewTreeWidget->clear();





    for(int i = 0; i  < m_bookCount; ++i){
        QTreeWidgetItem *b = createItem("book", ui->bookPreviewTreeWidget->invisibleRootItem());


        if(m_actCount == 0)
            for(int j = 0; j  < m_chapterCount; ++j){
                QTreeWidgetItem *c = createItem("chapter", b);
                for(int k = 0; k  < m_sceneCount; ++k){
                    this->createItem("scene", c);

                }
            }
        else
            for(int j = 0; j  < m_actCount; ++j){
                QTreeWidgetItem *a = createItem("act", b);
                for(int k = 0; k  < m_chapterCount; ++k){
                    QTreeWidgetItem *c = createItem("chapter", a);
                    for(int l = 0; l  < m_sceneCount; ++l){
                        this->createItem("scene", c);

                    }
                }
            }

    }

    ui->bookPreviewTreeWidget->expandAll();
}

QTreeWidgetItem *StructurePage::createItem(QString type, QTreeWidgetItem *parentItem)
{
    QTreeWidgetItem *item = new QTreeWidgetItem(parentItem);

    if(type == "book"){
        item->setText(0, tr("Book"));
        item->setIcon(0, QIcon(":/pics/tree/tree-book.png"));
    }
    if(type == "act"){
        item->setText(0,tr("Act"));
        item->setIcon(0, QIcon(":/pics/tree/tree-act.png"));
    }
    if(type == "chapter"){
        item->setText(0,tr("Chapter"));
        item->setIcon(0, QIcon(":/pics/tree/tree-chapter.png"));
    }
    if(type == "scene"){
        item->setText(0,tr("Scene"));
        item->setIcon(0, QIcon(":/pics/tree/tree-scene.png"));
    }
    return item;
}


void StructurePage::fillTemplateList()
{
    QListWidgetItem *item = new QListWidgetItem(tr("Short story"),ui->bookTemplatesListWidget);
    QList<QVariant> list;
    list << 1 << 0 << 3 << 0;// book// act// chapter// scene
    item->setData(32, list);


    item = new QListWidgetItem(tr("Short novel"),ui->bookTemplatesListWidget);
 list.clear();
 list << 1 << 0 << 20 << 5;// book// act// chapter// scene
    item->setData(32, list);


    item = new QListWidgetItem(tr("Novel"),ui->bookTemplatesListWidget);
    list.clear();
    list << 1 << 3 << 10 << 10;// book// act// chapter// scene
    item->setData(32, list);


    item = new QListWidgetItem(tr("Long novel"),ui->bookTemplatesListWidget);
    list.clear();
    list << 1 << 5 << 5 << 10;// book// act// chapter// scene
    item->setData(32, list);






}

void StructurePage::on_bookTemplatesListWidget_itemActivated(QListWidgetItem *item)
{
    QList<QVariant> list = item->data(32).toList();

    ui->bookCountSpinBox->setValue(list.at(0).toInt());
    ui->actCountSpinBox->setValue(list.at(1).toInt());
    ui->chapterCountSpinBox->setValue(list.at(2).toInt());
    ui->sceneCountSpinBox->setValue(list.at(3).toInt());

    resetPreviewTree();

}

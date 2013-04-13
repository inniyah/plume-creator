#include "attendmanager.h"
#include "ui_attendmanager.h"

AttendManager::AttendManager(QWidget *parent, AttendAbstractModel *absModel) :
    QDialog(parent), textAlreadyChanged(false),
    ui(new Ui::AttendManager)
{
    ui->setupUi(this);

    abstractModel = absModel;

    QList<QPushButton *> buttonList = this->findChildren<QPushButton *>();
    foreach(QPushButton *pb, buttonList) {
        pb->setDefault( false );
        pb->setAutoDefault( false );
    }
}


//-----------------------------------------------------------------------------------------------------------------------

void AttendManager::postConstructor()
{
    managerProxyModel = new AttendManagerTreeProxyModel;
    managerProxyModel->setHub(hub);
    managerProxyModel->postConstructor();
    managerProxyModel->setSourceModel(abstractModel);
    ui->managerTreeView->setModel(managerProxyModel );


    connect(managerProxyModel, SIGNAL(resetDomDocSignal()), abstractModel, SLOT(resetAbsModel()));
    connect(managerProxyModel, SIGNAL(resetDomElementForNumberSignal()), this, SLOT(resetDomElementForNumber()));
    connect(managerProxyModel, SIGNAL(setNameSignal(QString)), this, SLOT(setNameSlot(QString)));
    connect(managerProxyModel, SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SLOT(expandAll()));
    connect(managerProxyModel, SIGNAL(activateItemSignal(QModelIndex)), this, SLOT(setItemActivated(QModelIndex)));
    ui->textEdit->setHub(hub);




    QMenu *addMenu = new QMenu;
    addMenu->addAction(ui->addObjectAction);
    addMenu->addAction(ui->addGroupAction);
    ui->addButton->setMenu(addMenu);
    ui->addButton->addAction(ui->addObjectAction);

    ui->removeButton->addAction(ui->removeAction);

    connect(ui->managerTreeView, SIGNAL(activated(QModelIndex)), this, SLOT(setItemActivated(QModelIndex)));
    connect(ui->managerTreeView, SIGNAL(clicked(QModelIndex)), this, SLOT(setItemActivated(QModelIndex)));
    connect(ui->managerTreeView, SIGNAL(clicked(QModelIndex)), this, SLOT(editItemTitle(QModelIndex)));
    connect(ui->settingsButton, SIGNAL(clicked()), this, SLOT(launchAttendSettings()));
    connect(hub, SIGNAL(textAlreadyChangedSignal(bool)), this, SLOT(textAlreadyChangedSlot(bool)));
    resetDomElementForNumber();

    this->applySettings();
    this->connectAll();

    ui->optionsFrame->setEnabled(false);


    // open First :

    //QHash<int, QDomElement>::iterator i = domElementForNumber.begin();
    //  openedElement = i.value();

}


//-----------------------------------------------------------------------------------------------------------------------


AttendManager::~AttendManager()
{
    delete managerProxyModel;

    delete ui;
}

void AttendManager::setItemActivated(QModelIndex index)
{
    ui->optionsFrame->setEnabled(true);

    AttendTreeItem *sourceItem = static_cast<AttendTreeItem*>(managerProxyModel->mapToSource(index).internalPointer());

    attendTreeItemActivated = sourceItem;

    int number = sourceItem->idNumber();
    if (number == 0)
        return;
    this->disconnectAll();

    openedElement = domElementForNumber.value(number);


    ui->nameEdit->setText(openedElement.attribute("name", ""));
    ui->aliasesEdit->setText(openedElement.attribute("aliases", ""));
    ui->quickDetailsEdit->setText(openedElement.attribute("quickDetails", ""));


    ui->comboBox_1->setCurrentIndex(openedElement.attribute("box_1", "0").toInt());
    ui->comboBox_2->setCurrentIndex(openedElement.attribute("box_2", "0").toInt());
    ui->comboBox_3->setCurrentIndex(openedElement.attribute("box_3", "0").toInt());

    ui->spinBox_1->setValue(openedElement.attribute("spinBox_1", "20").toInt());

    ui->textEdit->openAttendDetail(hub->attendTree_numForDocHash().key(number));



    this->connectAll();

}


//-----------------------------------------------------------------------------------------------------------------------

void AttendManager::resetDomElementForNumber()
{
    domElementForNumber.clear();


    QDomDocument attendDomDoc = hub->attendTreeDomDoc();
    QDomElement root = attendDomDoc.documentElement();

    QDomNode m = root.firstChild();
    while(!m.isNull()) { // group level
        QDomElement e = m.toElement(); // try to convert the node to an element.
        if(!e.isNull()) {
            domElementForNumber.insert(e.attribute("number", "0").toInt(), e);

            QDomNode n = m.firstChild();
            while(!n.isNull()) { // object level
                QDomElement f = n.toElement(); // try to convert the node to an element.
                if(!f.isNull()) {
                    domElementForNumber.insert(f.attribute("number", "0").toInt(), f);

                }
                n = n.nextSibling();
            }
        }
        m = m.nextSibling();
    }


    ui->managerTreeView->expandAll();

}

//-----------------------------------------------------------------------------------------------------------------------

void AttendManager::launchAttendSettings()
{
    AttendSettings *attendSettings = new AttendSettings;
    attendSettings->setHub(hub);
    attendSettings->postConstructor();
    connect(attendSettings, SIGNAL(accepted()), this, SLOT(attendSettingsAccepted()));
    attendSettings->exec();


}

//-----------------------------------------------------------------------------------------------------------------------

void AttendManager::attendSettingsAccepted()
{
    this->applySettings();

}

void AttendManager::applySettings()
{
    int currentIndex_1 = ui->comboBox_1->currentIndex();
    int currentIndex_2 = ui->comboBox_2->currentIndex();
    int currentIndex_3 = ui->comboBox_3->currentIndex();
    ui->comboBox_1->clear();
    ui->comboBox_2->clear();
    ui->comboBox_3->clear();

    QDomElement root = hub->attendTreeDomDoc().documentElement().toElement();


    QStringList box1List = root.attribute("box_1", "None").split("--", QString::SkipEmptyParts);
    ui->comboBox_1->addItems(box1List);
    ui->comboBox_1->setCurrentIndex(currentIndex_1);

    QStringList box2List = root.attribute("box_2", "None").split("--", QString::SkipEmptyParts);
    ui->comboBox_2->addItems(box2List);
    ui->comboBox_2->setCurrentIndex(currentIndex_2);



    QStringList box3List = root.attribute("box_3", "None").split("--", QString::SkipEmptyParts);
    ui->comboBox_3->addItems(box3List);
    ui->comboBox_3->setCurrentIndex(currentIndex_3);



    ui->spinBox_1_label->setText(root.attribute("spinBox_1_label", tr("Age :")));



}

//-----------------------------------------------------------------------------------------------------------------------

void AttendManager::connectAll()
{
    connect(ui->nameEdit, SIGNAL(editingFinished()), this, SLOT(saveToElement()), Qt::UniqueConnection);
    connect(ui->nameEdit, SIGNAL(editingFinished()), this, SLOT(nameEditingFinished()), Qt::UniqueConnection);
    connect(ui->quickDetailsEdit, SIGNAL(editingFinished()), this, SLOT(saveToElement()), Qt::UniqueConnection);
    connect(ui->aliasesEdit, SIGNAL(editingFinished()), this, SLOT(saveToElement()), Qt::UniqueConnection);
    connect(ui->comboBox_1, SIGNAL(currentIndexChanged(int)), this, SLOT(saveToElement()), Qt::UniqueConnection);
    connect(ui->comboBox_2, SIGNAL(currentIndexChanged(int)), this, SLOT(saveToElement()), Qt::UniqueConnection);
    connect(ui->comboBox_3, SIGNAL(currentIndexChanged(int)), this, SLOT(saveToElement()), Qt::UniqueConnection);
    connect(ui->spinBox_1, SIGNAL(valueChanged(int)), this, SLOT(saveToElement()), Qt::UniqueConnection);

    connect(ui->textEdit, SIGNAL(textChanged()), this, SLOT(saveText()), Qt::UniqueConnection);

}


//-----------------------------------------------------------------------------------------------------------------------

void AttendManager::disconnectAll()
{
    disconnect(ui->nameEdit, SIGNAL(editingFinished()), this, SLOT(saveToElement()));
    disconnect(ui->nameEdit, SIGNAL(editingFinished()), this, SLOT(nameEditingFinished()));
    disconnect(ui->quickDetailsEdit, SIGNAL(editingFinished()), this, SLOT(saveToElement()));
    disconnect(ui->aliasesEdit, SIGNAL(editingFinished()), this, SLOT(saveToElement()));
    disconnect(ui->comboBox_1, SIGNAL(currentIndexChanged(int)), this, SLOT(saveToElement()));
    disconnect(ui->comboBox_2, SIGNAL(currentIndexChanged(int)), this, SLOT(saveToElement()));
    disconnect(ui->comboBox_3, SIGNAL(currentIndexChanged(int)), this, SLOT(saveToElement()));
    disconnect(ui->spinBox_1, SIGNAL(valueChanged(int)), this, SLOT(saveToElement()));
    disconnect(ui->textEdit, SIGNAL(textChanged()), this, SLOT(saveText()));

}

void AttendManager::saveToElement()
{




    openedElement.setAttribute("name", ui->nameEdit->text());
    openedElement.setAttribute("aliases", ui->aliasesEdit->text());
    openedElement.setAttribute("quickDetails", ui->quickDetailsEdit->text());


    openedElement.setAttribute("box_1", ui->comboBox_1->currentIndex());
    openedElement.setAttribute("box_2", ui->comboBox_2->currentIndex());
    openedElement.setAttribute("box_3", ui->comboBox_3->currentIndex());

    openedElement.setAttribute("spinBox_1", ui->spinBox_1->value());


    saveToAttendTreeItem();

    hub->addToSaveQueue();



}

//-----------------------------------------------------

void AttendManager::saveToAttendTreeItem()
{
    AttendTreeItem *treeItem = attendTreeItemActivated;

    treeItem->setName(ui->nameEdit->text());

    treeItem->setAliases(ui->aliasesEdit->text());
    treeItem->setQuickDetails(ui->quickDetailsEdit->text());

    treeItem->setBox_1Value(ui->comboBox_1->currentText());
    treeItem->setBox_2Value(ui->comboBox_2->currentText());
    treeItem->setBox_3Value(ui->comboBox_3->currentText());

    treeItem->setSpinbox_1Value(ui->spinBox_1->value());




}

//-----------------------------------------------------

void AttendManager::saveText()
{
    if(!textAlreadyChanged){
        textAlreadyChanged = true;

        QTimer::singleShot(20000, hub, SLOT(addToSaveQueue()));

    }
}


void AttendManager::editItemTitle(QModelIndex index)
{
    if(index != oldIndex){ // reset if change
        oneClickCheckpoint = false;
        twoClicksCheckpoint = false;
    }
    oldIndex = index;


    if( oneClickCheckpoint && twoClicksCheckpoint){ // third click
        ui->managerTreeView->edit(index);
        oneClickCheckpoint = false; //reset
        twoClicksCheckpoint = false;
    }
    else if(oneClickCheckpoint == true) // second click
        twoClicksCheckpoint = true;
    else
        oneClickCheckpoint = true; // first click
}

void AttendManager::setNameSlot(QString value)
{
    this->disconnectAll();
    ui->nameEdit->setText(value);
    this->connectAll();
}

void AttendManager::on_addObjectAction_triggered()
{
    if(!ui->managerTreeView->currentIndex().isValid())
        return;
    managerProxyModel->addObject(ui->managerTreeView->currentIndex());
    ui->managerTreeView->expandAll();

}

void AttendManager::on_addGroupAction_triggered()
{
    if(!ui->managerTreeView->currentIndex().isValid())
        return;
    managerProxyModel->addGroup(ui->managerTreeView->currentIndex());
    ui->managerTreeView->expandAll();

}

void AttendManager::on_removeAction_triggered()
{
    if(!ui->managerTreeView->currentIndex().isValid())
        return;
    managerProxyModel->remove(ui->managerTreeView->currentIndex());
    ui->managerTreeView->expandAll();

}

void AttendManager::expandAll()
{
    ui->managerTreeView->expandAll();
}

void AttendManager::nameEditingFinished()
{
//    qDebug() << "name editing finished";
    abstractModel->resetAbsModel();
    managerProxyModel->setNameSlot(ui->managerTreeView->currentIndex(), ui->nameEdit->text());
}

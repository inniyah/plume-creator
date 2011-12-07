#include <QtGui>
#include <QDomElement>

#include "attendbox.h"

AttendBox::AttendBox(QWidget *parent) :
    QFrame(parent), attendManagerLaunched(false)
{

    QVBoxLayout *mainLayout = new QVBoxLayout;

    attendList = new QListWidget;

    mainLayout->addWidget(attendList);
    this->setLayout(mainLayout);

}

bool AttendBox::readProjectAttendance(QFile *device)
{

    //clear all lists :

    domElementForItem.clear();
    domElementForItemNumber.clear();
    attendStringForNumber.clear();

    // read file :

    QFileInfo *dirInfo = new QFileInfo(*device);
    QString devicePath = dirInfo->path();


    QStringList filters;
    QDir dir(devicePath);
    filters.clear();
    filters << "*.attend";



    if(dir.entryList(filters).isEmpty()){
        filters.clear();
        filters << "*.plume";

        QFile plumeFile(dir.entryList(filters).first());
        QString projectName = plumeFile.fileName().split("/").last().remove(".plume");



        // create an empty file :

        QFile file(devicePath + "/" + projectName + ".attend");
        file.open(QIODevice::ReadWrite | QIODevice::Text);


        QDomDocument domDoc("plume-attendance");

        QDomElement root = domDoc.createElement("root");
        root.setTagName("attendance");
        root.setAttribute("projectName", projectName);
        root.setAttribute("version","0.1");
        domDoc.appendChild(root);

        file.write(domDoc.toByteArray(3));
        file.close();
    }




    filters.clear();
    filters << "*.attend";


    QFile *attFile = new QFile(devicePath + "/" + dir.entryList(filters).first());




    //opening :


    QString errorStr;
    int errorLine;
    int errorColumn;

    if (!domDocument.setContent(attFile, true, &errorStr, &errorLine,
                                &errorColumn)) {
        QMessageBox::information(this, tr("Plume Creator Tree"),
                                 tr("Parse error at line %1, column %2:\n%3\n")
                                 .arg(errorLine)
                                 .arg(errorColumn)
                                 .arg(errorStr));

        //        qDebug() << "File path:" << device->fileName();
        //        qDebug() << "File readable:" << device->isReadable();
        //        qDebug() << "File open:" << device->isOpen();


        return false;
    }




    root = domDocument.documentElement();
    if (root.tagName() != "attendance") {
        QMessageBox::information(this, tr("Plume Creator Tree"),
                                 tr("The file is not a a Plume Creator attendance file."));
        return false;
    } else if (root.hasAttribute("version")
               && root.attribute("version") != "0.1") {
        QMessageBox::information(this, tr("Plume Creator Tree"),
                                 tr("The file is not an Plume Creator attendance file version 0.1 "
                                    "file."));
        return false;
    }

    buildList();





    return true;
}



void AttendBox::buildList()
{
    //    disconnect(attendList, SIGNAL(itemChanged(QTreeWidgetItem*,int)),
    //               this, SLOT(updateDomElement(QTreeWidgetItem*,int)));

    //    clear();
    domElementForItem.clear();
    domElementForItemNumber.clear();


    QDomElement child = root.firstChildElement("char");
    while (!child.isNull()) {
        QListWidgetItem *characterItem = new QListWidgetItem;

        characterItem->setText(child.attribute("lastName") + " " + child.attribute("firstName"));

        createItem(child, characterItem);
        child = child.nextSiblingElement("char");
    }

    child = root.firstChildElement("item");
    while (!child.isNull()) {
        QListWidgetItem *itemItem = new QListWidgetItem;

        itemItem->setText(child.attribute("name"));

        createItem(child, itemItem);
        child = child.nextSiblingElement("item");
    }

    child = root.firstChildElement("place");
    while (!child.isNull()) {
        QListWidgetItem *placeItem = new QListWidgetItem;

        placeItem->setText(child.attribute("name"));

        createItem(child, placeItem);
        child = child.nextSiblingElement("place");
    }



    //    connect(attendList, SIGNAL(itemChanged(QTreeWidgetItem*,int)),
    //            this, SLOT(updateDomElement(QTreeWidgetItem*,int)));


}

QListWidgetItem *AttendBox::createItem(const QDomElement &element,
                                       QListWidgetItem *item)
{
    domElementForItem.insert(item, element);

    int itemNumber = element.attribute("number").toInt();
    domElementForItemNumber.insert(itemNumber, element);
    QString string;
    qDebug() << "__domElementForItem.size() : " << string.setNum(domElementForItem.size());
    qDebug() << "__domElementForItemNumber.size() : " << string.setNum(domElementForItemNumber.size());

    return item;
}





QList<QListWidgetItem *> *AttendBox::sortItems(QList<int> *attend, QString sorting)
{
    QList<QListWidgetItem *> *list = new QList<QListWidgetItem *>;
    QList<QListWidgetItem *> charList;
    QList<QListWidgetItem *> charList_level_zero;
    QList<QListWidgetItem *> charList_level_one;
    QList<QListWidgetItem *> charList_level_two;
    QList<QListWidgetItem *> itemList;
    QList<QListWidgetItem *> itemList_level_zero;
    QList<QListWidgetItem *> itemList_level_one;
    QList<QListWidgetItem *> itemList_level_two;
    QList<QListWidgetItem *> placeList;
    QList<QListWidgetItem *> placeList_level_zero;
    QList<QListWidgetItem *> placeList_level_one;
    QList<QListWidgetItem *> placeList_level_two;



    //        QHashIterator<QListWidgetItem *, QDomElement> i(domElementForItem);
    //        while (i.hasNext()) {
    //            i.next();
    //            list->append(i.key());

    //    }
    QString string;
    qDebug() << "domElementForItemNumber.size() : " << string.setNum(domElementForItemNumber.size());

    QMap<QString, QDomElement> map;

    for (int i = 0; i < attend->size(); ++i){
        QDomElement element = domElementForItemNumber.value(attend->at(i));
        if(element.tagName() == "char"){
            map.insert(element.attribute("lastName", "ZZZ") + " " + element.attribute("firstName", "ZZZ"),element);
        }
        if(element.tagName() == "item"){
            map.insert(element.attribute("name", "ZZZ"),element);
        }
        if(element.tagName() == "place"){
            map.insert(element.attribute("name", "ZZZ"),element);
        }

    }


    QMap<QString, QDomElement>::const_iterator i = map.constBegin();
    while (i != map.constEnd()) {

QDomElement mapElement = i.value();
        if(mapElement.tagName() == "char"){

            charList.append(domElementForItem.key(mapElement));

            if(mapElement.attribute("level") == "0")
                charList_level_zero.append(domElementForItem.key(mapElement));

            if(mapElement.attribute("level") == "1")
                charList_level_one.append(domElementForItem.key(mapElement));

            if(mapElement.attribute("level") == "2")
                charList_level_two.append(domElementForItem.key(mapElement));
        }

        if(mapElement.tagName() == "item"){

            itemList.append(domElementForItem.key(mapElement));

            if(mapElement.attribute("level") == "0")
                itemList_level_zero.append(domElementForItem.key(mapElement));

            if(mapElement.attribute("level") == "1")
                itemList_level_one.append(domElementForItem.key(mapElement));

            if(mapElement.attribute("level") == "2")
                itemList_level_two.append(domElementForItem.key(mapElement));
        }

        if(mapElement.tagName() == "place"){

            placeList.append(domElementForItem.key(mapElement));

            if(mapElement.attribute("level") == "0")
                placeList_level_zero.append(domElementForItem.key(mapElement));

            if(mapElement.attribute("level") == "1")
                placeList_level_one.append(domElementForItem.key(mapElement));

            if(mapElement.attribute("level") == "2")
                placeList_level_two.append(domElementForItem.key(mapElement));
        }

        ++i;
    }


    // set the different fonts :

    QFont textFont;
    textFont.setBold(true);

    for(int i = 0; i < charList_level_zero.size(); ++i)
        charList_level_zero.at(i)->setFont(textFont);

            for(int i = 0; i < itemList_level_zero.size(); ++i)
    itemList_level_zero.at(i)->setFont(textFont);

            for(int i = 0; i < placeList_level_zero.size(); ++i)
    placeList_level_zero.at(i)->setFont(textFont);







//    list->append(charList);
//    list->append(itemList);
//    list->append(placeList);
    list->append(separator(tr("Characters")));
    list->append(charList_level_zero);
    list->append(charList_level_one);
    list->append(charList_level_two);
    list->append(separator(tr("Items")));
    list->append(itemList_level_zero);
    list->append(itemList_level_one);
    list->append(itemList_level_two);
    list->append(separator(tr("Places")));
    list->append(placeList_level_zero);
    list->append(placeList_level_one);
    list->append(placeList_level_two);

    qDebug() << "list->size() : "<< string.setNum(list->size());
    return list;
}



QListWidgetItem *AttendBox::separator(QString separatorName)
{
    QListWidgetItem *separatorItem = new QListWidgetItem();
separatorItem->setFlags(Qt::NoItemFlags);
    separatorItem->setText(separatorName);
    separatorItem->setTextAlignment(Qt::AlignHCenter);



    QFont textFont(separatorItem->font());
    textFont.setBold(true);
    textFont.setPointSize(separatorItem->font().pointSize() + 2);

    separatorItem->setFont(textFont);

    return separatorItem;
}































void AttendBox::openSheetAttendList(int number ,QString attendString)
{
currentSheetNumber = number;

       // stock the strings for later use :
        attendStringForNumber.insert(number, attendString);






    QStringList thisAttendStringList = attendString.split("-", QString::SkipEmptyParts);


    QList<int> *attendIntList = new QList<int>;

    while(!thisAttendStringList.isEmpty()){
        attendIntList->append(thisAttendStringList.takeFirst().toInt());
    }

    if(attendIntList->contains(0))
    attendIntList->removeAt(attendIntList->indexOf(0));

    QList<QListWidgetItem *> *itemList = sortItems(attendIntList);

    showSheetAttendanceList(itemList);

}


bool AttendBox::showSheetAttendanceList(QList<QListWidgetItem *> *itemList)
{
    // clear list :
    while(attendList->count() != 0)
        attendList->takeItem(0);



    while(!itemList->isEmpty()){
        attendList->addItem(itemList->takeFirst());
    }


    addAttendManagerButton();

    connect(attendList, SIGNAL(itemDoubleClicked(QListWidgetItem*)),
            this, SLOT(itemDoubleClick(QListWidgetItem*)));


    return true;

}


void AttendBox::setCurrentList(int number)
{
    currentSheetNumber = number;


    // clear list :
    while(attendList->count() != 0)
        attendList->takeItem(0);

    // pick the right list :

    QString attendString = attendStringForNumber.value(number, "0");

    //display it :

    openSheetAttendList(number, attendString);

if(attendManagerLaunched)
    setManagerSheetList(number);
}







void AttendBox::addAttendManagerButton()
{

    managerLauncher = new QListWidgetItem;
    managerLauncher->setText(tr("Manage..."));
    managerLauncher->setBackgroundColor(QColor(Qt::darkYellow));
    attendList->addItem(managerLauncher);


}

void AttendBox::itemDoubleClick(QListWidgetItem* itemClicked)
{
    if(itemClicked == managerLauncher)
        launchAttendManager();
}








void AttendBox::launchAttendManager()
{
    if(attendManagerLaunched){
        QApplication::setActiveWindow(attendManager);
        return;
    }
    attendManager = new QWidget(0);
    attendManager->setAttribute(Qt::WA_DeleteOnClose);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    QHBoxLayout *listLayout = new QHBoxLayout;
    QListWidget *projectList = new QListWidget;
    managerSheetList = new QListWidget;

    QDialogButtonBox *buttons = new QDialogButtonBox((QDialogButtonBox::Close), Qt::Horizontal);
    connect(buttons, SIGNAL(clicked(QAbstractButton*)), this, SLOT(accept()));

    listLayout->addWidget(projectList);
    listLayout->addWidget(managerSheetList);
    mainLayout->addLayout(listLayout);
    mainLayout->addWidget(buttons);

    attendManager->setLayout(mainLayout);



    connect(attendManager, SIGNAL(destroyed()), this, SLOT(accept()));




    // display project list :

    QList<int> *intList = new QList<int>;
    QHashIterator<int , QDomElement> i(domElementForItemNumber);
    while (i.hasNext()) {
        i.next();
        intList->append(i.key());

    }

    QList<QListWidgetItem *> *list = sortItems(intList);

    for (int i = 0; i < list->size(); ++i){
        projectList->addItem(list->at(i));
    }
    QString string;
    qDebug() << "string.setNum(list->size() : " << string.setNum(list->size());

// set target list :

    setManagerSheetList(currentSheetNumber);





    readSettings();


    attendManager->show();

    attendManagerLaunched = true;

}




//---------------------------------------------------------------------------


void AttendBox::setManagerSheetList(int number)
{


QString attendString = attendStringForNumber.value(number);
qDebug() << "attendString : " << attendStringForNumber.value(number);







 QStringList thisAttendStringList = attendString.split("-", QString::SkipEmptyParts);
 QString string;
 qDebug() << "thisAttendStringList : " << string.setNum(thisAttendStringList.size());


 QList<int> *attendIntList = new QList<int>;

 while(!thisAttendStringList.isEmpty()){
     attendIntList->append(thisAttendStringList.takeFirst().toInt());
 }

 // remove the number 0 :
 if(attendIntList->contains(0))
     attendIntList->removeAt(attendIntList->indexOf(0));


 QList<QListWidgetItem *> *itemList = sortItems(attendIntList);




 // clear widgetlist :
 while(managerSheetList->count() != 0)
     managerSheetList->takeItem(0);

 qDebug() << "itemList : " << string.setNum(itemList->size());


 while(!itemList->isEmpty()){
     managerSheetList->addItem(itemList->takeFirst());
 }






}

//---------------------------------------------------------------------------

void AttendBox::accept()
{
    writeSettings();
    attendManagerLaunched = false;
    attendManager->close();

}



//---------------------------------------------------------------------------

void AttendBox::readSettings()
{
    QSettings settings;
    settings.beginGroup( "AttendManager" );
    attendManager->resize(settings.value( "size", QSize( 1000, 750 ) ).toSize() );
    attendManager->move(settings.value( "pos" ).toPoint() );
    settings.endGroup();




}

//---------------------------------------------------------------------------

void AttendBox::writeSettings()
{

    QSettings settings;
    settings.beginGroup( "AttendManager" );
    settings.setValue( "size", attendManager->size() );
    settings.setValue( "pos", attendManager->pos() );
    settings.endGroup();

}

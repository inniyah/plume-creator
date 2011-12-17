#include <QtGui>
#include <QDomElement>

#include "attendbox.h"

AttendBox::AttendBox(QWidget *parent) :
    QFrame(parent), attendManagerLaunched(false), newAttendName("*" + tr("new"))
{

    QVBoxLayout *mainLayout = new QVBoxLayout;

    attendList = new QListWidget;
    abstractList = new QListWidget;

    mainLayout->addWidget(attendList);
    this->setLayout(mainLayout);

}

//--------------------------------------------------------------------------------------

bool AttendBox::saveAll()
{



    saveDomDocument();


    QHash<QTextDocument *, QFile *>::iterator i = fileForDoc.begin();

    while (i != fileForDoc.end()) {


        QTextDocument *doc = i.key();


        QFile *file = fileForDoc.value(doc);
        file->close();
        QTextDocumentWriter docWriter(file, "HTML");
        bool written = docWriter.write(doc);





        ++i;
    }

    return true;
}

//--------------------------------------------------------------------------------------

bool AttendBox::saveDomDocument()
{
    attFile->waitForBytesWritten(500);
    attFile->close();
    attFile->open(QFile::ReadWrite | QFile::Text | QFile::Truncate);
    if(attFile->isWritable())
    {
        attFile->flush();


        const int IndentSize = 4;

        QTextStream out(attFile);
        out.flush();
        domDocument.save(out, IndentSize);
        attFile->close();
    }
}

bool AttendBox::saveThisDoc()
{




    QTextDocument *doc = editZone->document();


    QFile *file = fileForDoc.value(doc);
    file->close();
    QTextDocumentWriter docWriter(file, "HTML");
    bool written = docWriter.write(doc);


    //    qDebug() << "file : " << file->fileName();




    return written;
}
//--------------------------------------------------------------------------------------

bool AttendBox::readProjectAttendance(QFile *device)
{
    firstDetailOpening = true;
    //clear all lists :

    domElementForItem.clear();
    domElementForItemNumber.clear();
    attendStringForNumber.clear();
    fileForDoc.clear();


    // read file :

    QFileInfo *dirInfo = new QFileInfo(*device);
    devicePath = dirInfo->path();


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

        QString defaultLevelsNames = tr("None") + "--" + tr("Secondary") + "--" + tr("Main");
        root.setAttribute("levelsNames", defaultLevelsNames);
        QString defaultRolesNames = tr("None") + "--" + tr("Protagonist") + "--" + tr("Supporting") + "--" + tr("Neutral") + "--" + tr("Antagonist");
        root.setAttribute("rolesNames", defaultRolesNames);
        domDoc.appendChild(root);

        file.write(domDoc.toByteArray(3));
        file.close();
    }




    filters.clear();
    filters << "*.attend";


    attFile = new QFile(devicePath + "/" + dir.entryList(filters).first());




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


    emit projectAttendanceList(domElementForItem, domElementForItemNumber);




    //  open all attend docs :

    QApplication::setOverrideCursor(Qt::WaitCursor);


    //                 set up the progress bar :
    QWidget *progressWidget = new QWidget(this, Qt::Dialog | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    QHBoxLayout *progressLayout = new QHBoxLayout(progressWidget);
    QProgressBar *progressBar = new QProgressBar(progressWidget);
    int progressValue = 0;

    progressLayout->addWidget(progressBar);
    progressWidget->setLayout(progressLayout);

    progressBar->setMaximum(domDocument.elementsByTagName("char").size()
                            + domDocument.elementsByTagName("item").size()
                            + domDocument.elementsByTagName("place").size()
                            );
    progressBar->setValue(progressValue);
    progressWidget->show();






    QDomElement element;
    QString attendPath;



    for(int i = 0; i < abstractList->count(); ++i){


        //      itemList->append(iterator->operator *());
        element = domElementForItem.value(abstractList->item(i));

        attendPath = devicePath + element.attribute("attendPath");

        QFile *attendFile = new QFile;
        attendFile->setFileName(attendPath);
        attendFile->open(QFile::ReadOnly | QFile::Text);
        QTextStream attendFileStream( attendFile );
        QTextDocument *attendDocument = new QTextDocument(this);
        attendDocument->setHtml(attendFileStream.readAll());
        attendFile->close();
        attendDocument->setObjectName("attendDoc_" + element.attribute("number"));
        fileForDoc.insert(attendDocument, attendFile);


        //            qDebug() << "doc opened : "<< element.attribute("number");

        //        QFile *file = fileForDoc.value(attendDocument);
        //        qDebug() << "attendDoc : " << file->fileName();


        progressValue += 1;
        progressBar->setValue(progressValue);



    }
    QApplication::restoreOverrideCursor();
    progressWidget->close();
















    //    QString string;


    //    qDebug() << "attend domElementForItemNumber.size() : " << string.setNum(domElementForItemNumber.size());
    //    qDebug() << "attend domElementForItem.size() : " << string.setNum(domElementForItem.size());


    return true;
}

//--------------------------------------------------------------------------------------


void AttendBox::buildList()
{
    //    disconnect(attendList, SIGNAL(itemChanged(QTreeWidgetItem*,int)),
    //               this, SLOT(updateDomElement(QTreeWidgetItem*,int)));

    //    clear();

    domElementForItem.clear();
    domElementForItemNumber.clear();
    abstractList->clear();

    QDomElement child = root.firstChildElement("char");
    while (!child.isNull()) {
        QListWidgetItem *characterItem = new QListWidgetItem;

        characterItem->setText(child.attribute("lastName", newAttendName) + " " + child.attribute("firstName"));

        createItem(child, characterItem);
        child = child.nextSiblingElement("char");
    }

    child = root.firstChildElement("item");
    while (!child.isNull()) {
        QListWidgetItem *itemItem = new QListWidgetItem;

        itemItem->setText(child.attribute("name", newAttendName));

        createItem(child, itemItem);
        child = child.nextSiblingElement("item");
    }

    child = root.firstChildElement("place");
    while (!child.isNull()) {
        QListWidgetItem *placeItem = new QListWidgetItem;

        placeItem->setText(child.attribute("name", newAttendName));

        createItem(child, placeItem);
        child = child.nextSiblingElement("place");
    }



    //    connect(attendList, SIGNAL(itemChanged(QTreeWidgetItem*,int)),
    //            this, SLOT(updateDomElement(QTreeWidgetItem*,int)));


}
//--------------------------------------------------------------------------------------

QListWidgetItem *AttendBox::createItem(const QDomElement &element,
                                       QListWidgetItem *item)
{
    domElementForItem.insert(item, element);

    int itemNumber = element.attribute("number").toInt();
    domElementForItemNumber.insert(itemNumber, element);

    //    QListWidgetItem identification :
    item->setData(32, itemNumber);

    abstractList->addItem(item);

    return item;
}
//--------------------------------------------------------------------------------------

void AttendBox::updateAllAttendances(QHash<int,QString> allAttendancesForNumber)
{
    // stock from MainTree the strings for later use :

    attendStringForNumber = allAttendancesForNumber;

}

//--------------------------------------------------------------------------------------

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

    for (int i = 0; i < attend->size(); ++i) {
    }

    QMap<QString, QDomElement> map;

    for (int i = 0; i < attend->size(); ++i){
        QDomElement element = domElementForItemNumber.value(attend->at(i));
        if(element.tagName() == "char"){
            map.insert(element.attribute("lastName", newAttendName) + " " + element.attribute("firstName", ""),element);
        }
        if(element.tagName() == "item"){
            map.insert(element.attribute("name", newAttendName),element);
        }
        if(element.tagName() == "place"){
            map.insert(element.attribute("name", newAttendName),element);
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
    if(charList_level_zero.size() + charList_level_one.size() + charList_level_two.size() != 0)
        list->append(separator(tr("Characters")));
    list->append(charList_level_zero);
    list->append(charList_level_one);
    list->append(charList_level_two);
    if(itemList_level_zero.size() + itemList_level_one.size() + itemList_level_two.size() != 0)
        list->append(separator(tr("Items")));
    list->append(itemList_level_zero);
    list->append(itemList_level_one);
    list->append(itemList_level_two);
    if(placeList_level_zero.size() + placeList_level_one.size() + placeList_level_two.size() != 0)
        list->append(separator(tr("Places")));
    list->append(placeList_level_zero);
    list->append(placeList_level_one);
    list->append(placeList_level_two);

    return list;
}


//--------------------------------------------------------------------------------------

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
//--------------------------------------------------------------------------------------




























//--------------------------------------------------------------------------------------



void AttendBox::openSheetAttendList(int number ,QString attendString)
{
    currentSheetNumber = number;


    //    QString string;
    //    qDebug() << "attendStringForNumber.size() : "<<  string.setNum(attendStringForNumber.size());
    //    qDebug() << "attendStringForNumber.key() : "<<  string.setNum(attendStringForNumber.key(attendString));
    //    qDebug() << "attendStringForNumber.value() : "<<  attendStringForNumber.value(number);




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

//--------------------------------------------------------------------------------------


bool AttendBox::showSheetAttendanceList(QList<QListWidgetItem *> *itemList)
{
    // clear list :
    while(attendList->count() != 0)
        attendList->takeItem(0);



    while(!itemList->isEmpty()){
        attendList->addItem(itemList->takeFirst()->clone());
    }


    addAttendManagerButton();

    connect(attendList, SIGNAL(itemActivated(QListWidgetItem*)),
            this, SLOT(itemActivatedSlot(QListWidgetItem*)));


    return true;

}




//--------------------------------------------------------------------------------------




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



//--------------------------------------------------------------------------------------




void AttendBox::addAttendManagerButton()
{

    managerLauncher = new QListWidgetItem;
    managerLauncher->setText(tr("Manage..."));
    managerLauncher->setBackgroundColor(QColor(Qt::darkYellow));
    attendList->addItem(managerLauncher);


}
//--------------------------------------------------------------------------------------

void AttendBox::itemActivatedSlot(QListWidgetItem* itemActivated)
{
    if(itemActivated == managerLauncher){
        launchAttendManager();
        hideDetails();
        return;
    }

    launchAttendManager();

    QListWidgetItem* item = projectList->findItems(itemActivated->text(), Qt::MatchExactly).takeFirst();
    projectList->scrollToItem(item);
    projectList->setItemSelected(item, true);
    projectItemActivated(item);




    //    QString debug;
    //    qDebug() << "projectList index : " << debug.setNum(projectList->row(itemActivated));

}



//--------------------------------------------------------------------------------------





void AttendBox::launchAttendManager()
{
    if(attendManagerLaunched){
        attendManager->raise();
        QApplication::setActiveWindow(attendManager);
        return;
    }
    attendManager = new QWidget(0);
    attendManager->setWindowTitle(tr("Attendance Manager"));
    attendManager->setAttribute(Qt::WA_DeleteOnClose);

    Qt::WindowFlags flags;
    flags = (attendManager->windowFlags() | Qt::CustomizeWindowHint) & ~Qt::WindowMaximizeButtonHint & ~Qt::WindowCloseButtonHint;
    attendManager->setWindowFlags(flags);

    attendManager->raise();

    QVBoxLayout *mainLayout = new QVBoxLayout;
    QVBoxLayout *transfertLayout = new QVBoxLayout;
    QHBoxLayout *detailPlusListLayout = new QHBoxLayout;
    detailLayout =  new QGridLayout;
    QHBoxLayout *listLayout = new QHBoxLayout;
    projectList = new QListWidget;
    projectList->setSelectionMode(QAbstractItemView::ExtendedSelection);
    managerSheetList = new QListWidget;
     managerSheetList->setSelectionMode(QAbstractItemView::ExtendedSelection);
    showDetailButton = new QPushButton("<");
    hideDetailButton = new QPushButton(">");
    newCharButton = new QPushButton("+C");
    newItemButton = new QPushButton("+I");
    newPlaceButton = new QPushButton("+P");
    deleteButton = new QPushButton("-");
    toSheetButton = new QPushButton(">");
    toAllButton = new QPushButton("<");

    editZone = new NoteZone;

    firstnameEdit = new QLineEdit;
    firstnameEdit->setPlaceholderText(tr("First Name :"));
    lastnameEdit = new QLineEdit;
    lastnameEdit->setPlaceholderText(tr("Last Name :"));
    nameEdit = new QLineEdit;
    nameEdit->setPlaceholderText(tr("Name :"));
    levelComboBox = new QComboBox;
    roleComboBox = new QComboBox;

    connect(firstnameEdit, SIGNAL(editingFinished()), this, SLOT(firstnameChanged()));
    connect(lastnameEdit, SIGNAL(editingFinished()), this, SLOT(lastnameChanged()));
    connect(nameEdit, SIGNAL(editingFinished()), this, SLOT(nameChanged()));

    connect(newCharButton, SIGNAL(clicked()), this, SLOT(newCharSlot()));
    connect(newItemButton, SIGNAL(clicked()), this, SLOT(newItemSlot()));
    connect(newPlaceButton, SIGNAL(clicked()), this, SLOT(newPlaceSlot()));
    connect(deleteButton, SIGNAL(clicked()), this, SLOT(deleteItems()));

    connect(toSheetButton, SIGNAL(clicked()), this, SLOT(toSheetSlot()));
    connect(toAllButton, SIGNAL(clicked()), this, SLOT(toAllSlot()));

    connect(showDetailButton, SIGNAL(clicked()), this, SLOT(showDetailAnimation()));
    connect(hideDetailButton, SIGNAL(clicked()), this, SLOT(hideDetailAnimation()));

    QDialogButtonBox *buttons = new QDialogButtonBox((QDialogButtonBox::Close), Qt::Horizontal);
    connect(buttons, SIGNAL(clicked(QAbstractButton*)), this, SLOT(accept()));
    connect(attendManager, SIGNAL(destroyed()), this, SLOT(attendManagerDestroyed()));


    transfertLayout->addWidget(newCharButton);
    transfertLayout->addWidget(newItemButton);
    transfertLayout->addWidget(newPlaceButton);
    transfertLayout->addWidget(deleteButton);
    transfertLayout->addStretch();
    transfertLayout->addWidget(toSheetButton);
    transfertLayout->addWidget(toAllButton);
    transfertLayout->addStretch();

    //    listLayout->addWidget(showDetailButton);
    listLayout->addWidget(projectList);
    listLayout->addLayout(transfertLayout);
    listLayout->addWidget(managerSheetList);
    detailPlusListLayout->addLayout(detailLayout);
    detailPlusListLayout->addLayout(listLayout);
    mainLayout->addLayout(detailPlusListLayout);
    mainLayout->addWidget(buttons);


    detailLayout->addWidget(firstnameEdit, 0, 0);
    detailLayout->addWidget(nameEdit, 0, 0);
    detailLayout->addWidget(lastnameEdit, 0, 1);
    detailLayout->addWidget(levelComboBox, 0, 2);
    detailLayout->addWidget(roleComboBox, 0, 3);
    detailLayout->addWidget(hideDetailButton, 0, 4);
    detailLayout->addWidget(editZone, 1, 0, 1, 5);
    firstnameEdit->hide();
    lastnameEdit->hide();
    nameEdit->hide();
    hideDetailButton->hide();
    editZone->hide();
    levelComboBox->hide();
    roleComboBox->hide();

    attendManager->setLayout(mainLayout);



    //sizes :

    QSize buttonSize(25, 50);
    toSheetButton->setFixedSize(buttonSize);
    toAllButton->setFixedSize(buttonSize);
    showDetailButton->setFixedSize(buttonSize);
    hideDetailButton->setFixedSize(buttonSize);
    newCharButton->setFixedSize(buttonSize);
    newItemButton->setFixedSize(buttonSize);
    newPlaceButton->setFixedSize(buttonSize);
    deleteButton->setFixedSize(buttonSize);

    QSize listSize(250, 550);
    projectList->setMinimumSize(listSize);
    managerSheetList->setMinimumSize(listSize);


    // display project list :
    setProjectList();





    // set target list :

    setManagerSheetList(currentSheetNumber);

    connect(projectList, SIGNAL(itemActivated(QListWidgetItem*)), this, SLOT(projectItemActivated(QListWidgetItem*)));

    readSettings();

    attendManager->show();

    attendManagerLaunched = true;

}




//---------------------------------------------------------------------------


void AttendBox::setProjectList()
{
    // clear list :
    while(projectList->count() != 0)
        projectList->takeItem(0);

    // display project list :

    QList<int> *intList = new QList<int>;

    QHashIterator<int , QDomElement> i(domElementForItemNumber);
    while (i.hasNext()) {
        i.next();
        intList->append(i.key());
    }

    QList<QListWidgetItem *> *list = sortItems(intList);

    //    for (int i = 0; i < list->size(); ++i){
    //        projectList->addItem(list->at(i));
    //    }

    while(!list->isEmpty()){
        projectList->addItem(list->takeFirst()->clone());
    }
}
//---------------------------------------------------------------------------


void AttendBox::setManagerSheetList(int number)
{
    currentManagerSheetList = number;

    QString attendString = attendStringForNumber.value(number);
    //    qDebug() << "attendString : " << attendStringForNumber.value(number);







    QStringList thisAttendStringList = attendString.split("-", QString::SkipEmptyParts);


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



    while(!itemList->isEmpty()){
        managerSheetList->addItem(itemList->takeFirst()->clone());
    }






}

//---------------------------------------------------------------------------

void AttendBox::projectItemActivated(QListWidgetItem* itemActivated)
{
    openDetail(itemActivated);
}
//---------------------------------------------------------------------------

void AttendBox::openDetail(QListWidgetItem* item)
{
    if(firstDetailOpening == false){
        saveThisDoc();
    }

    //   editZone->clear();
    QListWidgetItem *abstractItem = abstractList->findItems(item->text(), Qt::MatchExactly).takeFirst();
    QString number = domElementForItem.value(abstractItem).attribute("number");

    qDebug() << "r";
    currentElement = domElementForItem.value(abstractItem);


    if(hideDetailsBool)
        showDetailAnimation();
    showDetails();

    // fill the QLineEdits and the QComboBoxes :

    qDebug() << "s";

    if(currentElement.tagName() == "char"){
        firstnameEdit->setText(currentElement.attribute("firstName", ""));
        lastnameEdit->setText(currentElement.attribute("lastName", ""));
    }
    if(currentElement.tagName() == "item" || currentElement.tagName() == "place"){
        nameEdit->setText(currentElement.attribute("name", ""));
    }

    disconnect(levelComboBox,SIGNAL(currentIndexChanged(int)), this, SLOT(levelChanged()));
    levelComboBox->clear();
    levelComboBox->addItems(root.attribute("levelsNames", tr("no entry")).split("--", QString::KeepEmptyParts));
    levelComboBox->setCurrentIndex(currentElement.attribute("level", "2").toInt());
    connect(levelComboBox,SIGNAL(currentIndexChanged(int)), this, SLOT(levelChanged()), Qt::UniqueConnection);

    disconnect(roleComboBox,SIGNAL(currentIndexChanged(int)), this, SLOT(roleChanged()));
    roleComboBox->clear();
    roleComboBox->addItems(root.attribute("rolesNames", tr("no entry")).split("--", QString::KeepEmptyParts));
    roleComboBox->setCurrentIndex(currentElement.attribute("role", "0").toInt());
    connect(roleComboBox,SIGNAL(currentIndexChanged(int)), this, SLOT(roleChanged()), Qt::UniqueConnection);


    qDebug() << "t";


    // fill the QtextEdit :

    QTextDocument *attendDoc = this->findChild<QTextDocument *>("attendDoc_" + number);
    //    qDebug() << "attendDoc_" + number;
    editZone->openAttendDetail(attendDoc);

    editZone->setFocus();
    qDebug() << "u";

    firstDetailOpening = false;
}

//---------------------------------------------------------------------------

void AttendBox::accept()
{

    if(attendManagerLaunched == false)
        return;

    if(firstDetailOpening == false)
        saveThisDoc();


    writeSettings();

    attendManager->close();

    attendManagerLaunched = false;
    firstDetailOpening = true;
}

//---------------------------------------------------------------------------

void AttendBox::attendManagerDestroyed()
{
    attendManagerLaunched = false;
    firstDetailOpening = true;
}


//---------------------------------------------------------------------------
void AttendBox::showDetailAnimation()
{

    QPropertyAnimation *animation = new QPropertyAnimation(attendManager, "geometry");
    animation->setEasingCurve(QEasingCurve::OutCubic);
    animation->setDuration(500);

    int xManager = attendManager->geometry().x();
    int yManager = attendManager->geometry().y();
    int wManager = attendManager->width();
    int hManager = attendManager->height();


    animation->setStartValue(QRect(xManager,yManager,wManager,hManager));
    animation->setEndValue(QRect(xManager - 500,yManager,wManager + 500,hManager));
    animation->start();


    // center the window if it leaves the screen :
    connect(animation, SIGNAL(stateChanged(QAbstractAnimation::State,QAbstractAnimation::State)), this, SLOT(centerWindow()), Qt::UniqueConnection);



    showDetails();




}
//---------------------------------------------------------------------------

void AttendBox::showDetails()
{
    if(currentElement.tagName() == "char"){
        nameEdit->hide();
        firstnameEdit->show();
        lastnameEdit->show();
    }
    if(currentElement.tagName() == "item" || currentElement.tagName() == "place"){
        firstnameEdit->hide();
        lastnameEdit->hide();
        nameEdit->show();
    }



    levelComboBox->show();
    roleComboBox->show();
    showDetailButton->hide();
    hideDetailButton->show();
    editZone->show();



    hideDetailsBool = false;

}


//---------------------------------------------------------------------------

void AttendBox::hideDetailAnimation()
{

    QPropertyAnimation *animation = new QPropertyAnimation(attendManager, "geometry");
    animation->setEasingCurve(QEasingCurve::OutCubic);
    animation->setDuration(500);

    int xManager = attendManager->geometry().x();
    int yManager = attendManager->geometry().y();
    int wManager = attendManager->width();
    int hManager = attendManager->height();


    animation->setStartValue(QRect(xManager,yManager,wManager,hManager));
    animation->setEndValue(QRect(xManager + 500,yManager,wManager - 500,hManager));
    animation->start();




    hideDetails();
}

//---------------------------------------------------------------------------

void AttendBox::hideDetails()
{
    //    showDetailButton->show();

    editZone->hide();

    firstnameEdit->hide();
    lastnameEdit->hide();
    nameEdit->hide();
    levelComboBox->hide();
    roleComboBox->hide();

    hideDetailButton->hide();


    hideDetailsBool = true;

}

//---------------------------------------------------------------------------

void AttendBox::saveAndUpdate()
{
    // saving :
    if(firstDetailOpening == false)
        saveThisDoc();
    saveDomDocument();

    // updating :
    buildList();
    setProjectList();
    setManagerSheetList(currentManagerSheetList);
    setCurrentList(currentSheetNumber);
}

//---------------------------------------------------------------------------

void AttendBox::firstnameChanged()
{
    currentElement.setAttribute("firstName", firstnameEdit->text());

    saveAndUpdate();

}

//---------------------------------------------------------------------------
void AttendBox::lastnameChanged()
{
    currentElement.setAttribute("lastName", lastnameEdit->text());

    saveAndUpdate();

}

//---------------------------------------------------------------------------
void AttendBox::nameChanged()
{
    currentElement.setAttribute("name", nameEdit->text());

    saveAndUpdate();

}

//---------------------------------------------------------------------------
void AttendBox::levelChanged()
{
    currentElement.setAttribute("level", levelComboBox->currentIndex());

    saveAndUpdate();

}

//---------------------------------------------------------------------------
void AttendBox::roleChanged()
{
    currentElement.setAttribute("role", roleComboBox->currentIndex());

    saveAndUpdate();

}


//---------------------------------------------------------------------------
void AttendBox::newCharSlot()
{

    newAttendElementSlot("char");
}


//---------------------------------------------------------------------------
void AttendBox::newItemSlot()
{
    newAttendElementSlot("item");

}


//---------------------------------------------------------------------------
void AttendBox::newPlaceSlot()
{
    newAttendElementSlot("place");

}

//---------------------------------------------------------------------------
void AttendBox::newAttendElementSlot(QString tagName)
{
    QDomElement newAttendElement = domDocument.createElement(tagName);
    newAttendElement.setTagName(tagName);
    qDebug() << "g";

    // search for the maximum item number available :
    QHash<int, QDomElement>::iterator i = domElementForItemNumber.begin();
    int max = 0;
    while (i != domElementForItemNumber.end()) {


        max = qMax(max, i.key());

        ++i;
    }
    qDebug() << "h";
    int number = max + 1;
    QString stringNumber;
    newAttendElement.setAttribute("number", stringNumber.setNum(number));
    newAttendElement.setAttribute("attendPath", "/attend/A"+ stringNumber +".html");
    newAttendElement.setAttribute("level", "2");
    if(tagName == "char")
        newAttendElement.setAttribute("lastName", newAttendName + " " + stringNumber);
    else if(tagName == "item" || tagName == "place")
        newAttendElement.setAttribute("name", newAttendName + " " + stringNumber);
    root.appendChild(newAttendElement);

    qDebug() << "i";




    QFile *attendFile = new QFile;
    attendFile->setFileName(devicePath + newAttendElement.attribute("attendPath"));
    attendFile->open(QFile::ReadOnly | QFile::Text);
    QTextStream attendFileStream( attendFile );
    QTextDocument *attendDocument = new QTextDocument(this);
    attendDocument->setHtml(attendFileStream.readAll());
    attendFile->close();
    attendDocument->setObjectName("attendDoc_" + stringNumber);
    fileForDoc.insert(attendDocument, attendFile);






    saveAndUpdate();

    QListWidgetItem* item = new QListWidgetItem;
    for(int i=0; i < projectList->count(); ++i)
        if(projectList->item(i)->data(32).toInt() == number){
            item = projectList->item(i);
            qDebug() << "item->text() : " << item->text();
}


    projectList->scrollToItem(item);
    qDebug() << "j";
    projectList->setItemSelected(item, true);
    qDebug() << "k";
    projectItemActivated(item);


    if(tagName == "char")
        firstnameEdit->setFocus();
    else if(tagName == "item" || tagName == "place")
        nameEdit->setFocus();

}


//---------------------------------------------------------------------------

void AttendBox::deleteItems()
{
    QList<QListWidgetItem *> itemList = projectList->selectedItems();
    if(itemList.size() == 0)
        return;

    if(hideDetailsBool == false)
    hideDetailAnimation();

    QList<int> projectDataList;
    for(int j=0; j < itemList.size(); ++j){
        projectDataList.append(itemList.at(j)->data(32).toInt());
    }

    QList<QDomElement> domElementList;
    for(int k=0; k < abstractList->count(); ++k){
        if(projectDataList.contains(abstractList->item(k)->data(32).toInt())){
            domElementList.append(domElementForItem.value(abstractList->item(k)));
            //           qDebug() << "dddd : " << domElementForItem.value(abstractList->item(k)).tagName();
        }
    }

    QString string;
    qDebug() << "domElementList->size() : " << string.setNum(domElementList.size());
    qDebug() << "itemList->size() : " << string.setNum(itemList.size());
    qDebug() << "abstractList->count() : " << string.setNum(abstractList->count());


    for(int l=0; l < domElementList.size(); ++l){
        qDebug() << "a";
        QTextDocument *text = this->findChild<QTextDocument *>("attendDoc_" + domElementList.at(l).attribute("number"));
        QFile *textFile = fileForDoc.value(text);
        textFile->remove();
        fileForDoc.remove(text);
        text->deleteLater();
        qDebug() << "b";


        emit removeAttendNumberSignal(domElementList.at(l).attribute("number").toInt());
        qDebug() << "c";

        domElementList.at(l).parentNode().removeChild(domElementList.at(l));
        qDebug() << "d";



    }
    firstDetailOpening = true;

    saveAndUpdate();

}
//---------------------------------------------------------------------------
void AttendBox::toSheetSlot()
{
    QList<QListWidgetItem *> itemList = projectList->selectedItems();
    if(itemList.size() == 0)
        return;



    QList<int> projectDataList;
    for(int i=0; i < itemList.size(); ++i){
        projectDataList.append(itemList.at(i)->data(32).toInt());
    }

    // remove doubles :
    for(int j=0; j < managerSheetList->count(); ++j){
    int itemNumber = managerSheetList->item(j)->data(32).toInt();
        if(projectDataList.contains(itemNumber))
           projectDataList.removeOne(itemNumber);
    }
QString string;
qDebug() << "projectDataList.size() : " << string.setNum(projectDataList.size());

    emit addAttendNumberToSheetSignal(projectDataList);

     saveAndUpdate();
}


//---------------------------------------------------------------------------
void AttendBox::toAllSlot()
{
    QList<QListWidgetItem *> itemList = managerSheetList->selectedItems();
    if(itemList.size() == 0)
        return;


    QList<int> sheetDataList;
    for(int i=0; i < itemList.size(); ++i){
        sheetDataList.append(itemList.at(i)->data(32).toInt());
    }


    emit removeAttendNumberFromSheetSignal(sheetDataList);
    saveAndUpdate();

}


//---------------------------------------------------------------------------
void AttendBox::readSettings()
{
    QSettings settings;
    settings.beginGroup( "AttendManager" );
    attendManager->resize(settings.value( "size", QSize( 1000, 750 ) ).toSize() );
    attendManager->move(settings.value( "pos" ).toPoint() );
    hideDetailsBool = settings.value( "hideDetails" ).toBool();
    settings.endGroup();

    if(!hideDetailsBool)
        showDetails();


}

//---------------------------------------------------------------------------

void AttendBox::writeSettings()
{

    QSettings settings;
    settings.beginGroup( "AttendManager" );
    settings.setValue( "size", attendManager->size() );
    settings.setValue( "pos", attendManager->pos() );
    settings.setValue( "hideDetails", hideDetailsBool );
    settings.endGroup();

}

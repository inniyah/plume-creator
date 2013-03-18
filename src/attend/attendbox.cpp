
#include "attendbox.h"
#include "fileupdater.h"


AttendBox::AttendBox(QWidget *parent) :
    QFrame(parent), attendManagerLaunched(false), deletingItemBool(false),newAttendName("*" + tr("new")), newAttendElementBool(false)
{


    attendList = new QListWidget;
    abstractList = new QListWidget;
    projectList = new QListWidget;
    dockProjectList = new QListWidget;
    //setProjectList();



    QVBoxLayout *mainLayout = new QVBoxLayout;
    QHBoxLayout *buttonsLayout = new QHBoxLayout;

    QPushButton *launchManagerButton = new QPushButton(tr("Manage"));
    QPushButton *POVButton = new QPushButton(tr("POV", "Point Of View"));

    //    buttonsLayout->addWidget(launchManagerButton);
    //    buttonsLayout->addWidget(POVButton);

    //    connect(launchManagerButton, SIGNAL(clicked()), this, SLOT(launchAttendManager()));
    //    connect(POVButton, SIGNAL(clicked()), this, SLOT(launchAttendManager()));


    mainLayout->addWidget(attendList);
    //    mainLayout->addWidget(dockProjectList);
    mainLayout->addLayout(buttonsLayout);

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
        QTextDocumentWriter *docWriter = new QTextDocumentWriter(file, "HTML");
        bool written = docWriter->write(doc);

        //        qDebug() << "QTextDocumentWriter : " << file->fileName();
        //                qDebug() << "QTextDocumentWriter : " << written;
        delete docWriter;

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

        //        qDebug() << "saveDomDocument()";
        return true;
    }
    else
        return false;
}
//--------------------------------------------------------------------------------------


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


bool AttendBox::closeAll()
{


    //close manager if opened :

    this->accept();


    // rename all docs :

    QHash<QTextDocument *, QFile *>::iterator i = fileForDoc.begin();

    while (i != fileForDoc.end()) {


        QTextDocument *doc = i.key();

        doc->setObjectName("");

        ++i;
    }




    //clear all lists :

    domElementForItem.clear();
    domElementForItemNumber.clear();
    attendStringForNumber.clear();
    fileForDoc.clear();

    return true;

}
//--------------------------------------------------------------------------------------

bool AttendBox::readProjectAttendance(QFile *device)
{
    firstDetailOpening = true;

    FileUpdater *fileUpdater = new FileUpdater;
    fileUpdater->checkAttendanceFile(device->fileName());

    // read file :

    QFileInfo *dirInfo = new QFileInfo(*device);
    devicePath = dirInfo->path();


    QStringList filters;
    QDir dir(devicePath);


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

QList<QListWidgetItem *> *AttendBox::sortItems(QList<int> attend, QString sorting)
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

    for (int i = 0; i < attend.size(); ++i) {
    }

    QMap<QString, QDomElement> map;

    for (int i = 0; i < attend.size(); ++i){
        QDomElement element = domElementForItemNumber.value(attend.at(i));
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

    QList<int> attendIntList;

    while(!thisAttendStringList.isEmpty()){
        attendIntList.append(thisAttendStringList.takeFirst().toInt());
    }

    if(attendIntList.contains(0))
        attendIntList.removeAt(attendIntList.indexOf(0));

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
            this, SLOT(itemActivatedSlot(QListWidgetItem*)), Qt::UniqueConnection);


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

}//--------------------------------------------------------------------------------------


void AttendBox::setCurrentListName(QString tabName)
{

    currentTabName = tabName;

    if(attendManagerLaunched)
        managerSheetListBox->setTitle(currentTabName);

}

//--------------------------------------------------------------------------------------




void AttendBox::addAttendManagerButton()
{

    managerLauncher = new QListWidgetItem;
    managerLauncher->setText(tr("Manage..."));
    managerLauncher->setTextAlignment(Qt::AlignHCenter);
    managerLauncher->setBackgroundColor(QColor(Qt::lightGray));
    managerLauncher->setTextColor(QColor(Qt::white));
    managerLauncher->setToolTip(tr("Launch a manager for characters, items and places"));
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
    else{
        launchAttendManager();

        QListWidgetItem* item = projectList->findItems(itemActivated->text(), Qt::MatchExactly).takeFirst();
        projectList->scrollToItem(item);
        projectList->setItemSelected(item, true);
        projectItemActivated(item);
    }



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

    QVBoxLayout *projectBoxLayout = new QVBoxLayout;
    QGroupBox *projectListBox = new QGroupBox;
    projectListBox->setAlignment(Qt::AlignHCenter);
    projectListBox->setTitle(tr("Project"));
    projectList = new QListWidget;
    projectList->setSelectionMode(QAbstractItemView::ExtendedSelection);
    projectBoxLayout->addWidget(projectList);
    projectListBox->setLayout(projectBoxLayout);

    QVBoxLayout *sheetBoxLayout = new QVBoxLayout;
    managerSheetListBox = new QGroupBox;
    managerSheetListBox->setAlignment(Qt::AlignHCenter);
    managerSheetList = new QListWidget;
    managerSheetList->setSelectionMode(QAbstractItemView::ExtendedSelection);
    sheetBoxLayout->addWidget(managerSheetList);
    managerSheetListBox->setLayout(sheetBoxLayout);





    showDetailButton = new QPushButton("<");
    hideDetailButton = new QPushButton(">");
    newCharButton = new QPushButton(QIcon(":/pics/addchar.png"),"");
    newItemButton = new QPushButton(QIcon(":/pics/addobject.png"),"");
    newPlaceButton = new QPushButton(QIcon(":/pics/addplace.png"),"");
    deleteButton = new QPushButton(QIcon(":/pics/removeItem.png"),"");
    toSheetButton = new QPushButton(">");
    toAllButton = new QPushButton("<");

    QSize iconSize(32,32);
    newCharButton->setIconSize(iconSize);
    newItemButton->setIconSize(iconSize);
    newPlaceButton->setIconSize(iconSize);
    deleteButton->setIconSize(iconSize);

    editZone = new NoteZone;

    connect(editZone, SIGNAL(textChanged()), SIGNAL(textChangedSignal()));

    firstnameEdit = new QLineEdit;
    firstnameEdit->setPlaceholderText(tr("First Name :"));
    lastnameEdit = new QLineEdit;
    lastnameEdit->setPlaceholderText(tr("Last Name :"));
    nameEdit = new QLineEdit;
    nameEdit->setPlaceholderText(tr("Name :"));
    levelComboBox = new QComboBox;
    roleComboBox = new QComboBox;

    projectList->setToolTip(tr("List all the characters, items and places in this project"));
    managerSheetList->setToolTip(tr("List all the characters, items and places in the current sheet"));
    showDetailButton->setToolTip(tr("Show this item details"));
    hideDetailButton->setToolTip(tr("Hide this item details"));
    newCharButton->setToolTip(tr("Add a new character"));
    newItemButton->setToolTip(tr("Add a new item"));
    newPlaceButton->setToolTip(tr("Add a new place"));
    deleteButton->setToolTip(tr("Delete an item"));
    toSheetButton->setToolTip(tr("Add the selected item(s) to the current sheet"));
    toAllButton->setToolTip(tr("Remove the selected item(s) from the current sheet"));
    editZone->setToolTip(tr("Write here the description"));
    firstnameEdit->setToolTip(tr("First name"));
    lastnameEdit->setToolTip(tr("Last name"));
    nameEdit->setToolTip(tr("Name"));
    levelComboBox->setToolTip(tr("Set the item level"));
    roleComboBox->setToolTip(tr("Set the item role"));

    connect(firstnameEdit, SIGNAL(editingFinished()), this, SLOT(firstnameChanged()));
    connect(lastnameEdit, SIGNAL(editingFinished()), this, SLOT(lastnameChanged()));
    connect(nameEdit, SIGNAL(editingFinished()), this, SLOT(nameChanged()));

    connect(newCharButton, SIGNAL(clicked()), this, SLOT(newCharSlot()));
    connect(newItemButton, SIGNAL(clicked()), this, SLOT(newItemSlot()));
    connect(newPlaceButton, SIGNAL(clicked()), this, SLOT(newPlaceSlot()));
    connect(deleteButton, SIGNAL(clicked()), this, SLOT(deleteItems()));

    connect(projectList, SIGNAL(itemSelectionChanged()), this, SLOT(projectListSelectionChanged()));
    connect(managerSheetList, SIGNAL(itemSelectionChanged()), this, SLOT(managerSheetListSelectionChanged()));
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
    listLayout->addWidget(projectListBox);
    listLayout->addLayout(transfertLayout);
    listLayout->addWidget(managerSheetListBox);
    detailPlusListLayout->addLayout(detailLayout);
    detailPlusListLayout->addLayout(listLayout);
    mainLayout->addLayout(detailPlusListLayout);
    mainLayout->addWidget(buttons);




    // font menu :

    QWidgetAction *detailFontAct = new QWidgetAction(this);
    QGroupBox *detailFontBox = new QGroupBox(tr("Detail font"), this);
    QGridLayout *detailFontLayout = new QGridLayout(detailFontBox);
    detailFontCombo = new QFontComboBox(this);
    detailTextHeightSpin = new QSpinBox;
    detailTextHeightSpin->setRange(6,30);
    QLabel *detailTextIndentLabel = new QLabel(tr("Indent :"), this);
    detailTextIndentSpin = new QSpinBox;
    detailTextIndentSpin->setRange(0,70);
    QLabel *detailTextMarginLabel = new QLabel(tr("Margin :"), this);
    detailTextMarginSpin = new QSpinBox;
    detailTextMarginSpin->setRange(0,70);
    detailFontLayout->addWidget(detailFontCombo, 0,0);
    detailFontLayout->addWidget(detailTextHeightSpin, 0,1);
    detailFontLayout->addWidget(detailTextIndentLabel, 1,0, Qt::AlignRight);
    detailFontLayout->addWidget(detailTextIndentSpin, 1,1);
    detailFontLayout->addWidget(detailTextMarginLabel, 2,0, Qt::AlignRight);
    detailFontLayout->addWidget(detailTextMarginSpin, 2,1);
    detailFontBox->setLayout(detailFontLayout);
    detailFontAct->setDefaultWidget(detailFontBox);


    connect(detailFontCombo, SIGNAL(currentFontChanged(QFont)), this, SLOT(writeSettings()));
    connect(detailTextHeightSpin, SIGNAL(valueChanged(int)), this, SLOT(writeSettings()));
    connect(detailTextIndentSpin, SIGNAL(valueChanged(int)), this, SLOT(writeSettings()));
    connect(detailTextMarginSpin, SIGNAL(valueChanged(int)), this, SLOT(writeSettings()));



    QMenu *fontMenu = new QMenu(this);
    fontMenu->addAction(detailFontAct);

    fontsMenuButton = new QToolButton(this);
    fontsMenuButton->setText(tr("Fonts"));
    fontsMenuButton->setPopupMode(QToolButton::InstantPopup);
    fontsMenuButton->setMenu(fontMenu);

    detailLayout->addWidget(firstnameEdit, 0, 0);
    detailLayout->addWidget(nameEdit, 0, 0);
    detailLayout->addWidget(lastnameEdit, 0, 1);
    detailLayout->addWidget(levelComboBox, 0, 2);
    detailLayout->addWidget(roleComboBox, 0, 3);
    detailLayout->addWidget(fontsMenuButton, 0, 4);
    detailLayout->addWidget(hideDetailButton, 0, 5);
    detailLayout->addWidget(editZone, 1, 0, 1, 6);
    firstnameEdit->hide();
    lastnameEdit->hide();
    nameEdit->hide();
    fontsMenuButton->hide();
    hideDetailButton->hide();
    editZone->hide();
    levelComboBox->hide();
    roleComboBox->hide();

    attendManager->setLayout(mainLayout);



    //sizes :

    QSize buttonSize(20, 30);
    toSheetButton->setFixedSize(buttonSize);
    toAllButton->setFixedSize(buttonSize);
    showDetailButton->setFixedSize(buttonSize);
    hideDetailButton->setFixedSize(buttonSize);
    newCharButton->setFixedSize(buttonSize);
    newItemButton->setFixedSize(buttonSize);
    newPlaceButton->setFixedSize(buttonSize);
    deleteButton->setFixedSize(buttonSize);

    QSize listSize(250, 250);
    projectList->setMinimumSize(listSize);
    managerSheetList->setMinimumSize(listSize);


    // display project list :
    setProjectList();





    // set target list :



    managerSheetListBox->setTitle(currentTabName);

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

    QList<int> intList;

    QHashIterator<int , QDomElement> i(domElementForItemNumber);
    while (i.hasNext()) {
        i.next();
        intList.append(i.key());
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


    QList<int>  attendIntList;

    while(!thisAttendStringList.isEmpty()){
        attendIntList.append(thisAttendStringList.takeFirst().toInt());
    }

    // remove the number 0 :
    if(attendIntList.contains(0))
        attendIntList.removeAt(attendIntList.indexOf(0));


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
    if(detailsHiddenBool == false ){
        //        saveThisDoc();
    }

    QListWidgetItem *abstractItem = abstractList->findItems(item->text(), Qt::MatchExactly).takeFirst();
    QString number = domElementForItem.value(abstractItem).attribute("number");
    currentElement = domElementForItem.value(abstractItem);


    if(detailsHiddenBool)
        showDetailAnimation();
    else
        showDetails();

    // fill the QLineEdits and the QComboBoxes :

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



    // fill the QtextEdit :

    QTextDocument *attendDoc = this->findChild<QTextDocument *>("attendDoc_" + number);
    editZone->openAttendDetail(attendDoc);

    editZone->setFocus();




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
    animation->setEndValue(QRect(xManager - 200,yManager,wManager + 200,hManager));
    animation->start();


    //     center the window if it leaves the screen :
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
    fontsMenuButton->show();
    showDetailButton->hide();
    hideDetailButton->show();
    editZone->show();



    detailsHiddenBool = false;


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
    animation->setEndValue(QRect(xManager + 200,yManager,wManager - 200,hManager));
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
    fontsMenuButton->hide();
    nameEdit->hide();
    levelComboBox->hide();
    roleComboBox->hide();

    hideDetailButton->hide();


    detailsHiddenBool = true;

}

//---------------------------------------------------------------------------

void AttendBox::saveAndUpdate()
{
    // saving :
    if(deletingItemBool == false && detailsHiddenBool == false ){
        saveThisDoc();
    }
    saveDomDocument();

    // updating :
    buildList();
    setProjectList();
    setManagerSheetList(currentManagerSheetList);
    setCurrentList(currentSheetNumber);
    emit projectAttendanceList(domElementForItem, domElementForItemNumber);
}

//---------------------------------------------------------------------------

void AttendBox::firstnameChanged()
{
    if(attendManagerLaunched == false)
        return;

    currentElement.setAttribute("firstName", firstnameEdit->text());

    saveAndUpdate();

}

//---------------------------------------------------------------------------
void AttendBox::lastnameChanged()
{
    if(attendManagerLaunched == false)
        return;

    currentElement.setAttribute("lastName", lastnameEdit->text());

    saveAndUpdate();

}

//---------------------------------------------------------------------------
void AttendBox::nameChanged()
{
    if(attendManagerLaunched == false)
        return;

    currentElement.setAttribute("name", nameEdit->text());

    saveAndUpdate();

}

//---------------------------------------------------------------------------
void AttendBox::levelChanged()
{
    if(attendManagerLaunched == false)
        return;

    currentElement.setAttribute("level", levelComboBox->currentIndex());

    saveAndUpdate();

}

//---------------------------------------------------------------------------
void AttendBox::roleChanged()
{
    if(attendManagerLaunched == false)
        return;

    currentElement.setAttribute("role", roleComboBox->currentIndex());

    saveAndUpdate();

}


//---------------------------------------------------------------------------
void AttendBox::newCharSlot()
{
    if(attendManagerLaunched == false)
        return;

    newAttendElementSlot("char");
}


//---------------------------------------------------------------------------
void AttendBox::newItemSlot()
{
    if(attendManagerLaunched == false)
        return;

    newAttendElementSlot("item");

}


//---------------------------------------------------------------------------
void AttendBox::newPlaceSlot()
{
    if(attendManagerLaunched == false)
        return;

    newAttendElementSlot("place");

}

//---------------------------------------------------------------------------
void AttendBox::newAttendElementSlot(QString tagName)
{
    newAttendElementBool = true;

    QDomElement newAttendElement = domDocument.createElement(tagName);
    newAttendElement.setTagName(tagName);

    // search for the maximum item number available :
    QHash<int, QDomElement>::iterator i = domElementForItemNumber.begin();
    int max = 0;
    while (i != domElementForItemNumber.end()) {


        max = qMax(max, i.key());

        ++i;
    }

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
        }


    projectList->scrollToItem(item);
    projectList->setItemSelected(item, true);
    projectItemActivated(item);


    if(tagName == "char")
        firstnameEdit->setFocus();
    else if(tagName == "item" || tagName == "place")
        nameEdit->setFocus();


    newAttendElementBool = false;
}


//---------------------------------------------------------------------------

void AttendBox::deleteItems()
{

    if(attendManagerLaunched == false)
        return;

    QList<QListWidgetItem *> itemList = projectList->selectedItems();
    if(itemList.size() == 0)
        return;

    int ret = QMessageBox::warning(attendManager, tr("Item Deletion"),
                                   tr("<p>The selected items will be"
                                      " permanently deleted.</p>\n"
                                      "<br>"
                                      "<p>Do you really want to continue ?</p>"),
                                   QMessageBox::Yes | QMessageBox::Cancel,
                                   QMessageBox::Cancel);

    switch (ret) {
    case QMessageBox::Yes:
    {
        deletingItemBool = true;


        if(detailsHiddenBool == false)
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

        //    QString string;
        //    qDebug() << "domElementList->size() : " << string.setNum(domElementList.size());
        //    qDebug() << "itemList->size() : " << string.setNum(itemList.size());
        //    qDebug() << "abstractList->count() : " << string.setNum(abstractList->count());


        for(int l=0; l < domElementList.size(); ++l){
            QTextDocument *text = this->findChild<QTextDocument *>("attendDoc_" + domElementList.at(l).attribute("number"));
            QFile *textFile = fileForDoc.value(text);
            textFile->remove();
            text->setObjectName("");
            fileForDoc.remove(text);


            emit removeAttendNumberSignal(domElementList.at(l).attribute("number").toInt());

            domElementList.at(l).parentNode().removeChild(domElementList.at(l));


        }
        saveAndUpdate();
        deletingItemBool =  false;

    }
        break;
    case QMessageBox::Cancel:
        return;

        break;
    default:
        // should never be reached
        break;


    }

}
//---------------------------------------------------------------------------
void AttendBox::toSheetSlot()
{
    QList<QListWidgetItem *> itemList = projectList->selectedItems();
    if(itemList.size() == 0)
        return;



    QList<int> projectDataList;
    for(int i=0; i < projectItemList.size(); ++i){
        projectDataList.append(projectItemList.at(i)->data(32).toInt());
    }

    // remove doubles :
    for(int j=0; j < managerSheetList->count(); ++j){
        int itemNumber = managerSheetList->item(j)->data(32).toInt();
        if(projectDataList.contains(itemNumber))
            projectDataList.removeOne(itemNumber);
    }



    emit addAttendNumberToSheetSignal(projectDataList, currentSheetNumber);
    setProjectList();
    setManagerSheetList(currentManagerSheetList);
    setCurrentList(currentSheetNumber);

}


//---------------------------------------------------------------------------
void AttendBox::toAllSlot()
{
    QList<QListWidgetItem *> itemList = managerSheetList->selectedItems();
    if(itemList.size() == 0)
        return;


    QList<int> sheetDataList;
    for(int i=0; i < managerSheetItemList.size(); ++i){
        sheetDataList.append(managerSheetItemList.at(i)->data(32).toInt());
    }


    emit removeAttendNumberFromSheetSignal(sheetDataList, currentSheetNumber);
    setProjectList();
    setManagerSheetList(currentManagerSheetList);
    setCurrentList(currentSheetNumber);

}
//---------------------------------------------------------------------------
void AttendBox::managerSheetListSelectionChanged()
{
    managerSheetItemList = managerSheetList->selectedItems();
}

//---------------------------------------------------------------------------
void AttendBox::projectListSelectionChanged()
{
    projectItemList = projectList->selectedItems();

}












//------------------------------------------------------------------------------
//------------Apply Config------------------------------------------------------
//------------------------------------------------------------------------------


void AttendBox::readSettings()
{
    QSettings settings;
    settings.beginGroup( "AttendManager" );
    attendManager->resize(settings.value( "size", QSize( 600, 400 ) ).toSize() );
    attendManager->move(settings.value( "pos" ).toPoint() );
    //    detailsHiddenBool = settings.value( "hideDetails" ).toBool();
    int detailTextHeight = settings.value("detailTextHeight", 12).toInt();
    int detailTextIndent = settings.value("detailTextIndent", 20).toInt();
    int detailTextMargin = settings.value("detailTextMargin", 5).toInt();
    QFont detailFont;
    detailFont.setFamily(settings.value("detailFontFamily", "Liberation Serif").toString());
    settings.endGroup();

    //    if(!detailsHiddenBool)
    //        showDetails();

    detailTextHeightSpin->setValue(detailTextHeight);
    detailFontCombo->setCurrentFont(detailFont);
    detailTextIndentSpin->setValue(detailTextIndent);
    detailTextMarginSpin->setValue(detailTextMargin);


    editZone->applyAttendConfig();

}

//---------------------------------------------------------------------------

void AttendBox::writeSettings()
{

    QSettings settings;
    settings.beginGroup( "AttendManager" );
    settings.setValue( "size", attendManager->size() );
    settings.setValue( "pos", attendManager->pos() );
    //    settings.setValue( "hideDetails", detailsHiddenBool );
    settings.setValue("detailTextHeight", detailTextHeightSpin->value());
    settings.setValue("detailTextIndent", detailTextIndentSpin->value());
    settings.setValue("detailTextMargin", detailTextMarginSpin->value());
    settings.setValue("detailFontFamily", detailFontCombo->currentFont());
    settings.endGroup();

    editZone->applyAttendConfig();
}


#if QT_VERSION >= 0x050000
#include <QtWidgets>
#endif 
#include <QtGui>   


#include "maintree.h"
//
MainTree::MainTree(QWidget *parent) :
    QTreeWidget(parent), outlinerLaunched(false)
{
    // The tree can accept drops
    setAcceptDrops( true );

    // The tree supports dragging of its own items
    setDragEnabled(true);

    setDragDropMode(QAbstractItemView::InternalMove);


    setExpandsOnDoubleClick(false);
    setAutoExpandDelay(1000);
    setAnimated(true);
    setHeaderHidden(true);

    treeOpened = false;
    setEnabled(false);


    folderIcon.addPixmap(style()->standardPixmap(QStyle::SP_DirClosedIcon),
                         QIcon::Normal, QIcon::Off);
    folderIcon.addPixmap(style()->standardPixmap(QStyle::SP_DirOpenIcon),
                         QIcon::Normal, QIcon::On);
    sceneIcon.addPixmap(style()->standardPixmap(QStyle::SP_FileIcon));


    prepareContextMenu();
    setContextMenuPolicy(Qt::PreventContextMenu);

}
bool MainTree::startTree()
{

    devicePath = hub->projectWorkPath();
    domDocument = hub->mainTreeDomDoc();

    //qDebug() << "domDocument : " << domDocument.toString();
    //qDebug() << "devicePath : " << devicePath;

    buildTree();


    //    connect(this, SIGNAL(itemClicked(QTreeWidgetItem*,int)),
    //            this, SLOT(openTextFile(QTreeWidgetItem*,int)));

    //setMouseTracking(true);

    //    connect(this, SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)),
    //            this, SLOT(rename(QTreeWidgetItem*)));

    connect(this, SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)),
            this, SLOT(openTextFile(QTreeWidgetItem*,int)));

    connect(this, SIGNAL(itemCollapsed(QTreeWidgetItem*)), this, SLOT(itemCollapsedSlot(QTreeWidgetItem*)));
    connect(this, SIGNAL(itemExpanded(QTreeWidgetItem*)), this, SLOT(itemExpandedSlot(QTreeWidgetItem*)));





    //    prjIsJustOpened = true;
    treeOpened = true;
    setEnabled(true);
    setContextMenuPolicy(Qt::DefaultContextMenu);


    //    QString debug;
    //    qDebug() << "fileForDoc : " << debug.setNum(fileForDoc.size());


    // read all attendance attributes :
    readAllAttendances();


    //open first book


    QDomElement child_ = domDocument.documentElement().firstChildElement("book");


    QTreeWidgetItem *item;
    item = domElementForItem.key(child_);
    openTextFile(domElementForItem.key(child_),0);
    item->setSelected(true);

    m_itemEntered = item;


    return true;
}





//---------------------------------------------------------------------------------------

void MainTree::closeTree()
{

    clear();


    if(outlinerLaunched){
        killOutliner();
    }



    //    qDebug() << "closeTree";


    //    QTreeWidgetItemIterator *iterator = new QTreeWidgetItemIterator(this);
    //    QDomElement element;

    //    while(iterator->operator *() != 0){

    //        MainTextDocument *doc = i.key();
    //        element = domElementForItem.value(iterator->operator *());
    //        if(element.tagName() != "separator")
    //        {
    //            if(element.tagName() != "separator"){
    //                QString num = element.attribute("number");
    //                MainTextDocument *textDocument = hub->findChild<MainTextDocument *>("textDoc_" + num);
    //                MainTextDocument *synDocument = hub->findChild<MainTextDocument *>("synDoc_" + num);
    //                MainTextDocument *noteDocument =  hub->findChild<MainTextDocument *>("noteDoc_" + num);

    //                textDocument->setObjectName("");
    //                synDocument->setObjectName("");
    //                noteDocument->setObjectName("");



    //            }


    //        }

    //        iterator->operator ++();

    //    }



    treeOpened = false;
    setEnabled(false);
    setContextMenuPolicy(Qt::PreventContextMenu);


    domElementForItem.clear();
    domElementForNumber.clear();
}

//---------------------------------------------------------------------------------------

void MainTree::updateDomElement(QTreeWidgetItem *item, int column)
{


    QDomElement newElement = domElementForItem.value(item);

    newElement.setAttribute("name", item->text(0));


    hub->addToSaveQueue();

    buildTree();

    //    if(newElement.attribute("name") == root.attribute("name") && newElement.tagName() == "book"){
    //        QStringList labels;


    //        header()->setResizeMode(QHeaderView::Stretch);
    //        setHeaderLabels(labels);
    //    }
    //else if(.attribute("name") == root.attribute("name")){
    //        setHeaderHidden(true);

    //    }
}

void MainTree::parseFolderElement(const QDomElement &element,
                                  QTreeWidgetItem *parentItem)
{
    QTreeWidgetItem *item = createItem(element, parentItem);

    QString title = element.attribute("name");
    if (title.isEmpty())
        title = QObject::tr("No Title");

    item->setFlags( Qt::ItemIsSelectable /*| Qt::ItemIsEditable*/ | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsDragEnabled);
    item->setIcon(0, folderIcon);
    item->setText(0, title);

    bool folded = (element.attribute("folded") != "no");
    setItemExpanded(item, !folded);

    QDomElement child = element.firstChildElement();
    while (!child.isNull()) {
        if (child.tagName() == "book") {
            item->setFlags(item->flags());
            parseFolderElement(child, item);

        } else if (child.tagName() == "chapter") {
            item->setFlags(item->flags());
            parseFolderElement(child, item);


        } else if (child.tagName() == "scene") {
            QTreeWidgetItem *childItem = createItem(child, item);

            QString title = child.attribute("name");
            if (title.isEmpty())
                title = QObject::tr("XML problem");


            childItem->setFlags(item->flags() /*| Qt::ItemIsEditable*/);
            childItem->setIcon(0, sceneIcon);
            childItem->setText(0, title);
        }

        else if (child.tagName() == "separator") {
            QTreeWidgetItem *childItem = createItem(child, item);
            childItem->setFlags(item->flags());
            childItem->setText(0, "             " + QString(10, 0xB7));
        }
        child = child.nextSiblingElement();
    }


}



QTreeWidgetItem *MainTree::createItem(const QDomElement &element,
                                      QTreeWidgetItem *parentItem)
{
    QTreeWidgetItem *item;
    if (parentItem) {
        item = new QTreeWidgetItem(parentItem);
    } else {
        item = new QTreeWidgetItem(this);
    }
    domElementForItem.insert(item, element);

    int number = element.attribute("number").toInt();
    domElementForNumber.insert(number, element);

    // ankward but temporary :
    hub->set_mainTree_domElementForNumberHash(domElementForNumber);
    return item;
}


bool MainTree::openTextFile(QTreeWidgetItem *treeItem,int column)
{
    //    if (prjIsJustOpened == false){
    //        QString action_Save;
    //        action_Save = "properSave";
    //        emit textAndNoteSignal(textFile, noteFile, synFile, name, action_Save);

    //    }


    if(domElementForItem.value(treeItem).tagName() == "separator")
        return true;

    itemTargetedForOutliner = treeItem;


    emit disconnectUpdateTextsSignal();


    //        qDebug() << "itemOpened :" << treeItem->text(0);

    QString action;
    action = "open";

    QDomElement domItem = domElementForItem.value(treeItem);
    int number = domItem.attribute("number").toInt();


    emit textAndNoteSignal(number, action);



    // for attendance :

    emit attendStringSignal(number, domItem.attribute("attend", "0"));


    //    prjIsJustOpened = false;

    emit connectUpdateTextsSignal();

    return true;
}


// context menu :

void MainTree::contextMenuEvent(QContextMenuEvent *event)
{



    QMenu menu(this);
    menu.addAction(renameAct);
    menu.addAction(addItemNextAct);
    menu.addAction(addChildAct);
    menu.addAction(addSeparatorAct);

    menu.addSeparator();

    menu.addAction(moveUpAct);
    menu.addAction(moveDownAct);

    menu.addSeparator();

    delItemMenu = menu.addMenu(tr("&Delete"));
    delItemMenu->addAction(delYesAct);

    menu.addSeparator();

    advancedMenu = menu.addMenu(tr("&Advanced"));
    advancedMenu->addAction(autoRenameChildsAct);
    advancedMenu->addAction(addMultiAct);
    advancedMenu->addAction(splitAct);

    menu.exec(event->globalPos());


}

void MainTree::prepareContextMenu()
{
    setMouseTracking(true);

    connect(this, SIGNAL(itemEntered(QTreeWidgetItem*,int)), this, SLOT(itemEnteredSlot(QTreeWidgetItem*,int)));
    connect(this, SIGNAL(itemActivated(QTreeWidgetItem*,int)),this,SLOT(itemActivatedSlot(QTreeWidgetItem*,int)));

    renameAct = new QAction(QIcon(":/pics/edit-rename.png"),tr("&Rename"), this);
    //renameAct->setShortcuts(QKeySequence::Rename);
    connect(renameAct, SIGNAL(triggered()), this, SLOT(rename()));

    addItemNextAct = new QAction(QIcon(":/pics/folder-new.png"),tr("Add &next"), this);
    //renameAct->setShortcuts(QKeySequence::Rename);
    connect(addItemNextAct, SIGNAL(triggered()), this, SLOT(addItemNext()));

    addChildAct = new QAction(QIcon(":/pics/document-new.png"),tr("Add &child"), this);
    //renameAct->setShortcuts(QKeySequence::Rename);
    connect(addChildAct, SIGNAL(triggered()), this, SLOT(addChild()));

    addSeparatorAct = new QAction(QIcon(":/pics/insert-horizontal-rule.png"),tr("Add &scene break"), this);
    //renameAct->setShortcuts(QKeySequence::Rename);
    connect(addSeparatorAct, SIGNAL(triggered()), this, SLOT(addSeparator()));

    moveUpAct = new QAction(QIcon(":/pics/arrow-up.png"),tr("Move Up"), this);
    //renameAct->setShortcuts(QKeySequence::Rename);
    connect(moveUpAct, SIGNAL(triggered()), this, SLOT(moveUp()));

    moveDownAct = new QAction(QIcon(":/pics/arrow-down.png"),tr("Move Down"), this);
    //renameAct->setShortcuts(QKeySequence::Rename);
    connect(moveDownAct, SIGNAL(triggered()), this, SLOT(moveDown()));

    delYesAct = new QAction(QIcon(":/pics/knotes_delete.png"),tr("C&onfirm"), this);
    //renameAct->setShortcuts(QKeySequence::Rename);
    connect(delYesAct, SIGNAL(triggered()), this, SLOT(delYesItem()));

    autoRenameChildsAct = new QAction(QIcon(":/pics/edit-rename.png"),tr("&Auto Rename Childs"), this);
    //renameAct->setShortcuts(QKeySequence::Rename);
    connect(autoRenameChildsAct, SIGNAL(triggered()), this, SLOT(autoRenameChilds()));

    splitAct = new QAction(tr("&Split"), this);
    //renameAct->setShortcuts(QKeySequence::Rename);
    connect(splitAct, SIGNAL(triggered()), this, SLOT(split()));

    addMultiAct = new QAction(QIcon(":/pics/document-multiple.png"),tr("Add &X Children"), this);
    //renameAct->setShortcuts(QKeySequence::Rename);
    connect(addMultiAct, SIGNAL(triggered()), this, SLOT(addMulti()));



}

//-------------------------------------------------------------------------------------

void MainTree::itemActivatedSlot(QTreeWidgetItem *treeItemPressed,int column)
{
    //    qDebug() << "jal itemActivatedSlot";
    openTextFile(treeItemPressed, column);


    //    qDebug() << "item activated : " << m_itemEntered->text(0);


}

void MainTree::itemEnteredSlot(QTreeWidgetItem *treeItemPressed,int column)
{


    m_itemEntered = treeItemPressed;
    m_preItemEntered = treeItemPressed; //for when it rename an item


    //    qDebug() << "item entered : " << m_itemEntered->text(0);

    //    if(treeItemPressed == 0){

    //            renameAct->setEnabled(false);
    //            addItemNextAct->setEnabled(false);
    //            addChildAct->setEnabled(false);
    //            //     delItemMenu->etEnabled(false);
    //            delYesAct->setEnabled(false);
    //    }
    //    else {
    //renameAct->setEnabled(true);
    //            addItemNextAct->setEnabled(true);
    //            addChildAct->setEnabled(true);
    //            //   delItemMenu->setEnabled(true);
    //            delYesAct->setEnabled(true);
    //    }
}

void MainTree::rename()
{
    // it's for context menu
    rename(m_itemEntered);
}

void MainTree::rename(QTreeWidgetItem *item)
{


    if(domElementForItem.value(item).tagName() == "separator")
        return;

    bool ok;
    QString text = QInputDialog::getText(this, tr("Rename sheet"),
                                         tr("Sheet name :"), QLineEdit::Normal,
                                         item->text(0), &ok);
    if (ok && !text.isEmpty()){
        item->setText(0, text);


        //  m_itemPressed->setFlags(m_itemPressed->flags());
        //editItem(m_itemPressed, 0);
        //  m_itemPressed->setFlags(m_preFlagItemPressed->flags());
        m_itemEntered = item;
        emit nameChangedSignal(text, domElementForItem.value(item).attribute("number").toInt());

        if(outlinerLaunched)
            updateOutliner();
    }
}

int MainTree::addItemNext(int baseNumber)
{
    int newNumber;

    QTreeWidgetItem *baseItem = domElementForItem.key(domElementForNumber.value(baseNumber));

    QTreeWidgetItem *newItem = addItemNext(baseItem);
    newNumber = domElementForNumber.key(domElementForItem.value(newItem));
    return newNumber;
}

QTreeWidgetItem* MainTree::addItemNext(QTreeWidgetItem *item)
{
    if(item == 0)
        item = m_itemEntered;


    // adding to Dom
    //        qDebug() << "Item worked with:" << item->text(0);
    QDomElement element = domElementForItem.value(item);



    QDomElement newElement = domDocument.createElement("nothing");

    //    qDebug() << "newElement : " << newElement.tagName();

    element.parentNode().insertAfter(newElement, element);


    modifyAttributes(element, newElement, "sibling");


    hub->addToSaveQueue();


    buildTree();

    //    qDebug() << "outlinerLaunched : " << outlinerLaunched;

    //    if(outlinerLaunched){
    //        killOutliner();
    //        launchOutliner();
    //    }
    if(outlinerLaunched)
        updateOutliner();

    return domElementForItem.key(newElement);


}

//------------------------------------------------------------------------------------

QTreeWidgetItem* MainTree::addChild(QTreeWidgetItem *item)
{
    if(item == 0)
        item = m_itemEntered;


    // adding to Dom
    QDomElement element = domElementForItem.value(item);
    //    qDebug() << element.tagName();
    if(element.tagName() == "scene" || element.tagName() == "separator")
        return 0;







    QDomElement newElement = domDocument.createElement("nothing");
    element.appendChild(newElement);




    newElement = modifyAttributes(element, newElement, "child");

    // expand the parent in order to see the child :
    itemExpandedSlot(item);


    hub->addToSaveQueue();

    buildTree();


    //    if(outlinerLaunched){
    //        killOutliner();
    //        launchOutliner();
    //        //        insertOutlinerItem(newElement.attribute("number").toInt(), element.attribute("number").toInt());
    //    }

    if(outlinerLaunched)
        updateOutliner();







    //QDomNode newNode;
    //    QDomNode parentNode = element;
    //    QDomNode childNode = parentNode.lastChild();
    //    QDomElement newElement;

    //    if(childNode.isNull()){
    //        qDebug() << "jal d1 ";
    //        QDomNode newNode = element.cloneNode(false);
    //        newNode = parentNode.appendChild(newNode);

    //        qDebug() << "jal e1 ";
    //        QString level = "child";


    //        newElement = modifyAttributes(parentNode.toElement(), newNode.toElement(), level);
    //        newElement.setAttribute("name", tr("new element"));
    //        qDebug() << "jal e ";



    //    }
    //    else if(!childNode.isNull()){
    //        qDebug() << "jal d2 ";

    //        newNode = childNode.cloneNode(false);
    //        newNode = parentNode.insertAfter(newNode, childNode);

    //        newElement = childNode.nextSiblingElement();

    //        //    QDomElement childElement = childNode.toElement();

    //        QString level = "child";


    //        newElement = modifyAttributes(parentNode.toElement(), newElement, level);
    //        newElement.setAttribute("name", tr("new element"));
    //        qDebug() << "jal e2 ";

    //    }



    // adding to treeWidget

    //    QTreeWidgetItem *parentItem;
    //    QTreeWidgetItem *_newItem;
    //    QTreeWidgetItem *scal_newItem = _newItem;

    //    parentItem = m_itemEntered;

    //    parentItem->addChild(_newItem);





    //    //adding to QHash

    //    domElementForItem.insert(scal_newItem ,newElement);


    //    this->write(deviceFile);


    //    buildTree();


    return domElementForItem.key(newElement);
}

//---------------------------------------------------------------------------------------

QTreeWidgetItem * MainTree::addSeparator(QTreeWidgetItem * item)
{
    if(item == 0)
        item = m_itemEntered;



    // adding to Dom
    //    qDebug() << "Item worked with:" << item->text(0);
    QDomElement element = domElementForItem.value(item);

    if(element.tagName() == "chapter" || element.tagName() == "book")
        return 0;



    QDomElement newElement = domDocument.createElement("nothing");
    element.parentNode().insertAfter(newElement, element);







    modifyAttributes(element, newElement, "separator");





    hub->addToSaveQueue();


    buildTree();

    if(outlinerLaunched)
        updateOutliner();


    return domElementForItem.key(newElement);
}

//---------------------------------------------------------------------------------------

void MainTree::moveUp()
{
    disconnect(this, SIGNAL(itemChanged(QTreeWidgetItem*,int)),
               this, SLOT(updateDomElement(QTreeWidgetItem*,int)));


    QDomElement element = domElementForItem.value(m_itemEntered);
    QDomNode newNode(element);

    QDomNode *parentElement = new QDomNode(element.parentNode());
    QDomNode *firstChildNode =  new QDomNode(parentElement->firstChild());
    QDomNode *prevParentElement = new QDomNode(parentElement->previousSibling());
    QDomNode prevChild(prevParentElement->lastChild().toElement());
    QDomNode prevSibling(element.previousSibling().toElement());
    QDomNode elementMoved;




    if(element == firstChildNode->toElement() && parentElement->previousSibling().isNull()){
        return;

    }

    if(element == firstChildNode->toElement()){
        if(prevChild.isNull()){
            elementMoved = prevParentElement->appendChild(newNode);

        }
        else{
            elementMoved = prevParentElement->insertAfter(newNode, prevChild);

        }


    }
    else{

        elementMoved = parentElement->insertBefore(newNode, prevSibling);

    }


    connect(this, SIGNAL(itemChanged(QTreeWidgetItem*,int)),
            this, SLOT(updateDomElement(QTreeWidgetItem*,int)));

    hub->addToSaveQueue();

    buildTree();

    //    if(outlinerLaunched){

    //        killOutliner();
    //        launchOutliner();
    //    }
    if(outlinerLaunched)
        updateOutliner();
    //    disconnect(this, SIGNAL(itemChanged(QTreeWidgetItem*,int)),
    //               this, SLOT(updateDomElement(QTreeWidgetItem*,int)));

    //    QTreeWidgetItem *item = new QTreeWidgetItem(domElementForItem.key(elementMoved.toElement()));
    //    //openTextFile(domElementForItem.key(element),0);
    //    item->setSelected(true);

    //    m_itemEntered = item;


    //    connect(this, SIGNAL(itemChanged(QTreeWidgetItem*,int)),
    //            this, SLOT(updateDomElement(QTreeWidgetItem*,int)));
}

//---------------------------------------------------------------------------------------

void MainTree::moveDown()
{

    disconnect(this, SIGNAL(itemChanged(QTreeWidgetItem*,int)),
               this, SLOT(updateDomElement(QTreeWidgetItem*,int)));


    QDomElement element = domElementForItem.value(m_itemEntered);
    QDomNode newNode(element);

    QDomNode *parentElement = new QDomNode(element.parentNode());
    QDomNode *lastChildNode =  new QDomNode(parentElement->lastChild());
    QDomNode *nextParentElement = new QDomNode(parentElement->nextSibling());
    QDomNode nextChild(nextParentElement->firstChild().toElement());
    QDomNode nextSibling(element.nextSibling().toElement());
    QDomNode elementMoved;




    if(element == lastChildNode->toElement() && parentElement->nextSibling().isNull()){
        return;

    }

    if(element == lastChildNode->toElement()){
        if(nextChild.isNull()){
            elementMoved = nextParentElement->appendChild(newNode);

        }
        else{
            elementMoved = nextParentElement->insertBefore(newNode, nextChild);

        }


    }
    else{

        elementMoved = parentElement->insertAfter(newNode, nextSibling);

    }


    connect(this, SIGNAL(itemChanged(QTreeWidgetItem*,int)),
            this, SLOT(updateDomElement(QTreeWidgetItem*,int)));

    hub->addToSaveQueue();

    buildTree();

    //    if(outlinerLaunched){

    //        killOutliner();
    //        launchOutliner();
    //    }
    if(outlinerLaunched)
        updateOutliner();
}

//---------------------------------------------------------------------------------------

void MainTree::delYesItem()
{
    int ret = QMessageBox::warning(this, tr("Sheet Deletion"),
                                   tr("<p>The selected sheet and its children "
                                      "will be permanently deleted.</p>\n"
                                      "<br>"
                                      "<p>Do you really want to continue ?</p>"),
                                   QMessageBox::Yes | QMessageBox::Cancel,
                                   QMessageBox::Cancel);

    switch (ret) {
    case QMessageBox::Yes:
    {


        // ignore the only book left :   (or crash...)
        if(this->topLevelItemCount() == 1 && this->topLevelItem(0) == m_itemEntered)
            return;





        QDomElement element = domElementForItem.value(m_itemEntered);


        if(!m_itemEntered->parent() && m_itemEntered->childCount() != 0){


            QMessageBox::information(this, tr("Plume Creator Info"),
                                     tr("For safety measure, you are\n"
                                        "not allowed delete a filled book."));


            return;

            //        if(current->parent())
            //            parentItem = current->parent();
            //        else
            //            parentItem = headerItem();
        }


        while(element.hasChildNodes()){

            //            while(element.hasChildNodes()){


            //                removeItem(element.firstChild().firstChild().toElement());
            //                element.removeChild(element.firstChild().firstChild());



            //            }
            removeItem(element.firstChild().toElement());
            element.removeChild(element.firstChild());




        }










        //    if(element.hasChildNodes()){



        //            if(nodeList.at(i).hasChildNodes()){

        //                }



        //            }


        //            textFile.setFileName(devicePath + nodeList.at(i).toElement().attribute("textPath") + ".temp");
        //            noteFile.setFileName(devicePath + nodeList.at(i).toElement().attribute("notePath") + ".temp");
        //            synFile.setFileName(devicePath + nodeList.at(i).toElement().attribute("synPath") + ".temp");
        //            textFile.remove();
        //            noteFile.remove();
        //            synFile.remove();





        //        while(element.hasChildNodes()){

        //            QDomNodeList nodeList = element.childNodes();

        //            int numFile;
        //            int numOriginalTextChar_point = 0;
        //            QString originalTextFile = element.firstChild().toElement().attribute("textPath");



        //            originalTextFile = originalTextFile.mid(7);

        //            for (int j = 0; j < originalTextFile.size(); ++j) {
        //                if (originalTextFile.at(j) == QChar('.'))
        //                    numOriginalTextChar_point = j;



        //            }
        //            qDebug()<< "erase childs parent";

        //            originalTextFile = originalTextFile.left(numOriginalTextChar_point);

        //            numFile = originalTextFile.toInt();


        //            freeNumList.append(numFile);





        //            while(element.firstChild().hasChildNodes()){




        //                int numFil;
        //                int numOriginalTxtChar_point = 0;
        //                QString originalTxtFile = element.firstChild().firstChild().toElement().attribute("textPath");



        //                originalTxtFile = originalTxtFile.mid(7);

        //                for (int p = 0; p < originalTxtFile.size(); ++p) {
        //                    if (originalTxtFile.at(p) == QChar('.'))
        //                        numOriginalTxtChar_point = p;



        //                }


        //                qDebug()<< "erase childs 3";

        //                originalTxtFile = originalTxtFile.left(numOriginalTxtChar_point);

        //                numFil = originalTxtFile.toInt();


        //                freeNumList.append(numFil);

        //                QString string = element.firstChild().firstChild().toElement().attribute("number");
        //                emit(textAndNoteSignal(string.toInt(),"close"));
        //                QFile txtFile(devicePath + element.firstChild().firstChild().toElement().attribute("textPath"));
        //                QFile notFile(devicePath + element.firstChild().firstChild().toElement().attribute("notePath"));
        //                QFile syFile(devicePath + element.firstChild().firstChild().toElement().attribute("synPath"));
        //                txtFile.remove();
        //                notFile.remove();
        //                syFile.remove();

        //                element.firstChild().removeChild(element.firstChild().firstChild());


        //            }


        //            emit textAndNoteSignal(element.firstChild().toElement().attribute("number").toInt(),"close");
        //            QFile textFil(devicePath + element.firstChild().toElement().attribute("textPath"));
        //            QFile noteFil(devicePath + element.firstChild().toElement().attribute("notePath"));
        //            QFile synFil(devicePath + element.firstChild().toElement().attribute("synPath"));
        //            textFil.remove();
        //            noteFil.remove();
        //            synFil.remove();

        //            element.removeChild(element.firstChild());





        //            //    QString debug;
        //            qDebug()<< "erase childs 1";

        //        }

        //    textFile.setFileName(devicePath + element.attribute("textPath") + ".temp");
        //    noteFile.setFileName(devicePath + element.attribute("notePath") + ".temp");
        //    synFile.setFileName(devicePath + element.attribute("synPath") + ".temp");
        //    textFile.remove();
        //    noteFile.remove();
        //    synFile.remove();







        //   domElementForItem.remove(m_itemEntered);

        removeItem(element);

        element.parentNode().removeChild(element);

        //       takeChild(numIndex);




        //free freeNumList



        hub->addToSaveQueue();
        hub->connectAllSheetsToWordCountThread();


        buildTree();

        //        if(outlinerLaunched){

        //            killOutliner();
        //            launchOutliner();
        //        }

        if(outlinerLaunched)
            updateOutliner();
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



void MainTree::removeItem(QDomElement element)
{
    if(element.toElement().tagName() == "separator")
        return;



    int number = domElementForNumber.key(element.toElement());


    freeNumList.append(number);

    emit(textAndNoteSignal(number,"close"));

    QString string;
    MainTextDocument *text = hub->findChild<MainTextDocument *>("textDoc_" + string.setNum(number));
    MainTextDocument *note = hub->findChild<MainTextDocument *>("noteDoc_" + string.setNum(number));
    MainTextDocument *syn = hub->findChild<MainTextDocument *>("synDoc_" + string.setNum(number));


    QHash<MainTextDocument *, QFile *> fileForDoc = hub->mainTree_fileForDocHash();

    QFile *textFile = fileForDoc.value(text);
    QFile *noteFile = fileForDoc.value(note);
    QFile *synFile = fileForDoc.value(syn);

    textFile->remove();
    noteFile->remove();
    synFile->remove();

    //temporary :
    hub->removeFileFromZipList("text", number);
    hub->removeFileFromZipList("note", number);
    hub->removeFileFromZipList("syn", number);

    fileForDoc.remove(text);
    fileForDoc.remove(note);
    fileForDoc.remove(syn);

    hub->set_mainTree_fileForDocHash(fileForDoc);


    QHash<MainTextDocument *, int> numForDoc = hub->mainTree_numForDocHash();


    numForDoc.remove(text);
    numForDoc.remove(note);
    numForDoc.remove(syn);

    hub->set_mainTree_numForDocHash(numForDoc);


    text->deleteLater();
    note->deleteLater();
    syn->deleteLater();

}


void MainTree::autoRenameChilds()
{

    QString chapterName = tr("Chapter");
    QString sceneName = tr("Scene");
    QString numString;
    QString newName;
    int preNum = 1;
    int num = 1;

    QDomElement element = domElementForItem.value(m_itemEntered);


    if(!element.hasChildNodes())
        return;

    if(element.tagName() == "book"){

        QDomElement first = element.firstChildElement("chapter");

        first.setAttribute("name", chapterName + " " + numString.setNum(num,10));
        emit nameChangedSignal(chapterName + " " + numString.setNum(num,10), first.attribute("number").toInt());

        while(!first.nextSiblingElement("chapter").isNull()){

            num = preNum + 1;
            newName = chapterName + " " + numString.setNum(num,10);
            QDomElement next = first.nextSiblingElement("chapter");
            next.setAttribute("name", newName);

            emit nameChangedSignal(newName, next.attribute("number").toInt());
            //            qDebug() << "renamingSlot : " << newName << " " << numString.setNum(next.attribute("number").toInt(),10);

            preNum = num;
            first = next;

            //            if(outlinerLaunched)
            //                updateOutliner();

        }

    }

    if(element.tagName() == "chapter"){

        QDomElement first = element.firstChildElement("scene");

        first.setAttribute("name", sceneName + " " + numString.setNum(num,10));
        emit nameChangedSignal(sceneName + " " + numString.setNum(num,10), first.attribute("number").toInt());

        while(!first.nextSiblingElement("scene").isNull()){

            num = preNum + 1;
            newName = sceneName + " " + numString.setNum(num,10);
            QDomElement next = first.nextSiblingElement("scene");
            next.setAttribute("name", newName);

            emit nameChangedSignal(newName, next.attribute("number").toInt());
            //            qDebug() << "renamingSlot : " << newName << " " << numString.setNum(next.attribute("number").toInt(),10);

            preNum = num;
            first = next;


        }
    }


    hub->addToSaveQueue();

    buildTree();


}

//-----------------------------------------------------------------------------------


void MainTree::split()
{
    QDialog *splitDialog = new QDialog(this);
    splitDialog->setWindowTitle(tr("Split Dialog"));
    QVBoxLayout *layout = new QVBoxLayout;

    QLabel *titleText = new QLabel(tr("<p>With this tool, you can split a text in several parts."
                                      "For each part, Plume will create a chapter or a scene.</p><br>"));
    QLabel *mainText = new QLabel(tr("<center><b>Each sheet type has different behaviour !</b></center>"
                                     "<br>"
                                     "<p>"
                                     "<blockquote>In a scene sheet : split <b>only</b> into scenes with *** .</blockquote>"
                                     "<blockquote>In a chapter sheet : split into scenes with *** and into chapters with ### .</blockquote>"
                                     "<blockquote>In a book sheet : split into scenes with *** and into chapters with ### .</blockquote></p>"
                                     ));

    QFormLayout *formLayout = new QFormLayout;

    QComboBox *textChoiceBox = new QComboBox;
    QStringList textChoices;
    textChoices << tr("Text") << tr("Synopsys") << tr("Note");
    textChoiceBox->addItems(textChoices);

    connect(textChoiceBox, SIGNAL(currentIndexChanged(int)), this, SLOT(splitChoiceChanged(int)));
    textChoiceBox->setCurrentIndex(0);
    splitChoiceChanged(0);

    formLayout->addRow(tr("Choose the type of document that you want to split :"), textChoiceBox);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Cancel | QDialogButtonBox::Ok);

    connect(buttonBox, SIGNAL(accepted()), splitDialog, SLOT(close()));
    connect(buttonBox, SIGNAL(accepted()), this, SLOT(splitYes()));
    connect(buttonBox, SIGNAL(rejected()), splitDialog, SLOT(close()));

    layout->addWidget(titleText);
    layout->addSpacing(30);
    layout->addWidget(mainText);
    layout->addSpacing(30);
    layout->addLayout(formLayout);
    layout->addWidget(buttonBox);

    splitDialog->setLayout(layout);

    splitDialog->exec();



}

//-------------------------------------------------------------------------------------
void MainTree::splitChoiceChanged(int choice)
{
    switch (choice) {
    case 0:
        splitChoice = "textDoc_";
        break;

    case 1:
        splitChoice = "synDoc_";
        break;
    case 2:
        splitChoice = "noteDoc_";
        break;
    default:
        splitChoice = "textDoc_";
        break;

        //        qDebug() << "splitChoice : " << splitChoice;
    }
}

//-------------------------------------------------------------------------------------
void MainTree::splitYes()
{
    // saving

    disconnect(this, SIGNAL(itemChanged(QTreeWidgetItem*,int)),
               this, SLOT(updateDomElement(QTreeWidgetItem*,int)));





    emit textAndNoteSignal(0,"save");


    QApplication::setOverrideCursor(Qt::WaitCursor);


    //                 set up the progress bar :

    QWidget *progressWidget = new QWidget(this, Qt::Dialog | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    QHBoxLayout *progressLayout = new QHBoxLayout(progressWidget);
    QProgressBar *progressBar = new QProgressBar(progressWidget);
    int progressValue = 0;

    progressLayout->addWidget(progressBar);
    progressWidget->setLayout(progressLayout);




    //splitting :

    itemOfWork = m_itemEntered;

    QFile *txtFile = new QFile;
    QString txtPath;
    QTreeWidgetItem *childItem;
    QTreeWidgetItem *childItem2;
    QTreeWidgetItem *item;
    QTreeWidgetItem *childItemOfWork;
    QTreeWidgetItem *itemOfWork2;
    QTreeWidgetItem *lastItem = new QTreeWidgetItem;
    QTreeWidgetItem *nextItem = new QTreeWidgetItem;
    QStringList chaptersList, scenesList, allScenesList;
    QList<bool> resultsList;
    QList<QTreeWidgetItem *> childsList;
    QTextDocumentWriter *docWriter = new QTextDocumentWriter;
    docWriter->setFormat("HTML");


    QDomElement domItem = domElementForItem.value(itemOfWork);

    int number = domItem.attribute("number").toInt();

    QString string;
    MainTextDocument *textDoc = hub->findChild<MainTextDocument *>( splitChoice + string.setNum(number,10));
    QString mainString = textDoc->toHtml("utf-8");


    if(domItem.tagName() == "scene"){

        scenesList = mainString.split("***", QString::KeepEmptyParts);

        progressBar->setMaximum(scenesList.size() - 1);
        progressBar->setValue(progressValue);
        progressWidget->show();

        if(scenesList.size() == 1){
            QMessageBox::information(this, tr("Splitting Task"), tr("This sheet does not contain *** ."));
            return;
        }
        //            QString debug;
        //            qDebug() << "scenesList.size() : " << debug.setNum(scenesList.size(), 10);
        for(int i = 0; i < scenesList.size(); ++i){

            item = addItemNext(itemOfWork);
            int num = domElementForItem.value(item).toElement().attribute("number").toInt();
            MainTextDocument *doc = hub->findChild<MainTextDocument *>(splitChoice + string.setNum(num,10));
            doc->setHtml(scenesList.at(i));

            itemOfWork = item;

            progressValue += 1;
            progressBar->setValue(progressValue);
        }


    }
    else if(domItem.tagName() == "chapter"){
        chaptersList = mainString.split("###", QString::KeepEmptyParts);

        progressBar->setMaximum(chaptersList.size() - 1);
        progressBar->setValue(progressValue);
        progressWidget->show();

        for(int i = 0; i < chaptersList.size(); ++i){
            item = addItemNext(itemOfWork);

            scenesList = chaptersList.at(i).split("***", QString::KeepEmptyParts);
            childItem = addChild(item);
            int num = domElementForItem.value(childItem).toElement().attribute("number").toInt();
            MainTextDocument *doc = hub->findChild<MainTextDocument *>(splitChoice + string.setNum(num,10));
            doc->setHtml(scenesList.at(0));

            progressValue += 1;
            progressBar->setValue(progressValue);

            for(int j = 1; j < scenesList.size(); ++j){

                itemOfWork2 = addItemNext(childItem);
                int num = domElementForItem.value(itemOfWork2).toElement().attribute("number").toInt();
                MainTextDocument *doc = hub->findChild<MainTextDocument *>(splitChoice + string.setNum(num,10));
                doc->setHtml(scenesList.at(j));
                childItem = itemOfWork2;
            }
            itemOfWork = item;

            progressValue += 1;
            progressBar->setValue(progressValue);
        }
    }
    else if(domItem.tagName() == "book"){
        chaptersList = mainString.split("###", QString::KeepEmptyParts);

        progressBar->setMaximum(chaptersList.size() - 1);
        progressBar->setValue(progressValue);
        progressWidget->show();

        // build the first child chapter and its first child :

        childItem = addChild(itemOfWork);
        //            int num = domElementForItem.value(childItem).toElement().attribute("number").toInt();
        //            MainTextDocument *doc = hub->findChild<MainTextDocument *>(splitChoice + string.setNum(num,10));
        //            doc->setHtml(chaptersList.at(0));

        scenesList = chaptersList.at(0).split("***", QString::KeepEmptyParts);
        childItem2 = addChild(childItem);
        int num = domElementForItem.value(childItem2).toElement().attribute("number").toInt();
        MainTextDocument *doc = hub->findChild<MainTextDocument *>(splitChoice + string.setNum(num,10));
        doc->setHtml(scenesList.at(0));

        progressValue += 1;
        progressBar->setValue(progressValue);

        for(int k = 1; k < scenesList.size(); ++k){

            itemOfWork2 = addItemNext(childItem2);
            int num = domElementForItem.value(itemOfWork2).toElement().attribute("number").toInt();
            MainTextDocument *doc = hub->findChild<MainTextDocument *>(splitChoice + string.setNum(num,10));
            doc->setHtml(scenesList.at(k));
            childItem2 = itemOfWork2;

        }



        // build the next chapters and their first child scenes :

        for(int i = 1; i < chaptersList.size(); ++i){
            item = addItemNext(childItem);

            scenesList = chaptersList.at(i).split("***", QString::KeepEmptyParts);
            childItem = addChild(item);
            int num = domElementForItem.value(childItem).toElement().attribute("number").toInt();
            MainTextDocument *doc = hub->findChild<MainTextDocument *>(splitChoice + string.setNum(num,10));
            doc->setHtml(scenesList.at(0));

            for(int j = 1; j < scenesList.size(); ++j){

                itemOfWork2 = addItemNext(childItem);
                int num = domElementForItem.value(itemOfWork2).toElement().attribute("number").toInt();
                MainTextDocument *doc = hub->findChild<MainTextDocument *>(splitChoice + string.setNum(num,10));
                doc->setHtml(scenesList.at(j));
                childItem = itemOfWork2;
            }
            childItem = item;

            progressValue += 1;
            progressBar->setValue(progressValue);
        }
    }
    else{
        qWarning() << "------------------ PROBLEM SPLIT";
        QApplication::restoreOverrideCursor();
        progressWidget->close();

        return;
    }

    QApplication::restoreOverrideCursor();
    progressWidget->close();



    connect(this, SIGNAL(itemChanged(QTreeWidgetItem*,int)),
            this, SLOT(updateDomElement(QTreeWidgetItem*,int)));


    chaptersList.clear();
    scenesList.clear();


    if(outlinerLaunched)
        updateOutliner();
}

//-------------------------------------------------------------------------------------
void MainTree::addMulti()
{
    bool ok;
    int numSheets = QInputDialog::getInt(this, tr("Add X Children"),
                                         tr("Please enter a number :"),
                                         2, 1, 100, 1, &ok);

    if (!ok || numSheets == 0)
        return;

    for(int i = 1; i <= numSheets; ++i){
        QTreeWidgetItem *item = m_itemEntered;
        QDomElement element = domElementForItem.value(item);
        if(element.tagName() == "scene")
            return;
        QDomElement newElement = domDocument.createElement("nothing");
        element.appendChild(newElement);



        newElement = modifyAttributes(element, newElement, "child");



    }

    hub->addToSaveQueue();


    buildTree();

    //    if(outlinerLaunched){

    //        killOutliner();
    //        launchOutliner();
    //    }
    if(outlinerLaunched)
        updateOutliner();
}












//-------------------------------------------------------------------------------------
QDomElement MainTree::modifyAttributes(QDomElement originalElement,QDomElement newElement, QString level )
{



    if(level == "separator"){



        newElement.setTagName("separator");
        newElement.setAttribute("name", "----");//for config




        //        if(originalElement.tagName() == "book"){
        //            newElement.setTagName("separator");
        //            newElement.setAttribute("name", "----");//for config

        //        }
        //        if(originalElement.tagName() == "chapter"){
        //            newElement.setTagName("separator");
        //            newElement.setAttribute("name", "----");//for config

        //        }

        return newElement;

    }




    QString textChar;
    int freeNum;



    if(freeNumList.isEmpty()){  // for the first time


        QDir dir(devicePath + "/text/");

        QStringList filters;
        filters << "*.html" << "*.html.temp";
        dir.setNameFilters(filters);
        dir.setSorting(QDir::Name);


        QList<int> numList;
        int numFile;

        QString originalTextFile;
        int numOriginalTextChar_point = 0;

        QStringList stringList;
        stringList = dir.entryList();


        for (int i = 0; i < stringList.size(); ++i) {
            originalTextFile = stringList.at(i);

            QString letter = originalTextFile;
            letter = letter.left(1);
            originalTextFile = originalTextFile.mid(1);

            for (int i = 0; i < originalTextFile.size(); ++i) {
                if (originalTextFile.at(i) == QChar('.'))
                    numOriginalTextChar_point = i;
            }

            originalTextFile = originalTextFile.left(numOriginalTextChar_point);

            numFile = originalTextFile.toInt();
            //            QString debug;
            //            qDebug() << "numFile : " << debug.setNum(numFile);
            if(letter == "T"){
                numList.append(numFile);
                //                                QString debug;
                //                                qDebug() << "numFile : " << debug.setNum(numFile);
            }
        }

        qSort(numList.begin(), numList.end());




        //        qDebug() << "numList.size() : "<< numList.size() ;

        int maxNumList = numList.last();
        //        qDebug() << "maxNumList : "  <<QString::number(maxNumList) ;

        int num = 1;
        while(!numList.isEmpty()){



            while(numList.first() > num && num < numList.last()){
                //                qDebug() << "numList.first() : " << debug.setNum(numList.first());



                freeNumList.append(num);
                num += 1;
            }
            num = numList.first() + 1;
            numList.removeFirst();
        }

        //int num = 0;
        //for(int k = 0; k < numList.size(); ++k){
        //    QString debug;
        //        while(numList.at(k) != num){
        //            freeNumList.append(num);


        //            qDebug() << "freeNum : " << debug.setNum(num);
        //           num += 1;
        //        }
        //         qDebug() << "numList.first() : " << debug.setNum(numList.at(k));
        //    num = numList.at(k) +1;
        //    }

        if(!freeNumList.isEmpty()){
            freeNum = freeNumList.takeFirst();
        }
        else {
            freeNum = maxNumList + 1;
        }

    }
    else {


        // save work time :-)
        freeNum = freeNumList.takeFirst();
    }
    if(freeNum == 0 && !freeNumList.isEmpty())
        freeNum = freeNumList.takeFirst();



    //    QString debug;
    //    qDebug() << "freeNumList is empty : " << freeNumList.isEmpty();
    //    qDebug() << "freeNum used : " << debug.setNum(freeNum);

    newElement.setAttribute("textPath", "/text/T" + textChar.setNum(freeNum, 10) + ".html");
    newElement.setAttribute("notePath", "/text/N" + textChar.setNum(freeNum, 10) + ".html");
    newElement.setAttribute("synPath", "/text/S" + textChar.setNum(freeNum, 10) + ".html");
    newElement.setAttribute("number", textChar.setNum(freeNum, 10));
    newElement.setAttribute("textCursorPos", textChar.setNum(0, 10));
    newElement.setAttribute("synCursorPos", textChar.setNum(0, 10));
    newElement.setAttribute("noteCursorPos", textChar.setNum(0, 10));


    QFile *textFile = new QFile(devicePath + newElement.attribute("textPath"));
    QFile *noteFile = new QFile(devicePath + newElement.attribute("notePath"));
    QFile *synFile = new QFile(devicePath + newElement.attribute("synPath"));

    //    textFile.remove();
    //    textFile.open(QFile::WriteOnly | QFile::Text);
    //    QTextStream out(&textFile);
    //    out << "";
    //    textFile.close();
    int number = newElement.attribute("number").toInt();
    QHash<MainTextDocument *, QFile *> fileForDoc = hub->mainTree_fileForDocHash();
    QHash<MainTextDocument *, int> numForDoc = hub->mainTree_numForDocHash();

    MainTextDocument *textDocument = new MainTextDocument(hub);
    textDocument->setIdNumber(number);
    textDocument->setDocType("text");
    textDocument->toHtml();
    QTextDocumentWriter textDocWriter(textFile, "HTML");
    textDocWriter.write(textDocument);
    textDocument->setObjectName("textDoc_" + QString::number(number));
    fileForDoc.insert(textDocument, textFile);
    numForDoc.insert(textDocument, number);


    MainTextDocument *noteDocument = new MainTextDocument(hub);
    noteDocument->setIdNumber(number);
    noteDocument->setDocType("note");
    noteDocument->toHtml();
    QTextDocumentWriter noteDocWriter(noteFile, "HTML");
    noteDocWriter.write(noteDocument);
    noteDocument->setObjectName("noteDoc_" + QString::number(number));
    fileForDoc.insert(noteDocument, noteFile);
    numForDoc.insert(noteDocument, number);


    MainTextDocument *synDocument = new MainTextDocument(hub);
    synDocument->setIdNumber(number);
    synDocument->setDocType("synopsis");
    synDocument->toHtml();
    QTextDocumentWriter synDocWriter(synFile, "HTML");
    synDocWriter.write(synDocument);
    synDocument->setObjectName("synDoc_" + QString::number(number));
    fileForDoc.insert(synDocument, synFile);
    numForDoc.insert(synDocument, number);

    hub->set_mainTree_fileForDocHash(fileForDoc);
    hub->set_mainTree_numForDocHash(numForDoc);
    hub->connectAllSheetsToWordCountThread();

    //temporary :
    hub->addFileToZipList("text", number);
    hub->addFileToZipList("note", number);
    hub->addFileToZipList("syn", number);

    if(level == "sibling"){
        newElement.setTagName(originalElement.tagName());
        if(originalElement.tagName() == "book"){
            newElement.setTagName("book");
            newElement.setAttribute("name", tr("New Book")); //for config

        }
        if(originalElement.tagName() == "chapter"){
            newElement.setTagName("chapter");
            newElement.setAttribute("name", tr("New Chapter"));//for config

        }
        if(originalElement.tagName() == "scene" || originalElement.tagName() == "separator"){
            newElement.setTagName("scene");
            newElement.setAttribute("name", tr("New Scene"));//for config

        }

    }
    if(level == "child"){
        if(originalElement.tagName() == "book"){
            newElement.setTagName("chapter");
            newElement.setAttribute("name", tr("New Chapter"));//for config

        }
        if(originalElement.tagName() == "chapter"){
            newElement.setTagName("scene");
            newElement.setAttribute("name", tr("New Scene"));//for config

        }
        //        }
        //        else{
        //            newElement.setTagName(originalElement.lastChild().toElement().tagName());
        //        }
    }
    return newElement;
}
//------------------------------------------------------------------------------------

void MainTree::buildTree()
{


    disconnect(this, SIGNAL(itemChanged(QTreeWidgetItem*,int)),
               this, SLOT(updateDomElement(QTreeWidgetItem*,int)));

    clear();


    QDomElement child = domDocument.documentElement().firstChildElement("book");
    while (!child.isNull()) {
        parseFolderElement(child);
        child = child.nextSiblingElement("book");
    }

    connect(this, SIGNAL(itemChanged(QTreeWidgetItem*,int)),
            this, SLOT(updateDomElement(QTreeWidgetItem*,int)));


    if(outlinerLaunched)
        updateOutliner();

}


//------------------------------------------------------------------------------------


//void MainTree::keyPressEvent(QKeyEvent *event)
//{
//    // shortcuts :


//     //       if(event->key() == Qt::Key_Return)



//}







void MainTree::dropEvent(QDropEvent* event)
{
    disconnect(this, SIGNAL(itemChanged(QTreeWidgetItem*,int)),
               this, SLOT(updateDomElement(QTreeWidgetItem*,int)));


    targetItem = itemAt(event->pos());

    if( mParent - 1== getQTreeWidgetItemDepth(targetItem) && dropIndicatorPosition() == QAbstractItemView::OnItem)
    {


        QDomElement draggedElement = domElementForItem.value(draggedItem);
        QDomElement targetElement = domElementForItem.value(targetItem);

        QDomNode draggedNode(draggedElement);
        QDomNode targetNode(targetElement);


        //check
        if(draggedElement.tagName() == "scene"){
            if(targetElement.tagName() == "chapter")
                targetNode.appendChild(draggedNode);
            else
                return;
        }

        if(draggedElement.tagName() == "separator"){
            if(targetElement.tagName() == "chapter")
                targetNode.appendChild(draggedNode);
            else
                return;
        }

        if(draggedElement.tagName() == "chapter"){
            if(targetElement.tagName() == "book")
                targetNode.appendChild(draggedNode);
            else
                return;
        }
        if(draggedElement.tagName() == "book"){
            return;
        }




        qWarning() << "drop event  mParent - 0";
        QTreeView::dropEvent(event);

        hub->addToSaveQueue();
        buildTree();

        if(outlinerLaunched){

            killOutliner();
            launchOutliner();
        }

    }
    else if ( mParent == getQTreeWidgetItemDepth(targetItem) && dropIndicatorPosition() == QAbstractItemView::AboveItem )
    {


        QDomElement draggedElement = domElementForItem.value(draggedItem);
        QDomElement targetElement = domElementForItem.value(targetItem);

        QDomNode draggedNode(draggedElement);
        QDomNode targetNode(targetElement);

        QDomNode *parentElement = new QDomNode(targetNode.parentNode());


        //check
        if(draggedElement.tagName() == targetElement.tagName()
                || (draggedElement.tagName() == "separator" && targetElement.tagName() == "scene")
                || (draggedElement.tagName() == "scene" && targetElement.tagName() == "separator"))
            parentElement->insertBefore(draggedNode, targetNode);
        else
            return;


        qWarning() << "drop event  mParent - 1 above";
        QTreeView::dropEvent(event);


        hub->addToSaveQueue();
        buildTree();


        if(outlinerLaunched){

            killOutliner();
            launchOutliner();
        }

    }
    else if ( mParent == getQTreeWidgetItemDepth(targetItem) && dropIndicatorPosition() == QAbstractItemView::BelowItem)
    {

        QDomElement draggedElement = domElementForItem.value(draggedItem);
        QDomElement targetElement = domElementForItem.value(targetItem);

        QDomNode draggedNode(draggedElement);
        QDomNode targetNode(targetElement);

        QDomNode *parentElement = new QDomNode(targetNode.parentNode());

        if(draggedElement.tagName() == targetElement.tagName()
                || (draggedElement.tagName() == "separator" && targetElement.tagName() == "scene")
                || (draggedElement.tagName() == "scene" && targetElement.tagName() == "separator")
                )
            parentElement->insertAfter(draggedNode, targetNode);
        else
            return;





        qWarning() << "drop event  mParent - 1 below";
        QTreeView::dropEvent(event);

        hub->addToSaveQueue();
        buildTree();

        if(outlinerLaunched){

            killOutliner();
            launchOutliner();
        }

    }
    else
    {

        event->ignore();


    }


    connect(this, SIGNAL(itemChanged(QTreeWidgetItem*,int)),
            this, SLOT(updateDomElement(QTreeWidgetItem*,int)), Qt::UniqueConnection);


}

int MainTree::getQTreeWidgetItemDepth(QTreeWidgetItem*item)
{
    if(item==NULL)
        return 0;
    if(item->parent()==NULL)
        return 1; //book
    if(item->parent()->parent()==NULL)
        return 2;  //chapter
    if(item->parent()->parent()->parent()==NULL)
        return 3; //scene

    return 0;

}

//--------------------------------------------------------------------------------------

void MainTree::mousePressEvent(QMouseEvent* event)
{

    QTreeWidgetItem* item= itemAt(event->pos());
    qWarning() << getQTreeWidgetItemDepth (item);
    mParent = getQTreeWidgetItemDepth (item);
    QTreeWidget::mousePressEvent(event);

    //    qDebug() << "pressed attribute : " << domElementForItem.value(item).attribute("textPath");
    //    qDebug() << "pressed attri num : " << domElementForItem.value(item).attribute("number");

}

//------------------------------------------------------------------------------------

void MainTree::dragEnterEvent( QDragEnterEvent *event )
{
    disconnect(this, SIGNAL(itemChanged(QTreeWidgetItem*,int)),
               this, SLOT(updateDomElement(QTreeWidgetItem*,int)));



    draggedItem= m_itemEntered;


    if(mParent != 1)
    {
        this->invisibleRootItem()->setFlags(Qt::ItemIsSelectable|Qt::ItemIsDragEnabled|Qt::ItemIsEnabled);
    }
    else
    {
        this->invisibleRootItem()->setFlags(Qt::ItemIsSelectable|Qt::ItemIsDragEnabled|Qt::ItemIsDropEnabled|Qt::ItemIsEnabled);
    }

    QTreeWidgetItemIterator it(this);
    while (*it)
    {

        if (getQTreeWidgetItemDepth(*it) == (mParent - 1))
        {
            (*it)->setFlags(Qt::ItemIsSelectable|Qt::ItemIsDragEnabled|Qt::ItemIsDropEnabled|Qt::ItemIsEnabled);
        }

        else
        {
            (*it)->setFlags(Qt::ItemIsSelectable|Qt::ItemIsDragEnabled|Qt::ItemIsEnabled);
        }


        ++it;
    }

    QTreeWidget::dragEnterEvent(event);

    connect(this, SIGNAL(itemChanged(QTreeWidgetItem*,int)),
            this, SLOT(updateDomElement(QTreeWidgetItem*,int)));
}















//-----------------------------------------------------------------------------------------

void MainTree::itemCollapsedSlot(QTreeWidgetItem* item)
{
    QDomElement element = domElementForItem.value(item);
    element.setAttribute("folded", "yes");
}

//-----------------------------------------------------------------------------------------

void MainTree::itemExpandedSlot(QTreeWidgetItem* item)
{
    QDomElement element = domElementForItem.value(item);
    element.setAttribute("folded", "no");

}
//-----------------------------------------------------------------------------------------

void MainTree::saveCursorPos(int textCursorPosition, int synCursorPosition, int noteCursorPosition, int number)
{
    QDomElement element = domElementForNumber.value(number);
    element.setAttribute("textCursorPos", textCursorPosition);
    element.setAttribute("synCursorPos", synCursorPosition);
    element.setAttribute("noteCursorPos", noteCursorPosition);
    //   this->write(deviceFile);

    hub->addToSaveQueue();

}


////-----------------------------------------------------------------------------------------


//bool MainTree::saveDoc(MainTextDocument *doc)
//{
//    //    qDebug() << "saveDoc";

//    QFile *file = hub->mainTree_fileForDocHash().value(doc);
//    //    qDebug() << "saveDoc : " << doc->objectName();
//    //    qDebug() << "saveDoc : " << file->fileName();
//    file->close();
//    QTextDocumentWriter *docWriter = new QTextDocumentWriter(file, "HTML");
//    MainTextDocument *clonedDoc = doc->clone();
//    bool written = docWriter->write(clonedDoc);

//    //    qDebug() << "saveDoc finished";

//    delete docWriter;
//    delete clonedDoc;

//    return written;
//}


//-----------------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------------

























//-----------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------
//------------Outliner-----------------------------------------------------
//-----------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------

void MainTree::launchOutliner()
{

    if(outlinerLaunched){
        // connect to raise and set active windows//        QApplication::setActiveWindow(outliner);
        //   outliner->raise();
        emit showOutlinerSignal();
        return;
    }

    outlinerBase = new OutlinerBase(0);
    outlinerBase->setHub(hub);

    connect(this, SIGNAL(showOutlinerSignal()), outlinerBase, SLOT(showOutliner()));


    // UpdatesToOutliner :

    //    connect(this,SIGNAL(), outlinerBase, SLOT());


    //for domDocument :
    //            to Outline :
outlinerBase->mtoO_setDomDoc(domDocument);
    connect(this, SIGNAL(resetDomDocSignal()), outlinerBase, SLOT(updateOutliner()));
   emit resetDomDocSignal();

    //                    from Outline :
    connect(outlinerBase, SIGNAL(updateMainDomDocSignal()), this,SLOT(updateMainDomDocFromOutliner()));
    connect(outlinerBase, SIGNAL(otoM_actionSignal(QString,int)), this, SLOT(otoM_actionSlot(QString,int)));
    //for TextDocuments :


    //            to Outline :

    connect(this, SIGNAL(setNumForDocSignal(QHash<MainTextDocument *, int>)), outlinerBase, SLOT(mtoO_setNumForDoc(QHash<MainTextDocument *, int>)));

    emit setNumForDocSignal(setNumForDoc());

    //                    from Outline :

    connect(outlinerBase, SIGNAL(applySynNoteFontConfigSignal()), this, SIGNAL(applySynNoteFontConfigSignal()));



// deprecated :
    //for attendance :
    //           to Outline :
    connect(this, SIGNAL(allAttendancesSignal(QHash<int,QString>)), outlinerBase, SLOT(mtoO_updateAttendances(QHash<int,QString>)));
    connect(this, SIGNAL(projectAttendanceList(QHash<QListWidgetItem*,QDomElement>,QHash<int,QDomElement>))
            ,outlinerBase, SLOT(mtoO_setProjectAttendanceList(QHash<QListWidgetItem*,QDomElement>,QHash<int,QDomElement>)));

    emit projectAttendanceList(attend_domElementForItem, attend_domElementForItemNumber);
    readAllAttendances();






    outlinerLaunched = true;



    //    if(outlinerLaunched){
    //        QApplication::setActiveWindow(outliner);
    //        outliner->raise();
    //        return;
    //    }

    //    outliner = new Outline(0);

    //    outliner->applyConfig();
    //    buildOutliner();

    //    connect(outliner, SIGNAL(destroyed()), this, SLOT(deletedSlot()));
    //    connect(outliner, SIGNAL(outlinerClosedSignal()), this, SLOT(outlinerClosed()));
    //    connect(this, SIGNAL(nameChangedSignal(QString,int)), outliner, SLOT(setItemTitle(QString,int)));
    //    connect(outliner, SIGNAL(newOutlineTitleSignal(QString,int)), this, SLOT(newOutlineTitleSlot(QString,int)));
    //    connect(this, SIGNAL(connectUpdateTextsSignal()), outliner, SIGNAL(connectUpdateTextsSignal()));
    //    connect(this, SIGNAL(disconnectUpdateTextsSignal()), outliner, SIGNAL(disconnectUpdateTextsSignal()));
    //    connect(outliner, SIGNAL(writeThisSignal(int)), this, SLOT(writeThisSlot(int)));

    //    //for attendance :
    //    connect(this, SIGNAL(allAttendancesSignal(QHash<int,QString>)), outliner, SLOT(updateAttendances(QHash<int,QString>)));
    //    connect(this, SIGNAL(projectAttendanceList(QHash<QListWidgetItem*,QDomElement>,QHash<int,QDomElement>))
    //            ,outliner, SLOT(setProjectAttendanceList(QHash<QListWidgetItem*,QDomElement>,QHash<int,QDomElement>)));

    //    outliner->setProjectAttendanceList(attend_domElementForItem, attend_domElementForItemNumber);

    //    readAllAttendances();

    //    outlinerLaunched = true;
}

//-----------------------------------------------------------------------------------------

QHash<MainTextDocument *, int> MainTree::setNumForDoc()
{

    return hub->mainTree_numForDocHash();
}
//-----------------------------------------------------------------------------------------

void MainTree::updateOutliner()
{
    if(!outlinerLaunched){
        return;
    }

    emit resetDomDocSignal();
    emit setNumForDocSignal(setNumForDoc());

    outlinerBase->updateOutliner();
    qDebug() << "Outliner updated";


}

void MainTree::updateMainDomDocFromOutliner()
{
    if(!outlinerLaunched){
        return;
    }

    buildTree();


}
//-----------------------------------------------------------------------------------------


void MainTree::otoM_actionSlot(QString action, int idNumber)
{
    m_itemEntered = domElementForItem.key(domElementForNumber.value(idNumber));

    if(action == "rename")
        this->rename();
    if(action == "addItemNext")
        this->addItemNext();
    if(action == "addChild")
        this->addChild();
    if(action == "addSeparator")
        this->addSeparator();
    if(action == "moveUp")
        this->moveUp();
    if(action == "moveDown")
        this->moveDown();
    if(action == "removeItem")
        this->delYesItem();
    if(action == "autoRenameChilds")
        this->autoRenameChilds();
    if(action == "addMulti")
        this->addMulti();
}








//-----------------------------------------------------------------------------------------


void MainTree::buildOutliner()
{

    //deprecated

    // build outline :

    //    QTreeWidgetItemIterator *iterator = new QTreeWidgetItemIterator(this);
    //    QDomElement element;

    //    while(iterator->operator *() != 0){


    //        element = domElementForItem.value(iterator->operator *());
    //        if(element.tagName() != "separator")
    //        {

    //            QString num = element.attribute("number");
    //            MainTextDocument *synDocument = hub->findChild<MainTextDocument *>("synDoc_" + num);
    //            MainTextDocument *noteDocument =  hub->findChild<MainTextDocument *>("noteDoc_" + num);

    //            outliner->buildItem(synDocument, noteDocument, element.attribute("name"), element.attribute("number").toInt(), element.tagName());





    //        }
    //        if(element.tagName() == "separator")
    //        {
    //            outliner->buildSeparator();
    //        }

    //        iterator->operator ++();

    //    }

    //    outliner->buildStretcher();
    //    QTimer::singleShot( 5, this, SLOT(setOutlineViewPos()) );










}

//-----------------------------------------------------------------------------------------

void MainTree::killOutliner()
{
    //    saveOutlineSettings();
    //    outliner->close();
    //    outliner->deleteLater();
    //    outlinerLaunched = false;

    //    if(outlinerLaunched){
    //        QApplication::setActiveWindow(outliner);
    //        outliner->raise();
    //        return;
    //    }

    outlinerBase->saveConfig();
    outlinerBase->deleteLater();
    outlinerLaunched = false;
}

//-----------------------------------------------------------------------------------------

void MainTree::deletedSlot()
{
    //    qDebug() << "Outliner destroyed !!!!!!!!!!!!!!" ;
}
//-----------------------------------------------------------------------------------------
void MainTree::newOutlineTitleSlot(QString newTitle,int number)
{
    QDomElement element = domElementForNumber.value(number);
    element.setAttribute("name", newTitle);

    hub->addToSaveQueue();


    buildTree();
}
//-----------------------------------------------------------------------------------------
void MainTree::insertOutlinerItem(int newNumber, int numberOfRef)
{


    //    QDomElement element = domElementForNumber.value(newNumber);

    //    QString string;
    //    MainTextDocument *synDocument = hub->findChild<MainTextDocument *>("synDoc_" + string.setNum(newNumber));
    //    MainTextDocument *noteDocument =  hub->findChild<MainTextDocument *>("noteDoc_" + string.setNum(newNumber));

    //    OutlineItem *newWidget = outliner->buildItem(synDocument, noteDocument, element.attribute("name"), element.attribute("number").toInt(), element.tagName());


    //    outliner->insertItemInOutliner(newWidget,numberOfRef,element.tagName());
}
//-----------------------------------------------------------------------------------------


void MainTree::writeThisSlot(int number)
{
    openTextFile(domElementForItem.key(domElementForNumber.value(number)), 0);
}




void MainTree::setOutlineViewPos()
{
    //               QString objName = doc->objectName();
    //                            int number = objName.mid(objName.indexOf("_")).toInt();
    //    QDomElement enteredElement = domElementForItem.value(/*itemTargetedForOutliner*/m_itemEntered);
    //    widgetTargetedNumber = enteredElement.attribute("number", "1").toInt();
    //    outliner->setOutlinerViewportPos(widgetTargetedNumber);

    //    qDebug() << "widgetTargetedNumber : " << enteredElement.attribute("number", "1");

    //    QDomElement targetElement;



    //    QDomNodeList nodeList = domDocument.childNodes();
    //    for (i = 0; i < nodeList.size(); ++i){

    //        QDomNode node = nodeList.at(i);

    //        if(node.toElement().tagName() == "outlinerSettings"){
    //            targetElement = node.toElement();
    //        }
    //    }

    //    if(targetElement.tagName() == "outlinerSettings"){
    //        QString num = targetElement.attribute("widgetTargetedNumber", "1");

    //         outliner->setOutlinerViewportPos(num.toInt());
    //    }
    //    else{  // if there isn't an element "outlinerSettings" :
    //        QDomElement element = domDocument.createElement("outlinerSettings");
    //        element.setAttribute("widgetTargetedNumber", "1");

    //        domDocument.appendChild(element);
    //        QString num = targetElement.attribute("widgetTargetedNumber", "1");

    //         outliner->setOutlinerViewportPos(num.toInt());
    //    }
}

//-----------------------------------------------------------------------------------------
void MainTree::saveOutlineSettings()
{



    //    QDomElement targetElement;
    //    QString string;
    //    QDomNodeList nodeList = domDocument.childNodes();
    //    for (int i = 0; i < nodeList.size(); ++i){

    //        QDomNode node = nodeList.at(i);

    //        if(node.toElement().tagName() == "outlinerSettings"){
    //            targetElement = node.toElement();
    //        }
    //    }


    //        targetElement.setAttribute("widgetTargetedNumber", string.setNum(widgetTargetedNumber));



}






//-----------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------





MainTextDocument * MainTree::prevText(int num)
{
    //find directly before :
    int prevNum = domElementForNumber.key(domElementForNumber.value(num).previousSiblingElement(domElementForNumber.value(num).tagName()));


    //find before the father :
    if(prevNum == 0){
        QDomElement father = domElementForNumber.value(num).parentNode().toElement();
        QDomElement prevFather = father.previousSiblingElement(father.tagName());
        QDomElement lastChild = prevFather.lastChild().toElement();
        prevNum = domElementForNumber.key(lastChild);
    }
    //cancel :
    else if(prevNum == 0)
        return 0;

    QString string;
    MainTextDocument *textDoc = hub->findChild<MainTextDocument *>("textDoc_" + string.setNum(prevNum,10));
    //    qDebug() << "prevNum : " << string;

    return textDoc;
}

//-----------------------------------------------------------------------------------------------

MainTextDocument * MainTree::nextText(int num)
{
    //find directly before :
    int nextNum = domElementForNumber.key(domElementForNumber.value(num).nextSiblingElement(domElementForNumber.value(num).tagName()));


    //find after the father :
    if(nextNum == 0){
        QDomElement father = domElementForNumber.value(num).parentNode().toElement();
        QDomElement nextFather = father.nextSiblingElement(father.tagName());
        QDomElement firstChild = nextFather.firstChild().toElement();
        nextNum = domElementForNumber.key(firstChild);
    }
    //cancel :
    else if(nextNum == 0)
        return 0;

    QString string;
    MainTextDocument *textDoc = hub->findChild<MainTextDocument *>("textDoc_" + string.setNum(nextNum,10));
    //    qDebug() << "nextNum : " << string;

    return textDoc;
}

//-----------------------------------------------------------------------------------------------

void MainTree::readAllAttendances()
{




    QHash<int, QString> attendStringForNumber;

    QHash<int, QDomElement>::const_iterator  i = domElementForNumber.constBegin();
    while (i != domElementForNumber.constEnd()) {
        QDomElement element = i.value();
        attendStringForNumber.insert(i.key(), element.attribute("attend", "0"));
        ++i;
    }

    emit allAttendancesSignal(attendStringForNumber);
}
//----------------------------------------------------------------------------------
void MainTree::setOutlinerProjectAttendList(QHash<QListWidgetItem *, QDomElement> domElementForItem_, QHash<int, QDomElement> domElementForItemNumber_)
{
    attend_domElementForItem = domElementForItem_;
    attend_domElementForItemNumber = domElementForItemNumber_;

}

//----------------------------------------------------------------------------------
void MainTree::removeAttendNumberSlot(int itemNumber)
{
    QHash<QTreeWidgetItem *, QDomElement>::const_iterator  i = domElementForItem.constBegin();
    while (i != domElementForItem.constEnd()) {
        QString attendString;
        QDomElement element = i.value();

        attendString = element.attribute("attend", "0");
        //      qDebug() << "attendString : " << attendString;

        QStringList thisAttendStringList = attendString.split("-", QString::SkipEmptyParts);

        QString string;

        if(thisAttendStringList.contains(string.setNum(itemNumber)))
            thisAttendStringList.removeAt(thisAttendStringList.indexOf(string.setNum(itemNumber)));

        QString newAttendString;

        for( int j = 0 ; j < thisAttendStringList.size(); ++j)
            newAttendString.append(thisAttendStringList.at(j) + "-");
        //      qDebug() << "newAttendString : " << newAttendString;

        element.setAttribute("attend", newAttendString);
        ++i;
    }

    hub->addToSaveQueue();
    readAllAttendances();
}

//----------------------------------------------------------------------------------

void MainTree::addAttendNumberToSheetSlot(QList<int> list, int sheetNumber)
{
    //    QHash<QTreeWidgetItem *, QDomElement>::const_iterator  i = domElementForItem.constBegin();
    //    while (i != domElementForItem.constEnd()) {
    QString attendString;
    QDomElement element = domElementForNumber.value(sheetNumber);

    attendString = element.attribute("attend", "0");

    QStringList thisAttendStringList = attendString.split("-", QString::SkipEmptyParts);

    //        qDebug() << "attendString : "<< attendString;
    QString string;
    for(int j=0 ; j < list.size(); ++j){
        int itemNumber = list.at(j);
        thisAttendStringList.append(string.setNum(itemNumber));
    }

    QString newAttendString;

    for( int i = 0 ; i < thisAttendStringList.size(); ++i)
        newAttendString.append(thisAttendStringList.at(i) + "-");

    //     qDebug() << "newAttendString : "<< newAttendString;

    element.setAttribute("attend", newAttendString);
    //        ++i;
    //    }


    hub->addToSaveQueue();
    readAllAttendances();
}

//----------------------------------------------------------------------------------

void MainTree::removeAttendNumberFromSheetSlot(QList<int> list, int sheetNumber)
{


    //    QHash<QTreeWidgetItem *, QDomElement>::const_iterator  i = domElementForItem.constBegin();
    //    while (i != domElementForItem.constEnd()) {
    QString attendString;
    QDomElement element = domElementForNumber.value(sheetNumber);

    attendString = element.attribute("attend", "0");

    QStringList thisAttendStringList = attendString.split("-", QString::SkipEmptyParts);

    //    qDebug() << "attendString : "<< attendString;
    QString string;
    for(int j=0 ; j < list.size(); ++j){
        int itemNumber = list.at(j);
        if(thisAttendStringList.contains(string.setNum(itemNumber)))
            thisAttendStringList.removeAt(thisAttendStringList.indexOf(string.setNum(itemNumber)));
    }
    QString newAttendString;

    for( int i = 0 ; i < thisAttendStringList.size(); ++i)
        newAttendString.append(thisAttendStringList.at(i) + "-");

    //    qDebug() << "newAttendString : "<< newAttendString;

    element.setAttribute("attend", newAttendString);
    //        ++i;
    //    }

    hub->addToSaveQueue();
    readAllAttendances();
}

//----------------------------------------------------------------------------------

void MainTree::giveDocsAndDomForProjectWordCount()
{
    emit docsForProjectWordCountSignal(hub->mainTree_fileForDocHash());
    emit domForProjectWordCountSignal(domDocument.cloneNode().toDocument());
}

//----------------------------------------------------------------------------------


void MainTree::changeAllDocsTextStyles()
{
    textStyles->loadBaseStyles();
    textStyles->loadStyles();

    QHash<MainTextDocument *, QFile *> fileForDoc = hub->mainTree_fileForDocHash();
    QHash<MainTextDocument *, QFile *>::const_iterator i = fileForDoc.constBegin();
    while (i != fileForDoc.constEnd()) {

        if(i.key()->objectName().left(7) == "textDoc"){

            textStyles->changeDocStyles(i.key());


        }

        ++i;
    }

}

//------------------------------------------------------------------------------------

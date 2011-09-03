#include <QtGui>


#include "maintree.h"

MainTree::MainTree(QWidget *parent) :
    QTreeWidget(parent)
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
bool MainTree::read(QFile *device)
{

    deviceFile = device;

    QString errorStr;
    int errorLine;
    int errorColumn;

    if (!domDocument.setContent(device, true, &errorStr, &errorLine,
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


    QFileInfo *dirInfo = new QFileInfo(*device);
    devicePath = dirInfo->path();
//    qDebug() << "File path:" << devicePath;


    root = domDocument.documentElement();
    if (root.tagName() != "plume") {
        QMessageBox::information(this, tr("Plume Creator Tree"),
                                 tr("The file is not a a Plume Creator project file."));
        return false;
    } else if (root.hasAttribute("version")
               && root.attribute("version") != "0.2") {
        QMessageBox::information(this, tr("Plume Creator Tree"),
                                 tr("The file is not an Plume Creator project file version 1.0 "
                                    "file."));
        return false;
    }



    buildTree();


    connect(this, SIGNAL(itemClicked(QTreeWidgetItem*,int)),
            this, SLOT(openTextFile(QTreeWidgetItem*,int)));

    //setMouseTracking(true);

    connect(this, SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)),
            this, SLOT(rename(QTreeWidgetItem*)));

    connect(this, SIGNAL(itemCollapsed(QTreeWidgetItem*)), this, SLOT(itemCollapsedSlot(QTreeWidgetItem*)));
    connect(this, SIGNAL(itemExpanded(QTreeWidgetItem*)), this, SLOT(itemExpandedSlot(QTreeWidgetItem*)));



    device->close();


    //    prjIsJustOpened = true;
    treeOpened = true;
    setEnabled(true);
    setContextMenuPolicy(Qt::DefaultContextMenu);


    //open first book


    QDomElement child_ = root.firstChildElement("book");


    QTreeWidgetItem *item;
    item = domElementForItem.key(child_);
    openTextFile(domElementForItem.key(child_),0);
    item->setSelected(true);

    m_itemEntered = item;

    return true;
}



bool MainTree::write(QFile *device)
{


    device->waitForBytesWritten(500);
    device->close();
    device->open(QFile::ReadWrite | QFile::Text | QFile::Truncate);
    if(device->isWritable())
    {
        device->flush();


        const int IndentSize = 4;

        QTextStream out(device);
        out.flush();
        domDocument.save(out, IndentSize);
        device->close();


        qDebug() << "Dom saved in xml via write(QFile *device)";

        return true;

    }
    else{


        qDebug() << "Dom saved in xml via write(QFile *device) :     error ";
        return false;
    }

}

//---------------------------------------------------------------------------------------

void MainTree::closeTree()
{

    clear();

    treeOpened = false;
    setEnabled(false);
    setContextMenuPolicy(Qt::PreventContextMenu);



}

//---------------------------------------------------------------------------------------

void MainTree::updateDomElement(QTreeWidgetItem *item, int column)
{


    QDomElement newElement = domElementForItem.value(item);

    newElement.setAttribute("name", item->text(0));



    this->write(deviceFile);

    buildTree();

    //    this->write(deviceFile);



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
        title = QObject::tr("XML problem : parseFolderElement(const QDomElement &element, QTreeWidgetItem *parentItem)");

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
   domElementForNumber[number] = element;
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


    qDebug() << "itemOpened :" << treeItem->text(0);


    QString action;
    action = "open";

    QDomElement domItem = domElementForItem.value(treeItem);
    QString textName;
    textName = domItem.attribute("textPath");
    QString noteName;
    noteName = domItem.attribute("notePath");
    QString synName;
    synName = domItem.attribute("synPath");
    name = domItem.attribute("name");
    int number = domItem.attribute("number").toInt();
    int textCursorPos = domItem.attribute("textCursorPos").toInt();
    int synCursorPos = domItem.attribute("synCursorPos").toInt();
    int noteCursorPos = domItem.attribute("noteCursorPos").toInt();
    textFile = new QFile(devicePath + textName);
    noteFile = new QFile(devicePath + noteName);
    synFile = new QFile(devicePath + synName);


    emit textAndNoteSignal(textFile, noteFile, synFile, textCursorPos, synCursorPos, noteCursorPos, name, number,  action);

    //    prjIsJustOpened = false;


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

    renameAct = new QAction(tr("&Rename"), this);
    //renameAct->setShortcuts(QKeySequence::Rename);
    connect(renameAct, SIGNAL(triggered()), this, SLOT(rename()));

    addItemNextAct = new QAction(tr("Add &next"), this);
    //renameAct->setShortcuts(QKeySequence::Rename);
    connect(addItemNextAct, SIGNAL(triggered()), this, SLOT(addItemNext()));

    addChildAct = new QAction(tr("Add &child"), this);
    //renameAct->setShortcuts(QKeySequence::Rename);
    connect(addChildAct, SIGNAL(triggered()), this, SLOT(addChild()));

    addSeparatorAct = new QAction(tr("Add &scene break"), this);
    //renameAct->setShortcuts(QKeySequence::Rename);
    connect(addSeparatorAct, SIGNAL(triggered()), this, SLOT(addSeparator()));

    moveUpAct = new QAction(tr("Move Up"), this);
    //renameAct->setShortcuts(QKeySequence::Rename);
    connect(moveUpAct, SIGNAL(triggered()), this, SLOT(moveUp()));

    moveDownAct = new QAction(tr("Move Down"), this);
    //renameAct->setShortcuts(QKeySequence::Rename);
    connect(moveDownAct, SIGNAL(triggered()), this, SLOT(moveDown()));

    delYesAct = new QAction(tr("C&onfirm"), this);
    //renameAct->setShortcuts(QKeySequence::Rename);
    connect(delYesAct, SIGNAL(triggered()), this, SLOT(delYesItem()));

    autoRenameChildsAct = new QAction(tr("&Auto Rename Childs"), this);
    //renameAct->setShortcuts(QKeySequence::Rename);
    connect(autoRenameChildsAct, SIGNAL(triggered()), this, SLOT(autoRenameChilds()));

    splitAct = new QAction(tr("&Split"), this);
    //renameAct->setShortcuts(QKeySequence::Rename);
    connect(splitAct, SIGNAL(triggered()), this, SLOT(split()));

    addMultiAct = new QAction(tr("Add &X Children"), this);
    //renameAct->setShortcuts(QKeySequence::Rename);
    connect(addMultiAct, SIGNAL(triggered()), this, SLOT(addMulti()));



}

//-------------------------------------------------------------------------------------

void MainTree::itemActivatedSlot(QTreeWidgetItem *treeItemPressed,int column)
{
    openTextFile(treeItemPressed, column);


    qDebug() << "item activated : " << m_itemEntered->text(0);


}

void MainTree::itemEnteredSlot(QTreeWidgetItem *treeItemPressed,int column)
{


    m_itemEntered = treeItemPressed;
    m_preItemEntered = treeItemPressed; //for when it rename an item


    qDebug() << "item entered : " << m_itemEntered->text(0);

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


void MainTree::rename(QTreeWidgetItem *item)
{

    if(domElementForItem.value(item).tagName() == "separator")
        return;

    bool ok;
    QString text = QInputDialog::getText(this, tr("QInputDialog::getText()"),
                                         tr("Sheet name :"), QLineEdit::Normal,
                                         item->text(0), &ok);
    if (ok && !text.isEmpty()){
        item->setText(0, text);


    //  m_itemPressed->setFlags(m_itemPressed->flags());
    //editItem(m_itemPressed, 0);
    //  m_itemPressed->setFlags(m_preFlagItemPressed->flags());
    m_itemEntered = item;
    emit nameChangedSignal(text, domElementForItem.value(item).attribute("number").toInt());
}
}

QTreeWidgetItem* MainTree::addItemNext(QTreeWidgetItem *item)
{
    if(item == 0)
        item = m_itemEntered;



    // adding to Dom
    qDebug() << "Item worked with:" << item->text(0);
    QDomElement element = domElementForItem.value(item);




    QDomElement newElement = domDocument.createElement("nothing");
    element.parentNode().insertAfter(newElement, element);





    //    QDomNode parentElement = element.parentNode();
    //    QDomNode newNode = element.cloneNode(false);
    //    parentElement.insertAfter(newNode, element);

    //    QDomElement newElement = element.nextSiblingElement();



    modifyAttributes(element, newElement, "sibling");






    //    qDebug() << "jal d ";



    //    //adding to QHash

    //    domElementForItem.insert(treeNewItem, newElement);

    this->write(deviceFile);


    buildTree();


    return domElementForItem.key(newElement);


}

//------------------------------------------------------------------------------------

QTreeWidgetItem* MainTree::addChild(QTreeWidgetItem *item)
{
    if(item == 0)
        item = m_itemEntered;
    qDebug() << "jal a ";

    // adding to Dom

    qDebug() << "jal b ";

    QDomElement element = domElementForItem.value(item);
    if(element.tagName() == "scene")
        return 0;



    qDebug() << "jal c ";




    QDomElement newElement = domDocument.createElement("nothing");
    element.appendChild(newElement);




    newElement = modifyAttributes(element, newElement, "child");



    this->write(deviceFile);


    buildTree();











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

    qDebug() << "jal f ";

    return domElementForItem.key(newElement);
}

//---------------------------------------------------------------------------------------

QTreeWidgetItem * MainTree::addSeparator(QTreeWidgetItem * item)
{
    if(item == 0)
        item = m_itemEntered;



    // adding to Dom
    qDebug() << "Item worked with:" << item->text(0);
    QDomElement element = domElementForItem.value(item);

    if(element.tagName() == "chapter" || element.tagName() == "book")
        return 0;



    QDomElement newElement = domDocument.createElement("nothing");
    element.parentNode().insertAfter(newElement, element);







    modifyAttributes(element, newElement, "separator");





    this->write(deviceFile);


    buildTree();

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

    this->write(deviceFile);

    buildTree();


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
            elementMoved = nextParentElement->insertAfter(newNode, nextChild);

        }


    }
    else{

        elementMoved = parentElement->insertAfter(newNode, nextSibling);

    }


    connect(this, SIGNAL(itemChanged(QTreeWidgetItem*,int)),
            this, SLOT(updateDomElement(QTreeWidgetItem*,int)));

    this->write(deviceFile);

    buildTree();



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



        while(element.hasChildNodes()){

            QDomNodeList nodeList = element.childNodes();

            int numFile;
            int numOriginalTextChar_point = 0;
            QString originalTextFile = element.firstChild().toElement().attribute("textPath");



            originalTextFile = originalTextFile.mid(7);

            for (int j = 0; j < originalTextFile.size(); ++j) {
                if (originalTextFile.at(j) == QChar('.'))
                    numOriginalTextChar_point = j;



            }
            qDebug()<< "erase childs parent";

            originalTextFile = originalTextFile.left(numOriginalTextChar_point);

            numFile = originalTextFile.toInt();


            freeNumList.append(numFile);





            while(element.firstChild().hasChildNodes()){




                int numFil;
                int numOriginalTxtChar_point = 0;
                QString originalTxtFile = element.firstChild().firstChild().toElement().attribute("textPath");



                originalTxtFile = originalTxtFile.mid(7);

                for (int p = 0; p < originalTxtFile.size(); ++p) {
                    if (originalTxtFile.at(p) == QChar('.'))
                        numOriginalTxtChar_point = p;



                }


                qDebug()<< "erase childs 3";

                originalTxtFile = originalTxtFile.left(numOriginalTxtChar_point);

                numFil = originalTxtFile.toInt();


                freeNumList.append(numFil);

                QString string = element.firstChild().firstChild().toElement().attribute("number");
                emit(textAndNoteSignal(string.toInt(),"close"));
                QFile txtFile(devicePath + element.firstChild().firstChild().toElement().attribute("textPath"));
                QFile notFile(devicePath + element.firstChild().firstChild().toElement().attribute("notePath"));
                QFile syFile(devicePath + element.firstChild().firstChild().toElement().attribute("synPath"));
                txtFile.remove();
                notFile.remove();
                syFile.remove();

                element.firstChild().removeChild(element.firstChild().firstChild());


            }


            emit(textAndNoteSignal(element.firstChild().toElement().attribute("number").toInt(),"close"));
            QFile textFil(devicePath + element.firstChild().toElement().attribute("textPath"));
            QFile noteFil(devicePath + element.firstChild().toElement().attribute("notePath"));
            QFile synFil(devicePath + element.firstChild().toElement().attribute("synPath"));
            textFil.remove();
            noteFil.remove();
            synFil.remove();

            element.removeChild(element.firstChild());





            //    QString debug;
            qDebug()<< "erase childs 1";

        }

        emit(textAndNoteSignal(element.attribute("number").toInt(),"close"));
        QFile textFile(devicePath + element.attribute("textPath"));
        QFile noteFile(devicePath + element.attribute("notePath"));
        QFile synFile(devicePath + element.attribute("synPath"));
        textFile.remove();
        noteFile.remove();
        synFile.remove();
        //    textFile.setFileName(devicePath + element.attribute("textPath") + ".temp");
        //    noteFile.setFileName(devicePath + element.attribute("notePath") + ".temp");
        //    synFile.setFileName(devicePath + element.attribute("synPath") + ".temp");
        //    textFile.remove();
        //    noteFile.remove();
        //    synFile.remove();







        //   domElementForItem.remove(m_itemEntered);
        element.parentNode().removeChild(element);


        QTreeWidgetItem *parentItem;
        QTreeWidgetItem *current;


        current = m_itemEntered;
        if(current->parent())
            parentItem = current->parent();
        else
            parentItem = headerItem();

        //  int numIndex = parentItem->indexOfChild(current);

        parentItem->removeChild(current);

        //       takeChild(numIndex);




        //free freeNumList


        int numFile;
        int numOriginalTextChar_point = 0;
        QString originalTextFile = element.attribute("textPath");


        originalTextFile = originalTextFile.mid(7);

        for (int i = 0; i < originalTextFile.size(); ++i) {
            if (originalTextFile.at(i) == QChar('.'))
                numOriginalTextChar_point = i;



        }

        originalTextFile = originalTextFile.left(numOriginalTextChar_point);

        numFile = originalTextFile.toInt();


        freeNumList.append(numFile);

        this->write(deviceFile);


        buildTree();
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

        while(!first.nextSiblingElement("chapter").isNull()){

            num = preNum + 1;
            newName = chapterName + " " + numString.setNum(num,10);
            first.nextSiblingElement("chapter").setAttribute("name", newName);

            emit nameChangedSignal(newName, first.attribute("number").toInt());
            qDebug() << "renamingSlot : " << newName << " " << numString.setNum(first.nextSiblingElement("scene").attribute("number").toInt(),10);

            preNum = num;
            first = first.nextSiblingElement("chapter");

        }

    }

    if(element.tagName() == "chapter"){

        QDomElement first = element.firstChildElement("scene");

        first.setAttribute("name", sceneName + " " + numString.setNum(num,10));

        while(!first.nextSiblingElement("scene").isNull()){

            num = preNum + 1;
            newName = sceneName + " " + numString.setNum(num,10);
            first.nextSiblingElement("scene").setAttribute("name", newName);

            emit nameChangedSignal(newName, first.attribute("number").toInt());
            qDebug() << "renamingSlot : " << newName << " " << numString.setNum(first.nextSiblingElement("scene").attribute("number").toInt(),10);

            preNum = num;
            first = first.nextSiblingElement("scene");


        }
    }


    this->write(deviceFile);

    buildTree();


}

//-----------------------------------------------------------------------------------


void MainTree::split()
{

    QMessageBox msgBox;
    msgBox.setText("Do you want to split this sheet ?");
    msgBox.setInformativeText(tr("<center><b>Each sheet type has different behaviour !</b></center>"
                                 "<br>"
                                 "<center><b>For the moment, only scene splitting works !</b></center>"
                                 "<br>"
                                 "<p>"
                                 "<blockquote>In a scene sheet : split <b>only</b> into scenes with *** .</blockquote>"
                                 "<blockquote>In a chapter sheet : split into scenes with *** and into chapters with ### .</blockquote>"
                                 "<blockquote>In a book sheet : split into scenes with *** and into chapters with ### .</blockquote></p>"

                                 ));
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Ok);
    int ret = msgBox.exec();

    switch (ret) {
    case QMessageBox::Ok:

    {

        // saving

        disconnect(this, SIGNAL(itemChanged(QTreeWidgetItem*,int)),
                   this, SLOT(updateDomElement(QTreeWidgetItem*,int)));





        emit textAndNoteSignal(0,"save");



        //splitting

        itemOfWork = m_itemEntered;

        QFile *txtFile = new QFile;
        QString txtPath;
        QTreeWidgetItem *childItem;
        QTreeWidgetItem *item;
        QTreeWidgetItem *lastItem = new QTreeWidgetItem;
        QTreeWidgetItem *nextItem = new QTreeWidgetItem;
        QStringList chaptersList, scenesList, allScenesList;
        QList<bool> resultsList;
        QList<QTreeWidgetItem *> childsList;
        QTextDocument *textDocument = new QTextDocument;
        QTextDocumentWriter *docWriter = new QTextDocumentWriter;
        docWriter->setFormat("HTML");



        QDomElement domItem = domElementForItem.value(itemOfWork);



        textFile->open(QFile::ReadOnly | QFile::Text);
        QApplication::setOverrideCursor(Qt::WaitCursor);
        QTextStream textFileStream( textFile );
        textDocument->setHtml(textFileStream.readAll());
        QString mainString = textDocument->toHtml("utf-8");
        textFile->close();

        //  QMessageBox msgBox;
        //  msgBox.setText(mainString);
        //  msgBox.exec();

        if(domItem.tagName() == "scene"){

            scenesList = mainString.split("***", QString::SkipEmptyParts);
            if(scenesList.size() == 1){
                QMessageBox::information(this, tr("Splitting Task"), tr("This sheet does not contain *** ."));
                return;
            }
            QString debug;
            qDebug() << "scenesList.size() : " << debug.setNum(scenesList.size(), 10);
            for(int i = 0; i < scenesList.size(); ++i){

                item = addItemNext(itemOfWork);
                txtPath = domElementForItem.value(item).toElement().attribute("textPath");
                qDebug() << "item text : " << item->text(0);
                txtFile->setFileName(devicePath + txtPath);
                qDebug() <<"(devicePath + txtPath) : "<< devicePath + txtPath;
                textDocument->setHtml(scenesList.at(i));
                docWriter->setDevice(txtFile);
                bool written = docWriter->write(textDocument);
                resultsList.append(written);


                itemOfWork = item;

            }


        }




        /*
        else if(domItem.tagName() == "chapter"){
            qDebug() << "jal AA ";



            //            chaptersList = mainString.split("###", QString::SkipEmptyParts);

            //            int chaptersSize = chaptersList.size();

            //            while(!chaptersList.isEmpty()){

            //                scenesList = chaptersList.takeFirst().split("***", QString::SkipEmptyParts);

            //                while (!scenesList.isEmpty())
            //                    allScenesList.append(scenesList.takeFirst());

            //                allScenesList.append("@@@@@@---break---@@@@@@");
            //            }
            //            qDebug() << "jal BB ";

            //            allScenesList.removeLast();

            //            for(int k = 0; k < chaptersSize; ++k){
            //                while(chaptersSize != 0){
            //                    qDebug() << "jal BBB ";

            //                    item = addItemNext(itemOfWork);
            //                    childsList.append(item);
            //                    itemOfWork = item;
            //                    chaptersSize -= 1;
            //                }
            //                qDebug() << "jal CC ";
            //          //      m_itemEntered = childsList.first();
            //                qDebug() << "jal CC 1 ";

            //                addChild();
            //                qDebug() << "jal DD ";

            //                lastItem = childsList.first()->child(0);
            //                while(!allScenesList.isEmpty()){

            //                    item = lastItem;
            //m_itemEntered = item;
            //                    nextItem = addItemNext();
            //                    qDebug() << "jal DDD ";

            //                    txtPath = domElementForItem.value(childItem).toElement().attribute("textPath");

            //                    txtFile->setFileName(devicePath + txtPath);
            //                    textDocument->setHtml(allScenesList.takeFirst());
            //                    qDebug() << "jal DDD ";

            //                    if(textDocument->firstBlock().text() == "@@@@@@---break---@@@@@@"){

            //                        m_itemEntered = lastItem;
            //                        moveDown();

            //                        qDebug() << "jal EEEE ";

            //                        allScenesList.removeFirst();
            //                    }
            //                    else{
            //                        docWriter->setDevice(txtFile);
            //                        bool written = docWriter->write(textDocument);
            //                        resultsList.append(written);
            //                    }
            //                    lastItem = nextItem;
            //                }




            chaptersList = mainString.split("###", QString::SkipEmptyParts);

            QString debug;
            qDebug() << "chaptersList.size() : " << debug.setNum(chaptersList.size());
            qDebug() << "mainString.size() : " << debug.setNum(mainString.size());






            QTreeWidgetItem *parentItem;
            QTreeWidgetItem *current;


            qDebug() << "itemOfWork name : " << itemOfWork->text(0);
            current = itemOfWork;
            if(current->parent())
                parentItem = current->parent();
            else
                parentItem = headerItem();

            QDomElement element;
            for(int j = 0; j < chaptersList.size(); ++j ){


                element = domElementForItem.value(current);


                QDomElement newElement = domDocument.createElement("nothing");
                element.parentNode().insertAfter(newElement, element);

                modifyAttributes(element, newElement, "sibling");






            }
            buildTree();


            for(int i = 0; i < chaptersList.size(); ++i){

                qDebug() << "A";

                qDebug() << "B";


                scenesList = chaptersList.at(i).split("***", QString::SkipEmptyParts);
                qDebug() << "scenesList.size() : " << debug.setNum(scenesList.size());

                //                if(scenesList.size() == 1 || chaptersList.size() == 1){
                //                    QMessageBox::information(window(), tr("Splitting Task"), tr("This sheet does not contain *** nor ### ."));
                //                    return;
                //                }



                item = current;


                //   for(int j = 0; j < scenesList.size(); ++j){

                //
                qDebug() << "T";

                while(!scenesList.isEmpty()){
                    qDebug() << "item name : " <<  item->text(0);
                    QString debug;
                    qDebug() << "int i : " << debug.setNum(i,10);

                    QDomElement element = domElementForItem.value(item);
                    qDebug() << "jal c ";

                    QDomElement newElement = domDocument.createElement("nothing");
                    element.appendChild(newElement);
                    newElement = modifyAttributes(element, newElement, "child");
                    qDebug() << "jal d ";

                    //                    childItem = domElementForItem.key(newElement);

                    //                    qDebug() << "childItem : " << childItem->text(0);

                    txtPath = newElement.toElement().attribute("textPath");
                    qDebug() <<   "U";
                    txtFile->setFileName(devicePath + txtPath);
                    textDocument->setHtml(scenesList.takeFirst());
                    docWriter->setDevice(txtFile);
                    bool written = docWriter->write(textDocument);

                    qDebug() <<   "V";
                    resultsList.append(written);


                    current = domElementForItem.key(element.nextSibling().toElement());

                }
                qDebug() <<   "W";
                this->write(deviceFile);

                qDebug() <<  "X";
            }







        }
        else if(domItem.tagName() == "book"){

            //            QTreeWidgetItem *itemToWorkWith = new QTreeWidgetItem;
            //            itemToWorkWith = itemOfWork;
            //            QTreeWidgetItem *childItem = new QTreeWidgetItem;


            chaptersList = mainString.split("###", QString::SkipEmptyParts);


            for(int l = 0; l < chaptersList.size(); ++l){

                scenesList = chaptersList.at(l).split("***", QString::SkipEmptyParts);

                childItem = addChild(itemOfWork);

                for(int m = 0; m < scenesList.size(); ++m){



                    childItem = addChild(childItem);
                    txtPath = domElementForItem.value(childItem).toElement().attribute("textPath");

                    txtFile->setFileName(devicePath + txtPath);
                    textDocument->setHtml(scenesList.at(m));
                    docWriter->setDevice(txtFile);
                    bool written = docWriter->write(textDocument);
                    resultsList.append(written);


                }

            }
        }
*/
        else{
            qDebug() << "------------------ PROBLEM SPLIT";
            QApplication::restoreOverrideCursor();

            return;
        }

        QApplication::restoreOverrideCursor();

        int resultsErrorsNum = 0;

        for(int l = 0; l < resultsList.size(); ++l)
            if(resultsList.at(l) == false)
                resultsErrorsNum += 1;


        connect(this, SIGNAL(itemChanged(QTreeWidgetItem*,int)),
                this, SLOT(updateDomElement(QTreeWidgetItem*,int)));

        if(resultsList.size() == 0)
            QMessageBox::critical(this, tr("Splitting Task"), tr("Splitting critical error !"));
        else
            QMessageBox::information(this, tr("Splitting Task"), tr("Splitting done with %1 errors !").arg(resultsErrorsNum));

        chaptersList.clear();
        scenesList.clear();


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

//-------------------------------------------------------------------------------------


void MainTree::addMulti()
{
    int numSheets = QInputDialog::getInt(this, tr("Add X Children"),
                                         tr("Please enter a number :"),
                                         1, 1, 100, 1);

    for(int i = 1; i <= numSheets; ++i){
        QTreeWidgetItem *item = m_itemEntered;
        QDomElement element = domElementForItem.value(item);
        if(element.tagName() == "scene")
            return;
        QDomElement newElement = domDocument.createElement("nothing");
        element.appendChild(newElement);



        newElement = modifyAttributes(element, newElement, "child");



    }

    this->write(deviceFile);


    buildTree();


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

            if(letter == "T"){
                numList.append(numFile);
                //                QString debug;
                //                qDebug() << "numFile : " << debug.setNum(numFile);
            }
        }

        qSort(numList.begin(), numList.end());





        int maxNumList = numList.last();

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
        freeNum == freeNumList.takeFirst();



    QString debug;
    qDebug() << "freeNumList is empty : " << freeNumList.isEmpty();
    qDebug() << "freeNum used : " << debug.setNum(freeNum);

    newElement.setAttribute("textPath", "/text/T" + textChar.setNum(freeNum, 10) + ".html");
    newElement.setAttribute("notePath", "/text/N" + textChar.setNum(freeNum, 10) + ".html");
    newElement.setAttribute("synPath", "/text/S" + textChar.setNum(freeNum, 10) + ".html");
    newElement.setAttribute("number", textChar.setNum(freeNum, 10));
    newElement.setAttribute("textCursorPos", textChar.setNum(0, 10));
    newElement.setAttribute("synCursorPos", textChar.setNum(0, 10));
    newElement.setAttribute("noteCursorPos", textChar.setNum(0, 10));


    QFile textFile(devicePath + newElement.attribute("textPath"));
    QFile noteFile(devicePath + newElement.attribute("notePath"));
    QFile synFile(devicePath + newElement.attribute("synPath"));

    textFile.remove();
    textFile.open(QFile::WriteOnly | QFile::Text);
    QTextStream out(&textFile);
    out << "";
    textFile.close();

    noteFile.remove();
    noteFile.open(QFile::WriteOnly | QFile::Text);
    QTextStream out_(&noteFile);
    out_ << "";
    noteFile.close();

    synFile.remove();
    synFile.open(QFile::WriteOnly | QFile::Text);
    QTextStream out__(&synFile);
    out__ << "";
    synFile.close();


    if(level == "sibling"){
        newElement.setTagName(originalElement.tagName());
        if(originalElement.tagName() == "book"){
            newElement.setAttribute("name", "New Book"); //for config

        }
        if(originalElement.tagName() == "chapter"){
            newElement.setAttribute("name", "New Chapter");//for config

        }
        if(originalElement.tagName() == "scene"){
            newElement.setAttribute("name", "New Scene");//for config

        }
    }
    if(level == "child"){
        if(originalElement.tagName() == "book"){
            newElement.setTagName("chapter");
            newElement.setAttribute("name", "New Chapter");//for config

        }
        if(originalElement.tagName() == "chapter"){
            newElement.setTagName("scene");
            newElement.setAttribute("name", "New Scene");//for config

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


    QDomElement child = root.firstChildElement("book");
    while (!child.isNull()) {
        parseFolderElement(child);
        child = child.nextSiblingElement("book");
    }

    connect(this, SIGNAL(itemChanged(QTreeWidgetItem*,int)),
            this, SLOT(updateDomElement(QTreeWidgetItem*,int)));
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


        this->write(deviceFile);
        buildTree();

    }
    else if ( mParent == getQTreeWidgetItemDepth(targetItem) && dropIndicatorPosition() == QAbstractItemView::AboveItem )
    {


        QDomElement draggedElement = domElementForItem.value(draggedItem);
        QDomElement targetElement = domElementForItem.value(targetItem);

        QDomNode draggedNode(draggedElement);
        QDomNode targetNode(targetElement);

        QDomNode *parentElement = new QDomNode(targetNode.parentNode());


        //check
        if(draggedElement.tagName() == targetElement.tagName())
            parentElement->insertBefore(draggedNode, targetNode);
        else
            return;


        qWarning() << "drop event  mParent - 1 above";
        QTreeView::dropEvent(event);


        this->write(deviceFile);
        buildTree();

    }
    else if ( mParent == getQTreeWidgetItemDepth(targetItem) && dropIndicatorPosition() == QAbstractItemView::BelowItem)
    {

        QDomElement draggedElement = domElementForItem.value(draggedItem);
        QDomElement targetElement = domElementForItem.value(targetItem);

        QDomNode draggedNode(draggedElement);
        QDomNode targetNode(targetElement);

        QDomNode *parentElement = new QDomNode(targetNode.parentNode());

        if(draggedElement.tagName() == targetElement.tagName())
            parentElement->insertAfter(draggedNode, targetNode);
        else
            return;





        qWarning() << "drop event  mParent - 1 below";
        QTreeView::dropEvent(event);

        this->write(deviceFile);
        buildTree();


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

    qDebug() << "pressed attribute : " << domElementForItem.value(item).attribute("textPath");
    qDebug() << "pressed attri num : " << domElementForItem.value(item).attribute("number");

}

//------------------------------------------------------------------------------------

void MainTree::dragEnterEvent( QDragEnterEvent *event )
{
    disconnect(this, SIGNAL(itemChanged(QTreeWidgetItem*,int)),
               this, SLOT(updateDomElement(QTreeWidgetItem*,int)));


    draggedItem= itemAt(event->pos());



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
    qDebug() << "collapsed";
    element.setAttribute("folded", "yes");
}

//-----------------------------------------------------------------------------------------

void MainTree::itemExpandedSlot(QTreeWidgetItem* item)
{
    QDomElement element = domElementForItem.value(item);
    qDebug() << "expanded";
    element.setAttribute("folded", "no");

}

void MainTree::saveCursorPos(int textCursorPosition, int synCursorPosition, int noteCursorPosition, int number)
{
    QDomElement element = domElementForNumber.value(number);
           element.setAttribute("textCursorPos", textCursorPosition);
           element.setAttribute("synCursorPos", synCursorPosition);
           element.setAttribute("noteCursorPos", noteCursorPosition);
           this->write(deviceFile);

}

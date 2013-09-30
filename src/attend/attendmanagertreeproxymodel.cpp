#include "attendmanagertreeproxymodel.h"

AttendManagerTreeProxyModel::AttendManagerTreeProxyModel(QObject *parent) :
    QSortFilterProxyModel(parent), maxNumber(1), m_indexTypeDragged("nothing")
{
}
//---------//--------------------------------------------------------------------------------------------------
//------------------Editing----------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------

void AttendManagerTreeProxyModel::postConstructor()
{
    this->domModified();

}

//------------------------------------------------------------------------------------------------------------------

bool AttendManagerTreeProxyModel::setData(const QModelIndex &index,
                                          const QVariant &value, int role)
{   QModelIndex sourceIndex = this->mapToSource(index);
    QVector<int> vector(1, role);

    if (sourceIndex.isValid() && role == Qt::EditRole) {

        QDomElement element = domElementForNumber.value(sourceIndex.data(Qt::UserRole).toInt());
        element.setAttribute("name", value.toString());

        emit resetAbsModelSignal();
        emit setNameSignal(value.toString());

        emit dataChanged(index, index);

        invalidateFilter();

        hub->addToSaveQueue();

        return true;
    }
    if (sourceIndex.isValid() && role == Qt::DecorationRole && sourceIndex.column() == 0) {


        int itemId = sourceIndex.data(Qt::UserRole).toInt();

        QDomElement element = hub->attendTree_domElementForNumberHash().value(itemId);
        if(value.toBool() == true)
            element.setAttribute("managerTreeExpanded", "yes");
        else
            element.setAttribute("managerTreeExpanded", "no");


        AttendTreeItem *item = static_cast<AttendTreeItem*>(sourceIndex.internalPointer());
        item->setIsExpanded(value.toBool(), AttendTreeItem::ManagerTree);

#if QT_VERSION < 0x050000
        emit dataChanged(index, index);
#endif
#if QT_VERSION >= 0x050000
        emit dataChanged(index, index, vector);
#endif

        hub->addToSaveQueue();

        return true;
    }

    return false;
}

//------------------------------------------------------------------------------------------------------------------


bool AttendManagerTreeProxyModel::insertRows(int position, int rows, const QModelIndex &parent)
{
    beginInsertRows(QModelIndex(), position, position+rows-1);




    //    for (int row = 0; row < rows; ++row) {
    //        stringList.insert(position, "");
    //    }

    this->domModified();









    endInsertRows();
    return true;
}

//------------------------------------------------------------------------------------------------------------------

bool AttendManagerTreeProxyModel::removeRows(int position, int rows, const QModelIndex &parent)
{
    beginRemoveRows(QModelIndex(), position, position+rows-1);


    this->domModified();
    endRemoveRows();
    return true;
}





//----------------------------------------------------------------------------------------

void AttendManagerTreeProxyModel::domModified()
{
    QList<int> numbers;
    domElementForNumber.clear();


    QDomDocument attendDomDoc = hub->attendTreeDomDoc();
    QDomElement root = attendDomDoc.documentElement();

    QDomNode m = root.firstChild();
    while(!m.isNull()) { // group level
        QDomElement e = m.toElement(); // try to convert the node to an element.
        if(!e.isNull()) {
            numbers.append(e.attribute("number", "0").toInt());
            domElementForNumber.insert(e.attribute("number", "0").toInt(), e);

            QDomNode n = m.firstChild();
            while(!n.isNull()) { // object level
                QDomElement f = n.toElement(); // try to convert the node to an element.
                if(!f.isNull()) {
                    numbers.append(f.attribute("number", "0").toInt());
                    domElementForNumber.insert(f.attribute("number", "0").toInt(), f);

                }
                n = n.nextSibling();
            }
        }
        m = m.nextSibling();
    }



    if(!numbers.isEmpty()){
        maxNumber = numbers.first();
        QListIterator<int> i(numbers);
        while (i.hasNext()){

            maxNumber = qMax(maxNumber, i.peekNext());
            i.next();
        }
    }
    else
        maxNumber = 1;




    invalidateFilter();
}






//------------------------------------------------------------------------------------------------------------------


void AttendManagerTreeProxyModel::addObject(QModelIndex index)
{
    QModelIndex sourceIndex = this->mapToSource(index);

    AttendTreeItem *sourceItem = static_cast<AttendTreeItem*>(sourceIndex.internalPointer());

    QDomElement element = domElementForNumber.value(sourceItem->idNumber());
    int number = freeNumber();
    QDomElement obj = hub->attendTreeDomDoc().createElement("obj");
    obj.setTagName("obj");
    obj.setAttribute("number", QString::number(number));
    obj.setAttribute("name", tr("new"));
    obj.setAttribute("attendPath", "/attend/A" + QString::number(number) + ".html" );

    // create doc :

    QFile *attendFile = new QFile(hub->projectWorkPath() + obj.attribute("attendPath"));
    QHash<QTextDocument *, QFile *> fileForDoc = hub->attendTree_fileForDocHash();
    QHash<QTextDocument *, int> numForDoc = hub->attendTree_numForDocHash();

    QTextDocument *attendDocument = new QTextDocument(hub);
    attendDocument->toHtml();
    QTextDocumentWriter attendDocWriter(attendFile, "HTML");
    attendDocWriter.write(attendDocument);
    attendDocument->setObjectName("attendDoc_" + QString::number(number));
    fileForDoc.insert(attendDocument, attendFile);
    numForDoc.insert(attendDocument, number);

    hub->set_attendTree_fileForDocHash(fileForDoc);
    hub->set_attendTree_numForDocHash(numForDoc);




    if(sourceItem->isGroup())
        element.appendChild(obj);
    else
        element.parentNode().appendChild(obj);


    emit resetAbsModelSignal();
    emit resetDomElementForNumberSignal();
//    emit dataChanged(index, index);

    invalidateFilter();

    hub->addFileToZipList("attend", number );


    hub->addToSaveQueue();

    // emit activateItemSignal(index);



}

//------------------------------------------------------------------------------------------------------------------

void AttendManagerTreeProxyModel::addGroup(QModelIndex index)
{
    QModelIndex sourceIndex = this->mapToSource(index);

    AttendTreeItem *sourceItem = static_cast<AttendTreeItem*>(sourceIndex.internalPointer());

    QDomElement element = domElementForNumber.value(sourceItem->idNumber());
    int number = freeNumber();
    QDomElement group = hub->attendTreeDomDoc().createElement("group");
    group.setTagName("group");
    group.setAttribute("number", QString::number(number));
    group.setAttribute("name", tr("new group"));
    group.setAttribute("attendPath", "/attend/A" + QString::number(number) + ".html" );

    // create doc :

    QFile *attendFile = new QFile(hub->projectWorkPath() + group.attribute("attendPath"));
    QHash<QTextDocument *, QFile *> fileForDoc = hub->attendTree_fileForDocHash();
    QHash<QTextDocument *, int> numForDoc = hub->attendTree_numForDocHash();

    QTextDocument *attendDocument = new QTextDocument(hub);
    attendDocument->toHtml();
    QTextDocumentWriter attendDocWriter(attendFile, "HTML");
    attendDocWriter.write(attendDocument);
    attendDocument->setObjectName("attendDoc_" + QString::number(number));
    fileForDoc.insert(attendDocument, attendFile);
    numForDoc.insert(attendDocument, number);

    hub->set_attendTree_fileForDocHash(fileForDoc);
    hub->set_attendTree_numForDocHash(numForDoc);




    if(sourceItem->isGroup())
        element.parentNode().appendChild(group);
    else
        element.parentNode().parentNode().appendChild(group);


    emit resetAbsModelSignal();
    emit resetDomElementForNumberSignal();
//    emit dataChanged(index, index);

    invalidateFilter();

    hub->addFileToZipList("attend", number );

    hub->addToSaveQueue();



}

//------------------------------------------------------------------------------------------------------------------

void AttendManagerTreeProxyModel::remove(QModelIndex index)
{
    QModelIndex sourceIndex = this->mapToSource(index);

    AttendTreeItem *sourceItem = static_cast<AttendTreeItem*>(sourceIndex.internalPointer());

    QDomElement element = domElementForNumber.value(sourceItem->idNumber());
int number = sourceItem->idNumber();

    if(sourceItem->isGroup()){

        if(sourceItem->childCount() != 0){
            QMessageBox::warning(0, tr("Plume Creator Attendance"),
                                 tr("You can't delete a group if it's not empty./n"
                                    "If you want to delete it, please delete its children first."),
                                 QMessageBox::Ok, QMessageBox::Ok);
            return;
        }
    }
    int ret = QMessageBox::warning(0, tr("Plume Creator Attendance"),
                                   tr("Do you really  want to delete it ?/n"
                                      "You will not be able to recover it afterwards."),
                                   QMessageBox::Ok | QMessageBox::Cancel,QMessageBox::Cancel);

    if(ret == QMessageBox::Cancel)
        return;

    QHash<QTextDocument *, QFile *> fileForDoc = hub->attendTree_fileForDocHash();
    QHash<QTextDocument *, int> numForDoc = hub->attendTree_numForDocHash();
    QTextDocument *doc = numForDoc.key(sourceItem->idNumber());

    QFile *attendFile = fileForDoc.value(doc);
    doc->setObjectName("");
    fileForDoc.remove(doc);
    numForDoc.remove(doc);

    hub->set_attendTree_fileForDocHash(fileForDoc);
    hub->set_attendTree_numForDocHash(numForDoc);

    attendFile->remove();

    element.parentNode().removeChild(element);



    this->removeAttendNumberFromSheets(number);



    emit resetAbsModelSignal();
    emit resetDomElementForNumberSignal();
//    emit dataChanged(index, index);

    invalidateFilter();

    hub->removeFileFromZipList("attend", number );

    hub->addToSaveQueue();

}

//------------------------------------------------------------------------------------------------------------------

int AttendManagerTreeProxyModel::freeNumber()
{
    maxNumber += 1;
    return maxNumber;
}
void AttendManagerTreeProxyModel::removeAttendNumberFromSheets(int itemNumber)
{

    QHash<int, QDomElement> mainTree_domElementForNumber;
    mainTree_domElementForNumber.clear();


    QDomDocument treeDomDoc = hub->mainTreeDomDoc();
    QDomElement root = treeDomDoc.documentElement();

    QDomNode m = root.firstChild();
    while(!m.isNull()) { // book level
        QDomElement e = m.toElement(); // try to convert the node to an element.
        if(!e.isNull()) {
            mainTree_domElementForNumber.insert(e.attribute("number", "0").toInt(), e);

            QDomNode n = m.firstChild();
            while(!n.isNull()) { // act level
                QDomElement f = n.toElement(); // try to convert the node to an element.
                if(!f.isNull()) {
                    mainTree_domElementForNumber.insert(f.attribute("number", "0").toInt(), f);

                    QDomNode o = n.firstChild();
                    while(!o.isNull()) { // chapter level
                        QDomElement g = o.toElement(); // try to convert the node to an element.
                        if(!g.isNull()) {
                            mainTree_domElementForNumber.insert(g.attribute("number", "0").toInt(), g);

                            QDomNode p = o.firstChild();
                            while(!p.isNull()) { // scene level
                                QDomElement r = p.toElement(); // try to convert the node to an element.
                                if(!r.isNull()) {
                                    mainTree_domElementForNumber.insert(r.attribute("number", "0").toInt(), r);

                                }
                                p = p.nextSibling();

                            }

                        }
                        o = o.nextSibling();

                    }

                }
                n = n.nextSibling();
            }
        }
        m = m.nextSibling();
    }




    QHash<int , QDomElement>::const_iterator  i = mainTree_domElementForNumber.constBegin();
    while (i != mainTree_domElementForNumber.constEnd()) {
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


        element.setAttribute("attend", newAttendString);
        ++i;
    }



}




void AttendManagerTreeProxyModel::setNameSlot(QModelIndex index, QString newName)
{
    // only display from the nameEdit in the manager, the saving is already done


    emit dataChanged(index, index);
}











//-----------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------
//---------------------------Drag & Drop-----------------------------------
//-----------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------










Qt::DropActions AttendManagerTreeProxyModel::supportedDropActions() const
{
    return /*Qt::CopyAction | */Qt::MoveAction;
}
//-------------------------------------------------------------------------------

QMimeData *AttendManagerTreeProxyModel::mimeData(const QModelIndexList &indexes) const
{
    QByteArray encodedData;
    QMimeData *mimeData = new QMimeData();


    foreach(const QModelIndex &index, indexes){
        encodedData.append("-" + index.data(Qt::UserRole).toString());


    }


//    qDebug() << "from maintree : encodedData.append : " << encodedData;
    mimeData->setData("application/x-plumecreator-attendmanagerdata", encodedData);
    return mimeData;
}
//-------------------------------------------------------------------------------

QStringList AttendManagerTreeProxyModel::mimeTypes () const
{
    QStringList list;
    list << "application/x-plumecreator-attendmanagerdata";
    return list;
}

//-------------------------------------------------------------------------------

bool AttendManagerTreeProxyModel::dropMimeData ( const QMimeData * data, Qt::DropAction action, int row, int column, const QModelIndex & parent )
{

    if (data->hasFormat("application/x-plumecreator-attendmanagerdata") && action == Qt::MoveAction)
    {



        QString numbersString = QString::fromUtf8(data->data("application/x-plumecreator-attendmanagerdata"));
        QStringList list = numbersString.split("-", QString::SkipEmptyParts);
        QList<int> objectsList;
        foreach(const QString &string, list)
            objectsList.append(string.toInt());


        int draggedInt = objectsList.first();

        QHash<int, QDomElement> domElementForNumber = hub->attendTree_domElementForNumberHash();

        QDomElement draggedElement = domElementForNumber.value(draggedInt);

        QDomElement parentTargetElement = domElementForNumber.value(parent.data(Qt::UserRole).toInt());

        if(draggedElement.tagName() == "group")
            parentTargetElement = hub->attendTreeDomDoc().documentElement();
        else if(!parent.isValid())
            return false;

        if(row == -1 && column == -1 ){

            parentTargetElement.appendChild(draggedElement);
        }
        else{

            if(parentTargetElement.hasChildNodes()){

                if(parentTargetElement.childNodes().size() > row){

                    parentTargetElement.insertBefore(draggedElement, parentTargetElement.childNodes().at(row));

                }
                else{
                    parentTargetElement.appendChild(draggedElement);
                }

            }

        }



        hub->addToSaveQueue();
        emit resetAbsModelSignal();




        return true;

    }


    return false;


}

//-------------------------------------------------------------------------------


Qt::ItemFlags AttendManagerTreeProxyModel::flags(const QModelIndex &index) const
{

    Qt::ItemFlags defaultFlags = QAbstractItemModel::flags(index);

    QStringList myOptions;
    myOptions << "group" << "object";

    if (!index.isValid()){
        return defaultFlags| Qt::ItemIsDropEnabled;
    }
    QString type = index.data(36).toString();

    if (index.column() == 0){

        switch (myOptions.indexOf(m_indexTypeDragged)) {

        case 0 :
            return defaultFlags ;
            break;

        case 1 :
            if(type == "group")
                return defaultFlags  | Qt::ItemIsDropEnabled ;
            else
                return defaultFlags ;
            break;


        default:

                return defaultFlags| Qt::ItemIsEditable | Qt::ItemIsDragEnabled;

            break;

        }

    }

    return defaultFlags;
}

void AttendManagerTreeProxyModel::modifyFlagsForDrops(QString indexType)
{
    m_indexTypeDragged = indexType;
}




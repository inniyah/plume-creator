#include "attendmanagertreeproxymodel.h"

AttendManagerTreeProxyModel::AttendManagerTreeProxyModel(QObject *parent) :
    QSortFilterProxyModel(parent), maxNumber(1)
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

Qt::ItemFlags AttendManagerTreeProxyModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::ItemIsEnabled;

    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}
//------------------------------------------------------------------------------------------------------------------

bool AttendManagerTreeProxyModel::setData(const QModelIndex &index,
                                          const QVariant &value, int role)
{
    if (index.isValid() && role == Qt::EditRole) {

        QDomElement element = domElementForNumber.value(index.data(Qt::UserRole).toInt());
        element.setAttribute("name", value.toString());

        emit resetDomDocSignal();
        emit setNameSignal(value.toString());

        emit dataChanged(index, index);

        invalidateFilter();

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

    //    for (int row = 0; row < rows; ++row) {
    //        stringList.removeAt(position);
    //    }

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


    emit resetDomDocSignal();
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


    emit resetDomDocSignal();
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



    emit resetDomDocSignal();
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
            while(!n.isNull()) { // chapter level
                QDomElement f = n.toElement(); // try to convert the node to an element.
                if(!f.isNull()) {
                    mainTree_domElementForNumber.insert(f.attribute("number", "0").toInt(), f);

                    QDomNode o = n.firstChild();
                    while(!o.isNull()) { // scene level
                        QDomElement g = o.toElement(); // try to convert the node to an element.
                        if(!g.isNull()) {
                            mainTree_domElementForNumber.insert(g.attribute("number", "0").toInt(), g);

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

        for( int i = 0 ; i < thisAttendStringList.size(); ++i)
            newAttendString.append(thisAttendStringList.at(i) + "-");
        //      qDebug() << "newAttendString : " << newAttendString;

        element.setAttribute("attend", newAttendString);
        ++i;
    }



}




void AttendManagerTreeProxyModel::setNameSlot(QModelIndex index, QString newName)
{
    // only display from the nameEdit in the manager, the saving is already done


    emit dataChanged(index, index);
}

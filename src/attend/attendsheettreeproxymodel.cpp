#include "attendsheettreeproxymodel.h"

AttendSheetTreeProxyModel::AttendSheetTreeProxyModel(QObject *parent) :
    QSortFilterProxyModel(parent)
{

}

bool AttendSheetTreeProxyModel::filterAcceptsRow(int sourceRow,
                                                 const QModelIndex &sourceParent) const
{
    QModelIndex index0 = sourceModel()->index(sourceRow, 0, sourceParent);

    AttendTreeItem *treeItem = static_cast<AttendTreeItem*>(index0.internalPointer());
    if(treeItem->isGroup()){         // groups

        QList<AttendTreeItem*> childrenList= treeItem->childrenItems();

        for(int i = 0; i < childrenList.size(); ++i)
            if(attendList.contains(childrenList.at(i)->idNumber()))
                return true;

        return false;

    }
    else if(attendList.contains(sourceModel()->data(index0,  Qt::UserRole).toInt()))
        return true;
    else
        return false;
}

//----------------------------------------------------------------------------------------

void AttendSheetTreeProxyModel::currentSheetModified(int sheetNumber)
{
    if(sheetNumber == -1)
        return;


    domElementForNumber.clear();
    attendList.clear();


    QDomDocument treeDomDoc = hub->mainTreeDomDoc();
    QDomElement root = treeDomDoc.documentElement();

    QDomNode m = root.firstChild();
    while(!m.isNull()) { // book level
        QDomElement e = m.toElement(); // try to convert the node to an element.
        if(!e.isNull()) {
            domElementForNumber.insert(e.attribute("number", "0").toInt(), e);

            QDomNode n = m.firstChild();
            while(!n.isNull()) { // chapter level
                QDomElement f = n.toElement(); // try to convert the node to an element.
                if(!f.isNull()) {
                    domElementForNumber.insert(f.attribute("number", "0").toInt(), f);

                    QDomNode o = n.firstChild();
                    while(!o.isNull()) { // scene level
                        QDomElement g = o.toElement(); // try to convert the node to an element.
                        if(!g.isNull()) {
                            domElementForNumber.insert(g.attribute("number", "0").toInt(), g);

                        }
                        o = o.nextSibling();

                    }

                }
                n = n.nextSibling();
            }
        }
        m = m.nextSibling();
    }



    openedElement = domElementForNumber.value(sheetNumber);


    QString attendString = openedElement.attribute("attend", "0");

    QStringList attendStringList = attendString.split("-", QString::SkipEmptyParts);

    foreach(const QString &str, attendStringList)
        attendList.append(str.toInt());

    invalidateFilter();
}

//----------------------------------------------------------------------
void AttendSheetTreeProxyModel::removeSheetObjects(QList<int> objectsList)
{
    if(objectsList.isEmpty())
        return;

    foreach(const int &object, objectsList)
        if(attendList.contains(object) && object != 0)
            attendList.removeOne(object);

    // modify DomDocument :

    QString attendString;
    foreach(const int &number, attendList)
        attendString.append("-" + QString::number(number));

    openedElement.setAttribute("attend", attendString);

    qDebug() << "REMOVE :";
    qDebug() << "openedElement : " << openedElement.tagName() << "  " << openedElement.attribute("name", "null");
    qDebug() << "attendList.size : " << QString::number(attendList.size());
    qDebug() << "attendString : " << attendString;
    qDebug() << hub->mainTreeDomDoc().toString(5);

    hub->addToSaveQueue();


    invalidateFilter();

}


//----------------------------------------------------------------------
void AttendSheetTreeProxyModel::addSheetObjects(QList<int> objectsList)
{
    if(objectsList.isEmpty())
        return;

    foreach(const int &object, objectsList)
        if(!attendList.contains(object) && object != 0)
            attendList.append(object);



    // modify DomDocument :

    QString attendString;
    foreach(const int &number, attendList)
        attendString.append("-" + QString::number(number));

    openedElement.setAttribute("attend", attendString);

    qDebug() << "ADD :";
    qDebug() << "attendList.size : " << QString::number(attendList.size());
    qDebug() << "attendString : " << attendString;
    qDebug() << hub->mainTreeDomDoc().toString(5);

    hub->addToSaveQueue();

    invalidateFilter();

}





//-------------------------------------------------------------------------------

Qt::DropActions  AttendSheetTreeProxyModel::supportedDropActions () const
{
    return Qt::CopyAction;
}

//-------------------------------------------------------------------------------

QMimeData *AttendSheetTreeProxyModel::mimeData(const QModelIndexList &indexes) const
{
    QByteArray encodedData;
    QMimeData *mimeData = new QMimeData();

    foreach(const QModelIndex &index, indexes){
        QModelIndex index0 = mapToSource(index);
        AttendTreeItem *treeItem = static_cast<AttendTreeItem*>(index0.internalPointer());
        if(treeItem->isGroup()){
            if(!treeItem->childrenItems().isEmpty()){

                foreach(const AttendTreeItem *item, treeItem->childrenItems())
                    encodedData.append("-" + QString::number(item->idNumber()));

            }
        }
        else
            encodedData.append("-" + index.data(Qt::UserRole).toString());
    }
    mimeData->setData("application/x-plumecreatorattendnumberdata", encodedData);
    return mimeData;
}
//-------------------------------------------------------------------------------

QStringList AttendSheetTreeProxyModel::mimeTypes () const
{
    QStringList list;
    list << "application/x-plumecreatorattendnumberdata";
    return list;
}

//-------------------------------------------------------------------------------

bool AttendSheetTreeProxyModel::dropMimeData ( const QMimeData * data, Qt::DropAction action, int row, int column, const QModelIndex & parent )
{



    if (data->hasFormat("application/x-plumecreatorattendnumberdata"))
    {
        qDebug() << "to sheet : data->text :  "<< QString::fromUtf8(data->data("application/x-plumecreatorattendnumberdata"));


        QString numbersString = QString::fromUtf8(data->data("application/x-plumecreatorattendnumberdata"));
        QStringList list = numbersString.split("-", QString::SkipEmptyParts);
        QList<int> objectsList;
        foreach(const QString &string, list)
            objectsList.append(string.toInt());

        this->addSheetObjects(objectsList);



        return true;

    }


    return false;


}

//-------------------------------------------------------------------------------

Qt::ItemFlags AttendSheetTreeProxyModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags defaultFlags = QSortFilterProxyModel::flags(index) ;



    if (index.isValid())
        return Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | defaultFlags;
    else
        return Qt::ItemIsDropEnabled |defaultFlags;
}

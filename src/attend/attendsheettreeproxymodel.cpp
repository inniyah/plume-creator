#include "attendsheettreeproxymodel.h"

AttendSheetTreeProxyModel::AttendSheetTreeProxyModel(QObject *parent) :
    QSortFilterProxyModel(parent), nothingWasClicked(true)
{

}


QVariant AttendSheetTreeProxyModel::headerData(int section, Qt::Orientation orientation,
                                               int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();


    if (orientation == Qt::Horizontal && role == Qt::DisplayRole && section != -1){

        return sheetName;
    }

    else
        return QString(tr("Sheet"));
}



//--------------------------------------------------------------------------------------------------


bool AttendSheetTreeProxyModel::filterAcceptsRow(int sourceRow,
                                                 const QModelIndex &sourceParent) const
{
    QModelIndex index0 = sourceModel()->index(sourceRow, 0, sourceParent);

    //    if(!index0.isValid())
    //        return false;

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
    povList.clear();


    QDomDocument treeDomDoc = hub->mainTreeDomDoc();
    QDomElement root = treeDomDoc.documentElement();

    QDomNode m = root.firstChild();
    while(!m.isNull()) { // trash level
        QDomElement e = m.toElement(); // try to convert the node to an element.
        if(!e.isNull()) {
            domElementForNumber.insert(e.attribute("number", "0").toInt(), e);

            QDomNode n = m.firstChild();
            while(!n.isNull()) { // book level
                QDomElement f = n.toElement(); // try to convert the node to an element.
                if(!f.isNull()) {
                    domElementForNumber.insert(f.attribute("number", "0").toInt(), f);

                    QDomNode o = n.firstChild();
                    while(!o.isNull()) { // act level
                        QDomElement g = o.toElement(); // try to convert the node to an element.
                        if(!g.isNull()) {
                            domElementForNumber.insert(g.attribute("number", "0").toInt(), g);


                            QDomNode p = o.firstChild();
                            while(!p.isNull()) { // chapter level
                                QDomElement q = p.toElement(); // try to convert the node to an element.
                                if(!q.isNull()) {
                                    domElementForNumber.insert(q.attribute("number", "0").toInt(), q);

                                    QDomNode r = p.firstChild();
                                    while(!r.isNull()) { // scene level
                                        QDomElement s = r.toElement(); // try to convert the node to an element.
                                        if(!s.isNull()) {
                                            domElementForNumber.insert(s.attribute("number", "0").toInt(), s);



                                        }
                                        r = r.nextSibling();

                                    }


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



    openedElement = domElementForNumber.value(sheetNumber);


    QString attendString = openedElement.attribute("attend", "0");
    QStringList attendStringList = attendString.split("-", QString::SkipEmptyParts);
    foreach(const QString &str, attendStringList)
        attendList.append(str.toInt());

    QString povString = openedElement.attribute("pov", "0");
    QStringList povStringList = povString.split("-", QString::SkipEmptyParts);
    foreach(const QString &str, povStringList)
        povList.append(str.toInt());


    //set Header title :
    sheetName = openedElement.attribute("name", tr("Sheet"));
    this->headerDataChanged(Qt::Horizontal,1,1);

    invalidateFilter();
    nothingWasClicked = true;
}

//----------------------------------------------------------------------
void AttendSheetTreeProxyModel::removeSheetObjects(QList<int> objectsList)
{
    if(objectsList.isEmpty())
        return;

    foreach(const int &object, objectsList)
        if(attendList.contains(object) && object != 0){
            attendList.removeOne(object);
            if(povList.contains(object))
                povList.removeOne(object);
        }
    // modify DomDocument :

    QString attendString;
    foreach(const int &number, attendList)
        attendString.append("-" + QString::number(number));

    openedElement.setAttribute("attend", attendString);



    QString povString;
    foreach(const int &number, povList)
        povString.append("-" + QString::number(number));

    openedElement.setAttribute("pov", povString);


    hub->resetSpreadsheetOutliner();
    hub->addToSaveQueue();
    this->dataChanged(clickedProxyIndex.parent(),clickedProxyIndex);

    invalidateFilter();
    nothingWasClicked = true;

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

    //    qDebug() << "ADD :";
    //    qDebug() << "attendList.size : " << QString::number(attendList.size());
    //    qDebug() << "attendString : " << attendString;
    //    qDebug() << hub->mainTreeDomDoc().toString(5);

    hub->addToSaveQueue();

    invalidateFilter();
    nothingWasClicked = true;

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
    mimeData->setData("application/x-plumecreator-attendnumberdata-fromsheettree", encodedData);
    return mimeData;
}
//-------------------------------------------------------------------------------

QStringList AttendSheetTreeProxyModel::mimeTypes () const
{
    QStringList list;
    list << "application/x-plumecreator-attendnumberdata-fromglobaltree";
    return list;
}

//-------------------------------------------------------------------------------

bool AttendSheetTreeProxyModel::dropMimeData ( const QMimeData * data, Qt::DropAction action, int row, int column, const QModelIndex & parent )
{



    if (data->hasFormat("application/x-plumecreator-attendnumberdata-fromglobaltree"))
    {
        //        qDebug() << "to sheet : data->text :  "<< QString::fromUtf8(data->data("application/x-plumecreator-attendnumberdata-fromglobaltree"));


        QString numbersString = QString::fromUtf8(data->data("application/x-plumecreator-attendnumberdata-fromglobaltree"));
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


//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------


QVariant AttendSheetTreeProxyModel::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    int col = index.column();

    if (!index.isValid())
        return QVariant();

    if ((role == Qt::DecorationRole) && col == 0){


        QModelIndex index0 = this->mapToSource(index);

        AttendTreeItem *treeItem = static_cast<AttendTreeItem*>(index0.internalPointer());
        if(treeItem->isGroup()){         // groups

            QList<AttendTreeItem*> childrenList= treeItem->childrenItems();

            for(int i = 0; i < childrenList.size(); ++i)
                if(povList.contains(childrenList.at(i)->idNumber()))
                    return QIcon(":/pics/eye_pencil.png");



        }
        else if(povList.contains(index0.data(Qt::UserRole).toInt()))
            return QIcon(":/pics/eye_pencil.png");
    }
    else
        return QSortFilterProxyModel::data(index,role);

    return QSortFilterProxyModel::data(index,role);
}

void AttendSheetTreeProxyModel::setPointOfView()
{
    if(nothingWasClicked)
        return;

    AttendTreeItem *treeItem = static_cast<AttendTreeItem*>(clickedSourceIndex.internalPointer());



    if(treeItem->isGroup())
        return;



    if(attendList.contains(treeItem->idNumber())
            && !povList.contains(treeItem->idNumber()))
        povList.append(treeItem->idNumber());

    else if(attendList.contains(treeItem->idNumber())
            && povList.contains(treeItem->idNumber()))
        povList.removeOne(treeItem->idNumber());



    // modify DomDocument :

    QString povString;
    foreach(const int &number, povList)
        povString.append("-" + QString::number(number));

    openedElement.setAttribute("pov", povString);

    hub->addToSaveQueue();


    hub->resetSpreadsheetOutliner();

    this->dataChanged(clickedProxyIndex.parent(),clickedProxyIndex);


    invalidateFilter();
}

void AttendSheetTreeProxyModel::setClickedIndex(QModelIndex proxyIndex)
{
    nothingWasClicked = false;

    clickedProxyIndex = proxyIndex;
    clickedSourceIndex = this->mapToSource(proxyIndex);
}


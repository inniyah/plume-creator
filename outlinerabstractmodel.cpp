#include <QDebug>
#include <QObject>
#include <QTextDocument>
#include <QTextDocumentFragment>
#include <QTextCursor>

#include "outlinerabstractmodel.h"
#include "outlinertreeitem.h"

OutlinerAbstractModel::OutlinerAbstractModel(QObject *parent) :
    QAbstractItemModel(parent),itemHeight(40),
    textZoneColumnOneWidth(300), textZoneColumnTwoWidth(300)
{

}

OutlinerAbstractModel::~OutlinerAbstractModel()
{
    delete rootItem;
}


int OutlinerAbstractModel::rowCount(const QModelIndex &parent) const
{

    OutlinerTreeItem *parentItem;
    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<OutlinerTreeItem*>(parent.internalPointer());


    return parentItem->childCount();

    //    return 10;
}


int OutlinerAbstractModel::columnCount(const QModelIndex &parent) const
{

    if (parent.isValid())
        return static_cast<OutlinerTreeItem*>(parent.internalPointer())->columnCount();
    else
        return rootItem->columnCount();

    //    return 3;

}




QVariant OutlinerAbstractModel::headerData(int section, Qt::Orientation orientation,
                                           int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();


    if (orientation == Qt::Horizontal && role == Qt::DisplayRole && section != -1){

        return rootItem->data(section);
    }
    //        if (orientation == Qt::Vertical)
    //        //        QString string;
    //        //        qDebug() << "header section : "  << string.setNum(section);
    //        //        qDebug() << "header at section : "  << leftHeadersList->at(section);
    //        //        qDebug() << "header size : "  << string.setNum(leftHeadersList->size());
    //            return leftHeadersList->at(section);
    //    }
    else
        return QString("hhhh");
}



//--------------------------------------------------------------------------------------------------

QVariant OutlinerAbstractModel::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    int col = index.column();



    //    if (!index.isValid())
    //        return QVariant();

    ////    if (row >= numberOfDomElements)
    ////        return QVariant();

    //    if (row >= 2)
    //        return QVariant();

    //    if (role == Qt::DisplayRole || role == Qt::EditRole){

    //        if(col == 0){
    //            if (!index.isValid())
    //                return QVariant();

    //            if (role == Qt::SizeHintRole)
    //                return QSize(100,100);

    //            if (role != Qt::DisplayRole)
    //                return QVariant();

    //            OutlinerTreeItem *item = static_cast<OutlinerTreeItem*>(index.internalPointer());
    //            qDebug() << "f";

    //            return item->data(0);
    //        }
    //        if(col == 1){

    //            QList<QTextDocument *> miniDocList = mtoO_numForClonedDoc.keys(numberList->at(row));
    //            for(int i = 0; i < miniDocList.size(); ++i){
    //                if(miniDocList.at(i)->objectName().left(11) == "synDocClone")
    //                    return miniDocList.at(i)->toHtml();

    //            }


    //        }
    //        if(col == 2){

    //            QList<QTextDocument *> miniDocList = mtoO_numForClonedDoc.keys(numberList->at(row));
    //            for(int i = 0; i < miniDocList.size(); ++i){
    //                if(miniDocList.at(i)->objectName().left(12) == "noteDocClone")
    //                    return miniDocList.at(i)->toHtml();
    //            }
    //        }
    //        else
    //            return QVariant();


    //    }


    //    if (role == Qt::UserRole)
    //        return numberList->at(row);

    //    else
    //        return QVariant();






    if (!index.isValid())
        return QVariant();

    if ((role == Qt::DisplayRole || role == Qt::EditRole) && col == 0){
        OutlinerTreeItem *item = static_cast<OutlinerTreeItem*>(index.internalPointer());
        return item->data(col).toString();
    }

    if (role == 33 && (col == 1 || col == 2)){
        OutlinerTreeItem *item = static_cast<OutlinerTreeItem*>(index.internalPointer());
        return item->data(col).toString();
    }

    //    if (role == Qt::SizeHintRole&& (col == 0))
    //        return (QSize(100,itemHeight));

    if (role == Qt::SizeHintRole){
        QTextDocument synDoc;
        synDoc.setTextWidth(textZoneColumnOneWidth);
        synDoc.setHtml(this->index(row,1, index.parent()).data(33).toString());
        synDoc.setDocumentMargin(1);
        QTextCursor cursor(&synDoc);
        cursor.setPosition(0);
        cursor.select(QTextCursor::BlockUnderCursor);
        synDoc.setHtml(cursor.selection().toHtml());
        QSize synSize(synDoc.size().toSize());
        synSize.setHeight(synSize.height() + 5);

        QTextDocument noteDoc;
        noteDoc.setTextWidth(textZoneColumnTwoWidth);
        noteDoc.setHtml(this->index(row,2, index.parent()).data(33).toString());
        noteDoc.setDocumentMargin(1);
        QTextCursor tcursor(&noteDoc);
        tcursor.setPosition(0);
        tcursor.select(QTextCursor::BlockUnderCursor);
        noteDoc.setHtml(tcursor.selection().toHtml());
        QSize noteSize(noteDoc.size().toSize());
        noteSize.setHeight(noteSize.height() + 5);

        if(qMax(synSize.height(), noteSize.height()) < itemHeight + 200
                && qMax(synSize.height(), noteSize.height()) > itemHeight){
            qDebug() << "qMax";
            if(synSize.height() >= noteSize.height())
                return synSize;
            else
                return noteSize;
        }
        else if((synSize.height() > itemHeight + 200 || noteSize.height() > itemHeight + 200)
                && (synSize.height() < itemHeight + 200 || noteSize.height() < itemHeight + 200)
                && (synSize.height() > itemHeight || noteSize.height() > itemHeight)){
            qDebug() << "minimum";
            if(synSize.height() >= noteSize.height())
                return noteSize;
            else
                return synSize;
        }
        else{

            return (QSize(100,itemHeight));
        }
    }

    if (role == Qt::UserRole){
        OutlinerTreeItem *item = static_cast<OutlinerTreeItem*>(index.internalPointer());
        return item->idNumber();
    }

    else
        return QVariant();
    //}

}

//--------------------------------------------------------------------------------------------------

QModelIndex OutlinerAbstractModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    //    if (index.column() == 0){
    OutlinerTreeItem *childItem = static_cast<OutlinerTreeItem*>(index.internalPointer());
    OutlinerTreeItem *parentItem = childItem->parent();

    if (parentItem == rootItem)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
    //    }
    //    return QModelIndex();

}



//--------------------------------------------------------------------------------------------------

QModelIndex OutlinerAbstractModel::index ( int row, int column, const QModelIndex & parent ) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    //    if (column == 0){

    OutlinerTreeItem *parentItem;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<OutlinerTreeItem*>(parent.internalPointer());

    OutlinerTreeItem *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
    //    }
    //    return QModelIndex();
}



//--------------------------------------------------------------------------------------------------
//------------------Editing----------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------

Qt::ItemFlags OutlinerAbstractModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    if (index.column() == 0 || index.column() == 1 || index.column() == 2)
        return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}

//--------------------------------------------------------------------------------------------------

bool OutlinerAbstractModel::setData(const QModelIndex &index,
                                    const QVariant &value, int role)
{
    if (index.isValid() && role == Qt::EditRole && index.column() == 0) {

        titlesList->replace(index.row(), value.toString());

        emit dataChanged(index, index);

        QDomElement element = domElementForNumber.value(index.data(Qt::UserRole).toInt());
        element.setAttribute("name", value.toString());

        OutlinerTreeItem *item = static_cast<OutlinerTreeItem*>(index.internalPointer());
        item->dataList()->replace(0,value.toString());

        updateMainDomDoc();

        return true;
    }
    if (index.isValid() && role == Qt::EditRole && index.column() == 1) {



        QList<QTextDocument *> miniDocList = mtoO_numForClonedDoc.keys(index.data(Qt::UserRole).toInt());
        QTextDocument *synDoc = new QTextDocument;
        for(int i = 0; i < miniDocList.size(); ++i){
            if(miniDocList.at(i)->objectName().left(11) == "synDocClone"){
                synDoc = miniDocList.at(i);
                synDoc->setHtml(value.toString());

                OutlinerTreeItem *item = static_cast<OutlinerTreeItem*>(index.internalPointer());
                //for separator which don't have data :
                if (item->dataList()->size() == 1)
                    return false;
                item->dataList()->replace(1,value.toString());
            }
        }

        emit dataChanged(index, index);


        updateMainTextDoc(synDoc, index.data(Qt::UserRole).toInt());


        return true;
    }
    if (index.isValid() && role == Qt::EditRole && index.column() == 2) {



        QList<QTextDocument *> miniDocList = mtoO_numForClonedDoc.keys(index.data(Qt::UserRole).toInt());
        QTextDocument *noteDoc = new QTextDocument;
        for(int i = 0; i < miniDocList.size(); ++i){

            if(miniDocList.at(i)->objectName().left(12) == "noteDocClone"){
                noteDoc = miniDocList.at(i);
                noteDoc->setHtml(value.toString());

                OutlinerTreeItem *item = static_cast<OutlinerTreeItem*>(index.internalPointer());
                //for separator which don't have data :
                if (item->dataList()->size() == 1)
                    return false;
                item->dataList()->replace(2,value.toString());

            }
        }

        emit dataChanged(index, index);

        updateMainTextDoc(noteDoc, index.data(Qt::UserRole).toInt());

        return true;
    }

    return false;
}

//--------------------------------------------------------------------------------------------------
void OutlinerAbstractModel::resetAbsModelColumnOne()
{
}

  //--------------------------------------------------------------------------------------------------
void OutlinerAbstractModel::resetAbsModelColumnTwo()
{

}

//--------------------------------------------------------------------------------------------------

void OutlinerAbstractModel::resetAbsModel()
{
    beginResetModel();

    QList<QVariant> rootData;
    rootData << tr("Titles") << tr("Synopsis") << tr("Notes");
    rootItem = new OutlinerTreeItem(rootData);

    numberOfDomElements = 0;
    mtoO_numForClonedDoc.clear();

    reset_mtoO_setNumForDoc();
    resetDomDoc();

    endResetModel();

}




void OutlinerAbstractModel::updateMainDomDoc()
{
    QDomDocument otoM_domDoc = mtoO_domDoc;
    emit updateMainDomDocSignal(otoM_domDoc);


}

void OutlinerAbstractModel::updateMainTextDoc(QTextDocument *textDoc, int number)
{

    QList<QTextDocument *> miniDocList = mtoO_numForDoc.keys(number);
    for(int i = 0; i < miniDocList.size(); ++i){


        if(miniDocList.at(i)->objectName().left(7) == "noteDoc" && textDoc->objectName().left(12) == "noteDocClone"){
            QTextDocument *doc = miniDocList.at(i);
            doc->setHtml(textDoc->toHtml()) ;
        }
        else if(miniDocList.at(i)->objectName().left(6) == "synDoc" && textDoc->objectName().left(11) == "synDocClone"){
            QTextDocument *doc = miniDocList.at(i);
            doc->setHtml(textDoc->toHtml()) ;

        }
    }

    emit applySynNoteFontConfigSignal();

}




















void OutlinerAbstractModel::mtoO_setDomDoc(QDomDocument domDoc)
{
    mtoO_domDoc = domDoc;
    root = mtoO_domDoc.documentElement();

}
void OutlinerAbstractModel::resetDomDoc()
{

    titlesList = new QStringList;
    leftHeadersList = new QStringList;
    numberList = new QList<int>;
    domElementForNumber.clear();


    numberOfDomElements = 0;
    numberOfBooks = 0;
    numberOfChapters = 0;
    numberOfScenes = 0;

    treeBookItemList = new QList<OutlinerTreeItem *>;
    treeChapterItemList = new QList<OutlinerTreeItem *>;

    parseFolderElement(root);



}


void OutlinerAbstractModel::parseFolderElement(const QDomElement &element)
{
    //    QTreeWidgetItem *item = createItem(element, parentItem);
    QString string1;
    QString string2;
    QString string3;
    QString title = element.attribute("name");
    if (title.isEmpty())
        title = QObject::tr("No Title");



    //    if (element.tagName() == "book") {
    //        numberOfDomElements += 1;
    //        numberOfBooks += 1;
    //        leftHeadersList->append(string1.setNum(numberOfBooks));
    //        numberOfChapters = 0;
    //  numberOfScenes = 0;
    //    }
    //    if(!leftHeadersList->isEmpty()){
    //        while(!leftHeadersList->isEmpty())
    //    qDebug() << "header : "  << leftHeadersList->takeFirst();
    //}
    //    item->setFlags( Qt::ItemIsSelectable /*| Qt::ItemIsEditable*/ | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsDragEnabled);
    //    item->setIcon(0, folderIcon);
    //    item->setText(0, title);

    //    bool folded = (element.attribute("folded") != "no");
    //    setItemExpanded(item, !folded);



    QDomElement child = element.firstChildElement();
    while (!child.isNull()) {

        titlesList->append(child.attribute("name"));
        numberList->append(child.attribute("number").toInt());
        domElementForNumber.insert(child.attribute("number").toInt(), child);

        if (child.tagName() == "book") {
            numberOfBooks += 1;
            leftHeadersList->append(string1.setNum(numberOfBooks));
            numberOfChapters = 0;
            numberOfScenes = 0;





            QList<QVariant> itemData;

            itemData.append(child.attribute("name", "error"));

            QList<QTextDocument *> miniDocList = mtoO_numForClonedDoc.keys(child.attribute("number").toInt());
            for(int i = 0; i < miniDocList.size(); ++i){
                if(miniDocList.at(i)->objectName().left(11) == "synDocClone")
                    itemData.append(miniDocList.at(i)->toHtml());



            }
            for(int i = 0; i < miniDocList.size(); ++i){
                if(miniDocList.at(i)->objectName().left(12) == "noteDocClone")
                    itemData.append(miniDocList.at(i)->toHtml());
            }

            OutlinerTreeItem *treeItem = new OutlinerTreeItem(itemData, rootItem);
            treeItem->setIdNumber(child.attribute("number").toInt());

            rootItem->appendChild(treeItem);

            treeBookItemList->append(treeItem);



            parseFolderElement(child);


        } else if (child.tagName() == "chapter") {
            numberOfChapters += 1;
            leftHeadersList->append(string1.setNum(numberOfBooks) + "." + string2.setNum(numberOfChapters));
            numberOfScenes = 0;



            QList<QVariant> itemData;

            itemData.append(child.attribute("name", "error"));

            QList<QTextDocument *> miniDocList = mtoO_numForClonedDoc.keys(child.attribute("number").toInt());
            for(int i = 0; i < miniDocList.size(); ++i){
                if(miniDocList.at(i)->objectName().left(11) == "synDocClone")
                    itemData.append(miniDocList.at(i)->toHtml());



            }
            for(int i = 0; i < miniDocList.size(); ++i){
                if(miniDocList.at(i)->objectName().left(12) == "noteDocClone")
                    itemData.append(miniDocList.at(i)->toHtml());
            }

            OutlinerTreeItem *treeItem = new OutlinerTreeItem(itemData, treeBookItemList->last());
            treeItem->setIdNumber(child.attribute("number").toInt());

            treeBookItemList->last()->appendChild(treeItem);

            treeChapterItemList->append(treeItem);




            parseFolderElement(child);


        } else if (child.tagName() == "scene") {
            //            QTreeWidgetItem *childItem = createItem(child, item);
            numberOfScenes += 1;
            leftHeadersList->append(string1.setNum(numberOfBooks) + "." +  string2.setNum(numberOfChapters) + "." + string3.setNum(numberOfScenes));

            //            QString title = child.attribute("name");
            //            if (title.isEmpty())
            //                title = QObject::tr("XML problem");




            QList<QVariant> itemData;

            itemData.append(child.attribute("name", "error"));

            QList<QTextDocument *> miniDocList = mtoO_numForClonedDoc.keys(child.attribute("number").toInt());
            for(int i = 0; i < miniDocList.size(); ++i){
                if(miniDocList.at(i)->objectName().left(11) == "synDocClone")
                    itemData.append(miniDocList.at(i)->toHtml());



            }
            for(int i = 0; i < miniDocList.size(); ++i){
                if(miniDocList.at(i)->objectName().left(12) == "noteDocClone")
                    itemData.append(miniDocList.at(i)->toHtml());
            }

            OutlinerTreeItem *treeItem = new OutlinerTreeItem(itemData, treeChapterItemList->last());
            treeItem->setIdNumber(child.attribute("number").toInt());

            treeChapterItemList->last()->appendChild(treeItem);






        }

        else if (child.tagName() == "separator") {
            //            QTreeWidgetItem *childItem = createItem(child);
            //            childItem->setText(0, "             " + QString(10, 0xB7));
            leftHeadersList->append("---");

            QList<QVariant> itemData;
            itemData.append(child.attribute("name", "error"));
            OutlinerTreeItem *treeItem = new OutlinerTreeItem(itemData, treeChapterItemList->last());
            treeChapterItemList->last()->appendChild(treeItem);
        }

        int number = element.attribute("number").toInt();
        domElementForNumber.insert(number, element);



        numberOfDomElements += 1;
        child = child.nextSiblingElement();
    }


}

//-----------------------------------------------------------------------------------------------------------

void OutlinerAbstractModel::mtoO_setNumForDoc(QHash<QTextDocument *, int> numForDoc)
{

    mtoO_numForDoc = numForDoc;
    //    qDebug() <<"mtoO_numForDoc size : "  << QString::number(mtoO_numForDoc.size());



}

//-----------------------------------------------------------------------------------------------------------


void OutlinerAbstractModel::reset_mtoO_setNumForDoc()
{
    mtoO_numForClonedDoc.clear();


    QHash<QTextDocument *, int>::const_iterator  i = mtoO_numForDoc.constBegin();
    while (i != mtoO_numForDoc.constEnd()) {
        QTextDocument *textDoc = i.key();
        QTextDocument *textDocClone = textDoc->clone();

        if(textDoc->objectName().left(6) == "synDoc")
            textDocClone->setObjectName("synDocClone_" + QString::number(i.value()));
        else if(textDoc->objectName().left(7) == "noteDoc")
            textDocClone->setObjectName("noteDocClone_" + QString::number(i.value()));

        mtoO_numForClonedDoc.insert(textDocClone, i.value());



        ++i;
    }

}

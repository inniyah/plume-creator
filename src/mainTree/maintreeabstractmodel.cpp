#include <QDebug>
#include <QObject>
#include <QTextDocument>
#include <QTextDocumentFragment>
#include <QTextCursor>

#include "mainTree/maintreeabstractmodel.h"
#include "mainTree/maintreeitem.h"

MainTreeAbstractModel::MainTreeAbstractModel(QObject *parent) :
    QAbstractItemModel(parent)
{
}

void MainTreeAbstractModel::setDomDocument(QDomDocument domDoc)
{
    mtoO_domDoc = domDoc;
    root = mtoO_domDoc.documentElement();
    freeNumList.clear();
    freeSeparatorsNumList.clear();

}

MainTreeAbstractModel::~MainTreeAbstractModel()
{
    delete rootItem;
}


int MainTreeAbstractModel::rowCount(const QModelIndex &parent) const
{

    MainTreeItem *parentItem;
    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<MainTreeItem*>(parent.internalPointer());


    return parentItem->childCount();

    //    return 10;
}


int MainTreeAbstractModel::columnCount(const QModelIndex &parent) const
{
    //    if (parent.isValid())
    //        return static_cast<MainTreeItem*>(parent.internalPointer())->columnCount();
    //    else
    //        return rootItem->columnCount();

    return 6;

}




QVariant MainTreeAbstractModel::headerData(int section, Qt::Orientation orientation,
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

QVariant MainTreeAbstractModel::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    int col = index.column();





    if (!index.isValid())
        return QVariant();

    if ((role == Qt::DisplayRole || role == Qt::EditRole) && col == 0){
        MainTreeItem *item = static_cast<MainTreeItem*>(index.internalPointer());
        return item->data(col).toString();
    }

    if (role == 33 && (col == 1 || col == 2)){
        MainTreeItem *item = static_cast<MainTreeItem*>(index.internalPointer());
        return item->data(col).toString();
    }
    // PoV :
    if (role == 35 && col == 3){
        MainTreeItem *item = static_cast<MainTreeItem*>(index.internalPointer());
        return item->data(col);
    }
    // status :
    if (role == 38 && col == 4){
        MainTreeItem *item = static_cast<MainTreeItem*>(index.internalPointer());
        return item->status();
    }
    //     badge :
    if ((role == Qt::DisplayRole || role == Qt::EditRole) && col == 5){
        MainTreeItem *item = static_cast<MainTreeItem*>(index.internalPointer());
        return item->badge();
    }

    if (role == Qt::UserRole){
        MainTreeItem *item = static_cast<MainTreeItem*>(index.internalPointer());
        return item->idNumber();
    }
    if (role == 34){
        MainTreeItem *item = static_cast<MainTreeItem*>(index.internalPointer());
        return item->isExpanded(MainTreeItem::DockedTree);
    }
    if (role == 39){
        MainTreeItem *item = static_cast<MainTreeItem*>(index.internalPointer());
        return item->isExpanded(MainTreeItem::Outliner);
    }
    if (role == 40){
        MainTreeItem *item = static_cast<MainTreeItem*>(index.internalPointer());
        return item->isExpanded(MainTreeItem::Exporter);
    }
    if (role == 36){
        MainTreeItem *item = static_cast<MainTreeItem*>(index.internalPointer());
        return item->type();
    }
    if (role == 37){
        MainTreeItem *item = static_cast<MainTreeItem*>(index.internalPointer());
        return item->isTrashed();
    }
    if (role == Qt::BackgroundRole && col == 0){
        MainTreeItem *item = static_cast<MainTreeItem*>(index.internalPointer());
        if(item->idNumber() == m_currentSheet)
            return QBrush(Qt::lightGray);
        else
            return QVariant();
    }
    if (role == Qt::FontRole && col == 0){
        MainTreeItem *item = static_cast<MainTreeItem*>(index.internalPointer());
        if(item->idNumber() == m_currentSheet){
            QFont font;
            font.setBold(true);
            return font;
        }
    }
    return QVariant();

}


//--------------------------------------------------------------------------------------------------

QModelIndex MainTreeAbstractModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();


    MainTreeItem *childItem = static_cast<MainTreeItem*>(index.internalPointer());
    MainTreeItem *parentItem = childItem->parent();

    if (parentItem == rootItem)
        return QModelIndex();


    QModelIndex parentIndex = createIndex(parentItem->row(), 0, parentItem);
    parentItem->setIndex(parentIndex);
    return parentIndex;

    //    return QModelIndex();

}



//--------------------------------------------------------------------------------------------------

QModelIndex MainTreeAbstractModel::index ( int row, int column, const QModelIndex & parent ) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    //    if (column == 0){

    MainTreeItem *parentItem;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<MainTreeItem*>(parent.internalPointer());

    MainTreeItem *childItem = parentItem->child(row);
    if (childItem){
        QModelIndex index = createIndex(row, column, childItem);
        childItem->setIndex(index);
        return index;
    }else
        return QModelIndex();
    //    }
    //    return QModelIndex();
}



//--------------------------------------------------------------------------------------------------
//------------------Editing----------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------

Qt::ItemFlags MainTreeAbstractModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags defaultFlags = QAbstractItemModel::flags(index);

    if (!index.isValid())
        return defaultFlags;

    if (index.column() == 0 || index.column() == 1 || index.column() == 2 || index.column() == 3
            || index.column() == 4 || index.column() == 5)
        return defaultFlags;

    return defaultFlags;
}

//--------------------------------------------------------------------------------------------------

bool MainTreeAbstractModel::setData(const QModelIndex &index,
                                    const QVariant &value, int role)
{
    QVector<int> vector(1, role);

    if (index.isValid() && role == Qt::EditRole && index.column() == 0) {

        titlesList->replace(index.row(), value.toString());


        MainTreeItem *item = static_cast<MainTreeItem*>(index.internalPointer());
        item->dataList()->replace(0,value.toString());

        if (item->type() == "separator" || item->type() == "trash")//for not sheets
            return false;

        QDomElement element = hub->mainTree_domElementForNumberHash().value(index.data(Qt::UserRole).toInt());
        element.setAttribute("name", value.toString());

        emit dataChanged(index, index);
        hub->addToSaveQueue();

        return true;
    }
    if (index.isValid() && role == Qt::EditRole && index.column() == 1) {



        QList<MainTextDocument *> miniDocList = mtoO_numForClonedDoc.keys(index.data(Qt::UserRole).toInt());
        MainTextDocument *synDoc = new MainTextDocument;
        for(int i = 0; i < miniDocList.size(); ++i){
            if(miniDocList.at(i)->objectName().left(11) == "synDocClone"){
                synDoc = miniDocList.at(i);
                synDoc->setHtml(value.toString());

                MainTreeItem *item = static_cast<MainTreeItem*>(index.internalPointer());

                if (item->type() == "separator" || item->type() == "trash")//for not sheets
                    return false;
                item->dataList()->replace(1,value.toString());
            }
        }

        emit dataChanged(index, index);


        updateMainTextDoc(synDoc, index.data(Qt::UserRole).toInt());

        hub->addToSaveQueue();

        return true;
    }
    if (index.isValid() && role == Qt::EditRole && index.column() == 2) {



        QList<MainTextDocument *> miniDocList = mtoO_numForClonedDoc.keys(index.data(Qt::UserRole).toInt());
        MainTextDocument *noteDoc = new MainTextDocument;
        for(int i = 0; i < miniDocList.size(); ++i){

            if(miniDocList.at(i)->objectName().left(12) == "noteDocClone"){
                noteDoc = miniDocList.at(i);
                noteDoc->setHtml(value.toString());

                MainTreeItem *item = static_cast<MainTreeItem*>(index.internalPointer());

                if (item->type() == "separator" || item->type() == "trash")//for not sheets
                    return false;
                item->dataList()->replace(2,value.toString());

            }
        }


        emit dataChanged(index, index);

        updateMainTextDoc(noteDoc, index.data(Qt::UserRole).toInt());

        hub->addToSaveQueue();

        return true;

    }
    if (index.isValid() && role == Qt::EditRole && index.column() == 4) {
        // status :


        MainTreeItem *item = static_cast<MainTreeItem*>(index.internalPointer());

        if (item->type() == "separator" || item->type() == "trash")//for not sheets
            return false;

        QDomElement element = hub->mainTree_domElementForNumberHash().value(index.data(Qt::UserRole).toInt());
        element.setAttribute("status", QString::number(value.toInt()));

        item->setStatus(value.toInt());


        emit dataChanged(index, index);

        hub->addToSaveQueue();

        return true;

    }
    if (index.isValid() && role == Qt::EditRole && index.column() == 5) {
        // badge :


        MainTreeItem *item = static_cast<MainTreeItem*>(index.internalPointer());

        if (item->type() == "trash")
            return false;

        QDomElement element = hub->mainTree_domElementForNumberHash().value(index.data(Qt::UserRole).toInt());
        element.setAttribute("badge", value.toString());

        item->setBadge(value.toString());


        emit dataChanged(index, index);

        hub->addToSaveQueue();

        return true;

    }



    return false;
}


//--------------------------------------------------------------------------------------------------

void MainTreeAbstractModel::resetAbsModel()
{
    beginResetModel();

    QList<QVariant> rootData;
    rootData << tr("Titles") << tr("Synopsis") << tr("Notes") << tr("PoV") << tr("Status") << tr("Badge");
    rootItem = new MainTreeItem(rootData);

    numberOfDomElements = 0;
    mtoO_numForClonedDoc.clear();

    reset_mtoO_setNumForDoc();
    resetDomDoc();


    endResetModel();

    emit applySettingsFromDataSignal();
}





void MainTreeAbstractModel::updateMainTextDoc(MainTextDocument *textDoc, int number)
{

    QList<MainTextDocument *> miniDocList = mtoO_numForDoc.keys(number);
    for(int i = 0; i < miniDocList.size(); ++i){


        if(miniDocList.at(i)->objectName().left(7) == "noteDoc" && textDoc->objectName().left(12) == "noteDocClone"){
            MainTextDocument *doc = miniDocList.at(i);
            doc->setHtml(textDoc->toHtml()) ;
        }
        else if(miniDocList.at(i)->objectName().left(6) == "synDoc" && textDoc->objectName().left(11) == "synDocClone"){
            MainTextDocument *doc = miniDocList.at(i);
            doc->setHtml(textDoc->toHtml()) ;

        }
    }

    emit applySynNoteFontConfigSignal();

}



















void MainTreeAbstractModel::resetDomDoc()
{
    if(root.childNodes().size() == 1) // if tree is empty, has only trash
        this->addSheet(root, "addChild", "book");

    titlesList = new QStringList;
    leftHeadersList = new QStringList;
    numberList = new QList<int>;
    domElementForNumber.clear();


    numberOfDomElements = 0;
    numberOfBooks = 0;
    numberOfActs = 0;
    numberOfChapters = 0;
    numberOfScenes = 0;

    treeBookItemList = new QList<MainTreeItem *>;
    treeActItemList = new QList<MainTreeItem *>;
    treeChapterItemList = new QList<MainTreeItem *>;
    treeTrashItemList = new QList<MainTreeItem *>;



    parseFolderElement(root);




    hub->set_mainTree_domElementForNumberHash(domElementForNumber);

    emit treeStructureChanged();
}


void MainTreeAbstractModel::parseFolderElement(const QDomElement &element)
{
    //    QTreeWidgetItem *item = createItem(element, parentItem);
    QString string1;
    QString string2;
    QString string3;
    QString title = element.attribute("name");
    if (title.isEmpty())
        title = QObject::tr("No Title");


    QDomElement child = element.firstChildElement();
    while (!child.isNull()) {

        titlesList->append(child.attribute("name"));
        numberList->append(child.attribute("number").toInt());
        domElementForNumber.insert(child.attribute("number").toInt(), child);
        int num = child.attribute("number").toInt();

        if (child.tagName() == "book") {
            numberOfBooks += 1;
            leftHeadersList->append(string1.setNum(numberOfBooks));
            numberOfActs = 0;
            numberOfChapters = 0;
            numberOfScenes = 0;





            QList<QVariant> itemData;

            itemData.append(child.attribute("name", "error"));

            QList<MainTextDocument *> miniDocList = mtoO_numForClonedDoc.keys(child.attribute("number").toInt());
            for(int i = 0; i < miniDocList.size(); ++i){
                if(miniDocList.at(i)->objectName().left(11) == "synDocClone")
                    itemData.append(miniDocList.at(i)->toHtml());


            }
            for(int i = 0; i < miniDocList.size(); ++i){
                if(miniDocList.at(i)->objectName().left(12) == "noteDocClone")
                    itemData.append(miniDocList.at(i)->toHtml());
            }


            itemData.append(this->givePovList(child.attribute("pov", "0")));


            MainTreeItem *treeItem = new MainTreeItem(itemData, rootItem);

            if(child.parentNode().toElement().tagName() == "trash")
                treeItem->setParentItem(treeTrashItemList->last());

            treeItem->setIdNumber(child.attribute("number").toInt());
            treeItem->setType("book");
            treeItem->setStatus(child.attribute("status", "0").toInt());
            treeItem->setBadge(child.attribute("badge", ""));
            treeItem->setCheckState(static_cast<Qt::CheckState>(child.attribute("exporterCheckState", "2").toInt()), MainTreeItem::Exporter);
            treeItem->setCheckState(static_cast<Qt::CheckState>(child.attribute("findReplaceCheckState", "2").toInt()), MainTreeItem::FindReplace);

            if(child.attribute("isTrashed", "no") == "yes")
                treeItem->setIsTrashed(true);
            else
                treeItem->setIsTrashed(false);

            if(child.attribute("dockedTreeExpanded", "yes") == "yes")
                treeItem->setIsExpanded(true, MainTreeItem::DockedTree);
            else
                treeItem->setIsExpanded(false, MainTreeItem::DockedTree);
            if(child.attribute("outlinerExpanded", "yes") == "yes")
                treeItem->setIsExpanded(true, MainTreeItem::Outliner);
            else
                treeItem->setIsExpanded(false, MainTreeItem::Outliner);
            if(child.attribute("exporterExpanded", "yes") == "yes")
                treeItem->setIsExpanded(true, MainTreeItem::Exporter);
            else
                treeItem->setIsExpanded(false, MainTreeItem::Exporter);
            if(child.attribute("findreplaceExpanded", "yes") == "yes")
                treeItem->setIsExpanded(true, MainTreeItem::FindReplace);
            else
                treeItem->setIsExpanded(false, MainTreeItem::FindReplace);


            if(child.parentNode().toElement().tagName() == "plume-tree")
                rootItem->appendChild(treeItem);
            else if(child.parentNode().toElement().tagName() == "trash")
                treeTrashItemList->last()->appendChild(treeItem);

            treeBookItemList->append(treeItem);



            parseFolderElement(child);


        } else if (child.tagName() == "act") {
            numberOfActs += 1;
            leftHeadersList->append(string1.setNum(numberOfBooks) + "." + string2.setNum(numberOfActs));
            numberOfChapters = 0;



            QList<QVariant> itemData;

            itemData.append(child.attribute("name", "error"));

            QList<MainTextDocument *> miniDocList = mtoO_numForClonedDoc.keys(child.attribute("number").toInt());
            for(int i = 0; i < miniDocList.size(); ++i){
                if(miniDocList.at(i)->objectName().left(11) == "synDocClone")
                    itemData.append(miniDocList.at(i)->toHtml());



            }
            for(int i = 0; i < miniDocList.size(); ++i){
                if(miniDocList.at(i)->objectName().left(12) == "noteDocClone")
                    itemData.append(miniDocList.at(i)->toHtml());
            }

            itemData.append(this->givePovList(child.attribute("pov", "0")));


            MainTreeItem *treeItem = new MainTreeItem(itemData, 0);

            if(child.parentNode().toElement().tagName() == "book")
                treeItem->setParentItem(treeBookItemList->last());
            else if(child.parentNode().toElement().tagName() == "trash")
                treeItem->setParentItem(treeTrashItemList->last());

            treeItem->setIdNumber(child.attribute("number").toInt());
            treeItem->setType("act");
            treeItem->setStatus(child.attribute("status", "0").toInt());
            treeItem->setBadge(child.attribute("badge", ""));
            treeItem->setCheckState(static_cast<Qt::CheckState>(child.attribute("exporterCheckState", "2").toInt()), MainTreeItem::Exporter);
            treeItem->setCheckState(static_cast<Qt::CheckState>(child.attribute("findReplaceCheckState", "2").toInt()), MainTreeItem::FindReplace);

            if(child.attribute("isTrashed", "no") == "yes")
                treeItem->setIsTrashed(true);
            else
                treeItem->setIsTrashed(false);


            if(child.attribute("dockedTreeExpanded", "yes") == "yes")
                treeItem->setIsExpanded(true, MainTreeItem::DockedTree);
            else
                treeItem->setIsExpanded(false, MainTreeItem::DockedTree);
            if(child.attribute("outlinerExpanded", "yes") == "yes")
                treeItem->setIsExpanded(true, MainTreeItem::Outliner);
            else
                treeItem->setIsExpanded(false, MainTreeItem::Outliner);
            if(child.attribute("exporterExpanded", "yes") == "yes")
                treeItem->setIsExpanded(true, MainTreeItem::Exporter);
            else
                treeItem->setIsExpanded(false, MainTreeItem::Exporter);
            if(child.attribute("findreplaceExpanded", "yes") == "yes")
                treeItem->setIsExpanded(true, MainTreeItem::FindReplace);
            else
                treeItem->setIsExpanded(false, MainTreeItem::FindReplace);


            if(child.parentNode().toElement().tagName() == "book")
                treeBookItemList->last()->appendChild(treeItem);
            else if(child.parentNode().toElement().tagName() == "trash")
                treeTrashItemList->last()->appendChild(treeItem);



            treeActItemList->append(treeItem);




            parseFolderElement(child);


        }else if (child.tagName() == "chapter") {
            numberOfChapters += 1;
            leftHeadersList->append(string1.setNum(numberOfBooks) + "." + string2.setNum(numberOfChapters));
            numberOfScenes = 0;



            QList<QVariant> itemData;

            itemData.append(child.attribute("name", "error"));

            QList<MainTextDocument *> miniDocList = mtoO_numForClonedDoc.keys(child.attribute("number").toInt());
            for(int i = 0; i < miniDocList.size(); ++i){
                if(miniDocList.at(i)->objectName().left(11) == "synDocClone")
                    itemData.append(miniDocList.at(i)->toHtml());



            }
            for(int i = 0; i < miniDocList.size(); ++i){
                if(miniDocList.at(i)->objectName().left(12) == "noteDocClone")
                    itemData.append(miniDocList.at(i)->toHtml());
            }

            itemData.append(this->givePovList(child.attribute("pov", "0")));


            MainTreeItem *treeItem = new MainTreeItem(itemData, 0);

            if(child.parentNode().toElement().tagName() == "book")
                treeItem->setParentItem(treeBookItemList->last());
            if(child.parentNode().toElement().tagName() == "act")
                treeItem->setParentItem(treeActItemList->last());
            if(child.parentNode().toElement().tagName() == "trash")
                treeItem->setParentItem(treeTrashItemList->last());

            treeItem->setIdNumber(child.attribute("number").toInt());
            treeItem->setType("chapter");
            treeItem->setStatus(child.attribute("status", "0").toInt());
            treeItem->setBadge(child.attribute("badge", ""));
            treeItem->setCheckState(static_cast<Qt::CheckState>(child.attribute("exporterCheckState", "2").toInt()), MainTreeItem::Exporter);
            treeItem->setCheckState(static_cast<Qt::CheckState>(child.attribute("findReplaceCheckState", "2").toInt()), MainTreeItem::FindReplace);

            if(child.attribute("isTrashed", "no") == "yes")
                treeItem->setIsTrashed(true);
            else
                treeItem->setIsTrashed(false);


            if(child.attribute("dockedTreeExpanded", "yes") == "yes")
                treeItem->setIsExpanded(true, MainTreeItem::DockedTree);
            else
                treeItem->setIsExpanded(false, MainTreeItem::DockedTree);
            if(child.attribute("outlinerExpanded", "yes") == "yes")
                treeItem->setIsExpanded(true, MainTreeItem::Outliner);
            else
                treeItem->setIsExpanded(false, MainTreeItem::Outliner);
            if(child.attribute("exporterExpanded", "yes") == "yes")
                treeItem->setIsExpanded(true, MainTreeItem::Exporter);
            else
                treeItem->setIsExpanded(false, MainTreeItem::Exporter);
            if(child.attribute("findreplaceExpanded", "yes") == "yes")
                treeItem->setIsExpanded(true, MainTreeItem::FindReplace);
            else
                treeItem->setIsExpanded(false, MainTreeItem::FindReplace);


            if(child.parentNode().toElement().tagName() == "book")
                treeBookItemList->last()->appendChild(treeItem);
            else if(child.parentNode().toElement().tagName() == "act")
                treeActItemList->last()->appendChild(treeItem);
            else if(child.parentNode().toElement().tagName() == "trash")
                treeTrashItemList->last()->appendChild(treeItem);

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

            QList<MainTextDocument *> miniDocList = mtoO_numForClonedDoc.keys(child.attribute("number").toInt());
            for(int i = 0; i < miniDocList.size(); ++i){
                if(miniDocList.at(i)->objectName().left(11) == "synDocClone")
                    itemData.append(miniDocList.at(i)->toHtml());



            }
            for(int i = 0; i < miniDocList.size(); ++i){
                if(miniDocList.at(i)->objectName().left(12) == "noteDocClone")
                    itemData.append(miniDocList.at(i)->toHtml());
            }

            itemData.append(this->givePovList(child.attribute("pov", "0")));


            MainTreeItem *treeItem = new MainTreeItem(itemData, 0);

            if(child.parentNode().toElement().tagName() == "chapter")
                treeItem->setParentItem(treeChapterItemList->last());
            else if(child.parentNode().toElement().tagName() == "trash")
                treeItem->setParentItem(treeTrashItemList->last());

            treeItem->setIdNumber(child.attribute("number").toInt());
            treeItem->setType("scene");
            treeItem->setStatus(child.attribute("status", "0").toInt());
            treeItem->setBadge(child.attribute("badge", ""));
            treeItem->setCheckState(static_cast<Qt::CheckState>(child.attribute("exporterCheckState", "2").toInt()), MainTreeItem::Exporter);
            treeItem->setCheckState(static_cast<Qt::CheckState>(child.attribute("findReplaceCheckState", "2").toInt()), MainTreeItem::FindReplace);

            if(child.attribute("isTrashed", "no") == "yes")
                treeItem->setIsTrashed(true);
            else
                treeItem->setIsTrashed(false);


            if(child.attribute("dockedTreeExpanded", "yes") == "yes")
                treeItem->setIsExpanded(true, MainTreeItem::DockedTree);
            else
                treeItem->setIsExpanded(false, MainTreeItem::DockedTree);
            if(child.attribute("outlinerExpanded", "yes") == "yes")
                treeItem->setIsExpanded(true, MainTreeItem::Outliner);
            else
                treeItem->setIsExpanded(false, MainTreeItem::Outliner);
            if(child.attribute("exporterExpanded", "yes") == "yes")
                treeItem->setIsExpanded(true, MainTreeItem::Exporter);
            else
                treeItem->setIsExpanded(false, MainTreeItem::Exporter);
            if(child.attribute("findreplaceExpanded", "yes") == "yes")
                treeItem->setIsExpanded(true, MainTreeItem::FindReplace);
            else
                treeItem->setIsExpanded(false, MainTreeItem::FindReplace);

            if(child.parentNode().toElement().tagName() == "chapter")
                treeChapterItemList->last()->appendChild(treeItem);
            else if(child.parentNode().toElement().tagName() == "trash")
                treeTrashItemList->last()->appendChild(treeItem);






        }

        else if (child.tagName() == "separator") {
            //            QTreeWidgetItem *childItem = createItem(child);
            //            childItem->setText(0, "             " + QString(10, 0xB7));
            leftHeadersList->append("---");

            QList<QVariant> itemData;
            itemData.append(child.attribute("name", "error"));
            MainTreeItem *treeItem = new MainTreeItem(itemData, 0);

            if(child.parentNode().toElement().tagName() == "chapter")
                treeItem->setParentItem(treeChapterItemList->last());
            else if(child.parentNode().toElement().tagName() == "trash")
                treeItem->setParentItem(treeTrashItemList->last());

            treeItem->setIdNumber(child.attribute("number").toInt());
            treeItem->setType("separator");
            treeItem->setStatus(-1);
            treeItem->setBadge(child.attribute("badge", ""));
            treeItem->setCheckState(static_cast<Qt::CheckState>(child.attribute("exporterCheckState", "2").toInt()), MainTreeItem::Exporter);
            treeItem->setCheckState(static_cast<Qt::CheckState>(child.attribute("findReplaceCheckState", "2").toInt()), MainTreeItem::FindReplace);

            if(child.attribute("isTrashed", "no") == "yes")
                treeItem->setIsTrashed(true);
            else
                treeItem->setIsTrashed(false);

            if(child.parentNode().toElement().tagName() == "chapter")
                treeChapterItemList->last()->appendChild(treeItem);
            else if(child.parentNode().toElement().tagName() == "trash")
                treeTrashItemList->last()->appendChild(treeItem);


        }

        else if (child.tagName() == "trash") {

            leftHeadersList->append("Bin");

            QList<QVariant> itemData;
            itemData.append(tr("Bin"));
            MainTreeItem *treeItem = new MainTreeItem(itemData, rootItem);
            treeItem->setIdNumber(child.attribute("number", "20000").toInt());
            treeItem->setIsTrashed(false);
            treeItem->setType("trash");
            treeItem->setStatus(-1);
            treeItem->setCheckState(static_cast<Qt::CheckState>(child.attribute("exporterCheckState", "2").toInt()), MainTreeItem::Exporter);
            treeItem->setCheckState(static_cast<Qt::CheckState>(child.attribute("findReplaceCheckState", "2").toInt()), MainTreeItem::FindReplace);
            rootItem->appendChild(treeItem);


            if(child.attribute("dockedTreeExpanded", "yes") == "yes")
                treeItem->setIsExpanded(true, MainTreeItem::DockedTree);
            else
                treeItem->setIsExpanded(false, MainTreeItem::DockedTree);
            if(child.attribute("outlinerExpanded", "yes") == "yes")
                treeItem->setIsExpanded(true, MainTreeItem::Outliner);
            else
                treeItem->setIsExpanded(false, MainTreeItem::Outliner);
            if(child.attribute("exporterExpanded", "yes") == "yes")
                treeItem->setIsExpanded(true, MainTreeItem::Exporter);
            else
                treeItem->setIsExpanded(false, MainTreeItem::Exporter);
            if(child.attribute("findreplaceExpanded", "yes") == "yes")
                treeItem->setIsExpanded(true, MainTreeItem::FindReplace);
            else
                treeItem->setIsExpanded(false, MainTreeItem::FindReplace);

            treeTrashItemList->append(treeItem);

            parseFolderElement(child);




        }


        //        int number = element.attribute("number").toInt();
        //        domElementForNumber.insert(number, element);



        numberOfDomElements += 1;
        child = child.nextSiblingElement();
    }


}

//-----------------------------------------------------------------------------------------------------------
QStringList MainTreeAbstractModel::givePovList(QString listOfPovNumbers)
{
    QList<QString> names;

    QList<int> intList;
    QStringList povStringList = listOfPovNumbers.split("-", QString::SkipEmptyParts);
    foreach(const QString &str, povStringList)
        intList.append(str.toInt());


    foreach(const int &number, intList){
        if(number != 0){
            QString name = hub->attendTree_domElementForNumberHash().value(number).attribute("name", "error");

            names << name;
            qDebug() <<"";
        }
    }
    QStringList nameList;
    nameList.append(names);
    return nameList;
}


//-----------------------------------------------------------------------------------------------------------
void MainTreeAbstractModel::mtoO_setNumForDoc(QHash<MainTextDocument *, int> numForDoc)
{

    mtoO_numForDoc = numForDoc;
    //    qDebug() <<"mtoO_numForDoc size : "  << QString::number(mtoO_numForDoc.size());



}

//-----------------------------------------------------------------------------------------------------------


void MainTreeAbstractModel::reset_mtoO_setNumForDoc()
{

    mtoO_numForClonedDoc.clear();


    QHash<MainTextDocument *, int>::const_iterator  i = mtoO_numForDoc.constBegin();
    while (i != mtoO_numForDoc.constEnd()) {
        MainTextDocument *textDoc = i.key();
        MainTextDocument *textDocClone = new MainTextDocument();
        textDocClone->setHtml(textDoc->toHtml());

        if(textDoc->objectName().left(6) == "synDoc")
            textDocClone->setObjectName("synDocClone_" + QString::number(i.value()));

        else if(textDoc->objectName().left(7) == "noteDoc")
            textDocClone->setObjectName("noteDocClone_" + QString::number(i.value()));

        mtoO_numForClonedDoc.insert(textDocClone, i.value());



        ++i;
    }

}
//-----------------------------------------------------------------------------------------
void MainTreeAbstractModel::actionSlot(QString action, int idNumber, QVariant var)
{

    QDomElement targetElement = hub->mainTree_domElementForNumberHash().value(idNumber);


    if( targetElement.isNull())
        return;

    else if(action == "rename"){
        targetElement.setAttribute("name", var.toString());
    }


    else if(action == "addMulti"){
        if(targetElement.tagName() == "scene" || targetElement.tagName() == "separator")
            return;

        int numSheets = var.toInt();
        QString type;
        if(targetElement.tagName() == "book")
            type = "chapter";
        else if(targetElement.tagName() == "act")
            type = "chapter";
        else if(targetElement.tagName() == "chapter")
            type = "scene";


        for(int i = 1; i <= numSheets; ++i)
            addSheet(targetElement, "addChild", type);



    }

    else if(action == "split"){

        QString splitChoice = var.toString();



        QApplication::setOverrideCursor(Qt::WaitCursor);


        //                 set up the progress bar :

        QWidget *progressWidget = new QWidget(0, Qt::Dialog | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
        QHBoxLayout *progressLayout = new QHBoxLayout(progressWidget);
        QProgressBar *progressBar = new QProgressBar(progressWidget);
        int progressValue = 0;

        progressLayout->addWidget(progressBar);
        progressWidget->setLayout(progressLayout);




        //splitting :


        QDomElement childItem;
        QDomElement childItem2;
        QDomElement item;
        QDomElement itemOfWork;
        QDomElement itemOfWork2;
        QStringList chaptersList, scenesList;
        QTextDocumentWriter *docWriter = new QTextDocumentWriter;
        docWriter->setFormat("HTML");

        itemOfWork = targetElement;


        int number = targetElement.attribute("number").toInt();

        MainTextDocument *textDoc = hub->findChild<MainTextDocument *>( splitChoice + QString::number(number));
        QString mainString = textDoc->toHtml("utf-8");


        if(targetElement.tagName() == "scene"){

            scenesList = mainString.split("***", QString::KeepEmptyParts);

            progressBar->setMaximum(scenesList.size() - 1);
            progressBar->setValue(progressValue);
            progressWidget->show();

            if(scenesList.size() == 1){
                QMessageBox::information(0, tr("Splitting Task"), tr("This sheet does not contain *** ."));
                return;
            }
            //            QString debug;
            //            qDebug() << "scenesList.size() : " << debug.setNum(scenesList.size(), 10);
            for(int i = 0; i < scenesList.size(); ++i){

                item = addSheet(itemOfWork, "addSibling", "scene");
                int num = item.attribute("number").toInt();
                MainTextDocument *doc = hub->findChild<MainTextDocument *>(splitChoice + QString::number(num));
                doc->setHtml(scenesList.at(i));

                itemOfWork = item;

                progressValue += 1;
                progressBar->setValue(progressValue);
            }


        }
        else if(targetElement.tagName() == "chapter"){
            chaptersList = mainString.split("###", QString::KeepEmptyParts);

            progressBar->setMaximum(chaptersList.size() - 1);
            progressBar->setValue(progressValue);
            progressWidget->show();

            for(int i = 0; i < chaptersList.size(); ++i){
                item = addSheet(itemOfWork, "addSibling", "chapter");

                scenesList = chaptersList.at(i).split("***", QString::KeepEmptyParts);
                childItem = addSheet(item, "addChild", "scene");
                int num = childItem.toElement().attribute("number").toInt();
                MainTextDocument *doc = hub->findChild<MainTextDocument *>(splitChoice + QString::number(num));
                doc->setHtml(scenesList.at(0));

                progressValue += 1;
                progressBar->setValue(progressValue);

                for(int j = 1; j < scenesList.size(); ++j){

                    itemOfWork2 = addSheet(childItem, "addSibling", "scene");
                    int num = itemOfWork2.toElement().attribute("number").toInt();
                    MainTextDocument *doc = hub->findChild<MainTextDocument *>(splitChoice + QString::number(num));
                    doc->setHtml(scenesList.at(j));
                    childItem = itemOfWork2;
                }
                itemOfWork = item;

                progressValue += 1;
                progressBar->setValue(progressValue);
            }
        }
        else if(targetElement.tagName() == "book"){
            chaptersList = mainString.split("###", QString::KeepEmptyParts);

            progressBar->setMaximum(chaptersList.size() - 1);
            progressBar->setValue(progressValue);
            progressWidget->show();

            // build the first child chapter and its first child :

            childItem = addSheet(itemOfWork, "addChild", "chapter");
            //            int num = domElementForItem.value(childItem).toElement().attribute("number").toInt();
            //            MainTextDocument *doc = hub->findChild<MainTextDocument *>(splitChoice + string.setNum(num,10));
            //            doc->setHtml(chaptersList.at(0));

            scenesList = chaptersList.at(0).split("***", QString::KeepEmptyParts);
            childItem2 = addSheet(childItem, "addChildren", "scene");
            int num = childItem2.toElement().attribute("number").toInt();
            MainTextDocument *doc = hub->findChild<MainTextDocument *>(splitChoice + QString::number(num));
            doc->setHtml(scenesList.at(0));

            progressValue += 1;
            progressBar->setValue(progressValue);

            for(int k = 1; k < scenesList.size(); ++k){

                itemOfWork2 = addSheet(childItem2, "addSibling", "scene");
                int num = itemOfWork2.toElement().attribute("number").toInt();
                MainTextDocument *doc = hub->findChild<MainTextDocument *>(splitChoice + QString::number(num));
                doc->setHtml(scenesList.at(k));
                childItem2 = itemOfWork2;

            }



            // build the next chapters and their first child scenes :

            for(int i = 1; i < chaptersList.size(); ++i){
                item = addSheet(childItem, "addSibling", "chapter");

                scenesList = chaptersList.at(i).split("***", QString::KeepEmptyParts);
                childItem = addSheet(item, "addChild", "scene");
                int num = childItem.toElement().attribute("number").toInt();
                MainTextDocument *doc = hub->findChild<MainTextDocument *>(splitChoice + QString::number(num));
                doc->setHtml(scenesList.at(0));

                for(int j = 1; j < scenesList.size(); ++j){

                    itemOfWork2 = addSheet(childItem, "addSibling", "scene");
                    int num = itemOfWork2.toElement().attribute("number").toInt();
                    MainTextDocument *doc = hub->findChild<MainTextDocument *>(splitChoice + QString::number(num));
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



    }
    else if(action == "displayBadge"){
        emit displayBadgeSignal(var.toBool());

    }
    else if(action == "setBadgeText"){
        targetElement.setAttribute("badge", var.toString());
    }
    else if(action == "setStatus"){
        targetElement.setAttribute("status", var.toString());
    }


    hub->addToSaveQueue();
    this->resetAbsModel();



}
//-----------------------------------------------------------------------------------------
void MainTreeAbstractModel::actionSlot(QString action, int idNumber)
{



    QDomElement targetElement = hub->mainTree_domElementForNumberHash().value(idNumber);

    if( targetElement.isNull())
        return;


    else if(action == "moveUp"){

        QDomNode newNode(targetElement);

        QDomNode parentElement(targetElement.parentNode());
        QDomNode firstChildNode(parentElement.firstChild());
        QDomNode prevParentElement(parentElement.previousSibling());
        QDomNode prevSibling(targetElement.previousSibling().toElement());
        QDomNode elementMoved;


        if(targetElement.toElement().tagName() == "chapter"
                && parentElement.toElement().tagName() == "act" )// to move chapter between books and acts
            prevParentElement = parentElement.previousSiblingElement("act");

        else if(targetElement.toElement().tagName() == "chapter"
                && parentElement.toElement().tagName() == "book" )
            prevParentElement = parentElement.previousSiblingElement("book");

        QDomNode prevChild(prevParentElement.lastChild().toElement());

        if(targetElement == firstChildNode.toElement() && parentElement.previousSibling().isNull())
            return;

        if(targetElement == firstChildNode.toElement()){
            if(prevChild.isNull())
                elementMoved = prevParentElement.appendChild(newNode);

            else
                elementMoved = prevParentElement.insertAfter(newNode, prevChild);
        }
        else
            elementMoved = parentElement.insertBefore(newNode, prevSibling);




    }
    else if(action == "moveDown"){
        QDomNode newNode(targetElement);

        QDomNode parentElement(targetElement.parentNode());
        QDomNode lastChildNode(parentElement.lastChild());
        QDomNode nextParentElement(parentElement.nextSibling());
        QDomNode nextSibling(targetElement.nextSibling().toElement());
        QDomNode elementMoved;


        if(targetElement.toElement().tagName() == "chapter"
                && parentElement.toElement().tagName() == "act" )// to move chapter between books and acts
            nextParentElement = parentElement.nextSiblingElement("act");

        else if(targetElement.toElement().tagName() == "chapter"
                && parentElement.toElement().tagName() == "book" )
            nextParentElement = parentElement.nextSiblingElement("book");

        QDomNode nextChild(nextParentElement.firstChild().toElement());


        if(targetElement == lastChildNode.toElement() && parentElement.nextSibling().isNull())
            return;

        if(targetElement == lastChildNode.toElement()){

            if(nextChild.isNull())
                elementMoved = nextParentElement.appendChild(newNode);
            else
                elementMoved = nextParentElement.insertBefore(newNode, nextChild);
        }
        else
            elementMoved = parentElement.insertAfter(newNode, nextSibling);



    }
    else if(action == "sendToTrash"){

        Utils::applyAttributeRecursively(targetElement, "isTrashed", "yes");
        hub->mainTreeDomDoc().documentElement().elementsByTagName("trash").at(0).appendChild(targetElement);
    }
    else if(action == "autoRenameChildren"){
        QString actName = tr("Act");
        QString chapterName = tr("Chapter");
        QString sceneName = tr("Scene");
        QString numString;
        QString newName;
        int preNum = 1;
        int num = 1;

        if(!targetElement.hasChildNodes())
            return;

        if(targetElement.tagName() == "book"){

            QDomElement first = targetElement.firstChildElement("chapter");

            first.setAttribute("name", chapterName + " " + numString.setNum(num,10));
            //            emit nameChangedSignal(chapterName + " " + numString.setNum(num,10), first.attribute("number").toInt());

            while(!first.nextSiblingElement("chapter").isNull()){

                num = preNum + 1;
                newName = chapterName + " " + numString.setNum(num,10);
                QDomElement next = first.nextSiblingElement("chapter");
                next.setAttribute("name", newName);

                preNum = num;
                first = next;

            }





            preNum = 1;
            num = 1;


            QDomElement _first = targetElement.firstChildElement("act");

            _first.setAttribute("name", actName + " " + numString.setNum(num,10));
            //            emit nameChangedSignal(actName + " " + numString.setNum(num,10), _first.attribute("number").toInt());

            while(!_first.nextSiblingElement("act").isNull()){

                num = preNum + 1;
                newName = actName + " " + numString.setNum(num,10);
                QDomElement _next = _first.nextSiblingElement("act");
                _next.setAttribute("name", newName);

                preNum = num;
                _first = _next;

            }
        }
        if(targetElement.tagName() == "act"){

            QDomElement first = targetElement.firstChildElement("chapter");

            first.setAttribute("name", chapterName + " " + numString.setNum(num,10));
            //            emit nameChangedSignal(actName + " " + numString.setNum(num,10), first.attribute("number").toInt());

            while(!first.nextSiblingElement("chapter").isNull()){

                num = preNum + 1;
                newName = chapterName + " " + numString.setNum(num,10);
                QDomElement next = first.nextSiblingElement("chapter");
                next.setAttribute("name", newName);

                preNum = num;
                first = next;

            }

        }
        if(targetElement.tagName() == "chapter"){

            QDomElement first = targetElement.firstChildElement("scene");

            first.setAttribute("name", sceneName + " " + numString.setNum(num,10));
            //            emit nameChangedSignal(sceneName + " " + numString.setNum(num,10), first.attribute("number").toInt());

            while(!first.nextSiblingElement("scene").isNull()){

                num = preNum + 1;
                newName = sceneName + " " + numString.setNum(num,10);
                QDomElement next = first.nextSiblingElement("scene");
                next.setAttribute("name", newName);

                preNum = num;
                first = next;


            }
        }

    }
    else if(action == "addBookNext")
        this->addSheet(targetElement, "addSibling", "book");

    else if(action == "addActNext")
        this->addSheet(targetElement, "addSibling", "act");

    else if(action == "addChapterNext")
        this->addSheet(targetElement, "addSibling", "chapter");

    else if(action == "addSceneNext")
        this->addSheet(targetElement, "addSibling", "scene");

    else if(action == "addSeparatorNext")
        this->addSheet(targetElement, "addSibling", "separator");

    else if(action == "addChildAct")
        this->addSheet(targetElement, "addChild", "act");

    else if(action == "addChildChapter")
        this->addSheet(targetElement, "addChild", "chapter");

    else if(action == "addChildScene")
        this->addSheet(targetElement, "addChild", "scene");

    else if(action == "addChildSeparator")
        this->addSheet(targetElement, "addChild", "separator");

    else if(action == "emptyTrash"){

        if(!targetElement.hasChildNodes())
            return;

        removeItem(targetElement);


    }


    hub->addToSaveQueue();
    this->resetAbsModel();

}

int MainTreeAbstractModel::addItemNext(int baseNumber)
{
    int newNumber;

    QDomElement baseItem = hub->mainTree_domElementForNumberHash().value(baseNumber);


    QString type;
    if(baseItem.tagName() == "book")
        type = "book";
    else if(baseItem.tagName() == "act")
        type = "act";
    else if(baseItem.tagName() == "chapter")
        type = "chapter";
    else if(baseItem.tagName() == "scene")
        type = "scene";





    QDomElement newItem = addSheet(baseItem, "addSibling", type);
    newNumber = hub->mainTree_domElementForNumberHash().key(newItem);




    hub->addToSaveQueue();
    this->resetAbsModel();


    return newNumber;

}


void MainTreeAbstractModel::removeItem(QDomElement element)
{


    if(element.isNull())
        return;





    QDomElement parentElement = element.parentNode().toElement();

    for(int i = 0 ; i < parentElement.childNodes().size() ; ++i ){

        removeItem(element.lastChild().toElement());

    }


    if(element.tagName() == "trash" || element.attribute("isTrashed", "no") == "no")
        return;


    int number = domElementForNumber.key(element);
    emit textAndNoteSignal(number, "close");

    if(element.tagName() == "separator"){
        freeSeparatorsNumList.append(number);

        parentElement.removeChild(element);
        return;
    }


    freeNumList.append(number);

    emit textAndNoteSignal(number,"close");

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



    parentElement.removeChild(element);




}



//-----------------------------------------------------------------------------------------

QDomElement MainTreeAbstractModel::addSheet(QDomElement targetElement, QString action, QString type)
{

    QDomElement newElement;

    if(action == "addChild" && (targetElement.tagName() == "scene" || targetElement.tagName() == "separator"))
        return QDomElement();


    if(action == "addChild"){

        newElement = hub->mainTreeDomDoc().createElement("nothing");
        targetElement.appendChild(newElement);
        newElement = modifyAttributes(newElement, type);
        // expand the parent in order to see the child :
        targetElement.setAttribute("folded", "no");

    }

    else if(action == "addSibling"){



        newElement = hub->mainTreeDomDoc().createElement("nothing");
        targetElement.parentNode().insertAfter(newElement, targetElement);
        newElement = modifyAttributes(newElement, type);


    }




    return newElement;

}
QDomElement MainTreeAbstractModel::modifyAttributes(QDomElement newElement, QString typeOfNewElement )
{





    int freeNum;
    int freeSeparatorsNum;



    if(freeNumList.isEmpty() || freeSeparatorsNumList.isEmpty()){  // for the first time



        QList<int> numList;
        QList<int> separatorsNumList;


        QDomNodeList bookNodes = hub->mainTreeDomDoc().documentElement().elementsByTagName("book");
        QDomNodeList actNodes = hub->mainTreeDomDoc().documentElement().elementsByTagName("act");
        QDomNodeList chapterNodes = hub->mainTreeDomDoc().documentElement().elementsByTagName("chapter");
        QDomNodeList sceneNodes = hub->mainTreeDomDoc().documentElement().elementsByTagName("scene");
        QDomNodeList separatorNodes = hub->mainTreeDomDoc().documentElement().elementsByTagName("separator");


        for(int i = 0; i < bookNodes.size(); ++i)
            numList.append(bookNodes.at(i).toElement().attribute("number").toInt());
        for(int i = 0; i < actNodes.size(); ++i)
            numList.append(actNodes.at(i).toElement().attribute("number").toInt());
        for(int i = 0; i < chapterNodes.size(); ++i)
            numList.append(chapterNodes.at(i).toElement().attribute("number").toInt());
        for(int i = 0; i < sceneNodes.size(); ++i)
            numList.append(sceneNodes.at(i).toElement().attribute("number").toInt());
        for(int i = 0; i < separatorNodes.size(); ++i)
            separatorsNumList.append(separatorNodes.at(i).toElement().attribute("number").toInt());



        qSort(numList.begin(), numList.end());


        if(numList.isEmpty())
            numList << 0;

        int maxNumList = numList.last();
        int num = 1;
        while(!numList.isEmpty()){

            while(numList.first() > num && num < numList.last()){

                freeNumList.append(num);
                num += 1;
            }
            num = numList.first() + 1;
            numList.removeFirst();
        }

        if(!freeNumList.isEmpty())
            freeNum = freeNumList.takeFirst();
        else
            freeNum = maxNumList + 1;





        qSort(separatorsNumList.begin(), separatorsNumList.end());


        if(separatorsNumList.isEmpty())
            separatorsNumList << 10000; // 10000 to 19999 are reserved to separator

        int maxSeparatorsNumList = separatorsNumList.last();
        int separatorsNum = 10001;
        while(!separatorsNumList.isEmpty()){

            while(separatorsNumList.first() > separatorsNum && separatorsNum < separatorsNumList.last()){

                freeSeparatorsNumList.append(separatorsNum);
                separatorsNum += 1;
            }
            separatorsNum = separatorsNumList.first() + 1;
            separatorsNumList.removeFirst();
        }

        if(!freeSeparatorsNumList.isEmpty())
            freeSeparatorsNum = freeSeparatorsNumList.takeFirst();
        else
            freeSeparatorsNum = maxSeparatorsNumList + 1;

    }
    else {

        // save work time :-)
        freeNum = freeNumList.takeFirst();
        freeSeparatorsNum = freeSeparatorsNumList.takeFirst();


    }




    if(freeNum == 0 && !freeNumList.isEmpty())
        freeNum = freeNumList.takeFirst();


    if(freeSeparatorsNum == 0 && !freeSeparatorsNumList.isEmpty())
        freeSeparatorsNum = freeSeparatorsNumList.takeFirst();





    if(typeOfNewElement != "separator"){




        newElement.setAttribute("textPath", "/text/T" + QString::number(freeNum) + ".html");
        newElement.setAttribute("notePath", "/text/N" + QString::number(freeNum) + ".html");
        newElement.setAttribute("synPath", "/text/S" + QString::number(freeNum) + ".html");
        newElement.setAttribute("number", QString::number(freeNum));
        newElement.setAttribute("textCursorPos", QString::number(0));
        newElement.setAttribute("synCursorPos", QString::number(0));
        newElement.setAttribute("noteCursorPos", QString::number(0));
        newElement.setAttribute("isTrashed", "no");


        QFile *textFile = new QFile(hub->projectWorkPath() + newElement.attribute("textPath"));
        QFile *noteFile = new QFile(hub->projectWorkPath() + newElement.attribute("notePath"));
        QFile *synFile = new QFile(hub->projectWorkPath() + newElement.attribute("synPath"));


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
        hub->connectAllSheetsToSpellChecker();

        //temporary :
        hub->addFileToZipList("text", number);
        hub->addFileToZipList("note", number);
        hub->addFileToZipList("syn", number);



    }

    if(typeOfNewElement == "book"){
        newElement.setTagName("book");
        newElement.setAttribute("name", tr("Book ", "add an empty space after so it's easier for the user to add quickly a number")); //for config

    }
    if(typeOfNewElement == "act"){
        newElement.setTagName("act");
        newElement.setAttribute("name", tr("Act ", "add an empty space after so it's easier for the user to add quickly a number"));//for config

    }
    if(typeOfNewElement == "chapter"){
        newElement.setTagName("chapter");
        newElement.setAttribute("name", tr("Chapter ", "add an empty space after so it's easier for the user to add quickly a number"));//for config

    }
    if(typeOfNewElement == "scene" ){
        newElement.setTagName("scene");
        newElement.setAttribute("name", tr("Scene ", "add an empty space after so it's easier for the user to add quickly a number"));//for config

    }
    if(typeOfNewElement == "separator"){
        newElement.setTagName("separator");
        newElement.setAttribute("name", "----");//for config
        newElement.setAttribute("number", QString::number(freeSeparatorsNum));



    }


    return newElement;
}
//------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------------
QString MainTreeAbstractModel::giveStatus(int statusInt)
{

    if(statusInt == -1) // for separator and trash
        return "";


    QStringList statusList = MainTreeAbstractModel::giveStatusList();

    if(statusInt < statusList.size())
        return statusList.at(statusInt);
    else
        return "";



}

//-----------------------------------------------------------------------------------------------------------
QStringList MainTreeAbstractModel::giveStatusList()
{



    QStringList statusList;
    statusList << tr("1st draft") //default
               <<   tr("2nd draft")
                 <<   tr("3rd draft")
                   <<   tr("1st Edit")
                     <<   tr("2nd Edit")
                       <<   tr("3rd Edit")
                         <<   tr("Proofread")
                           <<   tr("Finished");




    return statusList;




}


void MainTreeAbstractModel::modifyDataForOpenedSheetMarker(const int currentSheet)
{





    m_currentSheet = currentSheet;






}



QPixmap MainTreeAbstractModel::giveDecoration(QModelIndex index,MainTreeItem::Tree tree)
{
    MainTreeItem *item = static_cast<MainTreeItem*>(index.internalPointer());
    if(item->type() == "book"  && (!item->isExpanded(tree) || item->childCount() == 0))
        return QPixmap(":/pics/tree/tree-book.png");
    if(item->type() == "book" && item->isExpanded(tree))
        return QPixmap(":/pics/tree/tree-book-expanded.png");
    if(item->type() == "act"  && (!item->isExpanded(tree) || item->childCount() == 0))
        return QPixmap(":/pics/tree/tree-act.png");
    if(item->type() == "act"  && item->isExpanded(tree))
        return QPixmap(":/pics/tree/tree-act-expanded.png");
    if(item->type() == "chapter"  && (!item->isExpanded(tree) || item->childCount() == 0))
        return QPixmap(":/pics/tree/tree-chapter.png");
    if(item->type() == "chapter"  && item->isExpanded(tree))
        return QPixmap(":/pics/tree/tree-chapter-expanded.png");
    if(item->type() == "scene")
        return QPixmap(":/pics/tree/tree-scene.png");
    if(item->type() == "separator")
        return QPixmap("");
    if(item->type() == "trash")
        return QPixmap(":/pics/tree/tree-trash.png");
    else
        return QPixmap(":/pics/tree/tree-scene.png");
}

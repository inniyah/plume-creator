#include "outliner/outlinerbase.h"
#include "outliner/outlinerabstractmodel.h"
#include "outliner/outlineritemdelegate.h"

#include <QtGui>


OutlinerBase::OutlinerBase(QWidget *parent) :
    QWidget(parent), spreadsheetMode(false)
{

    setMinimumSize(500, 300);
    setWindowTitle(tr("Outliner"));
    setFocusPolicy(Qt::WheelFocus);
    QApplication::setActiveWindow(this);
    raise();






    QHBoxLayout *mainLayout = new QHBoxLayout();


    QWidget *mainZone = new QWidget;
    zoneLayout = new QVBoxLayout;
    mainZone->setLayout(zoneLayout);



    QToolBar *commonToolBar = new QToolBar();
    commonToolBar->setOrientation(Qt::Vertical);

    shiftToSpreadsheetAct = new QAction(QIcon(":/pics/table.png"),"", this);
    shiftToSpreadsheetAct->setToolTip(tr("Shift to spreadsheet mode"));
    shiftToSpreadsheetAct->setCheckable(true);
    connect(shiftToSpreadsheetAct, SIGNAL(toggled(bool)), this, SLOT(shiftToSpreadsheet()));

    QAction *quitAct = new QAction(QIcon(":/pics/window-close-32x32.png"),"", this);
    quitAct->setToolTip(tr("Close the Outliner"));
    connect(quitAct, SIGNAL(triggered()), this, SLOT(close()));

    expandSpreadsheetAct = new QAction(QIcon(":/pics/zoom-out.png"),"", this);
    expandSpreadsheetAct->setToolTip(tr("Expand spreadsheet"));

    shrinkSpreadsheetAct = new QAction(QIcon(":/pics/zoom-in.png"),"", this);
    shrinkSpreadsheetAct->setToolTip(tr("Shrink spreadsheet"));

    moveUpAct = new QAction(QIcon(":/pics/go-up-32x32.png"),"", this);
    moveUpAct->setToolTip(tr("Move up"));

    moveDownAct = new QAction(QIcon(":/pics/go-down-32x32.png"),"", this);
    moveDownAct->setToolTip(tr("Move down"));

    commonToolBar->addAction(shiftToSpreadsheetAct);
    commonToolBar->addSeparator();
    commonToolBar->addAction(quitAct);
    commonToolBar->addSeparator();
    commonToolBar->addAction(expandSpreadsheetAct);
    commonToolBar->addAction(shrinkSpreadsheetAct);
commonToolBar->addAction(moveUpAct);
commonToolBar->addAction(moveDownAct);



    mainLayout->addWidget(commonToolBar);
    mainLayout->addWidget(mainZone);

    mainLayout->setMargin(1);
    mainLayout->setSpacing(1);
    mainLayout->setContentsMargins(1,1,1,1);

    setLayout(mainLayout);


    show();



    applyConfig();


    //    QMessageBox msgBox;
    //    msgBox.setIcon(QMessageBox::Warning);
    //    msgBox.setText(tr("The Ouliner is in heavy maintenance !<br>For now, it's only an empty GUI. Please wait for a future release !"));
    //    msgBox.exec();


}
//------------------------------------------------------------------------------------

void OutlinerBase::saveConfig()
{
    QSettings settings;
    settings.beginGroup( "Outline");
    settings.setValue( "size",   size() );
    settings.setValue( "pos",   pos() );
    if(lastOpened == "spreadsheet"){
        //       settings.setValue( "spreadsheetState", spreadsheet->horizontalHeader()->saveState());
        settings.setValue( "spreadsheetState", spreadsheet->header()->saveState());
        settings.setValue( "lastOpened", "spreadsheet" );
        //       settings.setValue( "spreadsheetVertSize", spreadsheet->verticalHeader()->sectionSize(0));
    }

    settings.endGroup();

}

//------------------------------------------------------------------------------------


void OutlinerBase::applyConfig()
{
    QSettings settings;
    settings.beginGroup( "Outline" );
    resize(settings.value( "size", QSize( 1000, 750 ) ).toSize() );
    move(settings.value( "pos" ).toPoint() );
    QString lastOpened = settings.value( "lastOpened", "spreadsheet").toString();
    if(lastOpened == "spreadsheet")
        QTimer::singleShot(0, this, SLOT(shiftToSpreadsheet()));

    settings.endGroup();

}

//------------------------------------------------------------------------------------
void OutlinerBase::applySpreadsheetConfig()
{
    QSettings settings;
    settings.beginGroup( "Outline" );
    //    spreadsheet->horizontalHeader()->restoreState(settings.value( "spreadsheetState",0 ).toByteArray());
    spreadsheet->header()->restoreState(settings.value( "spreadsheetState",0 ).toByteArray());
    //    spreadsheet->verticalHeader()->setDefaultSectionSize(settings.value( "spreadsheetVertSize",40 ).toInt());
    settings.endGroup();



}
//------------------------------------------------------------------------------------


void OutlinerBase::showOutliner()
{


    QApplication::setActiveWindow(this);
    this->show();
    this->raise();
}

//------------------------------------------------------------------------------------
void OutlinerBase::closeEvent(QCloseEvent *event)
{
    saveConfig();
    this->hide();
    //propagate here


    event->ignore();

}

//------------------------------------------------------------------------------------


void OutlinerBase::shiftToSpreadsheet()
{
    shiftToSpreadsheetAct->setDisabled(true);
    spreadsheetMode = true;
    lastOpened = "spreadsheet";

    spreadsheet = new OutlinerSpreadsheet();
    spreadsheet->setHub(hub);


    absModel = new OutlinerAbstractModel();
    absModel->setHub(hub);
    absModel->mtoO_setDomDoc(mtoO_domDoc);
    absModel->mtoO_setNumForDoc(mtoO_numForDoc);

    absModel->resetAbsModel();


    QSortFilterProxyModel *proxyModel = new QSortFilterProxyModel;
    proxyModel->setSourceModel(absModel);
    spreadsheet->setModel(proxyModel );

    spreadsheet->setWordWrap(true);

    OutlinerItemDelegate *delegate = new OutlinerItemDelegate;
    delegate->setHub(hub);

    //spreadsheet->setItemDelegateForColumn(0, delegate); //tree
    spreadsheet->setItemDelegateForColumn(1, delegate); //syn
    spreadsheet->setItemDelegateForColumn(2, delegate); //notes











    zoneLayout->addWidget(spreadsheet);

    connect(spreadsheet, SIGNAL(columnOneResizedSignal(int)), absModel, SLOT(columnOneResizedSlot(int)));
    connect(spreadsheet, SIGNAL(columnTwoResizedSignal(int)), absModel, SLOT(columnTwoResizedSlot(int)));
    connect(spreadsheet, SIGNAL(columnOneResizedSignal(int)), absModel, SLOT(resetAbsModelColumnOne()));
    connect(spreadsheet, SIGNAL(columnTwoResizedSignal(int)), absModel, SLOT(resetAbsModelColumnTwo()));


    QTimer::singleShot(0, this, SLOT(applySpreadsheetConfig()));
    spreadsheet->reset();
    spreadsheet->expandAll();

    connect(absModel, SIGNAL(updateMainDomDocSignal(QDomDocument)), this, SIGNAL(updateMainDomDocSignal(QDomDocument)));
    connect(shrinkSpreadsheetAct, SIGNAL(triggered()), absModel, SLOT(shrinkRow()));
    connect(shrinkSpreadsheetAct, SIGNAL(triggered()), spreadsheet, SLOT(expandAll()));
    connect(expandSpreadsheetAct, SIGNAL(triggered()), absModel, SLOT(expandRow()));
    connect(expandSpreadsheetAct, SIGNAL(triggered()), spreadsheet, SLOT(expandAll()));
    connect(moveUpAct, SIGNAL(triggered()), spreadsheet, SLOT(temp_moveUp()));
    connect(moveDownAct, SIGNAL(triggered()), spreadsheet, SLOT(temp_moveDown()));
    connect(absModel, SIGNAL(applySynNoteFontConfigSignal()), this, SIGNAL(applySynNoteFontConfigSignal()));
    connect(spreadsheet, SIGNAL(otoM_actionSignal(QString,int)), this, SIGNAL(otoM_actionSignal(QString,int)));
}

//------------------------------------------------------------------------------------

void OutlinerBase::updateOutliner()
{

    if(spreadsheetMode){


    absModel->mtoO_setDomDoc(mtoO_domDoc);
    absModel->mtoO_setNumForDoc(mtoO_numForDoc);

    absModel->resetAbsModel();
    spreadsheet->expandAll();


    }
}

//------------------------------------------------------------------------------------

void OutlinerBase::moveViewTo(int hBarValue, int vBarValue)
{
    spreadsheet->verticalScrollBar()->setValue(vBarValue);
    spreadsheet->horizontalScrollBar()->setValue(hBarValue);
}


//void OutlinerBase::moveViewTo()
//{
//    moveViewTo(hBarValue,vBarValue);

//}






//------------------------------------------------------------------------------------
//-------DomDocument :-----------------------------------------------------------------
//------------------------------------------------------------------------------------
// mtoO means Main to Outliner


void OutlinerBase::mtoO_setDomDoc(QDomDocument domDoc)
{
    mtoO_domDoc = domDoc;

}
















//------------------------------------------------------------------------------------
//------- note / synopsis textDocuments :-----------------------------------------------------------------
//------------------------------------------------------------------------------------
// mtoO means Main to Outliner

void OutlinerBase::mtoO_setNumForDoc(QHash<MainTextDocument *, int> numForDoc)
{
    mtoO_numForDoc = numForDoc;
}







//------------------------------------------------------------------------------------
//-------Attendance :-----------------------------------------------------------------
//------------------------------------------------------------------------------------



void OutlinerBase::mtoO_setProjectAttendanceList(QHash<QListWidgetItem *, QDomElement> domElementForItem_, QHash<int, QDomElement> domElementForItemNumber_)
{
    attend_domElementForItem = domElementForItem_;
    attend_domElementForItemNumber = domElementForItemNumber_;


    QString string;


    //                 qDebug() << "*** domElementForItemNumber_.size() : " << string.setNum(domElementForItemNumber_.size());
    //                 qDebug() << "*** domElementForItem_.size() : " << string.setNum(domElementForItem_.size());

}


//------------------------------------------------------------------------------------

void OutlinerBase::mtoO_updateAttendances(QHash<int, QString> attendStringForNumber)
{
    QString string;


    //         qDebug() << "domElementForItemNumber.size() : " << string.setNum(domElementForItemNumber.size());

    QHash<int , QString>::const_iterator i = attendStringForNumber.constBegin();
    while (i != attendStringForNumber.constEnd()) {

        //qDebug() << "attendStringForNumber i.key() : " << string.setNum(i.key());
        //qDebug() << "attendStringForNumber i.value() : " << i.value();





        //         OutlineItem *item = areaWidget->findChild<OutlineItem *>("outlineItem_" + string.setNum(i.key()));

        //         if(item != 0)
        //         item->showSheetAttendanceList(openSheetAttendList(i.key(), i.value()));







        ++i;
    }

}







//--------------------------------------------------------------------------------------

QList<QListWidgetItem *> *OutlinerBase::sortAttendItems(QList<int> *attend, QString sorting)
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

    //    if(attend->isEmpty()){

    //        list->append(attendSeparator(tr("empty")));
    //        return list;
    //}





    //        QHashIterator<QListWidgetItem *, QDomElement> i(domElementForItem);
    //        while (i.hasNext()) {
    //            i.next();
    //            list->append(i.key());

    //    }

    for (int i = 0; i < attend->size(); ++i) {
    }

    QMap<QString, QDomElement> map;

    for (int i = 0; i < attend->size(); ++i){
        QDomElement element = attend_domElementForItemNumber.value(attend->at(i));
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

            charList.append(attend_domElementForItem.key(mapElement));

            if(mapElement.attribute("level") == "0")
                charList_level_zero.append(attend_domElementForItem.key(mapElement));

            if(mapElement.attribute("level") == "1")
                charList_level_one.append(attend_domElementForItem.key(mapElement));

            if(mapElement.attribute("level") == "2")
                charList_level_two.append(attend_domElementForItem.key(mapElement));
        }

        if(mapElement.tagName() == "item"){

            itemList.append(attend_domElementForItem.key(mapElement));

            if(mapElement.attribute("level") == "0")
                itemList_level_zero.append(attend_domElementForItem.key(mapElement));

            if(mapElement.attribute("level") == "1")
                itemList_level_one.append(attend_domElementForItem.key(mapElement));

            if(mapElement.attribute("level") == "2")
                itemList_level_two.append(attend_domElementForItem.key(mapElement));
        }

        if(mapElement.tagName() == "place"){

            placeList.append(attend_domElementForItem.key(mapElement));

            if(mapElement.attribute("level") == "0")
                placeList_level_zero.append(attend_domElementForItem.key(mapElement));

            if(mapElement.attribute("level") == "1")
                placeList_level_one.append(attend_domElementForItem.key(mapElement));

            if(mapElement.attribute("level") == "2")
                placeList_level_two.append(attend_domElementForItem.key(mapElement));
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
        list->append(attendSeparator(tr("Characters")));
    list->append(charList_level_zero);
    list->append(charList_level_one);
    list->append(charList_level_two);
    if(itemList_level_zero.size() + itemList_level_one.size() + itemList_level_two.size() != 0)
        list->append(attendSeparator(tr("Items")));
    list->append(itemList_level_zero);
    list->append(itemList_level_one);
    list->append(itemList_level_two);
    if(placeList_level_zero.size() + placeList_level_one.size() + placeList_level_two.size() != 0)
        list->append(attendSeparator(tr("Places")));
    list->append(placeList_level_zero);
    list->append(placeList_level_one);
    list->append(placeList_level_two);


    //    QString string;
    //    qDebug() << "post domElementForItem ->size() : " << string.setNum(domElementForItem.size());

    //    qDebug() << "post list->size() : " << string.setNum(list->size());



    return list;
}


//--------------------------------------------------------------------------------------

QListWidgetItem *OutlinerBase::attendSeparator(QString separatorName)
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



QList<QListWidgetItem *> *OutlinerBase::openSheetAttendList(int number ,QString attendString)
{

    //    qDebug() << "attendString : " << attendString;



    QStringList thisAttendStringList = attendString.split("-", QString::SkipEmptyParts);


    QList<int> *attendIntList = new QList<int>;

    while(!thisAttendStringList.isEmpty()){
        attendIntList->append(thisAttendStringList.takeFirst().toInt());
    }

    if(attendIntList->contains(0))
        attendIntList->removeAt(attendIntList->indexOf(0));


    //    QString string;
    //    qDebug() << "openSheetAttendList attendIntList->size() : " << string.setNum(attendIntList->size());

    QList<QListWidgetItem *> *itemList = sortAttendItems(attendIntList);

    //    qDebug() << "openSheetAttendList itemList->size() : " << string.setNum(itemList->size());

    return itemList;

}
//--------------------------------------------------------------------------------------

























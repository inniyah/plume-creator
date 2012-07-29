#include "outlinerbase.h"
#include "outlinerspreadsheet.h"

OutlinerBase::OutlinerBase(QWidget *parent) :
    QWidget(parent)
{

    setMinimumSize(500, 300);
    setWindowTitle(tr("Outliner"));
    QApplication::setActiveWindow(this);
    raise();






    QHBoxLayout *mainLayout = new QHBoxLayout();


    QWidget *mainZone = new QWidget;
    zoneLayout = new QVBoxLayout;
    mainZone->setLayout(zoneLayout);



    QToolBar *commonToolBar = new QToolBar();
    commonToolBar->setOrientation(Qt::Vertical);

    shiftToSpreadsheetAct = new QAction(QIcon(":/pics/addchar.png"),"", this);
    shiftToSpreadsheetAct->setToolTip(tr("Shift to spreadsheet mode"));
    shiftToSpreadsheetAct->setCheckable(true);
    connect(shiftToSpreadsheetAct, SIGNAL(toggled(bool)), this, SLOT(shiftToSpreadsheetSignal()));

    QAction *quitAct = new QAction(QIcon(":/pics/addplace.png"),"", this);
    quitAct->setToolTip(tr("Close the Outliner"));
    connect(quitAct, SIGNAL(triggered()), this, SLOT(close()));

    commonToolBar->addAction(shiftToSpreadsheetAct);
    commonToolBar->addSeparator();
    commonToolBar->addAction(quitAct);




    mainLayout->addWidget(commonToolBar);
    mainLayout->addWidget(mainZone);

    mainLayout->setMargin(1);
mainLayout->setSpacing(1);
mainLayout->setContentsMargins(1,1,1,1);

    setLayout(mainLayout);


    show();



    applyConfig();


    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Warning);
    msgBox.setText(tr("The Ouliner is in heavy maintenance !<br>For now, it's only an empty GUI. Please wait for a future release !"));
    msgBox.exec();


}
//------------------------------------------------------------------------------------

void OutlinerBase::saveConfig()
{
    QSettings settings;
    settings.beginGroup( "Outline");
    settings.setValue( "size",   size() );
    settings.setValue( "pos",   pos() );
    settings.endGroup();

}

//------------------------------------------------------------------------------------


void OutlinerBase::applyConfig()
{
    QSettings settings;
    settings.beginGroup( "Outline" );
    resize(settings.value( "size", QSize( 1000, 750 ) ).toSize() );
    move(settings.value( "pos" ).toPoint() );
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
    event->ignore();
    saveConfig();
 this->hide();
//propagate here
}


//------------------------------------------------------------------------------------


void OutlinerBase::shiftToSpreadsheetSignal()
{
shiftToSpreadsheetAct->setDisabled(true);

    OutlinerSpreadsheet *spreadsheet = new OutlinerSpreadsheet();
    zoneLayout->addWidget(spreadsheet);


}



















//------------------------------------------------------------------------------------
//-------DomDocument :-----------------------------------------------------------------
//------------------------------------------------------------------------------------
// mtoO means Main to Outliner


void OutlinerBase::mtoO_setDomDoc(QDomDocument domDoc)
{
    mtoO_domDoc = domDoc;
}


//------------------------------------------------------------------------------------
//------- :-----------------------------------------------------------------
//------------------------------------------------------------------------------------
// mtoO means Main to Outliner









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




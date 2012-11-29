#include <QtGui>

#include "outliner/outline.h"
#include "outliner/outlineitem.h"





// Soon, this class will be deprecated !




Outline::Outline(QWidget *parent) :
    QWidget(parent), newAttendName("*" + tr("new"))
{
    QVBoxLayout *layout = new QVBoxLayout();

    setMinimumSize(600, 600);
    setWindowTitle(tr("Outliner"));
    setAttribute(Qt::WA_DeleteOnClose);
    QApplication::setActiveWindow(this);
    this->raise();

    area = new QScrollArea;
    areaWidget = new QWidget(area);
    areaLayout = new QVBoxLayout;

    areaWidget->setMinimumSize(600,600);
    areaWidget->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Expanding );


    areaLayout->setSpacing(30);
    areaWidget->setObjectName("areaWidget");
    //   addItems();


    areaWidget->setLayout(areaLayout);
    area->setWidget(areaWidget);









    //setWindowState(Qt::WindowMaximized);


    showListsAct = new QAction(tr("Show all lists"), this);
    showListsAct->setCheckable(true);
    //   zoomInAct->setShortcuts(QKeySequence::Undo);
    //   zoomInAct->setStatusTip(tr("Undo the last operation"));
    connect(showListsAct, SIGNAL(toggled(bool)), this, SIGNAL(showListsSignal(bool)));

    showNotesAct = new QAction(tr("Show all notes"), this);
    showNotesAct->setCheckable(true);
    connect(showNotesAct, SIGNAL(toggled(bool)), this, SIGNAL(showNotesSignal(bool)));

    expandAllTextsAct = new QAction(tr("Expand all texts"), this);
    expandAllTextsAct->setCheckable(true);
    connect(expandAllTextsAct, SIGNAL(toggled(bool)), this, SIGNAL(expandAllTextsSignal(bool)));


    // sizes menu :

    QWidgetAction *listSliderAct = new QWidgetAction(this);
    QGroupBox *listSliderBox = new QGroupBox(tr("Attendances width"), this);
    QVBoxLayout *listSliderLayout = new QVBoxLayout(listSliderBox);
    listSlider = new QSlider(Qt::Horizontal, this);
    listSlider->setRange(200, 600);
    listSliderLayout->addWidget(listSlider);
    listSliderBox->setLayout(listSliderLayout);
    listSliderAct->setDefaultWidget(listSliderBox);

    QWidgetAction *synSliderAct = new QWidgetAction(this);
    QGroupBox *synSliderBox = new QGroupBox(tr("Synopses width"), this);
    QVBoxLayout *synSliderLayout = new QVBoxLayout(synSliderBox);
    synSlider = new QSlider(Qt::Horizontal, this);
    synSlider->setRange(200, 600);
    synSliderLayout->addWidget(synSlider);
    listSliderBox->setLayout(synSliderLayout);
    synSliderAct->setDefaultWidget(synSliderBox);

    QWidgetAction *noteSliderAct = new QWidgetAction(this);
    QGroupBox *noteSliderBox = new QGroupBox(tr("Notes width"), this);
    QVBoxLayout *noteSliderLayout = new QVBoxLayout(noteSliderBox);
    noteSlider = new QSlider(Qt::Horizontal, this);
    noteSlider->setRange(200, 600);
    noteSliderLayout->addWidget(noteSlider);
    noteSliderBox->setLayout(noteSliderLayout);
    noteSliderAct->setDefaultWidget(noteSliderBox);

    QMenu *sizeMenu = new QMenu(this);
    sizeMenu->addAction(listSliderAct);
    sizeMenu->addAction(synSliderAct);
    sizeMenu->addAction(noteSliderAct);


    QToolButton *sizeMenuButton = new QToolButton(this);
    sizeMenuButton->setText(tr("Size"));
    sizeMenuButton->setPopupMode(QToolButton::InstantPopup);
    sizeMenuButton->setMenu(sizeMenu);



    // fonts menu :

    QWidgetAction *synFontAct = new QWidgetAction(this);
    QGroupBox *synFontBox = new QGroupBox(tr("Synopses font"), this);
    QGridLayout *synFontLayout = new QGridLayout(synFontBox);
    synFontCombo = new QFontComboBox(this);
    synTextHeightSpin = new QSpinBox;
    synTextHeightSpin->setRange(6,30);
    QLabel *synTextIndentLabel = new QLabel(tr("Indent :"), this);
    synTextIndentSpin = new QSpinBox;
    synTextIndentSpin->setRange(0,70);
    QLabel *synTextMarginLabel = new QLabel(tr("Margin :"), this);
    synTextMarginSpin = new QSpinBox;
    synTextMarginSpin->setRange(0,70);
    synFontLayout->addWidget(synFontCombo, 0,0);
    synFontLayout->addWidget(synTextHeightSpin, 0,1);
    synFontLayout->addWidget(synTextIndentLabel, 1,0, Qt::AlignRight);
    synFontLayout->addWidget(synTextIndentSpin, 1,1);
    synFontLayout->addWidget(synTextMarginLabel, 2,0, Qt::AlignRight);
    synFontLayout->addWidget(synTextMarginSpin, 2,1);
    synFontBox->setLayout(synFontLayout);
    synFontAct->setDefaultWidget(synFontBox);

    QWidgetAction *noteFontAct = new QWidgetAction(this);
    QGroupBox *noteFontBox = new QGroupBox(tr("Notes font"), this);
    QGridLayout *noteFontLayout = new QGridLayout(noteFontBox);
    noteFontCombo = new QFontComboBox(this);
    noteTextHeightSpin = new QSpinBox;
    noteTextHeightSpin->setRange(6,30);
    QLabel *noteTextIndentLabel = new QLabel(tr("Indent :"), this);
    noteTextIndentSpin = new QSpinBox;
    noteTextIndentSpin->setRange(0,70);
    QLabel *noteTextMarginLabel = new QLabel(tr("Margin :"), this);
    noteTextMarginSpin = new QSpinBox;
    noteTextMarginSpin->setRange(0,70);
    noteFontLayout->addWidget(noteFontCombo, 0,0);
    noteFontLayout->addWidget(noteTextHeightSpin, 0,1);
    noteFontLayout->addWidget(noteTextIndentLabel, 1,0, Qt::AlignRight);
    noteFontLayout->addWidget(noteTextIndentSpin, 1,1);
    noteFontLayout->addWidget(noteTextMarginLabel, 2,0, Qt::AlignRight);
    noteFontLayout->addWidget(noteTextMarginSpin, 2,1);
    noteFontBox->setLayout(noteFontLayout);
    noteFontAct->setDefaultWidget(noteFontBox);

    QMenu *fontMenu = new QMenu(this);
    fontMenu->addAction(synFontAct);
    fontMenu->addAction(noteFontAct);


    QToolButton *fontsMenuButton = new QToolButton(this);
    fontsMenuButton->setText(tr("Fonts"));
    fontsMenuButton->setPopupMode(QToolButton::InstantPopup);
    fontsMenuButton->setMenu(fontMenu);


    // toolbar :

    QToolBar *toolBar = new QToolBar("Outliner Tools",this);
    toolBar->addAction(showListsAct);
    toolBar->addAction(showNotesAct);
    toolBar->addAction(expandAllTextsAct);

    toolBar->addWidget(sizeMenuButton);
    toolBar->addWidget(fontsMenuButton);

    layout->addWidget(toolBar);
    layout->addWidget(area);
    setLayout(layout);

    show();


//    qDebug() << "outline launched";






}



//------------------------------------------------------------------------------


//------------------------------------------------------------------------------


OutlineItem * Outline::buildItem(QTextDocument *synDocument, QTextDocument *noteDocument, QString title, int number, QString tagName)
{

    OutlineItem *item = new OutlineItem(areaWidget);
    QString string;
    item->setObjectName("outlineItem_" + string.setNum(number));
    item->setDocuments(synDocument, noteDocument);
    item->setTitle(title);



    connect(item,SIGNAL(resizingSignal()), this, SLOT(resizingSlot()));
    connect(this,SIGNAL(updateSizeSignal()), item, SLOT(updateSizeSlot()));
    connect(this,SIGNAL(showListsSignal(bool)), item, SLOT(showListSlot(bool)));
    connect(this,SIGNAL(showNotesSignal(bool)), item, SLOT(showNoteSlot(bool)));
    connect(this,SIGNAL(expandAllTextsSignal(bool)), item, SLOT(expandTexts(bool)));

item->showListSlot(showListsAct->isChecked());
item->showNoteSlot(showNotesAct->isChecked());
  item->expandTexts(expandAllTextsAct->isChecked());

    item->changeListWidth(listSlider->value());
     item->changeSynWidth(synSlider->value());
     item->changeNoteWidth(noteSlider->value());
     item->changeSynFont(synFontCombo->currentFont());
     item->changeSynTextHeight(synTextHeightSpin->value());
     item->changeSynIndent(synTextIndentSpin->value());
      item->changeSynMargin(synTextMarginSpin->value());
             item->changeNoteFont(noteFontCombo->currentFont());
     item->changeNoteTextHeight(noteTextHeightSpin->value());
             item->changeNoteIndent(noteTextIndentSpin->value());
             item->changeNoteMargin(noteTextMarginSpin->value());







    connect(listSlider,SIGNAL(valueChanged(int)), item, SLOT(changeListWidth(int)));
    connect(synSlider,SIGNAL(valueChanged(int)), item, SLOT(changeSynWidth(int)));
    connect(noteSlider,SIGNAL(valueChanged(int)), item, SLOT(changeNoteWidth(int)));
    connect(synFontCombo, SIGNAL(currentFontChanged(QFont)), item, SLOT(changeSynFont(QFont)));
    connect(synTextHeightSpin, SIGNAL(valueChanged(int)), item, SLOT(changeSynTextHeight(int)));
    connect(synTextIndentSpin, SIGNAL(valueChanged(int)), item, SLOT(changeSynIndent(int)));
    connect(synTextMarginSpin, SIGNAL(valueChanged(int)), item, SLOT(changeSynMargin(int)));
    connect(noteFontCombo, SIGNAL(currentFontChanged(QFont)), item, SLOT(changeNoteFont(QFont)));
    connect(noteTextHeightSpin, SIGNAL(valueChanged(int)), item, SLOT(changeNoteTextHeight(int)));
    connect(noteTextIndentSpin, SIGNAL(valueChanged(int)), item, SLOT(changeNoteIndent(int)));
    connect(noteTextMarginSpin, SIGNAL(valueChanged(int)), item, SLOT(changeNoteMargin(int)));
    connect(this, SIGNAL(connectUpdateTextsSignal()), item, SLOT(connectUpdateTextsSlot()));
    connect(this, SIGNAL(disconnectUpdateTextsSignal()), item, SLOT(disconnectUpdateTextsSlot()));

    connect(item,SIGNAL(newOutlineTitleSignal(QString,int)), this, SIGNAL(newOutlineTitleSignal(QString,int)));
    connect(item, SIGNAL(writeThisSignal(int)), this, SIGNAL(writeThisSignal(int)));






    if(tagName == "scene")
        areaLayout->addWidget(item,0,Qt::AlignRight);

    if(tagName == "chapter")
        areaLayout->addWidget(item,0,Qt::AlignCenter);

    if(tagName == "book")
        areaLayout->addWidget(item,0,Qt::AlignLeft);


    //   areaLayout->addStretch();

    //emit updateSizeSignal();

return item;
}

//------------------------------------------------------------------------------
void Outline::buildSeparator()
{

    QFrame *separator = new QFrame();
    separator->setFrameShape(QFrame::HLine);
    separator->setFrameShadow(QFrame::Raised);
    separator->setLineWidth(4);
    separator->setFixedWidth(400);
    areaLayout->addWidget(separator,0,Qt::AlignRight);

    //            emit updateSizeSignal();

}
void Outline::buildStretcher()
{
    QFrame *stretcher = new QFrame();
    stretcher->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);

    areaLayout->addWidget(stretcher);



    emit updateSizeSignal();
    resizingSlot();
    area->update();

 //   applyConfig();


}
//------------------------------------------------------------------------------


void Outline::setItemTitle(QString newTitle, int number)
{
    QString string;
    OutlineItem *item = areaWidget->findChild<OutlineItem *>("outlineItem_" + string.setNum(number));

    //    qDebug() << "setItemTitle on : " << "outlineItem_" << string.setNum(number);

    item->setTitle(newTitle);
}

//------------------------------------------------------------------------------


void Outline::cleanArea()
{
    QWidget *erasingWidget = new QWidget;
    QVBoxLayout *erasingLayout = new QVBoxLayout;

    QList<OutlineItem *> allOutlineItems = areaWidget->findChildren<OutlineItem *>();



    while(!allOutlineItems.isEmpty()){
//        qDebug() << "cleanArea ";

        OutlineItem *item = allOutlineItems.takeFirst();
        item->setObjectName("");
        //        allOutlineItems.first()->setParent(erasingLayout);
        erasingLayout->addWidget(item);
        item->setParent(erasingWidget);
        item->deleteLater();
    }



    QList<QFrame *> allOutlineSeparators = areaWidget->findChildren<QFrame *>();


    while(!allOutlineSeparators.isEmpty()){
        //       qDebug() << "cleanArea : " << areaLayout->children().first()->objectName();

        QFrame *item = allOutlineSeparators.takeFirst();
        item->setObjectName("");
        //        allOutlineItems.first()->setParent(erasingLayout);
        erasingLayout->addWidget(item);
        item->setParent(erasingWidget);
        item->deleteLater();
    }



    erasingWidget->setLayout(erasingLayout);
    erasingWidget->deleteLater();




    //    while(!areaLayout->children().isEmpty()){
    //        qDebug() << "cleanArea : " << areaLayout->children().first()->objectName();

    //        areaLayout->children().first()->setObjectName("");
    //        areaLayout->children().first()->setParent(erasingLayout);
    ////        if(areaLayout->children().first()->isWidgetType()){
    ////            QWidget *widget = areaLayout->children().first();
    ////            erasingLayout->addWidget(widget);
    ////        }
    //    }



    //    areaLayout = new QVBoxLayout;
    //    areaWidget->setLayout(areaLayout);
}





//------------------------------------------------------------------------------------
//-------Attendance :-----------------------------------------------------------------
//------------------------------------------------------------------------------------



void Outline::setProjectAttendanceList(QHash<QListWidgetItem *, QDomElement> domElementForItem_, QHash<int, QDomElement> domElementForItemNumber_)
{
    domElementForItem = domElementForItem_;
            domElementForItemNumber = domElementForItemNumber_;


            QString string;


//                 qDebug() << "*** domElementForItemNumber_.size() : " << string.setNum(domElementForItemNumber_.size());
//                 qDebug() << "*** domElementForItem_.size() : " << string.setNum(domElementForItem_.size());

}


//------------------------------------------------------------------------------------

void Outline::updateAttendances(QHash<int, QString> attendStringForNumber)
{
    QString string;


//         qDebug() << "domElementForItemNumber.size() : " << string.setNum(domElementForItemNumber.size());

    QHash<int , QString>::const_iterator i = attendStringForNumber.constBegin();
     while (i != attendStringForNumber.constEnd()) {

//qDebug() << "attendStringForNumber i.key() : " << string.setNum(i.key());
//qDebug() << "attendStringForNumber i.value() : " << i.value();

         OutlineItem *item = areaWidget->findChild<OutlineItem *>("outlineItem_" + string.setNum(i.key()));

         if(item != 0)
         item->showSheetAttendanceList(openSheetAttendList(i.key(), i.value()));







         ++i;
     }

}







//--------------------------------------------------------------------------------------

QList<QListWidgetItem *> *Outline::sortAttendItems(QList<int> *attend, QString sorting)
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
        QDomElement element = domElementForItemNumber.value(attend->at(i));
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

            charList.append(domElementForItem.key(mapElement));

            if(mapElement.attribute("level") == "0")
                charList_level_zero.append(domElementForItem.key(mapElement));

            if(mapElement.attribute("level") == "1")
                charList_level_one.append(domElementForItem.key(mapElement));

            if(mapElement.attribute("level") == "2")
                charList_level_two.append(domElementForItem.key(mapElement));
        }

        if(mapElement.tagName() == "item"){

            itemList.append(domElementForItem.key(mapElement));

            if(mapElement.attribute("level") == "0")
                itemList_level_zero.append(domElementForItem.key(mapElement));

            if(mapElement.attribute("level") == "1")
                itemList_level_one.append(domElementForItem.key(mapElement));

            if(mapElement.attribute("level") == "2")
                itemList_level_two.append(domElementForItem.key(mapElement));
        }

        if(mapElement.tagName() == "place"){

            placeList.append(domElementForItem.key(mapElement));

            if(mapElement.attribute("level") == "0")
                placeList_level_zero.append(domElementForItem.key(mapElement));

            if(mapElement.attribute("level") == "1")
                placeList_level_one.append(domElementForItem.key(mapElement));

            if(mapElement.attribute("level") == "2")
                placeList_level_two.append(domElementForItem.key(mapElement));
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

QListWidgetItem *Outline::attendSeparator(QString separatorName)
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



QList<QListWidgetItem *> *Outline::openSheetAttendList(int number ,QString attendString)
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























//------------------------------------------------------------------------------
void Outline::resizingSlot()
{
    //    QWidget *widget = areaWidget->findChild<QWidget *>(objectName);
    areaWidget->setMinimumSize(area->viewport()->width(), area->viewport()->height());
    areaWidget->adjustSize();

}


//------------------------------------------------------------------------------

void Outline::resizeEvent(QResizeEvent *event)
{

    event->accept();
    resizingSlot();
}







//------------------------------------------------------------------------------
//------------Apply Config------------------------------------------------------
//------------------------------------------------------------------------------

void Outline::applyConfig()
{
    QSettings settings;
    settings.beginGroup( "Outline" );
    resize(settings.value( "size", QSize( 1000, 750 ) ).toSize() );
    move(settings.value( "pos" ).toPoint() );
    bool showListsBool = settings.value("showLists", false).toBool();
    bool showNotesBool = settings.value("showNotes", false).toBool();
    bool expandAllTextsBool = settings.value("expandAllTexts", false).toBool();
    int listsWidth = settings.value("listsWidth", 250).toInt();
    int synsWidth = settings.value("synsWidth", 250).toInt();
    int notesWidth = settings.value("notesWidth", 250).toInt();
    int synTextHeight = settings.value("synTextHeight", 12).toInt();
    int synTextIndent = settings.value("synTextIndent", 30).toInt();
    int synTextMargin = settings.value("synTextMargin", 5).toInt();
    QFont synFont;
    synFont.setFamily(settings.value("synFontFamily", "Liberation Serif").toString());
    int noteTextHeight = settings.value("noteTextHeight", 12).toInt();
    int noteTextIndent = settings.value("noteTextIndent", 30).toInt();
    int noteTextMargin = settings.value("noteTextMargin", 5).toInt();
    QFont noteFont;
    noteFont.setFamily(settings.value("noteFontFamily", "Liberation Serif").toString());
    settings.endGroup();

    synTextHeightSpin->setValue(synTextHeight);
    synFontCombo->setCurrentFont(synFont);
    synTextIndentSpin->setValue(synTextIndent);
    synTextMarginSpin->setValue(synTextMargin);
    noteTextHeightSpin->setValue(noteTextHeight);
    noteFontCombo->setCurrentFont(noteFont);
    noteTextIndentSpin->setValue(noteTextIndent);
    noteTextMarginSpin->setValue(noteTextMargin);
    listSlider->setValue(listsWidth);
    synSlider->setValue(synsWidth);
    noteSlider->setValue(notesWidth);
    showListsAct->setChecked(showListsBool);
    showNotesAct->setChecked(showNotesBool);
    expandAllTextsAct->setChecked(expandAllTextsBool);

    resizingSlot();

}

//------------------------------------------------------------------------------------

void Outline::saveConfig()
{
    QSettings settings;
    settings.beginGroup( "Outline");
    settings.setValue( "size", size() );
    settings.setValue( "pos", pos() );
    settings.setValue("showLists", showListsAct->isChecked());
    settings.setValue("showNotes", showNotesAct->isChecked());
    settings.setValue("expandAllTexts", expandAllTextsAct->isChecked());
    settings.setValue("listsWidth", listSlider->value());
    settings.setValue("synsWidth", synSlider->value());
    settings.setValue("notesWidth", noteSlider->value());
    settings.setValue("synTextHeight", synTextHeightSpin->value());
    settings.setValue("synTextIndent", synTextIndentSpin->value());
    settings.setValue("synTextMargin", synTextMarginSpin->value());
    settings.setValue("synFontFamily", synFontCombo->currentFont());
    settings.setValue("noteTextHeight", noteTextHeightSpin->value());
    settings.setValue("noteTextIndent", noteTextIndentSpin->value());
    settings.setValue("noteTextMargin", noteTextMarginSpin->value());
    settings.setValue("noteFontFamily", noteFontCombo->currentFont());
    settings.endGroup();



}

//------------------------------------------------------------------------------------
void Outline::setOutlinerViewportPos(int number)
{
    QString string;
    QWidget *widget = areaWidget->findChild<QWidget *>("outlineItem_" + string.setNum(number));
    area->ensureWidgetVisible(widget, 0, 200);

//    qDebug() << "setOutlinerViewportPos : " << widget->objectName();
//    QString debug;
//    qDebug() << "setOutlinerViewportPos x : " << debug.setNum(widget->mapToParent(QPoint(0,0)).x());
//    qDebug() << "setOutlinerViewportPos y : " << debug.setNum(widget->mapToParent(QPoint(0,0)).y());
//    qDebug() << "setOutlinerViewportPos areaWidget w : " << debug.setNum(areaWidget->width());
//    qDebug() << "setOutlinerViewportPos areaWidget h : " << debug.setNum(areaWidget->height());
//    qDebug() << "setOutlinerViewportPos parent : " << widget->parentWidget()->objectName();


}
//------------------------------------------------------------------------------------
void Outline::insertItemInOutliner(QWidget *newWidget, int number, QString tagName)
{
    QString string;
    QWidget *refWidget = areaWidget->findChild<QWidget *>("outlineItem_" + string.setNum(number + 1));

//    qDebug() << "insertItemInOutliner  : " << refWidget->objectName();


    if(tagName == "scene")
    areaLayout->insertWidget(areaLayout->indexOf(refWidget),newWidget,0,Qt::AlignRight);

    if(tagName == "chapter")
    areaLayout->insertWidget(areaLayout->indexOf(refWidget),newWidget,0,Qt::AlignCenter);

    if(tagName == "book")
    areaLayout->insertWidget(areaLayout->indexOf(refWidget),newWidget,0,Qt::AlignLeft);


    emit updateSizeSignal();
    resizingSlot();
    area->update();


}

//------------------------------------------------------------------------------------


void Outline::closeEvent(QCloseEvent* event)
{

    saveConfig();
    //    qDebug() << "Outline closeEvent";

    emit outlinerClosedSignal();

    event->accept();
}



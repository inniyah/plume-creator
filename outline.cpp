#include <QtGui>

#include "outline.h"
#include "outlineitem.h"

Outline::Outline(QWidget *parent) :
    QWidget(parent)
{

    QVBoxLayout *layout = new QVBoxLayout();

    setMinimumSize(600, 600);
    setWindowTitle(tr("Outliner"));
    setWindowFlags(Qt::Window);



    areaWidget = new QWidget;
    areaLayout = new QVBoxLayout;
    area = new QScrollArea;

    areaWidget->setMinimumSize(600,600);
    areaWidget->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding );


    areaLayout->setSpacing(30);

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


    QToolBar *toolBar = new QToolBar("Outliner Tools",this);
    toolBar->addAction(showListsAct);
    toolBar->addAction(showNotesAct);
    toolBar->addAction(expandAllTextsAct);

    layout->addWidget(toolBar);
    layout->addWidget(area);
    setLayout(layout);

    show();


    qDebug() << "outline launched";
}



//------------------------------------------------------------------------------


//------------------------------------------------------------------------------


void Outline::buildItem(QTextDocument *synDocument, QTextDocument *noteDocument, QString title, int number, QString tagName)
{

    OutlineItem *item = new OutlineItem(areaWidget);
    QString string;
    item->setObjectName("outlineItem_" + string.setNum(number));
    item->setDocuments(synDocument, noteDocument);
    item->setTitle(title);

    connect(this,SIGNAL(showListsSignal(bool)), item, SLOT(showListSlot(bool)));
    connect(this,SIGNAL(showNotesSignal(bool)), item, SLOT(showNoteSlot(bool)));
    connect(this,SIGNAL(expandAllTextsSignal(bool)), item, SLOT(expandTexts(bool)));
    connect(item,SIGNAL(resizingSignal()), this, SLOT(resizingSlot()));
    connect(this,SIGNAL(updateSizeSignal()), item, SLOT(updateSizeSlot()));

    connect(item,SIGNAL(newOutlineTitleSignal(QString,int)), this, SIGNAL(newOutlineTitleSignal(QString,int)));

    if(tagName == "scene")
        areaLayout->addWidget(item,0,Qt::AlignRight);

    if(tagName == "chapter")
        areaLayout->addWidget(item,0,Qt::AlignCenter);

    if(tagName == "book")
        areaLayout->addWidget(item,0,Qt::AlignLeft);


    //   areaLayout->addStretch();

    //emit updateSizeSignal();


}

//------------------------------------------------------------------------------
void Outline::buildSeparator()
{

    QFrame *separator = new QFrame();
    separator->setFrameShape(QFrame::HLine);
    separator->setFrameShadow(QFrame::Raised);
    separator->setLineWidth(4);
    separator->setFixedWidth(100);
    areaLayout->addWidget(separator,0,Qt::AlignRight);

    //            emit updateSizeSignal();

}
void Outline::buildStretcher()
{
    //  areaLayout->addStretch();
    QFrame *stretcher = new QFrame();
    stretcher->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);

    areaLayout->addWidget(stretcher);

    applyConfig();

    emit updateSizeSignal();
    resizingSlot();
    area->update();

}
//------------------------------------------------------------------------------


void Outline::setItemTitle(QString newTitle, int number)
{
    QString string;
    OutlineItem *item = areaWidget->findChild<OutlineItem *>("outlineItem_" + string.setNum(number));

    qDebug() << "setItemTitle on : " << "outlineItem_" << string.setNum(number);

    item->setTitle(newTitle);
}

//------------------------------------------------------------------------------


void Outline::cleanArea()
{
    QWidget *erasingWidget = new QWidget;
    QVBoxLayout *erasingLayout = new QVBoxLayout;

    QList<OutlineItem *> allOutlineItems = areaWidget->findChildren<OutlineItem *>();



    while(!allOutlineItems.isEmpty()){
        //       qDebug() << "cleanArea : " << areaLayout->children().first()->objectName();

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

//------------------------------------------------------------------------------
void Outline::resizingSlot()
{
    //    QWidget *widget = areaWidget->findChild<QWidget *>(objectName);
    areaWidget->adjustSize();

}


//------------------------------------------------------------------------------

void Outline::resizeEvent(QResizeEvent *event)
{
    if(area->viewport()->width() > 600 && area->viewport()->height() > 600)
        areaWidget->setMinimumSize(area->viewport()->width(), area->viewport()->height());
    areaWidget->adjustSize();

    QString debug;
    qDebug() << "w : " << debug.setNum(areaWidget->size().width());
    qDebug() << "h : " << debug.setNum(areaWidget->size().height());
}







//------------------------------------------------------------------------------
//------------Apply Config------------------------------------------------------
//------------------------------------------------------------------------------

void Outline::applyConfig()
{
    QSettings settings;
    settings.beginGroup( "Outline" );
    bool showListsBool = settings.value("showLists", false).toBool();
    bool showNotesBool = settings.value("showNotes", false).toBool();
    bool expandAllTextsBool = settings.value("expandAllTexts", false).toBool();
    settings.endGroup();


    showListsAct->setChecked(showListsBool);
    showNotesAct->setChecked(showNotesBool);
    expandAllTextsAct->setChecked(expandAllTextsBool);


}
void Outline::closeEvent(QCloseEvent* event)
{

    QSettings settings;
    settings.beginGroup( "Outline");
    settings.setValue("showLists", showListsAct->isChecked());
    settings.setValue("showNotes", showNotesAct->isChecked());
    settings.setValue("expandAllTexts", expandAllTextsAct->isChecked());
    settings.endGroup();


    qDebug() << "Outline closeEvent";



    event->accept();
}

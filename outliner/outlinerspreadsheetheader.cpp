#include <QtGui>

#include "outlinerspreadsheetheader.h"
#include "outliner/outlinerspreadsheetheadersection.h"

OutlinerSpreadsheetHeader::OutlinerSpreadsheetHeader(Qt::Orientation orientation, QWidget *parent) :
    QHeaderView(Qt::Horizontal, parent)
{


    this->setMinimumSectionSize(40);
    this->setDefaultSectionSize(50);
    this->setMovable(true);
    this->setClickable(true);
    this->setStretchLastSection(true);
    this->setResizeMode(QHeaderView::Interactive);


}

//--------------------------------------------------------------------------------

void OutlinerSpreadsheetHeader::contextMenuEvent(QContextMenuEvent *event)
{
    int clickedSectionIndex = this->logicalIndexAt(event->pos());
    QString clickedSectionData = this->model()->headerData(clickedSectionIndex, Qt::Horizontal).toString();

    OutlinerSpreadsheetHeaderSection *clickedSection = new OutlinerSpreadsheetHeaderSection();
    clickedSection->setId(clickedSectionIndex);

    QAction *clickedSectionAction = new QAction(tr("Hide ") + clickedSectionData + tr(""), this);
    connect(clickedSectionAction, SIGNAL(triggered()), clickedSection, SLOT(hideThisSection()));
    connect(clickedSection, SIGNAL(hideOrShowSectionSignal(int,bool)), this, SLOT(hideOrShowSectionSlot(int,bool)));



    QMenu menu(this);
    if(clickedSectionIndex != 0) // we can't hide the title column
        menu.addAction(clickedSectionAction);

    menu.addSeparator();

    int count = this->model()->columnCount();

    for(int i = 1; i < count ; i++){
        if(i == clickedSectionIndex)
            continue;

        QString otherSectionData = this->model()->headerData(i, Qt::Horizontal).toString();
        OutlinerSpreadsheetHeaderSection *otherSection = new OutlinerSpreadsheetHeaderSection();
        otherSection->setId(i);
        QAction *otherSectionAction = new QAction(otherSectionData, this);
        otherSectionAction->setCheckable(true);

        if(this->isSectionHidden(i))
            otherSectionAction->setChecked(false);
        else
            otherSectionAction->setChecked(true);

        connect(otherSectionAction, SIGNAL(toggled(bool)), otherSection, SLOT(hideOrShowThisSection(bool)));
        connect(otherSection, SIGNAL(hideOrShowSectionSignal(int,bool)), this, SLOT(hideOrShowSectionSlot(int,bool)));

        menu.addAction(otherSectionAction);

    }


    menu.exec(event->globalPos());
}


//---------------------------------------------------------

void OutlinerSpreadsheetHeader::hideOrShowSectionSlot(int logicIndex, bool showSectionBool)
{
    if(showSectionBool == false)
        this->hideSection(logicIndex);
    else
        this->showSection(logicIndex);

    if(logicIndex == 0)
        return;

    int previousVisualSection = this->visualIndex(logicIndex) - 1;
    int previousSection = logicalIndex(previousVisualSection);

    int visibleCount = this->count() - this->hiddenSectionCount();

    if(previousSection == 0 && visibleCount < 2)
        this->resizeSection(previousSection, this->sectionSize(previousSection) / 4);
    else
        this->resizeSection(previousSection, this->sectionSize(previousSection) / 2);

    if(this->sectionSize(previousSection) < 50) // set minimum limit
        this->resizeSection(previousSection, 100);

    if(visibleCount < 4 && this->viewport()->width() > 600){
        for(int i = 0 ; i < visibleCount; i++)
        this->resizeSection(logicalIndex(i), this->viewport()->width() /4);
    }
}

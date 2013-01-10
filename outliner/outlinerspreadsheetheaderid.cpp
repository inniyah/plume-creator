#include "outliner/outlinerspreadsheetheaderid.h"

OutlinerSpreadsheetHeaderId::OutlinerSpreadsheetHeaderId(QObject *parent) :
    QObject(parent)
{
}

void OutlinerSpreadsheetHeaderId::setId(int id)
{
    sectionId = id;
}

int OutlinerSpreadsheetHeaderId::id()
{
    return sectionId;
}

void OutlinerSpreadsheetHeaderId::hideOrShowSection(bool hideSectionBool)
{
    emit hideOrShowSectionSignal(sectionId, hideSectionBool);
}

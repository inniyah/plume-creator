#include "outliner/outlinerspreadsheetheadersection.h"

OutlinerSpreadsheetHeaderSection::OutlinerSpreadsheetHeaderSection(QObject *parent) :
    QObject(parent)
{
}

void OutlinerSpreadsheetHeaderSection::setId(int id)
{
    sectionId = id;
}

int OutlinerSpreadsheetHeaderSection::id()
{
    return sectionId;
}

void OutlinerSpreadsheetHeaderSection::hideOrShowThisSection(bool showSectionBool)
{
    emit hideOrShowSectionSignal(sectionId, showSectionBool);
}

void OutlinerSpreadsheetHeaderSection::hideThisSection()
{
    emit hideOrShowSectionSignal(sectionId, false);
}

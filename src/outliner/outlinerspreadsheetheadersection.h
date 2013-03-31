#ifndef OUTLINERSPREADSHEETHEADERID_H
#define OUTLINERSPREADSHEETHEADERID_H

#include <QObject>

#include "hub.h"


class OutlinerSpreadsheetHeaderSection : public QObject
{
    Q_OBJECT
public:
    explicit OutlinerSpreadsheetHeaderSection(QObject *parent = 0);
    
signals:
    void hideOrShowSectionSignal(int id, bool hideSectionBool);

public slots:
    void setHub(Hub *varHub){hub = varHub;}
    void hideOrShowThisSection(bool hideSectionBool);
    void hideThisSection();
    int id();
    void setId(int id);

private:
    Hub *hub;
int sectionId;
};

#endif // OUTLINERSPREADSHEETHEADERID_H

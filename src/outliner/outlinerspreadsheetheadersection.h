#ifndef OUTLINERSPREADSHEETHEADERID_H
#define OUTLINERSPREADSHEETHEADERID_H

#include <QObject>

class OutlinerSpreadsheetHeaderSection : public QObject
{
    Q_OBJECT
public:
    explicit OutlinerSpreadsheetHeaderSection(QObject *parent = 0);
    
signals:
    void hideOrShowSectionSignal(int id, bool hideSectionBool);

public slots:
    void hideOrShowThisSection(bool hideSectionBool);
    void hideThisSection();
    int id();
    void setId(int id);

private:
int sectionId;
};

#endif // OUTLINERSPREADSHEETHEADERID_H

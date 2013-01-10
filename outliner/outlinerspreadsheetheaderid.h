#ifndef OUTLINERSPREADSHEETHEADERID_H
#define OUTLINERSPREADSHEETHEADERID_H

#include <QObject>

class OutlinerSpreadsheetHeaderId : public QObject
{
    Q_OBJECT
public:
    explicit OutlinerSpreadsheetHeaderId(QObject *parent = 0);
    
signals:
    void hideOrShowSectionSignal(int id, bool hideSectionBool);

public slots:
    void hideOrShowSection(bool hideSectionBool);
    int id();
    void setId(int id);

private:
int sectionId;
};

#endif // OUTLINERSPREADSHEETHEADERID_H

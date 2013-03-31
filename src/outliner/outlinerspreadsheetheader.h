#ifndef OUTLINERSPREADSHEETHEADER_H
#define OUTLINERSPREADSHEETHEADER_H

#include <QHeaderView>

#include "hub.h"

class OutlinerSpreadsheetHeader : public QHeaderView
{
    Q_OBJECT
public:
    explicit OutlinerSpreadsheetHeader(Qt::Orientation orientation, QWidget * parent = 0);
    
protected:
    void contextMenuEvent(QContextMenuEvent *event);

signals:
    
public slots:
    void setHub(Hub *varHub){hub = varHub;}

private slots:
    void hideOrShowSectionSlot(int logicalIndex, bool showSectionBool);

private:
    Hub *hub;

};

#endif // OUTLINERSPREADSHEETHEADER_H

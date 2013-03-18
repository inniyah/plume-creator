#ifndef OUTLINERSPREADSHEETHEADER_H
#define OUTLINERSPREADSHEETHEADER_H

#include <QHeaderView>

class OutlinerSpreadsheetHeader : public QHeaderView
{
    Q_OBJECT
public:
    explicit OutlinerSpreadsheetHeader(Qt::Orientation orientation, QWidget * parent = 0);
    
protected:
    void contextMenuEvent(QContextMenuEvent *event);

signals:
    
public slots:
    
private slots:
    void hideOrShowSectionSlot(int logicalIndex, bool showSectionBool);
};

#endif // OUTLINERSPREADSHEETHEADER_H

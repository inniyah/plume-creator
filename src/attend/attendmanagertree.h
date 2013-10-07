#ifndef ATTENDMANAGERTREE_H
#define ATTENDMANAGERTREE_H

#include <QTreeView>
#include "hub.h"
#include "attend/attendabstractmodel.h"

class AttendManagerTree : public QTreeView
{
    Q_OBJECT
public:
    explicit AttendManagerTree(QWidget *parent = 0);
    void postConstructor();

protected:
    void selectionChanged(const QItemSelection & selected, const QItemSelection & deselected);
 void dragEnterEvent(QDragEnterEvent *event);
 void dragLeaveEvent(QDragLeaveEvent *event);
 void dropEvent(QDropEvent *event);
 void contextMenuEvent(QContextMenuEvent *event);

signals:
 void modifyFlagsForDropsSignal(QString type);
 void textAndNoteSignal(int number, QString action);
 void currentOpenedSheetSignal(int sheetIdNumber);

public slots:
    void setHub(Hub *varHub){hub = varHub;}
    void setAttendAbstractModel(AttendAbstractModel *tree){absTreeModel = tree;}
    void applySettingsFromData();

private slots:
    void itemCollapsedSlot(QModelIndex index);
    void itemExpandedSlot(QModelIndex index);
    void itemClicked(QModelIndex index);

private:
    Hub *hub;
   AttendAbstractModel  *absTreeModel;

    QList<QModelIndex> allIndexesFromModel(QAbstractItemModel *model, const QModelIndex &parent);
QModelIndexList m_selectedIndexes;
QModelIndex draggedIndex;

// clicks :
bool oneClickCheckpoint, twoClicksCheckpoint, threeClicksCheckpoint;
QModelIndex oldIndex;


};

#endif // ATTENDMANAGERTREE_H

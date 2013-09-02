#ifndef EXPORTERTREE_H
#define EXPORTERTREE_H

#include <QTreeView>
#include "hub.h"
#include "mainTree/maintreeabstractmodel.h"


class ExporterTree : public QTreeView
{
    Q_OBJECT
public:
    explicit ExporterTree(QWidget *parent = 0);
    void postConstructor();


    void clickCheckBox(QModelIndex index);
protected:

//    void mousePressEvent(QMouseEvent *event);


signals:
    void clickCheckBoxSignal(QModelIndex index);

public slots:
    void setHub(Hub *varHub){hub = varHub;}
    void setMainTreeAbstractModel(MainTreeAbstractModel *tree){absTreeModel = tree;}
    void applySettingsFromData();

private slots:
    void itemCollapsedSlot(QModelIndex index);
    void itemExpandedSlot(QModelIndex index);

private:
    Hub *hub;
    MainTreeAbstractModel  *absTreeModel;

    QList<QModelIndex> allIndexesFromModel(QAbstractItemModel *model, const QModelIndex &parent);
QModelIndexList m_selectedIndexes;


};

#endif // EXPORTERTREE_H

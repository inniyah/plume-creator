#ifndef OUTLINERABSTRACTMODEL_H
#define OUTLINERABSTRACTMODEL_H

#include <QAbstractTableModel>
#include <QDomDocument>
#include <QStringList>
#include <QObject>
#include <QTextDocument>

#include "outlinertreeitem.h"

class OutlinerAbstractModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit OutlinerAbstractModel(QObject *parent = 0);
~OutlinerAbstractModel();

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
        int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const;

    Qt::ItemFlags flags(const QModelIndex &index) const;
    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole);
QModelIndex index(int, int, const QModelIndex&) const;
QModelIndex parent(const QModelIndex&) const;
signals:

    void updateMainDomDocSignal(QDomDocument otoM_domDoc);
void applySynNoteFontConfigSignal();

public slots:

// set the size hint :
void shrinkRow(){itemHeight += 20; reset();}
void expandRow(){itemHeight -= 20; reset();}
void setRowHeight(int height){itemHeight = height; reset();}

    void resetAbsModel();


    void mtoO_setDomDoc(QDomDocument domDoc);
    void resetDomDoc();
    void parseFolderElement(const QDomElement &element);

    void mtoO_setNumForDoc(QHash<QTextDocument *, int> numForDoc);
void reset_mtoO_setNumForDoc();


private slots:

void updateMainDomDoc();
void updateMainTextDoc(QTextDocument *textDoc, int number);


private:
OutlinerTreeItem *rootItem;
int itemHeight;

    QDomDocument mtoO_domDoc;
    QHash<int, QDomElement> domElementForNumber;
    QDomElement root;
    int numberOfDomElements, numberOfBooks, numberOfChapters, numberOfScenes;
    QStringList *titlesList;
    QStringList *leftHeadersList;
    QList<int> *numberList;
    QHash<QTextDocument *, int> mtoO_numForDoc;
    QHash<QTextDocument *, int> mtoO_numForClonedDoc;

QList<OutlinerTreeItem *> *treeBookItemList;
QList<OutlinerTreeItem *> *treeChapterItemList;
};

#endif // OUTLINERABSTRACTMODEL_H

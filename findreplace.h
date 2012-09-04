#ifndef FINDREPLACE_H
#define FINDREPLACE_H

#include <QDialog>
#include <QtGui>
#include <QDomDocument>

namespace Ui {
class FindReplace;
}

class FindReplace : public QDialog
{
    Q_OBJECT
    
public:
    explicit FindReplace(QFile *device = 0, QWidget *parent = 0);
    ~FindReplace();
    
signals:

public slots:

private slots:
    //tree :
    void createTree(QFile *device);
    bool read(QFile *file);
    void closeTree();
    void buildTree();
    void itemClickedSlot(QTreeWidgetItem* item, int column);




private:
    Ui::FindReplace *ui;



    QFile *targetDevice;

    //tree :

    void parseFolderElement(const QDomElement &element,
                            QTreeWidgetItem *parentItem = 0);
    QTreeWidgetItem *createItem(const QDomElement &element,
                                QTreeWidgetItem *parentItem = 0);

    QDomDocument domDocument;
    QDomElement root;
    QHash<QTreeWidgetItem *, QDomElement> domElementForItem;
    QHash<QTreeWidgetItem *, QDomElement>::iterator h;
    QHash<int, QDomElement> domElementForNumber;
    QHash<int, QDomElement>::iterator t;
    QIcon folderIcon;
    QIcon sceneIcon;


    // accept :
    QTextDocument * buildFinalDoc();
//    QTextDocument* prepareTextDoc(QFile *textFile);
//    QTextDocument* prepareSynDoc(QFile *synFile);
//    QTextDocument* prepareNoteDoc(QFile *noteFile);

};

#endif // FINDREPLACE_H

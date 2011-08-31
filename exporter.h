#ifndef EXPORTER_H
#define EXPORTER_H

#include <QtGui>
#include <QDomDocument>

class Exporter : public QDialog
{
    Q_OBJECT
public:
    explicit Exporter(QFile *device = 0, QWidget *parent = 0);

protected:
    void accept();

signals:

public slots:

private slots:
    void setExistingDirectory();

    //tree :
    void createTree(QFile *device);
    bool read(QFile *device);
    void closeTree();
    void buildTree();
    void itemClickedSlot(QTreeWidgetItem* item, int column);

private:

    QLineEdit *directoryLabelLineEdit;
    QLineEdit *projectNameLabelLineEdit;
    QDialogButtonBox *buttonBox;
    QTreeWidget *tree;
    QCheckBox *textCheckBox,*synCheckBox,*noteCheckBox;
    QComboBox *fileTypeCombo;
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

    void exportDoc();
QTextDocument* prepareTextDoc(QFile *textFile);
QTextDocument* prepareSynDoc(QFile *synFile);
QTextDocument* prepareNoteDoc(QFile *noteFile);

};

#endif // EXPORTER_H

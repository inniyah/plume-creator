/***************************************************************************
 *   Copyright (C) 2011 by Cyril Jacquet                                   *
 *   cyril.jacquet@plume-creator.eu                                                 *
 *                                                                         *
 *  This file is part of Plume Creator.                                    *
 *                                                                         *
 *  Plume Creator is free software: you can redistribute it and/or modify  *
 *  it under the terms of the GNU General Public License as published by   *
 *  the Free Software Foundation, either version 3 of the License, or      *
 *  (at your option) any later version.                                    *
 *                                                                         *
 *  Plume Creator is distributed in the hope that it will be useful,       *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *  GNU General Public License for more details.                           *
 *                                                                         *
 *  You should have received a copy of the GNU General Public License      *
 *  along with Plume Creator.  If not, see <http://www.gnu.org/licenses/>. *
 ***************************************************************************/
#ifndef EXPORTER_H
#define EXPORTER_H

#if QT_VERSION >= 0x050000
#include <QtWidgets>
#endif 
#include <QtGui>   
#include <QDomDocument>
#include <QPrinter>
#if QT_VERSION >= 0x050000
#include <QtPrintSupport/QPrinter>
#include <QtPrintSupport/QPrintDialog>
#include <QtPrintSupport/QPrintPreviewDialog>
#endif

#include "hub.h"
#include "mainTree/maintreeabstractmodel.h"
#include "exporter/exportertreeproxy.h"


namespace Ui {
class Exporter;
}

class Exporter : public QDialog
{
    Q_OBJECT
public:
    explicit Exporter(QString mode = "export", QWidget *parent = 0);
    void postConstructor();
    ~Exporter();

protected:
    void accept();

signals:

public slots:
    void setHub(Hub *varHub){hub = varHub;}
    void setMainTreeAbstractModel(MainTreeAbstractModel *tree){absTreeModel = tree;}

private slots:
    void setExistingDirectory();
    void seePreview();
    void closePreview(){previewDialog->close();}
    //tree :
    void createTree();
    QList<QDomElement> searchForCheckedItems(QDomElement element);
    void closeTree();
    void exportInCSV();
    void exportInPDF();


    void print();
    void previewPrint(QPrinter *printer);

    void on_fileTypeComboBox_currentIndexChanged(int index);

private:
    void applyConfig();
    void saveConfig();

    Hub *hub;
    MainTreeAbstractModel  *absTreeModel;
    ExporterTreeProxy *proxy;
    Ui::Exporter *ui;


    QString dialogMode;


    QDialog *previewDialog;

    //tree :


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
    void exportDoc();
    QTextDocument* prepareTextDoc(QTextDocument *textDoc);
    QTextDocument* prepareSynDoc(QTextDocument *synDoc);
    QTextDocument* prepareNoteDoc(QTextDocument *noteDoc);
    QString format;

};

#endif // EXPORTER_H

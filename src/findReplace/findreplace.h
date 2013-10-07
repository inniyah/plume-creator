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
#ifndef FINDREPLACE_H
#define FINDREPLACE_H

#if QT_VERSION >= 0x050000
#include <QtWidgets>
#endif 
#include <QtGui>   
#include <QDialog>
#include <QDomDocument>

#include "hub.h"
#include "mainTree/maintreeabstractmodel.h"
#include "common/trees/checkabletreeproxy.h"
#include "maintextdocument.h"
#include "textzone.h"

namespace Ui {
class FindReplace;
}

class FindReplace : public QDialog
{
    Q_OBJECT
    
public:
    explicit FindReplace(QWidget *parent = 0);
    void postConstructor();
    ~FindReplace();
    

protected:
    void keyPressEvent(QKeyEvent *event);


signals:
void currentOpenedSheetSignal(int id);

public slots:
    void setHub(Hub *varHub){hub = varHub;}
    void setMainTreeAbstractModel(MainTreeAbstractModel *tree){absTreeModel = tree;}
    void setTextStyles(TextStyles *styles){textStyles = styles;}

private slots:
    //tree :
    void createTree();

    QList<QDomElement> searchForCheckedItems(QDomElement element);
    QList<int> findTextPos(QString text, MainTextDocument *document);

    void cloneAllDocs();
    void applyConfig();
    void saveConfig();
    void openText(int id);
    void closeText(int id);
    void resetSearch();

    void accept();
    void reject();

    void on_findButton_clicked();

    void on_replaceDocButton_clicked();

    void on_replaceAllButton_clicked();

    void on_prevDocButton_clicked();

    void on_nextDocButton_clicked();

    void on_prevButton_clicked();

    void on_nextButton_clicked();

    void on_replaceAndNextButton_clicked();

    void on_caseSensitiveCheckBox_toggled(bool checked);

    void on_textCheckBox_toggled(bool checked);

    void on_synCheckBox_toggled(bool checked);

    void on_noteCheckBox_toggled(bool checked);

    void on_findEdit_textChanged(const QString &arg1);

    void on_findEdit_editingFinished();

private:
    Hub *hub;
    Ui::FindReplace *ui;
    MainTreeAbstractModel  *absTreeModel;
    TextStyles *textStyles;
    CheckableTreeProxy *proxy;

    bool caseSensitive;
    QTextDocument::FindFlags findFlags;
QHash<QString, QList<int> > listOfPositionsForDocObjectName;

    QDomDocument domDocument;
    QDomElement root;
QString currentWordToFind, currentSheetObjectName;

    int progressTotal, docProgressTotal, currentTextOpened;
bool m_textCheckBox, m_synCheckBox, m_noteCheckBox;

QList<QString> objectNamesListOfFoundDocs;

bool allowRestartFromBeginning, allowRestartFromEnd, startingOrClosing;
int oldPosition;
QList<int> positionsList;

bool configApplying;

};

#endif // FINDREPLACE_H

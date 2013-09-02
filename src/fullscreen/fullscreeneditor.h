/***************************************************************************
 *   Copyright (C) 2012 by Cyril Jacquet                                   *
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
#ifndef FULLSCREENEDITOR_H
#define FULLSCREENEDITOR_H

#if QT_VERSION >= 0x050000
#include <QtWidgets>
#endif 
#include <QtGui>   
#include <QLabel>
#include <QTextDocument>
#include <QPushButton>
#include <QSettings>

#include "hub.h"
#include "textstyles.h"
#include "editmenu.h"
#include "wordgoalprogressbar.h"
#include "fulltextzone.h"
#include "dockedTree/dockedtreeproxy.h"
#include "mainTree/maintreeabstractmodel.h"

namespace Ui {
class FullscreenEditor;
}

class FullscreenEditor : public QWidget
{
    Q_OBJECT

public:
    explicit FullscreenEditor(QWidget *parent = 0);
    ~FullscreenEditor();
    void postConstructor();

protected:
    void closeEvent(QCloseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);

signals:
    void closeSignal();
    void manageStylesSignal();
    void newSheetSignal(int number);
    void openSheetOnMainWindow(int sheetNumber = 0, int textCursorPos = 0, int noteCursorPos = 0, int synCursorPos = 0);

public slots:
    void setHub(Hub *varHub){hub = varHub;}
    void setMainTreeAbstractModel(MainTreeAbstractModel *tree){absTreeModel = tree;}

    void setTimer(QString);
    void applyConfig();
    void saveConfig();
  void setTextStyles(TextStyles *styles){textStyles = styles;}
    void resetFullscreenTextWidthSlot();
    void openBySheetNumber(int number);

private slots:

    void createNotesMenu();
    void createOptionMenu();

    void setWordCount(QString type, int id, int count);

    void callColorDialog();
    void setBaseStyleSheet();
    void setBackColorDialog();
    void setBackColor();
    void setTextBackColorDialog();
    void setTextBackColor();
    void setTextColorDialog();
    void setTextColor();
    void setAddOnColorDialog();
    void setAddOnColor();
    void hideMouse();
    void applyStyleSheet();

    void showSyn();
    void showNote();

    void loadTextWidthSliderValue();
    void sliderTextWidthValueChanged(int sliderValue);
    void setWidth();
    void setZoom();

    void zoomIn();
    void zoomOut();

    void cursorPositionChangedSlot();   
    void changeTextStyleSlot(int styleIndex);
    void noteCursorPositionChangedSlot(int position);
    void synCursorPositionChangedSlot(int position);


    void restoreDoc();
    void wordCountChangedSlot(QString type, int id, int count);

    void on_newButton_clicked();
    void on_nextButton_clicked();
    void on_prevButton_clicked();


    void on_treeButton_toggled(bool checked);
    void setTreeViewVisible(bool isVisible);
    void createTreeView();

private:
    void setText(MainTextDocument *doc);
    void setSyn(MainTextDocument *synDocument);
    void setNote(MainTextDocument *noteDocument);
    void setTextCursorPos(int pos);
    void setSynCursorPos(int pos);
    void setNoteCursorPos(int pos);
    void resetNavigatorTree();
    Hub *hub;
    MainTreeAbstractModel  *absTreeModel;
    Ui::FullscreenEditor *ui;
    MainTextDocument *clonedDoc, *clonedSynDoc, *clonedNoteDoc;

    TextStyles *textStyles;
    EditMenu *editWidget;
    int sliderCurrentValue;
    MainTextDocument *originalDoc, *originalSynDoc, *originalNoteDoc;


    QPushButton *backColorButton,
    *textBackColorButton,
    *textColorButton,
    *addOnColorButton;

    QSettings settings;

    int synCursorPos;
    int noteCursorPos;
    //QTextDocument *synDoc;
    //QTextDocument *noteDoc;
FullTextZone *fullSynEdit, *fullNoteEdit;
QWidget *synWidget,*noteWidget;

    QTimer *mouseTimer;

    bool firstLaunch;
int currentCursorPos, currentNoteCursoPos, currentSynCursoPos;

    // style sheets
    QString backColorString,
    textBackColorString,
    textColorString,
    addOnColorString;

    int baseWordCount;


    // navigator :
    QString treeString;
    QHash<int, QDomElement> domElementForNumber;
    QHash<int, QString> nameForNumber;
    QHash<int, int> indexForNumber;
    int numberOfCurrentFullscreenSheet;


};

#endif // FULLSCREENEDITOR_H

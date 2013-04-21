/***************************************************************************
 *   Copyright (C) 2012 by Cyril Jacquet                                   *
 *   terreville@gmail.com                                                 *
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
    void openBySheetNumber(int number);

protected:
    void closeEvent(QCloseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);

signals:
    void closeSignal();
    void manageStylesSignal();
    void newSheetSignal(int number);

public slots:
    void setHub(Hub *varHub){hub = varHub;}
    void setTimer(QString);
    void applyConfig();
  void setTextStyles(TextStyles *styles){textStyles = styles;}
    void resetFullscreenTextWidthSlot();

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

    void restoreDoc();
    void wordCountChangedSlot(QString type, int id, int count);

    void on_newButton_clicked();
    void on_nextButton_clicked();
    void on_prevButton_clicked();
    void on_navigatorComboBox_currentIndexChanged(const int index);


private:
    void setText(MainTextDocument *doc);
    void setSyn(MainTextDocument *synDocument);
    void setNote(MainTextDocument *noteDocument);
    void setTextCursorPos(int pos);
    void setSynCursorPos(int pos);
    void setNoteCursorPos(int pos);
    void resetNavigatorTree();
    Hub *hub;
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

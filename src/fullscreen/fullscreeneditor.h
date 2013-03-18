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

#include "textstyles.h"
#include "editmenu.h"
#include "wordcount.h"
#include "wordgoalprogressbar.h"


namespace Ui {
class FullscreenEditor;
}

class FullscreenEditor : public QWidget
{
    Q_OBJECT

public:
    explicit FullscreenEditor(QWidget *parent = 0);
    ~FullscreenEditor();
    void createContent(QTextDocument *doc = 0, int cursorPos = 0);

protected:
    void closeEvent(QCloseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);

signals:
    void closeSignal();
    void manageStylesSignal();

public slots:
    void setWordCount(int num);
    void setTimer(QString);
    void applyConfig();
    void setSyn(QTextDocument *synDocument, int cursorPos);
    void setNote(QTextDocument *noteDocument, int cursorPos);
    void setTextStyles(TextStyles *styles){textStyles = styles;}
    void resetFullscreenTextWidthSlot();
    void setProgressBarValue(int value);
    void setProgressBarGoal(int goal);

private slots:

    void createNotesMenu();
    void createOptionMenu();


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

private:
    Ui::FullscreenEditor *ui;

    TextStyles *textStyles;
    EditMenu *editWidget;
    int sliderCurrentValue;
QTextDocument *originalDoc;
WordCount *fullscreenWordCount;

    QPushButton *backColorButton,
    *textBackColorButton,
    *textColorButton,
    *addOnColorButton;

    QSettings settings;

    int synCursorPos;
    int noteCursorPos;
    //QTextDocument *synDoc;
    //QTextDocument *noteDoc;
    QWidget *synWidget;
    QWidget *noteWidget;

    QTimer *mouseTimer;


    // style sheets
    QString backColorString,
    textBackColorString,
    textColorString,
    addOnColorString;


};

#endif // FULLSCREENEDITOR_H

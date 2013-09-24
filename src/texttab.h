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
#ifndef TEXTTAB_H
#define TEXTTAB_H

#if QT_VERSION >= 0x050000
#include <QtWidgets>
#endif 
#include <QtGui>   
#include <QWidget>

#include "hub.h"
#include "textzone.h"
#include "textstyles.h"
#include "slimfindreplace.h"
//
namespace Ui {
class TextTab;
}
class TextTab : public QWidget
{
    Q_OBJECT
public:
    explicit TextTab(QWidget *parent = 0);
    ~TextTab();

    bool openText(MainTextDocument *doc);
    //    bool saveText(QFile *textFile, QString name);

    //for wordcount:
    MainTextDocument* document();



    int idNumber() const;
    void setIdNumber(int idNumber);

signals:


    void charFormatChangedSignal(QTextCharFormat format);
    void setStyleSelectionSignal(int styleIndex);
    void textChangedSignal();
    void manageStylesSignal();

protected:
    void paintEvent(QPaintEvent *);
void resizeEvent(QResizeEvent * event);

public slots:
    void setHub(Hub *varHub){hub = varHub;}
    void changeWidthSlot(int width = -1);
    void changeTextFontSlot(QFont font);
    void changeTextHeightSlot(int height);
    void changeTextStyleSlot(int styleIndex);
    void setTextFocus();
    void setCursorPos(int pos);
    int cursorPos();
    void applyConfig();
    QTextCharFormat tabFontChangedSlot();
    void updateTextZone();

    void showPrevText(bool showPrevTextBool);
    bool setShowPrevTextButton();
    void setPrevText(MainTextDocument *prevDoc);
    void showNextText(bool showNextTextBool);
    bool setShowNextTextButton();
    void setNextText(MainTextDocument *nextDoc);

    void setTextStyles(TextStyles *styles);

private slots:
    void launchSlimFindReplace();
    void cursorPositionChangedSlot();
    void giveStyle();
    void scrollBar_setRange(int min, int max);


    void on_prevTextToolButton_clicked();

    void on_nextTextToolButton_clicked();

    void on_splitter_splitterMoved(int pos, int index);

    void modifySize(int modifier);


private:
    Ui::TextTab *ui;

  void  setPrevButtonState(bool state);
    void  setNextButtonState(bool state);
    void addSlimFindReplaceAction();
    Hub *hub;
    TextStyles *textStyles;

    MainTextDocument *prevTextDocument;
    MainTextDocument *textDocument;
    MainTextDocument *nextTextDocument;

    QString stackName;
int m_idNumber;
bool firstTime;
};

#endif // TEXTTAB_H

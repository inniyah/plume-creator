/***************************************************************************
 *   Copyright (C) 2011 by Cyril Jacquet                                   *
 *   terreville@google.com                                                 *
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

#include <QtGui>
#include <QWidget>

#include "textzone.h"
#include "wordcount.h"
//

class TextTab : public QWidget
{
    Q_OBJECT
public:
    explicit TextTab(QWidget *parent = 0);

    bool openText(QTextDocument *doc);
    //    bool saveText(QFile *textFile, QString name);

    //for wordcount:
    QTextDocument* document();
    void updateWordCounts();

signals:

    void wordCountSignal(int wordCount);
    void blockCountSignal(int blockCount);
    void charFormatChangedSignal(QTextCharFormat format);

protected:



public slots:
    void changeWidthSlot(int width);
    void changeTextFontSlot(QFont font);
    void changeTextHeightSlot(int height);
    void setTextFocus();
    void setCursorPos(int pos);
    int saveCursorPos();
    void applyConfig();
    QTextCharFormat tabFontChangedSlot();
    void updateTextZone();
void showPrevText(bool showPrevTextBool);
bool setShowPrevTextButton();
void setPrevText(QTextDocument *prevDoc);

private slots:

    void wordCountUpdated(int wordCount);
    //void charCountUpdated(int charCount);
    void blockCountUpdated(int blockCount);
void cursorPositionChangedSlot();

private:

QTextDocument *prevTextDocument;
    QTextDocument *textDocument;

    QString stackName;

TextZone *prevTextZone;
    TextZone *textZone;



    WordCount *tabWordCount;

};

#endif // TEXTTAB_H

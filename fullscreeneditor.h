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
#ifndef FULLSCREENEDITOR_H
#define FULLSCREENEDITOR_H


#include <QWidget>
#include <QLabel>
#include <QTextDocument>
#include <QPushButton>
#include <QSettings>

class FullscreenEditor : public QWidget
{
    Q_OBJECT
public:
    explicit FullscreenEditor(QTextDocument *doc = 0, int cursorPos = 0, QWidget *parent = 0);


protected:
    void closeEvent(QCloseEvent* event);

signals:
    void closeSignal();

public slots:
    void setWordCount(int num);
    void setTimer(QString);
    void applyConfig();
    void setSyn(QTextDocument *synDocument, int cursorPos);
    void setNote(QTextDocument *noteDocument, int cursorPos);

private slots:
void callColorDialog();
void setBaseStyleSheet();
void setBackColorDialog();
void setBackColor();
void setTextBackColorDialog();
void setTextBackColor();
void setTextColorDialog();
void setTextColor();
void applyStyleSheet();

void showSyn();
void showNote();
private:
    QLabel *wordCountLabel;
    QLabel *timerLabel;

    QPushButton *backColorButton,
    *textBackColorButton,
    *textColorButton;

    QSettings settings;

int synCursorPos;
int noteCursorPos;
//QTextDocument *synDoc;
//QTextDocument *noteDoc;
QWidget *synWidget;
QWidget *noteWidget;

    // style sheets
    QString backColorString,
    textBackColorString,
    textColorString;

};

#endif // FULLSCREENEDITOR_H

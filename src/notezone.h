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
#ifndef NOTEZONE_H
#define NOTEZONE_H

#if QT_VERSION >= 0x050000
#include <QtWidgets>
#endif 
#include <QtGui>   
#include <QTextEdit>
//
#include "hub.h"

class NoteZone : public QTextEdit
{
    Q_OBJECT
public:
    explicit NoteZone(QWidget *parent = 0);


    bool openNote(MainTextDocument *noteDoc);
    //    bool saveNote(QFile *noteFile, QString name);
    bool closeNote();


    bool openSyn(MainTextDocument *synDoc);
    //    bool saveSyn(QFile *synFile, QString name);
    bool closeSyn();

    bool openOutlinerDoc(QTextDocument *outlinerDoc);
    bool closeOutlinerDoc();

    bool openAttendDetail(QTextDocument *attendDoc);
    bool closeAttendDetail();


signals:
    void connectUpdateTextsSignal();
    void disconnectUpdateTextsSignal();
    void  noteFocusOutSignal();
    void cursorPositionChanged(int pos);

protected:

    void contextMenuEvent(QContextMenuEvent *event);
    void insertFromMimeData (const QMimeData *source);
    bool canInsertFromMimeData (const QMimeData *source) const;
    void resizeEvent(QResizeEvent* event);
    void keyPressEvent(QKeyEvent *event);
    void focusOutEvent ( QFocusEvent * event );


public slots:

    void setHub(Hub *varHub){hub = varHub;}

    void setTextFont(QFont font);
    void setTextHeight(int height);
    void centerCursor();
    void setCursorPos(int pos);
    int saveCursorPos();
    void updateTextZone();
    void applyNoteConfig();
    void applySynConfig();
    void applyOutlinerDocConfig();
    void applyAttendConfig();

private slots:

    void undo();
    void redo();
    void cut();
    void copy();
    void paste();
    void bold(bool boldBool);
    void italic(bool italBool);
    void leftAlign(bool leftBool);
    void rightAlign(bool rightBool);
    void justify(bool justBool);
    void center(bool centBool);


    void charFormat(QTextCharFormat cFormat);


    void mergeFormatOnWordOrSelection(const QTextCharFormat &format);
    void cursorPositionChangedSlot();


    void forceSynFirstCharFont();
    void forceNoteFirstCharFont();
    void forceOutlinerDocFirstCharFont();
    void forceAttendFirstCharFont();

    void applySynFontConfig();
    void applyNoteFontConfig();
    void applyOutlinerDocFontConfig();
    void applyAttendFontConfig();

void giveOutlinerStyle();
void preventDoubleSpace();


private:

Hub *hub;
    QTextDocument *textDocument;

    //    QString synStackName;
    //    QString noteStackName;

    void createActions();

    QAction *undoAct;
    QAction *redoAct;
    QAction *cutAct;
    QAction *copyAct;
    QAction *pasteAct;
    QAction *boldAct;
    QAction *italicAct;
    QAction *leftAlignAct;
    QAction *rightAlignAct;
    QAction *justifyAct;
    QAction *centerAct;

    QMenu *alignmentGroup;


    bool alwaysCenter;
    bool preventDoubleSpaceOption;

};

#endif // NOTEZONE_H

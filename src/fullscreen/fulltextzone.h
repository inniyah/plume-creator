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


#ifndef FULLTEXTZONE_H
#define FULLTEXTZONE_H


#if QT_VERSION >= 0x050000
#include <QtWidgets>
#endif 
#include <QtGui>   
#include <QTextEdit>
#include <QSettings>
#include <QSlider>

#include "hub.h"
#include "textstyles.h"
//
class FullTextZone : public QTextEdit
{
    Q_OBJECT
public:
    explicit FullTextZone(QWidget *parent = 0);
    void setDoc(MainTextDocument *doc);

signals:
    void charFormatChangedSignal(QTextCharFormat format);
    void quitFullScreen();

protected:
    void contextMenuEvent(QContextMenuEvent *event);
    void insertFromMimeData (const QMimeData *source);
    bool canInsertFromMimeData (const QMimeData *source) const;
    void resizeEvent(QResizeEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void mousePressEvent( QMouseEvent* event);
    void mouseReleaseEvent( QMouseEvent* event);
    void keyPressEvent(QKeyEvent *event);

public slots:

    void setHub(Hub *varHub){hub = varHub;}
    void setTextFont(QFont font);
    void setTextHeight(int height);
    void centerCursor();
    void applyTextConfig();
    void applySynConfig();
    void applyNoteConfig();
    void setTextStyles(TextStyles *styles){textStyles = styles;}


private slots:

    void undo();
    void redo();
    void cut();
    void copy();
    void paste();
    void bold(bool boldBool);
    void italic(bool italBool);
//    void leftAlign(bool leftBool);
//    void rightAlign(bool rightBool);
//    void justify(bool justBool);
//    void center(bool centBool);


    void charFormat(QTextCharFormat cFormat);

    void mergeFormatOnWordOrSelection(const QTextCharFormat &format);
    void cursorPositionChangedSlot();

    void preventDoubleSpace();


private:

    Hub *hub;
    MainTextDocument *textDocument;

    //context menu:

    void createActions();

    QAction *undoAct,
    *redoAct,
    *cutAct,
    *copyAct,
    *pasteAct,
    *boldAct,
    *italicAct,
//    *leftAlignAct,
//    *rightAlignAct,
//    *justifyAct,
//    *centerAct,
    *setWidthAct,
    *setColorsAct,
    *setZoomAct;

    QMenu *optionGroup;
    QPoint mousePos;

    bool mDragging;
    QPoint mDragStartPosition;

    bool alwaysCenter;

    QSlider *widthSlider;
    int sliderValue;
    int xMax;

    bool preventDoubleSpaceOption;

    TextStyles *textStyles;

};


#endif // FULLTEXTZONE_H

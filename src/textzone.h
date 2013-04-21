/***************************************************************************
 *   Copyright (C) 2011 by Cyril Jacquet                                   *
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
#ifndef TEXTZONE_H
#define TEXTZONE_H

#include <QWidget>
#include <QTextEdit>
#include <QSettings>

#include "hub.h"
#include "textstyles.h"
#include "editmenu.h"
//
class TextZone : public QTextEdit
{
    Q_OBJECT
public:
    explicit TextZone(QWidget *parent = 0);
    ~TextZone();
    void createContent();
    void setDoc(MainTextDocument *doc);
    void addActions(QList<QAction*> actions);

signals:
    void charFormatChangedSignal(QTextCharFormat format);
    void setStyleSelectionSignal(int styleIndex);
    void styleSelectedSignal(int styleIndex);
    void manageStylesSignal();
    void cursorPositionChanged(int pos);

protected:
    void contextMenuEvent(QContextMenuEvent *event);
    void insertFromMimeData (const QMimeData *source);
    bool canInsertFromMimeData (const QMimeData *source) const;
    void resizeEvent(QResizeEvent* event);
    void keyPressEvent(QKeyEvent *event);

public slots:

    void setHub(Hub *varHub){hub = varHub;}
    void setTextFont(QFont font);
    void setTextHeight(int height);
    void centerCursor();
    void applyConfig();
    void scrollBy(QPoint viewportPoint);

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
    void createEditWidget();

    void charFormat(QTextCharFormat cFormat);

    void mergeFormatOnWordOrSelection(const QTextCharFormat &format);
    void cursorPositionChangedSlot();

    void preventDoubleSpace();

private:
    Hub *hub;

    MainTextDocument *textDocument;

    //context menu:

    void createActions();

    QAction *undoAct;
    QAction *redoAct;
    QAction *cutAct;
    QAction *copyAct;
    QAction *pasteAct;
    QAction *boldAct;
    QAction *italicAct;
    QAction *manageStylesAct;
    //    QAction *leftAlignAct;
    //    QAction *rightAlignAct;
    //    QAction *justifyAct;
    //    QAction *centerAct;
    EditMenu *editWidget;

    QMenu *alignmentGroup;
    QMenu *stylesGroup;

    TextStyles *textStyles;

    QPoint mousePos;

    bool alwaysCenter;
    bool showScrollbar;

    bool preventDoubleSpaceOption;

    QList<QAction*> m_actions;
};

#endif // TEXTZONE_H

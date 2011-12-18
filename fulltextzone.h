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


#ifndef FULLTEXTZONE_H
#define FULLTEXTZONE_H


#include <QWidget>
#include <QTextEdit>
#include <QSettings>
#include <QSlider>
//
class FullTextZone : public QTextEdit
{
    Q_OBJECT
public:
    explicit FullTextZone(QTextDocument *doc, QWidget *parent = 0);

signals:
    void charFormatChangedSignal(QTextCharFormat format);
    void callColorDialogSignal();
protected:
    void contextMenuEvent(QContextMenuEvent *event);
    void insertFromMimeData (const QMimeData *source);
    bool canInsertFromMimeData (const QMimeData *source) const;
    void resizeEvent(QResizeEvent* event);
    void mouseMoveEvent(QMouseEvent* event);

public slots:

    void setTextFont(QFont font);
    void setTextHeight(int height);
    void centerCursor();
    void applyTextConfig();
    void applySynConfig();
    void applyNoteConfig();
    void loadSliderValue();
    void setXMax(int value);

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
    void setWidth();
    void setColors();
    void setZoom();

    void charFormat(QTextCharFormat cFormat);

    void mergeFormatOnWordOrSelection(const QTextCharFormat &format);
    void cursorPositionChangedSlot();


    void sliderValueChanged(int sliderValue);

private:

    QTextDocument *textDocument;

    //context menu:

    void createActions();

    QAction *undoAct,
    *redoAct,
    *cutAct,
    *copyAct,
    *pasteAct,
    *boldAct,
    *italicAct,
    *leftAlignAct,
    *rightAlignAct,
    *justifyAct,
    *centerAct,
    *setWidthAct,
    *setColorsAct,
    *setZoomAct;

    QMenu *alignmentGroup,
    *optionGroup;
    QPoint mousePos;

    bool alwaysCenter;

    QSlider *widthSlider;
    int sliderValue;
    int xMax;
};


#endif // FULLTEXTZONE_H

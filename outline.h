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
#ifndef OUTLINE_H
#define OUTLINE_H

#include <QWidget>
#include <QtGui>

class Outline : public QWidget
{
    Q_OBJECT
public:
    explicit Outline(QWidget *parent = 0);


protected:
    void resizeEvent (QResizeEvent *event);
    void closeEvent(QCloseEvent* event);

signals:
    void showListsSignal(bool buttonToggled);
    void showNotesSignal(bool buttonToggled);
    void expandAllTextsSignal(bool buttonToggled);
    void updateSizeSignal();
    void newOutlineTitleSignal(QString newTitle, int number);
    void disconnectUpdateTextsSignal();
    void connectUpdateTextsSignal();

public slots:
    void buildItem(QTextDocument *synDocument, QTextDocument *noteDocument,QString title, int number, QString tagName);
    void buildSeparator();
    void buildStretcher();
    void applyConfig();
    void setItemTitle(QString newTitle, int number);
    void cleanArea();



private slots:

    void resizingSlot();

private:
    QScrollArea *area;
    QWidget *areaWidget;
    QVBoxLayout *areaLayout;

    QAction *showListsAct;
    QAction *showNotesAct;
    QAction *expandAllTextsAct;
    QSlider *listSlider;
    QSlider *synSlider;
    QSlider *noteSlider;
    QFontComboBox *synFontCombo;
    QSpinBox *synTextHeightSpin;
    QFontComboBox *noteFontCombo;
    QSpinBox *noteTextHeightSpin;
};

#endif // OUTLINE_H

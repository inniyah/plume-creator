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
#include <QDomElement>

#include "outlineitem.h"

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
    void writeThisSignal(int number);
    void outlinerClosedSignal();

public slots:
    OutlineItem * buildItem(QTextDocument *synDocument, QTextDocument *noteDocument,QString title, int number, QString tagName);
    void buildSeparator();
    void buildStretcher();
    void applyConfig();
    void saveConfig();
    void setItemTitle(QString newTitle, int number);
    void cleanArea();

    void setOutlinerViewportPos(int number);
void insertItemInOutliner(QWidget *newWidget, int number, QString tagName);

//for attendances :
void updateAttendances(QHash<int, QString> attendStringForNumber);
void setProjectAttendanceList(QHash<QListWidgetItem *, QDomElement> domElementForItem_, QHash<int, QDomElement> domElementForItemNumber_);

private slots:

    void resizingSlot();


    //for attendance :
    QList<QListWidgetItem *> *sortAttendItems(QList<int> *attend, QString sorting = "nothing");
    QListWidgetItem *attendSeparator(QString separatorName);
    QList<QListWidgetItem *> *openSheetAttendList(int number ,QString attendString);


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
    QSpinBox *synTextHeightSpin, *synTextIndentSpin, *synTextMarginSpin;
    QFontComboBox *noteFontCombo;
    QSpinBox *noteTextHeightSpin, *noteTextIndentSpin, *noteTextMarginSpin;

    QHash<QListWidgetItem *, QDomElement> domElementForItem;
    QHash<int, QDomElement> domElementForItemNumber;

    QString newAttendName;
};

#endif // OUTLINE_H

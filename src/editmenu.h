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
#ifndef EDITMENU_H
#define EDITMENU_H

#include <QtGui>
#include <QSettings>
#include <QTextCharFormat>

#include "textstyles.h"

namespace Ui {
class EditMenu;
}

class EditMenu : public QWidget
{
    Q_OBJECT
    
public:
    explicit EditMenu(QWidget *parent = 0);
    ~EditMenu();
    void createContent();

signals:
    void widthChangedSignal(int sliderValue);
    void textFontChangedSignal(QFont font);
    void textHeightChangedSignal(int textHeight);
    void styleSelectedSignal(int styleIndex);
    void textWidthSliderValueChanged(int value);
    void zoomOutSignal();
    void zoomInSignal();

public slots:
    void charFormatChangedSlot(QTextCharFormat format);
    void tabChangedSlot(QTextCharFormat newTabFormat);
    void setStyleSelectionSlot(int selection);
    void applyConfig();
    void setTextStyles(TextStyles *styles){textStyles = styles;}
    void hideWidgetsByName(QStringList widgetToHideList);
    void setTextWidthMax(int max);
    void setTextWidthSliderValue(int sliderValue);

    int textWidthSliderValue();

private slots:
    void styleSelectedSlot(QListWidgetItem *item);
    void zoomIn();
    void zoomOut();

private:
    Ui::EditMenu *ui;

    int xMax;
    int sliderValue;
    QSettings settings;
    QFont textFont;
    int textSpinValue;;

    TextStyles *textStyles;


};

#endif // EDITMENU_H

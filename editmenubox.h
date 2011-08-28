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
#ifndef EDITMENUBOX_H
#define EDITMENUBOX_H

#include <QtGui>
#include <QWidget>
#include <QFrame>
#include <QToolButton>

class EditMenuBox : public QFrame
{
    Q_OBJECT
public:
    explicit EditMenuBox(QWidget *parent = 0);

signals:
    void widthChangedSignal(int sliderValue);
    void textFontChangedSignal(QFont font);
    void textHeightChangedSignal(int textHeight);
public slots:
    void tabWitdhChangedSlot(int value);
    void loadSliderValue();
    void applyConfig();
     void charFormatChangedSlot(QTextCharFormat format);
     void tabChangedSlot(QTextCharFormat newTabFormat);

private slots:
    void sliderValueChanged(int sliderValue);

private:
    int xMax;
    QSlider *widthSlider;
    int sliderValue;
    QSettings settings;
    QFontComboBox *textFontCombo;
    QFont textFont;
    QSpinBox *textSpin;
    int textSpinValue;
};

#endif // EDITMENUBOX_H

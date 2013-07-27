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
#ifndef ORIENTATIONBUTTON_H
#define ORIENTATIONBUTTON_H

#include <QPushButton>
#include <QStyleOptionButton>

class OrientationButton : public QPushButton
{
public:
    OrientationButton(QWidget* parent = 0);
    OrientationButton(const QString& text, QWidget* parent = 0);
    OrientationButton(const QIcon& icon, const QString& text, QWidget* parent = 0);
    ~OrientationButton();

    Qt::Orientation orientation() const;
    void setOrientation(Qt::Orientation orientation);

    bool mirrored() const;
    void setMirrored(bool mirrored);

    QSize sizeHint() const;

protected:
    void paintEvent(QPaintEvent* event);

private:
    QStyleOptionButton getStyleOption() const;
    void init();

    Qt::Orientation orientation_;
    bool mirrored_;
};

#endif // ORIENTATIONBUTTON_H

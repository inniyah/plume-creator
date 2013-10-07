/***************************************************************************
 *   Copyright (C) 2013 by Cyril Jacquet                                   *
 *   cyril.jacquet@plume-creator.eu                                        *
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
#ifndef SIZEHANDLE_H
#define SIZEHANDLE_H

#if QT_VERSION >= 0x050000
#include <QtWidgets>
#endif
#include <QtGui>
#include <QApplication>
#include <QWidget>
#include <QStyleOption>

class SizeHandle : public QWidget
{

    Q_OBJECT
public:


    enum Side {
        Left = 0x0,
        Right = 0x1
    };
    Q_DECLARE_FLAGS(Sides, Side)


    explicit SizeHandle(QWidget *parent = 0);

    SizeHandle::Sides side() const;
    void setSide(const SizeHandle::Sides &side);

protected:
    void leaveEvent(QEvent *event);
    void enterEvent(QEvent *event);
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);


signals:
    void modifySizeSignal(int mousePosChangeDelta);

public slots:

private:
    QPoint mousePos;

    SizeHandle::Sides m_side;

    bool isPressed;



};
Q_DECLARE_OPERATORS_FOR_FLAGS(SizeHandle::Sides)

#endif // SIZEHANDLE_H

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
#ifndef OUTLINERSPREADSHEETHEADER_H
#define OUTLINERSPREADSHEETHEADER_H

#include <QHeaderView>

class OutlinerSpreadsheetHeader : public QHeaderView
{
    Q_OBJECT
public:
    explicit OutlinerSpreadsheetHeader(Qt::Orientation orientation, QWidget * parent = 0);
    
protected:
    void contextMenuEvent(QContextMenuEvent *event);

signals:
    
public slots:
    
private slots:
    void hideOrShowSectionSlot(int logicalIndex, bool showSectionBool);
};

#endif // OUTLINERSPREADSHEETHEADER_H

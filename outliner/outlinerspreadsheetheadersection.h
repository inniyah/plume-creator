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
#ifndef OUTLINERSPREADSHEETHEADERID_H
#define OUTLINERSPREADSHEETHEADERID_H

#include <QObject>

class OutlinerSpreadsheetHeaderSection : public QObject
{
    Q_OBJECT
public:
    explicit OutlinerSpreadsheetHeaderSection(QObject *parent = 0);
    
signals:
    void hideOrShowSectionSignal(int id, bool hideSectionBool);

public slots:
    void hideOrShowThisSection(bool hideSectionBool);
    void hideThisSection();
    int id();
    void setId(int id);

private:
int sectionId;
};

#endif // OUTLINERSPREADSHEETHEADERID_H

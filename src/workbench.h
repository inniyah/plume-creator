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
#ifndef WORKBENCH_H
#define WORKBENCH_H

#include <QWidget>

#include "hub.h"
#include "mainTree/maintreeabstractmodel.h"
#include "outliner/outlinerbase.h"
#include "infoSheet/infosheetbase.h"

class Workbench : public QWidget
{
    Q_OBJECT
public:
    explicit Workbench(QWidget *parent = 0);
    void postConstructor();
    ~Workbench();

signals:


    void applySynNoteFontConfigSignal();
    void updateOutlinerSignal();

public slots:
    void setHub(Hub *varHub){hub = varHub;}
    void setMainTreeAbstractModel(MainTreeAbstractModel *tree){absTree = tree;}
    void saveConfig();
    void applyConfig();

private slots:
    void resetCommonToolBar();
    void shiftToSpreadsheet();
    void shiftToSpreadsheetSingleShot();
void shiftToInfoSheet();
void shiftToInfoSheetSingleShot();
void quitCurrentMode();

private:
Hub *hub;
MainTreeAbstractModel  *absTree;

QAction *shiftToSpreadsheetAct, *shiftToInfoSheetAct;
QString lastOpened;
QVBoxLayout *zoneLayout;
QToolBar *commonToolBar;
OutlinerBase *outlinerBase;
InfoSheetBase *infoSheetBase;
QWidget *currentModeWidget;








};

#endif // WORKBENCH_H

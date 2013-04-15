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
#ifndef WORDGOALPROGRESSBAR_H
#define WORDGOALPROGRESSBAR_H

#include <QProgressBar>

#include "hub.h"

namespace Ui {
class WordGoalProgressBar;
}

class WordGoalProgressBar : public QWidget
{
    Q_OBJECT
    
public:
    explicit WordGoalProgressBar(QWidget *parent = 0);
    ~WordGoalProgressBar();
    void postConstructor();
    void init();


protected:
    void contextMenuEvent(QContextMenuEvent *event);

signals:

public slots:
    void setHub(Hub *varHub){hub = varHub;}
    void changeProgressBy(int progress);

private slots:
    void setGoal(int goal);
    int goal();
    void setBase(int base);
    void setValue(int value);
    int value();
    void setGoalDialog();
    void createActions();
    void setWordGoalActivated(bool wordGoalActivated);
    void reset();

private:
    void setColors();
    Hub *hub;
    Ui::WordGoalProgressBar *ui;
    QAction *resetAct, *setGoalAct;
    bool m_isWordGoalActivated;

};

#endif // WORDGOALPROGRESSBAR_H

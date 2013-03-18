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
#ifndef TIMER_H
#define TIMER_H

#include <QtGui>
#include <QtGui>
//
class Timer : public QWidget
{
    Q_OBJECT
public:
    explicit Timer(QWidget *parent = 0);

signals:
    void timerSignal(QString string);

public slots:

private slots:

    void secSpinFormat(int sec);
    void minSpinFormat(int min);
    void startTime();
    void resetTime();
    void pauseTime(bool paused);
    void showTime();

private:

    QTimer *timer;

    QFrame *timerRunningFrame, *timerSettingFrame;
    QSpinBox *hourEdit, *minEdit, *secEdit;
    QLabel *timeLabel;
    double timeValue;

    int h;
    int m;
    int s;

};

#endif // TIMER_H

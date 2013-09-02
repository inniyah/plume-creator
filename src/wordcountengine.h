/***************************************************************************
 *   Copyright (C) 2012 by Cyril Jacquet                                   *
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
#ifndef WORDCOUNTENGINE_H
#define WORDCOUNTENGINE_H

#include <QObject>
#if QT_VERSION >= 0x050000
#include <QtWidgets>
#endif 
#include <QtGui>   

class WordCountEngine : public QObject
{
    Q_OBJECT
public:
    explicit WordCountEngine(QObject *parent = 0, QTextDocument *doc = 0);
    int wordCount(){return m_wordCount;}

signals:
    void wordCountChanged(int);

public slots:
    void update();

private slots:
    void updateCursorPosition(QTextCursor textCursor);
private:
    int m_wordCount, preBlockCount, preBlockNum,
    blockNum, wordCountWithoutBlock;
    QTextDocument *m_doc;
};

#endif // WORDCOUNTENGINE_H

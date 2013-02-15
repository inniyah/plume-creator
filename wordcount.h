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
#ifndef WORDCOUNT_H
#define WORDCOUNT_H

#include <QObject>
#include <QTextDocument>
#include <QTextCursor>
#include <QProcess>
//

class WordCount : public QObject
{
    Q_OBJECT
public:
    explicit WordCount(QTextDocument *doc, QObject *parent = 0);
    int updateAll();


signals:

    //    void charCountSignal(int charCount);
    void wordCountSignal(int wordCount);
    void blockCountSignal(int blockCount);
    void countDeltaSignal(int countDelta);

public slots:

private slots:


    //    void updateCharCount(/*int position,int charsRemoved,int charsAdded*/);
    void updateWordCount();
    void updateCursorPosition(QTextCursor textCursor);
    void updateBlockCount(int blockCount);

private:


    QTextDocument *document;
    QStringList wordsList;

    //    int finalCharCount;
    //    int preCharCount;


    int finalWordCount;
    int preWordCount;

    int blockNum;
    int preBlockNum;
    int preBlockCount;
    int finalBlockCount;
};

#endif // WORDCOUNT_H

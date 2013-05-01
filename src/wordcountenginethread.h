/***************************************************************************
 *   Copyright (C) 2012 by Cyril Jacquet                                   *
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
#ifndef WORDCOUNTENGINETHREAD_H
#define WORDCOUNTENGINETHREAD_H

#include <QThread>
#include <QHash>
#include <QDomDocument>

#include "maintextdocument.h"

class WordCountEngineThread : public QThread
{
    Q_OBJECT
public:
    explicit WordCountEngineThread(QObject *parent = 0);
    void set_mainTree_numForDocHash(QHash<MainTextDocument *, int> numForDoc){m_numForDoc = numForDoc;}

    void set_mainTree_DomDoc(QDomDocument domDoc){m_domDoc = domDoc;}

    int projectWordCount() const{ return m_projectWordCount;}

protected:
    void run();

signals:
    void projectWordCount(int count);
    void bookWordCount(int count);
    void chapterWordCount(int count);
    void sceneWordCount(int count);
    void currentSheetWordCount(int count);

public slots:
    void changeCurrentSheetNumber(int number){currentSheetNumber = number; this->start();}

private:
    void resetTree();
    QList<QDomElement> allChildNodes(QDomElement node);
    int size(QDomElement element);
    int parentSize(QDomElement element);
    QDomElement parentElement(QDomElement element){return element.parentNode().toElement();}

    int currentSheetNumber;
    QString treeString;
    QHash<MainTextDocument *, int> m_numForDoc;
    QDomDocument m_domDoc;

    QHash<int, QDomElement> domElementForNumber;
    QDomElement currentElement;
    QHash<int,int> m_wordCountForNumber;

    int m_projectWordCount;
};

#endif // WORDCOUNTENGINETHREAD_H

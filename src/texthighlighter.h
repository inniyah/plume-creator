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
#ifndef TEXTHIGHLIGHTER_H
#define TEXTHIGHLIGHTER_H

#include <QSyntaxHighlighter>


class TextHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT
public:
    TextHighlighter(QTextDocument *parentDoc = 0);
    void setTextToHighlight(QString string);
    void setCaseSensitivity(bool isCaseSensitive);

protected:
    void highlightBlock(const QString &text);
signals:

public slots:
    
private:
    QString textToHighLight;
    Qt::CaseSensitivity sensitivity;
};

#endif // TEXTHIGHLIGHTER_H

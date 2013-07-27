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
#ifndef SLIMFINDREPLACE_H
#define SLIMFINDREPLACE_H

#include <QWidget>

#include "maintextdocument.h"
#include "hub.h"

namespace Ui {
class SlimFindReplace;
}

class SlimFindReplace : public QWidget
{
    Q_OBJECT
    
public:
    explicit SlimFindReplace(QWidget *parent = 0);
    ~SlimFindReplace();
    void setDocument(MainTextDocument *doc);
    void setTextEdit(QTextEdit *txtEdit);

protected:


public slots:
    void setHub(Hub *varHub){hub = varHub;}
    void closeSlot();

private slots:

    void on_findLineEdit_textChanged(const QString &arg1);

    void on_replaceLineEdit_textChanged(const QString &arg1);

    void on_closeButton_clicked();

    void on_previousButton_clicked();

    void on_replaceAndNextButton_clicked();

    void on_nextButton_clicked();


    void on_caseSensitiveCheckBox_toggled(bool checked);

private:
    void applyConfig();
    void saveConfig();
    QList<int> findTextPos(QString text);

    Ui::SlimFindReplace *ui;
    Hub *hub;
    MainTextDocument *document;
    QTextEdit *textEdit;
    QString findText;
    QString replaceText;
    QList<int> positionsList;
    bool caseSensitive;
    QTextDocument::FindFlags findFlags;
    bool allowRestartFromBeginning, allowRestartFromEnd;
    int oldPosition;

};

#endif // SLIMFINDREPLACE_H

/***************************************************************************
 *   Copyright (C) 2011 by Cyril Jacquet                                   *
 *   terreville@google.com                                                 *
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
#ifndef NOTEZONE_H
#define NOTEZONE_H

#include <QtGui>
#include <QTextEdit>

class NoteZone : public QTextEdit
{
    Q_OBJECT
public:
    explicit NoteZone(QWidget *parent = 0);


    bool openNote(QFile *noteFile, QString name);
    bool saveNote(QFile *noteFile, QString name);
    bool closeNote(QFile *noteFile, QString name);


    bool openSyn(QFile *synFile, QString name);
    bool saveSyn(QFile *synFile, QString name);
    bool closeSyn(QFile *synFile, QString name);

signals:

protected:

    void contextMenuEvent(QContextMenuEvent *event);
// void keyPressEvent(QKeyEvent *event);
public slots:


private slots:

    void undo();
    void redo();
    void cut();
    void copy();
    void paste();
    void bold(bool boldBool);
    void italic(bool italBool);
    void leftAlign(bool leftBool);
    void rightAlign(bool rightBool);
    void justify(bool justBool);
    void center(bool centBool);


   void charFormat(QTextCharFormat cFormat);

private:

   QTextDocument *textDocument;

    QString synStackName;
    QString noteStackName;

    void createActions();

    QAction *undoAct;
    QAction *redoAct;
    QAction *cutAct;
    QAction *copyAct;
    QAction *pasteAct;
    QAction *boldAct;
    QAction *italicAct;
    QAction *leftAlignAct;
    QAction *rightAlignAct;
    QAction *justifyAct;
    QAction *centerAct;

    QMenu *alignmentGroup;

};

#endif // NOTEZONE_H

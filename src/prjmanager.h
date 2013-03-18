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
#ifndef PRJMANAGER_H
#define PRJMANAGER_H

#include <QtGui>
//

//QT_BEGIN_NAMESPACE
//class DeletePrjButton;
//class OpenPrjButton;
//QT_END_NAMESPACE


class PrjManager : public QDialog
{
    Q_OBJECT
public:
    explicit PrjManager(QWidget *parent = 0);

signals:

    void openPrjManagerSignal(); // relay from deletePrjButton to MainWindow
    void newPrjSignal();
    void openProjectSignal(QFile *device);
    void openProjectNumberSignal(int prjNumber);
    void deleteProjectNum(int numPrj);

    public slots:
    void projectAlreadyOpened(bool projectAlreadyOpened){if(projectAlreadyOpened) noProjectAlreadyOpened = false; else noProjectAlreadyOpened = true;}

private slots:
    static bool removeDir(const QString &dirName);
    void deleteProjectQuestion();
    void delProject();
    void displayProjects();
    void newPrj();
void setPrjActivated(int row,int column);
void openPrj();
void renamePrj();
void acceptRenaming();
void renameXML(QFile *xmlFile, QString oldName, QString newName);
void itemsSelectionChangedSlot();

private:
    QVBoxLayout *projListLayout;
    QGridLayout *itemLayout;
QTableWidget *table;
int prjActivated;
int currentRow;
QLineEdit *renameEdit;
QDialog *renameDialog;
QLabel *nameLabel,*creationDateLabel,*modifiedDateLabel,*pathLabel;
bool noProjectAlreadyOpened;
bool prjOpened;

    QFrame *stackItem;
    int numberOfProjects;
    int i;
};


////$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
////$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$



//class DeletePrjButton : public QPushButton
//{
//    Q_OBJECT
//public:
//    explicit DeletePrjButton(QWidget *parent = 0);

//signals:

//    void openProjectManagerSignal();

//public slots:
//    void deleteProjectQuestion();
//    void deleteProjectNum(int numPrj);
//    void delProject();

//private slots:
//    static bool removeDir(const QString &dirName);

//private:
//    int del_numPrj;
//};



////$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
////$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$



//class OpenPrjButton : public QPushButton
//{
//    Q_OBJECT
//public:
//    explicit OpenPrjButton(QWidget *parent = 0);

//signals:
//    void openProjectSignal(QFile *device);
//    void openProjectNumberSignal(int prjNumber);

//public slots:
//    void openProjectFile(QFile *device);
//    void openProjectSettingNumber(int prjNumber);

//private slots:
//    void opProject();
//private:
//    QFile *prjFile;
//    int projectNumber;
//};


#endif // PRJMANAGER_H

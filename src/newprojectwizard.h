/***************************************************************************
 *   Copyright (C) 2011 by Cyril Jacquet                                   *
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
#ifndef NEWPROJECTWIZARD_H
#define NEWPROJECTWIZARD_H

#if QT_VERSION >= 0x050000
#include <QtWidgets>
#endif 
#include <QtGui>   
#include <QWizard>
#include <QtXml>
#if QT_VERSION >= 0x050000
#include <QtWidgets>
#endif 
#include <QtGui>   
//
#include "hub.h"

QT_BEGIN_NAMESPACE
class QCheckBox;
class QGroupBox;
class QLabel;
class QLineEdit;
class QRadioButton;
QT_END_NAMESPACE


class NewProjectWizard : public QWizard
{
    Q_OBJECT

public:
    NewProjectWizard(QWidget *parent = 0);

    void accept();

    QHash<QString, int> structureToCreate();
    QString newProjectFileName();

signals:
    void openProjectSignal(QString fileName);

private:


QHash<QString, int> m_structureToCreate;
QString m_newProjectFileName;


};



//--------------------------------------------------


class IntroPage : public QWizardPage
{
    Q_OBJECT

public:
    IntroPage(QWidget *parent = 0);

protected:
    void initializePage();

private:
    QLabel *label;
};



//-----------------------------------------------



class SelectPage : public QWizardPage
{
    Q_OBJECT

public:
    SelectPage(QWidget *parent = 0);

protected:
    void initializePage();

private slots:
    void setExistingDirectory();

private:
    QLabel *label;
    QLabel *directoryLabel;
    QLineEdit *directoryLabelLineEdit;
    QLabel *projectNameLabel;
    QLineEdit *projectNameLabelLineEdit;

};

#endif // NEWPROJECTWIZARD_H

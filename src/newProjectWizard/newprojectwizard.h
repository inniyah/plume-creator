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

class NewProjectWizard : public QWizard
{
    Q_OBJECT

public:
    explicit NewProjectWizard(QWidget *parent = 0);
    void accept();

     QHash<QString, int> structureToCreate();
     QString newProjectFileName();

private slots:


private:
     QWizardPage *createIntroPage();
     QWizardPage *createSelectPage();
     QWizardPage *createStructurePage();

    QHash<QString, int> m_structureToCreate;
    QString m_newProjectFileName;

};

#endif // NEWPROJECTWIZARD_H

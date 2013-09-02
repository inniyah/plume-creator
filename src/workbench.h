#ifndef WORKBENCH_H
#define WORKBENCH_H

#include <QWidget>

#include "hub.h"
#include "mainTree/maintreeabstractmodel.h"
#include "outliner/outlinerbase.h"

class Workbench : public QWidget
{
    Q_OBJECT
public:
    explicit Workbench(QWidget *parent = 0);
    void postConstructor();
    ~Workbench();

signals:


    void applySynNoteFontConfigSignal();
    void updateOutlinerSignal();

public slots:
    void setHub(Hub *varHub){hub = varHub;}
    void setMainTreeAbstractModel(MainTreeAbstractModel *tree){absTree = tree;}
    void saveConfig();
    void applyConfig();

private slots:
    void shiftToSpreadsheet();

private:
Hub *hub;
MainTreeAbstractModel  *absTree;

QAction *shiftToSpreadsheetAct;
QString lastOpened;
QVBoxLayout *zoneLayout;
QToolBar *commonToolBar;
OutlinerBase *outlinerBase;










};

#endif // WORKBENCH_H

#include "wordgoalprogressbar.h"
#include <QInputDialog>
#include <QMenu>
#include <QContextMenuEvent>
#include "ui_wordgoalprogressbar.h"

WordGoalProgressBar::WordGoalProgressBar(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WordGoalProgressBar)
{
    ui->setupUi(this);

    createActions();

    ui->progressBar->setFormat("%v");
    ui->progressBar->setRange(0,1000);
ui->progressBar->reset();
}

WordGoalProgressBar::~WordGoalProgressBar()
{
    delete ui;
}

void WordGoalProgressBar::setGoalDialog()
{
    bool ok;
    int i = QInputDialog::getInt(this, tr("Set Session Target"),
                                 tr("Word count target :"), 1000, 0, 999999, 1, &ok);
    if (ok)
        setGoal(i);
    ui->wordGoalLabel->setText(tr("/%1 words").arg(i));
}


void WordGoalProgressBar::setGoal(int goal)
{
    ui->progressBar->setMaximum(goal);
}

void WordGoalProgressBar::setBase(int base)
{
    ui->progressBar->setMinimum(base);
}

void WordGoalProgressBar::changeProgressBy(int progress)
{
    if(ui->progressBar->value() + progress > ui->progressBar->maximum())
        ui->progressBar->setMaximum(ui->progressBar->value() + progress);


    ui->progressBar->setValue(ui->progressBar->value() + progress);


    // set colors :

    int max = ui->progressBar->maximum();
    int val = ui->progressBar->value();

    int percent = val*100/max;



    QString red = QString::number(255*(100-percent)/100);
    QString green = QString::number(255*percent/100);



    ui->progressBar->setStyleSheet("QProgressBar {"
                                   "border: 2px solid grey;"
                                   "border-radius: 5px;"
                                   "text-align: center;"
                                   "}"
                                    "QProgressBar::chunk {"
                                   "background-color: rgb("+ red +","+ green + ", 0);"
                                   "}");

}



//--------- Context Menu :------------------------------------------------------------------


void WordGoalProgressBar::createActions()
{
    setGoalAct = new QAction(tr("&Set target"), this);
    setGoalAct->setStatusTip(tr("Set word count target"));
    connect(setGoalAct, SIGNAL(triggered()), this, SLOT(setGoalDialog()));

    resetAct = new QAction(QIcon(":/pics/edit-undo.png"),tr("&Reset"), this);
    resetAct->setStatusTip(tr("Reset the progress to zero"));
    connect(resetAct, SIGNAL(triggered()), ui->progressBar, SLOT(reset()));



}

void WordGoalProgressBar::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu menu(this);

    menu.addAction(setGoalAct);
    menu.addAction(resetAct);

    menu.exec(event->globalPos());
}

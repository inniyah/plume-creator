#if QT_VERSION >= 0x050000
#include <QtWidgets>
#endif
#include <QtGui>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "../../externals/quazip/JlCompress.h"
#include "../../src/common/utils.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QLabel *label = new QLabel("Version" + QApplication::applicationVersion());
    ui->statusBar->addPermanentWidget(label);


}

MainWindow::~MainWindow()
{
    delete ui;
}

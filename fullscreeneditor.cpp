#include <QtGui>

#include "fullscreeneditor.h"

FullscreenEditor::FullscreenEditor(QWidget *parent, QTextDocument *doc) :
    QWidget(parent)
{

    QTextEdit *fullTextEdit = new QTextEdit(this);
    fullTextEdit->setDocument(doc);


    QPushButton *exitFullscreenButton = new QPushButton(tr("Exit"), this);
    connect(exitFullscreenButton, SIGNAL(clicked()),this,SLOT(close()));




        QVBoxLayout *layout = new QVBoxLayout();
        layout->addWidget(fullTextEdit);
        layout->addWidget(exitFullscreenButton);
        setLayout(layout);




        showFullScreen();
}

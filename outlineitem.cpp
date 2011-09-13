#include <QtGui>

#include "outlineitem.h"

OutlineItem::OutlineItem(QWidget *parent) :
    QWidget(parent)
{

    QHBoxLayout *mainLayout = new QHBoxLayout;
    frame = new QFrame(this);
    frame->setObjectName("frame");

    QGridLayout *layout = new QGridLayout;

    titleEdit = new QLineEdit;
    synEdit = new QTextEdit;

    synLineCount = synEdit->document()->lineCount();

    QPushButton *writeButton = new QPushButton("w");
    writeButton->setObjectName("writeButton");
    expandButton = new QPushButton("e");
    collapseButton = new QPushButton("c");
    expandButton1 = new QPushButton("e1");
    collapseButton1 = new QPushButton("c1");

    listWidget = new QListWidget;
    listLabel = new QLabel(tr("Attendance :"));
    showListButton = new QPushButton("<<");
    hideListButton = new QPushButton(">>");

    noteEdit = new QTextEdit;

    noteLineCount = noteEdit->document()->lineCount();

    noteLabel = new QLabel(tr("Note :"));
    showNoteButton = new QPushButton(">>");
    hideNoteButton = new QPushButton("<<");

    QFrame *hSeparator = new QFrame;
    vSeparator1 = new QFrame;
    vSeparator2 = new QFrame;

    connect(synEdit, SIGNAL(textChanged()), this, SLOT(resizeExpandedText()));
    connect(noteEdit, SIGNAL(textChanged()), this, SLOT(resizeExpandedText()));
    int buttonsWidth(20);
    fixedHeight = 100;
    fixedWidth = 200;
    titleEdit->setFixedSize(fixedWidth - buttonsWidth,20);
    synEdit->setFixedSize(fixedWidth,fixedHeight);
    writeButton->setFixedSize(buttonsWidth,titleEdit->height());
    expandButton->setFixedSize(buttonsWidth,titleEdit->height());
    collapseButton->setFixedSize(buttonsWidth,titleEdit->height());
    expandButton1->setFixedSize(buttonsWidth,titleEdit->height());
    collapseButton1->setFixedSize(buttonsWidth,titleEdit->height());    listWidget->setFixedSize(200,fixedHeight);
    showListButton->setFixedSize(buttonsWidth,fixedHeight);
    hideListButton->setFixedSize(buttonsWidth,fixedHeight);
    noteEdit->setFixedSize(fixedWidth,fixedHeight);
    showNoteButton->setFixedSize(buttonsWidth,fixedHeight);
    hideNoteButton->setFixedSize(buttonsWidth,fixedHeight);
    hSeparator->setFixedHeight(2);
    vSeparator1->setFixedWidth(2);
    vSeparator2->setFixedWidth(2);

    //   setFixedWidth(fixedWidth*4);


    titleEdit->setContentsMargins(0,0,0,0);
    synEdit->setContentsMargins(0,0,0,0);
    writeButton->setContentsMargins(0,0,0,0);
    expandButton->setContentsMargins(0,0,0,0);
    collapseButton->setContentsMargins(0,0,0,0);
    expandButton1->setContentsMargins(0,0,0,0);
    collapseButton1->setContentsMargins(0,0,0,0);
    listWidget->setContentsMargins(0,0,0,0);
    showListButton->setContentsMargins(0,0,0,0);
    hideListButton->setContentsMargins(0,0,0,0);
    noteEdit->setContentsMargins(0,0,0,0);
    showNoteButton->setContentsMargins(0,0,0,0);
    hideNoteButton->setContentsMargins(0,0,0,0);
    hSeparator->setContentsMargins(0,0,0,0);
    vSeparator1->setContentsMargins(0,0,0,0);
    vSeparator2->setContentsMargins(0,0,0,0);

    titleEdit->setFrame(false);
    synEdit->setFrameShape(QFrame::NoFrame);
    listLabel->setAlignment(Qt::AlignCenter);
    listWidget->setFrameShape(QFrame::NoFrame);
    showListButton->setFlat(true);
    hideListButton->setFlat(true);
    noteLabel->setAlignment(Qt::AlignCenter);
    noteEdit->setFrameShape(QFrame::NoFrame);
    showNoteButton->setFlat(true);
    hideNoteButton->setFlat(true);
    expandButton->setFlat(true);
    collapseButton->setFlat(true);
    expandButton1->setFlat(true);
    collapseButton1->setFlat(true);
    hSeparator->setFrameShape(QFrame::HLine);
    hSeparator->setFrameShadow(QFrame::Plain);
    hSeparator->setLineWidth(2);
    vSeparator1->setFrameShape(QFrame::VLine);
    vSeparator1->setFrameShadow(QFrame::Plain);
    vSeparator1->setLineWidth(2);
    vSeparator2->setFrameShape(QFrame::VLine);
    vSeparator2->setFrameShadow(QFrame::Plain);
    vSeparator2->setLineWidth(2);

    layout->addWidget(expandButton, 0, 0);
    layout->addWidget(hideListButton, 2, 0, Qt::AlignVCenter);
    layout->addWidget(collapseButton, 3, 0, Qt::AlignBottom);
    layout->addWidget(listLabel, 0, 1);
    layout->addWidget(listWidget, 2, 1, 2, 1, Qt::AlignTop);
    layout->addWidget(expandButton1, 0, 2);
    layout->addWidget(showListButton, 2, 2, Qt::AlignVCenter);
    layout->addWidget(collapseButton1, 3, 2, Qt::AlignBottom);
    layout->addWidget(vSeparator1, 2, 3, 2, 1);

    layout->addWidget(titleEdit, 0, 4);
    layout->addWidget(writeButton, 0, 5);
    layout->addWidget(hSeparator, 1, 4, 1, 2);
    layout->addWidget(synEdit, 2, 4, 2, 2, Qt::AlignTop);


    layout->addWidget(vSeparator2, 2, 6, 2, 1);
    layout->addWidget(showNoteButton, 2, 7, 2, 1);
    layout->addWidget(noteLabel, 0, 8);
    layout->addWidget(noteEdit, 2, 8, 2, 1, Qt::AlignTop);
    layout->addWidget(hideNoteButton, 2, 9, 2, 1);

    layout->setSpacing(0);
    layout->setMargin(0);
    layout->setContentsMargins(0,0,0,0);



    baseStyleSheet ="QListWidget {border-width: 1px ; margin: 1px ;}"
            "QPushButton {border-width: 1px ; margin: 1px ;}"
            "QLineEdit {border-width: 1px ; margin: 1px ;}"
            "QTextEdit {border-width: 1px ; margin: 1px ;}";

    QString backgroundStyleSheet(" QFrame#frame {"
                                 "background-image: url(:/pics/parchemin_double.png);"
                                 "}"
                                 "QTextEdit, QListWidget, QLineEdit, QPushButton {"
                                 "background-color: rgba(0, 0, 0, 0%);"
                                 "}"
                                 "QPushButton#writeButton {"
                                 "border: 2px solid rgba(0, 0, 0, 10%);"
                                 "}");

    setStyleSheet(baseStyleSheet + backgroundStyleSheet);










    connect(showListButton,SIGNAL(clicked()),this,SLOT(showList()));
    connect(hideListButton,SIGNAL(clicked()),this,SLOT(hideList()));
    connect(showNoteButton,SIGNAL(clicked()),this,SLOT(showNote()));
    connect(hideNoteButton,SIGNAL(clicked()),this,SLOT(hideNote()));
    connect(writeButton,SIGNAL(clicked()),this,SLOT(writeThis()));
    connect(expandButton1,SIGNAL(clicked()),this,SLOT(expandTexts()));
    connect(collapseButton1,SIGNAL(clicked()),this,SLOT(collapseTexts()));
    connect(expandButton,SIGNAL(clicked()),this,SLOT(expandTexts()));
    connect(collapseButton,SIGNAL(clicked()),this,SLOT(collapseTexts()));



    connect(titleEdit, SIGNAL(editingFinished()), this, SLOT(editTitleSlot()));

    mainLayout->addStretch();
    mainLayout->addWidget(frame);
    mainLayout->addStretch();

    frame->setLayout(layout);
    frame->setFrameStyle(QFrame::Panel | QFrame::Raised);
    setLayout(mainLayout);

    itemWidth = width();

    hideList();
    hideNote();
    collapseTexts();
    updateSizeSlot();

    titleEdit->setPlaceholderText(tr("Title"));
    listWidget->addItem("List item sample");






}

//--------------------------------------------------------------------

void OutlineItem::showList()
{
    listLabel->show();
    listWidget->show();
    showListButton->hide();
    hideListButton->show();
    vSeparator1->show();


    if(m_expand){
        expandButton->hide();
        collapseButton->show();
        expandButton1->hide();
        collapseButton1->hide();
    }
    else{
        expandButton->show();
        collapseButton->hide();
        expandButton1 ->hide();
        collapseButton1->hide();
    }
    updateSizeSlot();

}

//--------------------------------------------------------------------

void OutlineItem::hideList()
{
    listLabel->hide();
    listWidget->hide();
    showListButton->show();
    hideListButton->hide();
    vSeparator1->hide();

    if(m_expand){
        expandButton->hide();
        collapseButton->hide();
        expandButton1->hide();
        collapseButton1->show();
    }
    else{
        expandButton->hide();
        collapseButton->hide();
        expandButton1->show();
        collapseButton1->hide();
    }

    updateSizeSlot();

}

//--------------------------------------------------------------------

void OutlineItem::showNote()
{
    noteEdit->show();
    noteLabel->show();
    showNoteButton->hide();
    hideNoteButton->show();
    vSeparator2->show();

    updateSizeSlot();

}

//--------------------------------------------------------------------


void OutlineItem::hideNote()
{
    noteEdit->hide();
    noteLabel->hide();
    showNoteButton->show();
    hideNoteButton->hide();
    vSeparator2->hide();

    updateSizeSlot();

}


//--------------------------------------------------------------------
void OutlineItem::writeThis()
{
    qDebug() << "writeThis name : " << this->objectName();
}

//--------------------------------------------------------------------

void OutlineItem::showListSlot(bool buttonToggled)
{
    if(buttonToggled)
        showList();
    else
        hideList();

    updateSizeSlot();

}

//--------------------------------------------------------------------

void OutlineItem::showNoteSlot(bool buttonToggled)
{
    if(buttonToggled)
        showNote();
    else
        hideNote();

    updateSizeSlot();

}

//--------------------------------------------------------------------

void OutlineItem::expandTexts(bool expand)
{
    m_expand = expand;
    if(expand){

        synEdit->setFixedHeight(synEdit->document()->size().rheight() + 40);
        noteEdit->setFixedHeight(noteEdit->document()->size().rheight() + 40);



        if(listWidget->isHidden()){
            expandButton->hide();
            collapseButton->hide();
            expandButton1->hide();
            collapseButton1->show();
        }
        else{
            expandButton->hide();
            collapseButton->show();
            expandButton1->hide();
            collapseButton1->hide();
        }
    }

    else{
        synEdit->setFixedHeight(fixedHeight);
        noteEdit->setFixedHeight(fixedHeight);

        if(listWidget->isHidden()){
            expandButton->hide();
            collapseButton->hide();
            expandButton1->show();
            collapseButton1->hide();
        }
        else{
            expandButton->show();
            collapseButton->hide();
            expandButton1->hide();
            collapseButton1->hide();
        }
    }
    updateSizeSlot();



    QString debug;
    qDebug() << "synEdit doc height = " << debug.setNum(synEdit->document()->size().rheight());
}


//--------------------------------------------------------------------

void OutlineItem::resizeExpandedText()
{
    int maxCount = qMax(synEdit->document()->lineCount(), noteEdit->document()->lineCount());
    int maxHeight = qMax(synEdit->document()->size().rheight(), noteEdit->document()->size().rheight());


    if(m_expand && maxCount > qMax(synLineCount, noteLineCount)){
        qDebug() << "resizing expanded text";
        synEdit->setFixedHeight(maxHeight + 40);
        noteEdit->setFixedHeight(maxHeight + 40);

        updateSizeSlot();
        //  emit resizingSignal();
    }


    synLineCount = synEdit->document()->lineCount();
    noteLineCount = noteEdit->document()->lineCount();
}

//--------------------------------------------------------------------

void OutlineItem::updateSizeSlot()
{
    qDebug() << "resizing slot" ;
    //   frame->adjustSize();
    adjustSize();
}



//--------------------------------------------------------------------

void OutlineItem::setTitle(QString title)
{
    disconnect(titleEdit, SIGNAL(editingFinished()), this, SLOT(editTitleSlot()));

    titleEdit->setText(title);

    connect(titleEdit, SIGNAL(editingFinished()), this, SLOT(editTitleSlot()));
}

//--------------------------------------------------------------------

void OutlineItem::editTitleSlot()
{

    emit newOutlineTitleSignal(titleEdit->text(), number);
}

//--------------------------------------------------------------------



void OutlineItem::setDocuments(QTextDocument *synDocument, QTextDocument *noteDocument)
{
    synDoc = synDocument;
    noteDoc = noteDocument;
    qDebug() << "jalon 1a";
    qDebug() << "jalon 1a" << synDocument->objectName();
    qDebug() << "jalon 1a" << noteDocument->objectName();

    synOutlineDoc = synDocument->clone(this);
    noteOutlineDoc = noteDocument->clone(this);
    qDebug() << "jalon 1b";

    synEdit->setDocument(synOutlineDoc);
    noteEdit->setDocument(noteOutlineDoc);

    qDebug() << "jalon 1c";

    synEdit->setObjectName(this->objectName() + "-synEdit");
    noteEdit->setObjectName(this->objectName() + "-noteEdit");


    connect(noteDoc, SIGNAL(contentsChange(int,int,int)), this, SLOT(updateNoteClone(int, int, int)));
    connect(noteOutlineDoc, SIGNAL(contentsChange(int,int,int)), this, SLOT(updateNote(int, int, int)));
    connect(synDoc, SIGNAL(contentsChange(int,int,int)), this, SLOT(updateSynClone(int, int, int)));
    connect(synOutlineDoc, SIGNAL(contentsChange(int,int,int)), this, SLOT(updateSyn(int, int, int)));


    // number :
    QString objName = this->objectName();
    number = objName.mid(objName.indexOf("_") + 1).toInt();

}


//-----------------------------------------------------------------------------------------


void OutlineItem::updateSynClone(int position, int removed, int added)
{

    disconnect(synOutlineDoc, SIGNAL(contentsChange(int,int,int)), this, SLOT(updateSyn(int, int, int)));


    QTextCursor *cursor = new QTextCursor(synDoc);
    QTextCursor *cloneCursor = new QTextCursor(synOutlineDoc);
    cursor->setPosition(position);
    cloneCursor->setPosition(position);

    if(removed != 0){
        cursor->movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor,removed);
        cloneCursor->movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor,removed);
        cloneCursor->deleteChar();
    }

    if(added != 0){
        cursor->setPosition(position);
        cloneCursor->setPosition(position);
        cursor->movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor,added);

        cloneCursor->insertFragment(cursor->selection());
    }
    //    if(added == 0)
    //        this->textCursor().setPosition(position);


    connect(synOutlineDoc, SIGNAL(contentsChange(int,int,int)), this, SLOT(updateSyn(int, int, int)));

    QString debug;
    QString debug1;
    QString debug2;
    qDebug() << "updateSynClone  " << debug.setNum(position) << " , "  << debug1.setNum(removed) << " , "  << debug2.setNum(added);

    qDebug() << "updateSynClone  " << this->objectName();
}


//-----------------------------------------------------------------------------------------


void OutlineItem::updateSyn(int position, int removed, int added)
{
    disconnect(synDoc, SIGNAL(contentsChange(int,int,int)), this, SLOT(updateSynClone(int, int, int)));



    QTextCursor *cursor = new QTextCursor(synDoc);
    QTextCursor *cloneCursor = new QTextCursor(synOutlineDoc);
    cursor->setPosition(position);
    cloneCursor->setPosition(position);

    if(removed != 0){
        cursor->movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor,removed);
        cloneCursor->movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor,removed);
        cursor->deleteChar();
    }

    if(added != 0){
        cursor->setPosition(position);
        cloneCursor->setPosition(position);
        cloneCursor->movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor,added);

        cursor->insertFragment(cloneCursor->selection());
    }


    connect(synDoc, SIGNAL(contentsChange(int,int,int)), this, SLOT(updateSynClone(int, int, int)));

    qDebug() << "updateSyn  " << this->objectName();
}
//-----------------------------------------------------------------------------------------


void OutlineItem::updateNoteClone(int position, int removed, int added)
{

    disconnect(noteOutlineDoc, SIGNAL(contentsChange(int,int,int)), this, SLOT(updateNote(int, int, int)));


    QTextCursor *cursor = new QTextCursor(noteDoc);
    QTextCursor *cloneCursor = new QTextCursor(noteOutlineDoc);
    cursor->setPosition(position);
    cloneCursor->setPosition(position);

    if(removed != 0){
        cursor->movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor,removed);
        cloneCursor->movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor,removed);
        cloneCursor->deleteChar();
    }

    if(added != 0){
        cursor->setPosition(position);
        cloneCursor->setPosition(position);
        cursor->movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor,added);

        cloneCursor->insertFragment(cursor->selection());
    }
    //    if(added == 0)
    //        this->textCursor().setPosition(position);


    connect(noteOutlineDoc, SIGNAL(contentsChange(int,int,int)), this, SLOT(updateNote(int, int, int)));

    qDebug() << "updateNoteClone  " << this->objectName();
}


//-----------------------------------------------------------------------------------------


void OutlineItem::updateNote(int position, int removed, int added)
{
    disconnect(noteDoc, SIGNAL(contentsChange(int,int,int)), this, SLOT(updateNoteClone(int, int, int)));



    QTextCursor *cursor = new QTextCursor(noteDoc);
    QTextCursor *cloneCursor = new QTextCursor(noteOutlineDoc);
    cursor->setPosition(position);
    cloneCursor->setPosition(position);

    if(removed != 0){
        cursor->movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor,removed);
        cloneCursor->movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor,removed);
        cursor->deleteChar();
    }

    if(added != 0){
        cursor->setPosition(position);
        cloneCursor->setPosition(position);
        cloneCursor->movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor,added);

        cursor->insertFragment(cloneCursor->selection());
    }


    connect(noteDoc, SIGNAL(contentsChange(int,int,int)), this, SLOT(updateNoteClone(int, int, int)));

    qDebug() << "updateNote  " << this->objectName();


}


//--------------------------------------------------------------------

void OutlineItem::resizeEvent (QResizeEvent *event)
{
    qDebug() << "resizing" ;
    //   frame->adjustSize();
    adjustSize();
    emit resizingSignal();
    event->accept();
}

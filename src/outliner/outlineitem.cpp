#include <QtGui>

#include "outliner/outlineitem.h"
//
OutlineItem::OutlineItem(QWidget *parent) :
    QWidget(parent)
{

    QHBoxLayout *mainLayout = new QHBoxLayout;
    frame = new QFrame(this);
    frame->setObjectName("frame");

    QGridLayout *layout = new QGridLayout;

    titleEdit = new QLineEdit;
    synEdit = new NoteZone;

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

    noteEdit = new NoteZone;

    noteLabel = new QLabel(tr("Note :"));
    showNoteButton = new QPushButton(">>");
    hideNoteButton = new QPushButton("<<");

    QFrame *hSeparator = new QFrame;
    vSeparator1 = new QFrame;
    vSeparator2 = new QFrame;


    synLineCount = synEdit->document()->size().rheight();
    noteLineCount = noteEdit->document()->size().rheight();


    connect(synEdit, SIGNAL(textChanged()), this, SLOT(resizeExpandedText()));
    connect(noteEdit, SIGNAL(textChanged()), this, SLOT(resizeExpandedText()));


    int buttonsWidth(20);
    fixedHeight = 100;
    fixedWidth = 200;
    //    titleEdit->setFixedSize(fixedWidth - buttonsWidth,20);
    titleEdit->setFixedHeight(25);
    //    synEdit->setFixedSize(fixedWidth,fixedHeight);
    writeButton->setFixedSize(buttonsWidth,titleEdit->height());
    expandButton->setFixedSize(buttonsWidth,titleEdit->height());
    collapseButton->setFixedSize(buttonsWidth,titleEdit->height());
    expandButton1->setFixedSize(buttonsWidth,titleEdit->height());
    collapseButton1->setFixedSize(buttonsWidth,titleEdit->height());
    //    listWidget->setFixedSize(200,fixedHeight);
    listWidget->setFixedHeight(noteEdit->height());
    //    showListButton->setFixedSize(buttonsWidth,fixedHeight);
    //    hideListButton->setFixedSize(buttonsWidth,fixedHeight);
    showListButton->setFixedWidth(buttonsWidth);
    showListButton->setMinimumHeight(fixedHeight);
    //    showListButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    hideListButton->setFixedWidth(buttonsWidth);
    hideListButton->setMinimumHeight(fixedHeight);
    //    noteEdit->setFixedSize(fixedWidth,fixedHeight);
    //    showNoteButton->setFixedSize(buttonsWidth,fixedHeight);
    //    hideNoteButton->setFixedSize(buttonsWidth,fixedHeight);
    showNoteButton->setFixedWidth(buttonsWidth);
    showNoteButton->setMinimumHeight(fixedHeight);
    hideNoteButton->setFixedWidth(buttonsWidth);
    hideNoteButton->setMinimumHeight(fixedHeight);
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
    titleEdit->setAlignment(Qt::AlignCenter);
    QFont font;
    font.setFamily("DejaVu Mono");
    font.setPointSize(15);
    font.setBold(true);
    titleEdit->setFont(font);




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
    layout->addWidget(showNoteButton, 2, 7, 2, 1, Qt::AlignVCenter);
    layout->addWidget(noteLabel, 0, 8);
    layout->addWidget(noteEdit, 2, 8, 2, 1, Qt::AlignTop);
    layout->addWidget(hideNoteButton, 2, 9, 2, 1, Qt::AlignVCenter);

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

    QString scrollbarStyleSheet("  QScrollBar:vertical {"
                                "border: 0px solid beige;"
                                "background-image: url(:/pics/parchemin_double.png);"
                                "width: 10px;"
                                "margin: 22px 0 22px 0;"
                                "}"
                                "QScrollBar::handle:vertical {"
                                "border: 2px solid brown;"
                                "background-image: url(:/pics/parchemin_double.png);"
                                "min-height: 10px;"
                                "}"
                                "QScrollBar::add-line:vertical {"
                                "border: 2px solid beige;"
                                "background-image: url(:/pics/parchemin_double.png);"
                                "height: 10px;"
                                "subcontrol-position: bottom;"
                                "subcontrol-origin: margin;"
                                "}"

                                "QScrollBar::sub-line:vertical {"
                                "border: 2px solid beige;"
                                "background-image: url(:/pics/parchemin_double.png);"
                                "height: 10px;"
                                "subcontrol-position: top;"
                                "subcontrol-origin: margin;"
                                "}"
                                "QScrollBar::up-arrow:vertical, QScrollBar::down-arrow:vertical {"
                                "border: 0px solid beige;"
                                "width: 3px;"
                                "height: 3px;"
                                "background: black;"
                                "}"
                                " QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical {"
                                "background: none;"
                                "}"
                                );


    setStyleSheet(baseStyleSheet + backgroundStyleSheet + scrollbarStyleSheet);










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



    //   applyConfig();

    emit resizingSignal();

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
    //   updateSizeSlot();
    emit resizingSignal();

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

    //    updateSizeSlot();
    emit resizingSignal();

}

//--------------------------------------------------------------------

void OutlineItem::showNote()
{
    noteEdit->show();
    noteLabel->show();
    showNoteButton->hide();
    hideNoteButton->show();
    vSeparator2->show();

    //   updateSizeSlot();
    emit resizingSignal();

}

//--------------------------------------------------------------------


void OutlineItem::hideNote()
{
    noteEdit->hide();
    noteLabel->hide();
    showNoteButton->show();
    hideNoteButton->hide();
    vSeparator2->hide();

    //   updateSizeSlot();
    emit resizingSignal();

}


//--------------------------------------------------------------------
void OutlineItem::writeThis()
{
//    qDebug() << "writeThis name : " << this->objectName();
    emit writeThisSignal(number);
}

//--------------------------------------------------------------------

void OutlineItem::showListSlot(bool buttonToggled)
{
    if(buttonToggled)
        showList();
    else
        hideList();

    //   updateSizeSlot();
    emit resizingSignal();

}

//--------------------------------------------------------------------

void OutlineItem::showNoteSlot(bool buttonToggled)
{
    if(buttonToggled)
        showNote();
    else
        hideNote();

    //    updateSizeSlot();
    emit resizingSignal();

}

//--------------------------------------------------------------------

void OutlineItem::expandTexts(bool expand)
{
    m_expand = expand;
    if(expand){



        synEdit->setFixedHeight(synEdit->document()->size().rheight() + 40);
        noteEdit->setFixedHeight(noteEdit->document()->size().rheight() + 40);
        listWidget->setFixedHeight(noteEdit->height());

//        qDebug() << "expandTexts";

        synEdit->append("o");
        QTextCursor *synCursor = new QTextCursor(synEdit->document());
        synCursor->movePosition(QTextCursor::End, QTextCursor::MoveAnchor,1);
        synCursor->deletePreviousChar();
        synCursor->deletePreviousChar();


noteEdit->append("o");
QTextCursor *noteCursor = new QTextCursor(noteEdit->document());
noteCursor->movePosition(QTextCursor::End, QTextCursor::MoveAnchor,1);
noteCursor->deletePreviousChar();
noteCursor->deletePreviousChar();

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
        listWidget->setFixedHeight(noteEdit->height());
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
    //    updateSizeSlot();

    frame->adjustSize();
    emit resizingSignal();
 //   resizeExpandedText();



    //    QString debug;
    //    qDebug() << "synEdit doc height = " << debug.setNum(synEdit->document()->size().rheight());
}


//--------------------------------------------------------------------

void OutlineItem::resizeExpandedText()
{
    int maxHeight = qMax(synEdit->document()->size().rheight(), noteEdit->document()->size().rheight());


    if(m_expand && maxHeight != qMax(synLineCount, noteLineCount)){
        //        qDebug() << "resizing expanded text";
        synEdit->setFixedHeight(maxHeight + 40);
        noteEdit->setFixedHeight(maxHeight + 40);
        listWidget->setFixedHeight(noteEdit->height());
        //       updateSizeSlot();
        emit resizingSignal();
    }


    synLineCount = synEdit->document()->size().rheight();
    noteLineCount = noteEdit->document()->size().rheight();
}

//--------------------------------------------------------------------

void OutlineItem::updateSizeSlot()
{
    //    qDebug() << "resizing slot" ;
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
void OutlineItem::changeListWidth(int width)
{
    listWidget->setFixedWidth(width);
    emit resizingSignal();

}

//--------------------------------------------------------------------

void OutlineItem::changeSynWidth(int width)
{
    synEdit->setFixedWidth(width);
    emit resizingSignal();
}

//--------------------------------------------------------------------

void OutlineItem::changeNoteWidth(int width)
{
    noteEdit->setFixedWidth(width);
    emit resizingSignal();
}


//--------------------------------------------------------------------
void OutlineItem::changeSynFont(QFont font)
{
    disconnect(synOutlineDoc, SIGNAL(contentsChange(int,int,int)), this, SLOT(updateSyn(int, int, int)));
    disconnect(synDoc, SIGNAL(contentsChange(int,int,int)), this, SLOT(updateSynClone(int, int, int)));

    synDefaultFont = font;

    QTextCharFormat charFormat;
    charFormat.setFontFamily(font.family());

    QTextCursor *tCursor = new QTextCursor(synEdit->document());
    tCursor->movePosition(QTextCursor::Start, QTextCursor::MoveAnchor,1);
    tCursor->movePosition(QTextCursor::End, QTextCursor::KeepAnchor,1);
    tCursor->mergeCharFormat(charFormat);



    connect(synOutlineDoc, SIGNAL(contentsChange(int,int,int)), this, SLOT(updateSyn(int, int, int)));
    connect(synDoc, SIGNAL(contentsChange(int,int,int)), this, SLOT(updateSynClone(int, int, int)));
}

//--------------------------------------------------------------------
void OutlineItem::changeSynTextHeight(int height)
{
    disconnect(synOutlineDoc, SIGNAL(contentsChange(int,int,int)), this, SLOT(updateSyn(int, int, int)));
    disconnect(synDoc, SIGNAL(contentsChange(int,int,int)), this, SLOT(updateSynClone(int, int, int)));

    synDefaultHeight = height;

    QTextCharFormat charFormat;
    charFormat.setFontPointSize(height);

    QTextCursor *tCursor = new QTextCursor(synEdit->document());
    tCursor->movePosition(QTextCursor::Start, QTextCursor::MoveAnchor,1);
    tCursor->movePosition(QTextCursor::End, QTextCursor::KeepAnchor,1);
    tCursor->mergeCharFormat(charFormat);

    connect(synOutlineDoc, SIGNAL(contentsChange(int,int,int)), this, SLOT(updateSyn(int, int, int)));
    connect(synDoc, SIGNAL(contentsChange(int,int,int)), this, SLOT(updateSynClone(int, int, int)));
}


//--------------------------------------------------------------------
void OutlineItem::changeSynIndent(int indent)
{
    disconnect(synOutlineDoc, SIGNAL(contentsChange(int,int,int)), this, SLOT(updateSyn(int, int, int)));
    disconnect(synDoc, SIGNAL(contentsChange(int,int,int)), this, SLOT(updateSynClone(int, int, int)));

    synDefaultIndent = indent;

    QTextBlockFormat blockFormat;
    blockFormat.setTextIndent(indent);

    QTextCursor *tCursor = new QTextCursor(synEdit->document());
    tCursor->movePosition(QTextCursor::Start, QTextCursor::MoveAnchor,1);
    tCursor->movePosition(QTextCursor::End, QTextCursor::KeepAnchor,1);
    tCursor->mergeBlockFormat(blockFormat);

    connect(synOutlineDoc, SIGNAL(contentsChange(int,int,int)), this, SLOT(updateSyn(int, int, int)));
    connect(synDoc, SIGNAL(contentsChange(int,int,int)), this, SLOT(updateSynClone(int, int, int)));
}
//--------------------------------------------------------------------
void OutlineItem::changeSynMargin(int margin)
{
    disconnect(synOutlineDoc, SIGNAL(contentsChange(int,int,int)), this, SLOT(updateSyn(int, int, int)));
    disconnect(synDoc, SIGNAL(contentsChange(int,int,int)), this, SLOT(updateSynClone(int, int, int)));

    synDefaultMargin = margin;

    QTextBlockFormat blockFormat;
    blockFormat.setBottomMargin(margin);

    QTextCursor *tCursor = new QTextCursor(synEdit->document());
    tCursor->movePosition(QTextCursor::Start, QTextCursor::MoveAnchor,1);
    tCursor->movePosition(QTextCursor::End, QTextCursor::KeepAnchor,1);
    tCursor->mergeBlockFormat(blockFormat);

    connect(synOutlineDoc, SIGNAL(contentsChange(int,int,int)), this, SLOT(updateSyn(int, int, int)));
    connect(synDoc, SIGNAL(contentsChange(int,int,int)), this, SLOT(updateSynClone(int, int, int)));
}
//--------------------------------------------------------------------
void OutlineItem::changeNoteFont(QFont font)
{
    disconnect(noteOutlineDoc, SIGNAL(contentsChange(int,int,int)), this, SLOT(updateNote(int, int, int)));
    disconnect(noteDoc, SIGNAL(contentsChange(int,int,int)), this, SLOT(updateNoteClone(int, int, int)));

    noteDefaultFont = font;

    QTextCharFormat charFormat;
    charFormat.setFontFamily(font.family());

    QTextCursor *tCursor = new QTextCursor(noteEdit->document());
    tCursor->movePosition(QTextCursor::Start, QTextCursor::MoveAnchor,1);
    tCursor->movePosition(QTextCursor::End, QTextCursor::KeepAnchor,1);
    tCursor->mergeCharFormat(charFormat);

    connect(noteOutlineDoc, SIGNAL(contentsChange(int,int,int)), this, SLOT(updateNote(int, int, int)));
    connect(noteDoc, SIGNAL(contentsChange(int,int,int)), this, SLOT(updateNoteClone(int, int, int)));
}

//--------------------------------------------------------------------
void OutlineItem::changeNoteTextHeight(int height)
{
    disconnect(noteOutlineDoc, SIGNAL(contentsChange(int,int,int)), this, SLOT(updateNote(int, int, int)));
    disconnect(noteDoc, SIGNAL(contentsChange(int,int,int)), this, SLOT(updateNoteClone(int, int, int)));

    noteDefaultHeight = height;


    QTextCharFormat charFormat;
    charFormat.setFontPointSize(height);

    QTextCursor *tCursor = new QTextCursor(noteEdit->document());
    tCursor->movePosition(QTextCursor::Start, QTextCursor::MoveAnchor,1);
    tCursor->movePosition(QTextCursor::End, QTextCursor::KeepAnchor,1);
    tCursor->mergeCharFormat(charFormat);

    connect(noteOutlineDoc, SIGNAL(contentsChange(int,int,int)), this, SLOT(updateNote(int, int, int)));
    connect(noteDoc, SIGNAL(contentsChange(int,int,int)), this, SLOT(updateNoteClone(int, int, int)));
}

//--------------------------------------------------------------------
void OutlineItem::changeNoteIndent(int indent)
{
    disconnect(noteOutlineDoc, SIGNAL(contentsChange(int,int,int)), this, SLOT(updateNote(int, int, int)));
    disconnect(noteDoc, SIGNAL(contentsChange(int,int,int)), this, SLOT(updateNoteClone(int, int, int)));

    noteDefaultIndent = indent;

    QTextBlockFormat blockFormat;
    blockFormat.setTextIndent(indent);

    QTextCursor *tCursor = new QTextCursor(noteEdit->document());
    tCursor->movePosition(QTextCursor::Start, QTextCursor::MoveAnchor,1);
    tCursor->movePosition(QTextCursor::End, QTextCursor::KeepAnchor,1);
    tCursor->mergeBlockFormat(blockFormat);

    connect(noteOutlineDoc, SIGNAL(contentsChange(int,int,int)), this, SLOT(updateNote(int, int, int)));
    connect(noteDoc, SIGNAL(contentsChange(int,int,int)), this, SLOT(updateNoteClone(int, int, int)));
}

//--------------------------------------------------------------------
void OutlineItem::changeNoteMargin(int margin)
{
    disconnect(noteOutlineDoc, SIGNAL(contentsChange(int,int,int)), this, SLOT(updateNote(int, int, int)));
    disconnect(noteDoc, SIGNAL(contentsChange(int,int,int)), this, SLOT(updateNoteClone(int, int, int)));

    noteDefaultMargin = margin;

    QTextBlockFormat blockFormat;
    blockFormat.setBottomMargin(margin);

    QTextCursor *tCursor = new QTextCursor(noteEdit->document());
    tCursor->movePosition(QTextCursor::Start, QTextCursor::MoveAnchor,1);
    tCursor->movePosition(QTextCursor::End, QTextCursor::KeepAnchor,1);
    tCursor->mergeBlockFormat(blockFormat);

    connect(noteOutlineDoc, SIGNAL(contentsChange(int,int,int)), this, SLOT(updateNote(int, int, int)));
    connect(noteDoc, SIGNAL(contentsChange(int,int,int)), this, SLOT(updateNoteClone(int, int, int)));
}
//--------------------------------------------------------------------

void OutlineItem::mergeFormatOnWordOrSelection(const QTextCharFormat &format)
{
    QTextCursor cursor = synEdit->textCursor();
    cursor.mergeCharFormat(format);
    synEdit->mergeCurrentCharFormat(format);
}

//--------------------------------------------------------------------

void OutlineItem::setDocuments(QTextDocument *synDocument, QTextDocument *noteDocument)
{
    synDoc = synDocument;
    noteDoc = noteDocument;
    //    qDebug() << "jalon 1a";
    //    qDebug() << "jalon 1a" << synDocument->objectName();
    //    qDebug() << "jalon 1a" << noteDocument->objectName();

    synOutlineDoc = synDocument->clone(this);
    noteOutlineDoc = noteDocument->clone(this);
    //    qDebug() << "jalon 1b";


    synEdit->openSyn(synOutlineDoc);
    noteEdit->openNote(noteOutlineDoc);

    //    qDebug() << "jalon 1c";

    synEdit->setObjectName(this->objectName() + "-synEdit");
    noteEdit->setObjectName(this->objectName() + "-noteEdit");


    connect(noteDoc, SIGNAL(contentsChange(int,int,int)), this, SLOT(updateNoteClone(int, int, int)));
    connect(noteOutlineDoc, SIGNAL(contentsChange(int,int,int)), this, SLOT(updateNote(int, int, int)));
    connect(synDoc, SIGNAL(contentsChange(int,int,int)), this, SLOT(updateSynClone(int, int, int)));
    connect(synOutlineDoc, SIGNAL(contentsChange(int,int,int)), this, SLOT(updateSyn(int, int, int)));

    connect(synOutlineDoc,SIGNAL(contentsChanged()), this, SLOT(setSynFirstLineFont()));
    connect(noteOutlineDoc,SIGNAL(contentsChanged()), this, SLOT(setNoteFirstLineFont()));


    // number :
    QString objName = this->objectName();
    number = objName.mid(objName.indexOf("_") + 1).toInt();


    resizeExpandedText();
    emit resizingSignal();


}


//-----------------------------------------------------------------------------------------


void OutlineItem::updateSynClone(int position, int removed, int added)
{

    disconnect(synOutlineDoc, SIGNAL(contentsChange(int,int,int)), this, SLOT(updateSyn(int, int, int)));
    disconnect(synDoc, SIGNAL(contentsChange(int,int,int)), this, SLOT(updateSynClone(int, int, int)));


    QTextCursor *cursor = new QTextCursor(synDoc);
    QTextCursor *cloneCursor = new QTextCursor(synOutlineDoc);
    cursor->setPosition(position);
    cloneCursor->setPosition(position);

//    if(synEdit->textCursor().blockNumber() == 0 || synEdit->textCursor().block().text() == ""){
//        QTextBlockFormat blockFormat;
//        blockFormat.setBottomMargin(synDefaultMargin);
//        blockFormat.setTextIndent(synDefaultIndent);

//        QTextCharFormat charFormat;
//        charFormat.setFontPointSize(synDefaultHeight);
//        charFormat.setFontFamily(synDefaultFont.family());

//        QTextCursor *ctCursor = new QTextCursor(synOutlineDoc);
//        ctCursor->movePosition(QTextCursor::Start, QTextCursor::MoveAnchor,1);
//        ctCursor->movePosition(QTextCursor::End, QTextCursor::KeepAnchor,1);
//        ctCursor->mergeCharFormat(charFormat);
//        ctCursor->mergeBlockFormat(blockFormat);

//        blockFormat.setBottomMargin(synDoc->firstBlock().blockFormat().bottomMargin());
//        blockFormat.setTextIndent(synDoc->firstBlock().blockFormat().textIndent());
//        charFormat.setFont(synDoc->defaultFont());

//        QTextCursor *tCursor = new QTextCursor(synDoc);
//        tCursor->movePosition(QTextCursor::Start, QTextCursor::MoveAnchor,1);
//        tCursor->movePosition(QTextCursor::End, QTextCursor::KeepAnchor,1);
//        tCursor->mergeCharFormat(charFormat);
//        tCursor->mergeBlockFormat(blockFormat);

//    }

    QTextBlockFormat blockFormat;
    blockFormat.setBottomMargin(synDefaultMargin);
    blockFormat.setTextIndent(synDefaultIndent);
    int synHeight = synDefaultHeight;
    QString family = synDefaultFont.family();

    //    QString debug4;
    //    qDebug() << "synHeight : " << debug4.setNum(synHeight);
    //    qDebug() << "family : " << family;

    if(removed != 0){
        cursor->movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor,removed);
        cloneCursor->movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor,removed);
        cloneCursor->deleteChar();
    }

    if(added != 0){
        cursor->setPosition(position);
        cloneCursor->setPosition(position);
        cursor->movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor,added);

        QTextEdit *editor = new QTextEdit;
        QTextDocumentFragment fragment = cursor->selection();
        editor->append(fragment.toHtml());

        QTextCharFormat charFormat;
        charFormat.setFontPointSize(synHeight);
        charFormat.setFontFamily(family);

        QTextCursor *tCursor = new QTextCursor(editor->document());
        tCursor->movePosition(QTextCursor::Start, QTextCursor::MoveAnchor,1);
        tCursor->movePosition(QTextCursor::End, QTextCursor::KeepAnchor,1);
        tCursor->mergeCharFormat(charFormat);
        tCursor->mergeBlockFormat(blockFormat);

        cloneCursor->insertHtml(editor->document()->toHtml());
    }
    //    if(added == 0)
    //        this->textCursor().setPosition(position);


    connect(synOutlineDoc, SIGNAL(contentsChange(int,int,int)), this, SLOT(updateSyn(int, int, int)));
    connect(synDoc, SIGNAL(contentsChange(int,int,int)), this, SLOT(updateSynClone(int, int, int)));

    //    QString debug;
    //    QString debug1;
    //    QString debug2;
    //    qDebug() << "updateSynClone  " << debug.setNum(position) << " , "  << debug1.setNum(removed) << " , "  << debug2.setNum(added);

    //    qDebug() << "updateSynClone  " << this->objectName();
}


//-----------------------------------------------------------------------------------------


void OutlineItem::updateSyn(int position, int removed, int added)
{
    disconnect(synDoc, SIGNAL(contentsChange(int,int,int)), this, SLOT(updateSynClone(int, int, int)));
    disconnect(synOutlineDoc, SIGNAL(contentsChange(int,int,int)), this, SLOT(updateSyn(int, int, int)));



    QTextCursor *cursor = new QTextCursor(synDoc);
    QTextCursor *cloneCursor = new QTextCursor(synOutlineDoc);
    cursor->setPosition(position);
    cloneCursor->setPosition(position);


//    if(synEdit->textCursor().blockNumber() == 0 || synEdit->textCursor().block().text() == "" ){
//        QTextBlockFormat blockFormat;
//        blockFormat.setBottomMargin(synDefaultMargin);
//        blockFormat.setTextIndent(synDefaultIndent);

//        QTextCharFormat charFormat;
//        charFormat.setFontPointSize(synDefaultHeight);
//        charFormat.setFontFamily(synDefaultFont.family());

//        QTextCursor *ctCursor = new QTextCursor(synOutlineDoc);
//        ctCursor->movePosition(QTextCursor::Start, QTextCursor::MoveAnchor,1);
//        ctCursor->movePosition(QTextCursor::End, QTextCursor::KeepAnchor,1);
//        ctCursor->mergeCharFormat(charFormat);
//        ctCursor->mergeBlockFormat(blockFormat);

//        blockFormat.setBottomMargin(synDoc->firstBlock().blockFormat().bottomMargin());
//        blockFormat.setTextIndent(synDoc->firstBlock().blockFormat().textIndent());
//        charFormat.setFont(synDoc->defaultFont());

//        QTextCursor *tCursor = new QTextCursor(synDoc);
//        tCursor->movePosition(QTextCursor::Start, QTextCursor::MoveAnchor,1);
//        tCursor->movePosition(QTextCursor::End, QTextCursor::KeepAnchor,1);
//        tCursor->mergeCharFormat(charFormat);
//        tCursor->mergeBlockFormat(blockFormat);

//    }

    QTextBlockFormat blockFormat;
    blockFormat.setBottomMargin(synDoc->firstBlock().blockFormat().bottomMargin());
    blockFormat.setTextIndent(synDoc->firstBlock().blockFormat().textIndent());
    int synHeight = cursor->charFormat().fontPointSize();
    QString family = cursor->charFormat().fontFamily();

    if(removed != 0){
        cursor->movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor,removed);
        cloneCursor->movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor,removed);
        cursor->deleteChar();
    }

    if(added != 0){
        cursor->setPosition(position);
        cloneCursor->setPosition(position);
        cloneCursor->movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor,added);

        QTextEdit *editor = new QTextEdit;
        QTextDocumentFragment fragment = cloneCursor->selection();
        editor->append(fragment.toHtml());

        QTextCharFormat charFormat;
        charFormat.setFontPointSize(synHeight);
        charFormat.setFontFamily(family);

        QTextCursor *tCursor = new QTextCursor(editor->document());
        tCursor->movePosition(QTextCursor::Start, QTextCursor::MoveAnchor,1);
        tCursor->movePosition(QTextCursor::End, QTextCursor::KeepAnchor,1);
        tCursor->mergeCharFormat(charFormat);
        tCursor->mergeBlockFormat(blockFormat);

        cursor->insertHtml(editor->document()->toHtml());
    }


    connect(synDoc, SIGNAL(contentsChange(int,int,int)), this, SLOT(updateSynClone(int, int, int)));
    connect(synOutlineDoc, SIGNAL(contentsChange(int,int,int)), this, SLOT(updateSyn(int, int, int)));


    //    QString debug;
    //    QString debug1;
    //    QString debug2;
    //    qDebug() << "updateSyn :  " << debug.setNum(position) << " , "  << debug1.setNum(removed) << " , "  << debug2.setNum(added);

    //    qDebug() << "updateSyn  " << this->objectName();
}
//-----------------------------------------------------------------------------------------


void OutlineItem::updateNoteClone(int position, int removed, int added)
{

    disconnect(noteOutlineDoc, SIGNAL(contentsChange(int,int,int)), this, SLOT(updateNote(int, int, int)));
    disconnect(noteDoc, SIGNAL(contentsChange(int,int,int)), this, SLOT(updateNoteClone(int, int, int)));


    QTextCursor *cursor = new QTextCursor(noteDoc);
    QTextCursor *cloneCursor = new QTextCursor(noteOutlineDoc);
    cursor->setPosition(position);
    cloneCursor->setPosition(position);

//    if(noteDoc->firstBlock().text() != noteOutlineDoc->firstBlock().text() || noteEdit->textCursor().block().text() == ""){

//        QTextBlockFormat blockFormat;
//        blockFormat.setBottomMargin(noteDefaultMargin);
//        blockFormat.setTextIndent(noteDefaultIndent);

//        QTextCharFormat charFormat;
//        charFormat.setFontPointSize(noteDefaultHeight);
//        charFormat.setFontFamily(noteDefaultFont.family());

//        QTextCursor *ctCursor = new QTextCursor(noteOutlineDoc);
//        ctCursor->movePosition(QTextCursor::Start, QTextCursor::MoveAnchor,1);
//        ctCursor->movePosition(QTextCursor::End, QTextCursor::KeepAnchor,1);
//        ctCursor->mergeCharFormat(charFormat);
//        ctCursor->mergeBlockFormat(blockFormat);

//        blockFormat.setBottomMargin(noteDoc->firstBlock().blockFormat().bottomMargin());
//        blockFormat.setTextIndent(noteDoc->firstBlock().blockFormat().textIndent());
//        charFormat.setFont(noteDoc->defaultFont());

//        QTextCursor *tCursor = new QTextCursor(noteDoc);
//        tCursor->movePosition(QTextCursor::Start, QTextCursor::MoveAnchor,1);
//        tCursor->movePosition(QTextCursor::End, QTextCursor::KeepAnchor,1);
//        tCursor->mergeCharFormat(charFormat);
//        tCursor->mergeBlockFormat(blockFormat);

//    }

    QTextBlockFormat blockFormat;
    blockFormat.setBottomMargin(noteDefaultMargin);
    blockFormat.setTextIndent(noteDefaultIndent);
    int noteHeight = noteDefaultHeight;
    QString family = noteDefaultFont.family();

    if(removed != 0){
        cursor->movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor,removed);
        cloneCursor->movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor,removed);
        cloneCursor->deleteChar();
    }

    if(added != 0){
        cursor->setPosition(position);
        cloneCursor->setPosition(position);
        cursor->movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor,added);

        QTextEdit *editor = new QTextEdit;
        QTextDocumentFragment fragment = cursor->selection();
        editor->append(fragment.toHtml());

        QTextCharFormat charFormat;
        charFormat.setFontPointSize(noteHeight);
        charFormat.setFontFamily(family);

        QTextCursor *tCursor = new QTextCursor(editor->document());
        tCursor->movePosition(QTextCursor::Start, QTextCursor::MoveAnchor,1);
        tCursor->movePosition(QTextCursor::End, QTextCursor::KeepAnchor,1);
        tCursor->mergeCharFormat(charFormat);
        tCursor->mergeBlockFormat(blockFormat);

        cloneCursor->insertHtml(editor->document()->toHtml());
    }
    //    if(added == 0)
    //        this->textCursor().setPosition(position);


    connect(noteOutlineDoc, SIGNAL(contentsChange(int,int,int)), this, SLOT(updateNote(int, int, int)));
    connect(noteDoc, SIGNAL(contentsChange(int,int,int)), this, SLOT(updateNoteClone(int, int, int)));

    //    qDebug() << "updateNoteClone  " << this->objectName();
}


//-----------------------------------------------------------------------------------------


void OutlineItem::updateNote(int position, int removed, int added)
{
    disconnect(noteDoc, SIGNAL(contentsChange(int,int,int)), this, SLOT(updateNoteClone(int, int, int)));
    disconnect(noteOutlineDoc, SIGNAL(contentsChange(int,int,int)), this, SLOT(updateNote(int, int, int)));



    QTextCursor *cursor = new QTextCursor(noteDoc);
    QTextCursor *cloneCursor = new QTextCursor(noteOutlineDoc);
    cursor->setPosition(position);
    cloneCursor->setPosition(position);

//    if(noteDoc->firstBlock().text() != noteOutlineDoc->firstBlock().text() || noteEdit->textCursor().block().text() == ""){

//        QTextBlockFormat blockFormat;
//        blockFormat.setBottomMargin(noteDefaultMargin);
//        blockFormat.setTextIndent(noteDefaultIndent);

//        QTextCharFormat charFormat;
//        charFormat.setFontPointSize(noteDefaultHeight);
//        charFormat.setFontFamily(noteDefaultFont.family());

//        QTextCursor *ctCursor = new QTextCursor(noteOutlineDoc);
//        ctCursor->movePosition(QTextCursor::Start, QTextCursor::MoveAnchor,1);
//        ctCursor->movePosition(QTextCursor::End, QTextCursor::KeepAnchor,1);
//        ctCursor->mergeCharFormat(charFormat);
//        ctCursor->mergeBlockFormat(blockFormat);

//        blockFormat.setBottomMargin(noteDoc->firstBlock().blockFormat().bottomMargin());
//        blockFormat.setTextIndent(noteDoc->firstBlock().blockFormat().textIndent());
//        charFormat.setFont(noteDoc->defaultFont());

//        QTextCursor *tCursor = new QTextCursor(noteDoc);
//        tCursor->movePosition(QTextCursor::Start, QTextCursor::MoveAnchor,1);
//        tCursor->movePosition(QTextCursor::End, QTextCursor::KeepAnchor,1);
//        tCursor->mergeCharFormat(charFormat);
//        tCursor->mergeBlockFormat(blockFormat);

//    }

    QTextBlockFormat blockFormat;
    blockFormat.setBottomMargin(noteDoc->firstBlock().blockFormat().bottomMargin());
    blockFormat.setTextIndent(noteDoc->firstBlock().blockFormat().textIndent());
    int noteHeight = cursor->charFormat().fontPointSize();
    QString family = cursor->charFormat().fontFamily();

    if(removed != 0){
        cursor->movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor,removed);
        cloneCursor->movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor,removed);
        cursor->deleteChar();
    }

    if(added != 0){
        cursor->setPosition(position);
        cloneCursor->setPosition(position);
        cloneCursor->movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor,added);

        QTextEdit *editor = new QTextEdit;
        QTextDocumentFragment fragment = cloneCursor->selection();
        editor->append(fragment.toHtml());

        QTextCharFormat charFormat;
        charFormat.setFontPointSize(noteHeight);
        charFormat.setFontFamily(family);

        QTextCursor *tCursor = new QTextCursor(editor->document());
        tCursor->movePosition(QTextCursor::Start, QTextCursor::MoveAnchor,1);
        tCursor->movePosition(QTextCursor::End, QTextCursor::KeepAnchor,1);
        tCursor->mergeCharFormat(charFormat);
tCursor->mergeBlockFormat(blockFormat);

        cursor->insertHtml(editor->document()->toHtml());
    }


    connect(noteDoc, SIGNAL(contentsChange(int,int,int)), this, SLOT(updateNoteClone(int, int, int)));
    connect(noteOutlineDoc, SIGNAL(contentsChange(int,int,int)), this, SLOT(updateNote(int, int, int)));

    //    qDebug() << "updateNote  " << this->objectName();


}

//-------------------------------------------------------------------

void OutlineItem::connectUpdateTextsSlot()
{
    connect(synDoc, SIGNAL(contentsChange(int,int,int)), this, SLOT(updateSynClone(int, int, int)));
    connect(synOutlineDoc, SIGNAL(contentsChange(int,int,int)), this, SLOT(updateSyn(int, int, int)));
    connect(noteDoc, SIGNAL(contentsChange(int,int,int)), this, SLOT(updateNoteClone(int, int, int)));
    connect(noteOutlineDoc, SIGNAL(contentsChange(int,int,int)), this, SLOT(updateNote(int, int, int)));

    connect(noteOutlineDoc,SIGNAL(contentsChanged()), this, SLOT(setNoteFirstLineFont()));
    connect(synOutlineDoc,SIGNAL(contentsChanged()), this, SLOT(setSynFirstLineFont()));

}
//-------------------------------------------------------------------
void OutlineItem::disconnectUpdateTextsSlot()
{
    disconnect(synDoc, SIGNAL(contentsChange(int,int,int)), this, SLOT(updateSynClone(int, int, int)));
    disconnect(synOutlineDoc, SIGNAL(contentsChange(int,int,int)), this, SLOT(updateSyn(int, int, int)));
    disconnect(noteDoc, SIGNAL(contentsChange(int,int,int)), this, SLOT(updateNoteClone(int, int, int)));
    disconnect(noteOutlineDoc, SIGNAL(contentsChange(int,int,int)), this, SLOT(updateNote(int, int, int)));

    disconnect(noteOutlineDoc,SIGNAL(contentsChanged()), this, SLOT(setNoteFirstLineFont()));
    disconnect(synOutlineDoc,SIGNAL(contentsChanged()), this, SLOT(setSynFirstLineFont()));

}

//--------------------------------------------------------------------

void OutlineItem::resizeEvent (QResizeEvent *event)
{
    //   qDebug() << "resizing" ;
    //    frame->adjustSize();
    //  adjustSize();
    emit resizingSignal();
    event->accept();
}

//--------------------------------------------------------------------

void OutlineItem::setSynFirstLineFont()
{
            if(synEdit->textCursor().atStart() == true
                    || synEdit->textCursor().position() == 1
                    || synEdit->canPaste() == true)
applySynFont();

}
//--------------------------------------------------------------------

void OutlineItem::setNoteFirstLineFont()
{
            if(noteEdit->textCursor().atStart() == true
                    || noteEdit->textCursor().position() == 1
                    || synEdit->canPaste() == true)
    applyNoteFont();

}

//--------------------------------------------------------------------------------



void OutlineItem::applySynFont()
{
    disconnectUpdateTextsSlot();

    QTextCharFormat charFormat;
    charFormat.setFontPointSize(synDefaultHeight);
    charFormat.setFontFamily(synDefaultFont.family());
    QTextBlockFormat blockFormat;
    blockFormat.setBottomMargin(synDefaultMargin);
    blockFormat.setTextIndent(synDefaultIndent);




    QTextCursor *tCursor = new QTextCursor(synOutlineDoc);
    tCursor->movePosition(QTextCursor::Start, QTextCursor::MoveAnchor,1);
    tCursor->movePosition(QTextCursor::End, QTextCursor::KeepAnchor,1);
    tCursor->mergeCharFormat(charFormat);
    tCursor->mergeBlockFormat(blockFormat);

    QFont font;
    font.setFamily(synDefaultFont.family());
    font.setPointSize(synDefaultHeight);
    synOutlineDoc->setDefaultFont(font);

    connectUpdateTextsSlot();
}

//--------------------------------------------------------------------------------


void OutlineItem::applyNoteFont()
{
    disconnectUpdateTextsSlot();

        QTextCharFormat charFormat;
        charFormat.setFontPointSize(noteDefaultHeight);
        charFormat.setFontFamily(noteDefaultFont.family());
        QTextBlockFormat blockFormat;
        blockFormat.setBottomMargin(noteDefaultMargin);
        blockFormat.setTextIndent(noteDefaultIndent);

      QTextCursor *tCursor = new QTextCursor(noteOutlineDoc);
      tCursor->movePosition(QTextCursor::Start, QTextCursor::MoveAnchor,1);
      tCursor->movePosition(QTextCursor::End, QTextCursor::KeepAnchor,1);
      tCursor->mergeCharFormat(charFormat);
      tCursor->mergeBlockFormat(blockFormat);

      QFont font;
      font.setFamily(noteDefaultFont.family());
      font.setPointSize(noteDefaultHeight);
      synOutlineDoc->setDefaultFont(font);

    connectUpdateTextsSlot();
}











//-------------------------------------------------------------------------
//------Attendance----------------------------------------------------------
//-------------------------------------------------------------------------

bool OutlineItem::showSheetAttendanceList(QList<QListWidgetItem *> *itemList)
{

    // clear list :
    while(listWidget->count() != 0)
        listWidget->takeItem(0);


    while(!itemList->isEmpty()){
        QListWidgetItem *item = itemList->takeFirst()->clone();
        listWidget->addItem(item);
    }








//    addAttendManagerButton();

//    connect(attendList, SIGNAL(itemDoubleClicked(QListWidgetItem*)),
//            this, SLOT(itemDoubleClick(QListWidgetItem*)));


    return true;

}


//---------------------------------------------------------------------------------
//--------------Apply Config----------------------------------------------------
//---------------------------------------------------------------------------------

void OutlineItem::applyConfig()
{


}

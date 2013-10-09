#if QT_VERSION >= 0x050000
#include <QtWidgets>
#endif 
#include <QtGui>   

#include "texttab.h"
#include "ui_texttab.h"
//


TextTab::TextTab(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TextTab), m_idNumber(-1), firstTime(true)
{
    ui->setupUi(this);
    css = this->styleSheet();



    prevTextDocument = new MainTextDocument(this);
    textDocument = new MainTextDocument(this);
    nextTextDocument = new MainTextDocument(this);

    QScrollBar *baseScrollBar = ui->textZone->verticalScrollBar();

    connect(baseScrollBar, SIGNAL(rangeChanged(int,int)), this, SLOT(scrollBar_setRange(int, int)));
    connect(baseScrollBar, SIGNAL(valueChanged(int)), ui->verticalScrollBar, SLOT(setValue(int)));
    connect(ui->verticalScrollBar, SIGNAL(valueChanged(int)), baseScrollBar, SLOT(setValue(int)));
    ui->textZone->verticalScrollBar()->hide();
    //    ui->textZone->setVerticalScrollBar(vertScrollBar);


    ui->leftHandle->setSide(SizeHandle::Left);
    ui->rightHandle->setSide(SizeHandle::Right);



    setContextMenuPolicy(Qt::PreventContextMenu);




    ui->prevTextZone->hide();
    ui->nextTextZone->hide();


    QList<int> sizesList;
    sizesList << 0 << ui->textZoneWidget->height()  << 0;
    ui->splitter->setSizes(sizesList);




    overlay = new Overlay(this);
    connect(ui->textZone, SIGNAL(textZoneResized(QRect)), overlay,SLOT(editZoneMoved(QRect)));

    connect(ui->textZone, SIGNAL(textZoneResized(QRect)), this, SLOT(editZoneMoved(QRect)));

    connect(ui->textZone, SIGNAL(textZoneResized(QRect)), ui->editToolBar, SLOT(editZoneMoved(QRect)));
    connect(ui->editToolBar, SIGNAL(actionSignal(QString,bool)), ui->textZone, SLOT(actionSlot(QString,bool)));
    connect(ui->textZone, SIGNAL(cursorStateSignal(QString,bool)), ui->editToolBar, SLOT(cursorStateSlot(QString,bool)));



    giveStyle();

    connect(ui->leftHandle, SIGNAL(modifySizeSignal(int)), this, SLOT(modifySize(int)));
    connect(ui->rightHandle, SIGNAL(modifySizeSignal(int)), this, SLOT(modifySize(int)));

}

TextTab::~TextTab()
{

}

void TextTab::setTextStyles(TextStyles *styles)
{
    textStyles = styles;


}



//---------------------------------------------------------------------------------------

bool TextTab::openText(MainTextDocument *doc)
{

    ui->prevTextZone->setTextStyles(textStyles);
    ui->prevTextZone->createContent();
    ui->nextTextZone->setTextStyles(textStyles);
    ui->nextTextZone->createContent();
    ui->textZone->setTextStyles(textStyles);
    ui->textZone->createContent();






    //  stackName = name;

    textDocument = doc;
    QApplication::processEvents();
    ui->textZone->setDoc(textDocument);
    QApplication::processEvents();
    ui->textZone->document()->adjustSize();

    // slim find replace :

    this->addSlimFindReplaceAction();
    ui->findReplace->setDocument(textDocument);
    ui->findReplace->setHub(hub);
    ui->findReplace->setTextEdit(ui->textZone);
    ui->findReplace->postConstructor();
    ui->findReplace->hide();


    //    connect(ui->textZone,SIGNAL(textChanged()), this,SIGNAL(textChangedSignal()));
    //    connect(ui->textZone,SIGNAL(manageStylesSignal()), this,SIGNAL(manageStylesSignal()));

    connect(ui->textZone,SIGNAL(charFormatChangedSignal(QTextCharFormat)), this,SIGNAL(charFormatChangedSignal(QTextCharFormat)));
    connect(ui->prevTextZone,SIGNAL(charFormatChangedSignal(QTextCharFormat)), this,SIGNAL(charFormatChangedSignal(QTextCharFormat)));
    connect(ui->nextTextZone,SIGNAL(charFormatChangedSignal(QTextCharFormat)), this,SIGNAL(charFormatChangedSignal(QTextCharFormat)));
    connect(ui->textZone,SIGNAL(textChanged()), this,SIGNAL(textChangedSignal()));
    connect(ui->prevTextZone,SIGNAL(textChanged()), this,SIGNAL(textChangedSignal()));
    connect(ui->nextTextZone,SIGNAL(textChanged()), this,SIGNAL(textChangedSignal()));
    connect(ui->textZone,SIGNAL(manageStylesSignal()), this,SIGNAL(manageStylesSignal()));
    connect(ui->prevTextZone,SIGNAL(manageStylesSignal()), this,SIGNAL(manageStylesSignal()));
    connect(ui->nextTextZone,SIGNAL(manageStylesSignal()), this,SIGNAL(manageStylesSignal()));
    connect(ui->textZone, SIGNAL(setStyleSelectionSignal(int)),this , SIGNAL(setStyleSelectionSignal(int)));
    connect(ui->prevTextZone, SIGNAL(setStyleSelectionSignal(int)),this , SIGNAL(setStyleSelectionSignal(int)));
    connect(ui->nextTextZone, SIGNAL(setStyleSelectionSignal(int)),this , SIGNAL(setStyleSelectionSignal(int)));
    connect(this, SIGNAL(changeTextStyleSignal(int)), ui->textZone, SLOT(changeTextStyleSlot(int)));
    connect(this, SIGNAL(changeTextStyleSignal(int)), ui->prevTextZone, SLOT(changeTextStyleSlot(int)));
    connect(this, SIGNAL(changeTextStyleSignal(int)), ui->nextTextZone, SLOT(changeTextStyleSlot(int)));
    connect(ui->textZone, SIGNAL(activateSpellcheckSignal(bool)),this , SIGNAL(activateSpellcheckSignal(bool)));
    connect(ui->prevTextZone, SIGNAL(activateSpellcheckSignal(bool)),this , SIGNAL(activateSpellcheckSignal(bool)));
    connect(ui->nextTextZone, SIGNAL(activateSpellcheckSignal(bool)),this , SIGNAL(activateSpellcheckSignal(bool)));

    //    QString debug;
    //    qDebug() << "doc witdh : " << debug.setNum(textDocument->textWidth());
    //    qDebug() << "doc witdh : " << debug.setNum(doc->textWidth());





    return true;

}

//---------------------------------------------------------------------------------------


//bool TextTab::saveText(QFile *textFile, QString name)
//{

//    if(name != stackName)
//        return false;



//    QTextDocumentWriter docWriter(textFile, "HTML");
//    bool written = docWriter.write(textDocument);




//    return written;


//}


//-------------------------------------------------------------------------------
MainTextDocument* TextTab::document()
{
    return textDocument;
}
//-------------------------------------------------------------------------------

TextZone *TextTab::textZone()
{
    return ui->textZone;
}

//-------------------------------------------------------------------------------

void TextTab::changeWidthSlot(int width)
{


    QSettings settings;
    if(width == -1){
        width = settings.value("Settings/TextArea/textWidth", this->width()/2 ).toInt();
        //        qDebug() << "eeeee : " + QString::number(width);
    }




    ui->textZone->setFixedWidth(width);
    //    ui->textZone->document()->setTextWidth(width);
    ui->prevTextZone->setFixedWidth(width);
    //    ui->prevTextZone->document()->setTextWidth(width - scrollBarWidth);
    ui->nextTextZone->setFixedWidth(width);
    //    ui->nextTextZone->document()->setTextWidth(width - scrollBarWidth);

    settings.setValue("Settings/TextArea/textWidth", width);

}
//-------------------------------------------------------------------------------
void TextTab::changeTextFontSlot(QFont font)
{
    ui->textZone->setTextFont(font);
}
//-------------------------------------------------------------------------------
void TextTab::changeTextHeightSlot(int height)
{
    ui->textZone->setTextHeight(height);
}

//-------------------------------------------------------------------------------


//-------------------------------------------------------------------------------
void TextTab::setTextFocus()
{
    if(ui->textZone->isVisible())
        ui->textZone->setFocus();

}
//-------------------------------------------------------------------------------
void TextTab::setCursorPos(int pos)
{
    ui->textZone->moveCursor(QTextCursor::Start, QTextCursor::MoveAnchor);
    for(int i = 0; i < pos ; i++)
        ui->textZone->moveCursor(QTextCursor::NextCharacter, QTextCursor::MoveAnchor);


    ui->textZone->ensureCursorVisible();
}
//-------------------------------------------------------------------------------
int TextTab::cursorPos()
{
    QTextCursor cursor = ui->textZone->textCursor();
    return cursor.position();

}


//-------------------------------------------------------------------------------

//-------------------------------------------------------------------------------

void TextTab::showPrevText(bool showPrevTextBool)
{
    ui->prevTextZone->setHidden(!showPrevTextBool);
    ui->prevTextZone->setMaximumHeight(ui->textZone->height()/3);
    ui->textZone->setFocus();
    ui->textZone->ensureCursorVisible();

}

//-------------------------------------------------------------------------------
bool TextTab::setShowPrevTextButton()
{
    return !ui->prevTextZone->isHidden();
}

//-------------------------------------------------------------------------------
void  TextTab::setPrevText(MainTextDocument *prevDoc)
{
    if(prevDoc == 0){
        ui->prevTextZone->setHidden(true);
        ui->prevTextToolButton->setHidden(true);
        return;
    }
    ui->prevTextZone->setHidden(false);
    ui->prevTextToolButton->setHidden(false);

    prevTextDocument = prevDoc;
    ui->prevTextZone->setDoc(prevTextDocument);
    ui->prevTextZone->document()->adjustSize();


    QTextCursor curs =  ui->prevTextZone->textCursor();
    curs.movePosition(QTextCursor::End);
    ui->prevTextZone->setTextCursor(curs);
}

//-------------------------------------------------------------------------------
void  TextTab::setPrevButtonState(bool state)
{
    if(state)
        ui->prevTextToolButton->setArrowType(Qt::UpArrow);
    else
        ui->prevTextToolButton->setArrowType(Qt::DownArrow);
}

//-------------------------------------------------------------------------------

void TextTab::on_prevTextToolButton_clicked()
{
    if(ui->prevTextToolButton->arrowType() == Qt::DownArrow){


        QList<int> sizesList;
        sizesList << ui->textZone->height()/3 << ui->textZone->height()  << ui->nextTextZone->height();
        ui->splitter->setSizes(sizesList);

        ui->textZone->setFocus();
        ui->textZone->ensureCursorVisible();

        this->setPrevButtonState(true);
    }
    else{
        QList<int> sizesList;
        sizesList << 0 << ui->textZone->height()  << ui->nextTextZone->height() ;
        ui->splitter->setSizes(sizesList);


        ui->textZone->setFocus();
        ui->textZone->ensureCursorVisible();
        this->setPrevButtonState(false);

    }
}



//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------

void TextTab::showNextText(bool showNextTextBool)
{
    ui->nextTextZone->setHidden(!showNextTextBool);
    ui->nextTextZone->setMaximumHeight(ui->textZone->height()/3);
    ui->textZone->setFocus();
    ui->textZone->ensureCursorVisible();
}

//-------------------------------------------------------------------------------
bool TextTab::setShowNextTextButton()
{
    return !ui->nextTextZone->isHidden();
}

//-------------------------------------------------------------------------------
void  TextTab::setNextText(MainTextDocument *nextDoc)
{
    if(nextDoc == 0){
        ui->nextTextZone->setHidden(true);
        ui->nextTextToolButton->setHidden(true);
        return;
    }
    ui->nextTextZone->setHidden(false);
    ui->nextTextToolButton->setHidden(false);

    nextTextDocument = nextDoc;
    ui->nextTextZone->setDoc(nextTextDocument);
    ui->nextTextZone->document()->adjustSize();


    QTextCursor curs =  ui->nextTextZone->textCursor();
    curs.movePosition(QTextCursor::Start);
    ui->nextTextZone->setTextCursor(curs);
}

//-------------------------------------------------------------------------------
void  TextTab::setNextButtonState(bool state)
{
    if(state)
        ui->nextTextToolButton->setArrowType(Qt::DownArrow);
    else
        ui->nextTextToolButton->setArrowType(Qt::UpArrow);
}

//-------------------------------------------------------------------------------

void TextTab::on_nextTextToolButton_clicked()
{
    if(ui->nextTextToolButton->arrowType() == Qt::UpArrow){


        QList<int> sizesList;
        sizesList << ui->prevTextZone->height() << ui->textZone->height()  << ui->textZone->height()/3;
        ui->splitter->setSizes(sizesList);

        ui->textZone->setFocus();
        ui->textZone->ensureCursorVisible();

        this->setNextButtonState(true);
    }
    else{
        QList<int> sizesList;
        sizesList << ui->prevTextZone->height() << ui->textZone->height()  << 0;
        ui->splitter->setSizes(sizesList);


        ui->textZone->setFocus();
        ui->textZone->ensureCursorVisible();
        this->setNextButtonState(false);

    }

}

//-------------------------------------------------------------------------------
void TextTab::on_splitter_splitterMoved(int pos, int index)
{
    //    qDebug() << "pos : "<< QString::number(pos) << "index : "<<QString::number(index) ;
    if(index == 1 && pos != 0)
        this->setPrevButtonState(true);
    if(index == 1 && pos == 0)
        this->setPrevButtonState(false);
    if(index == 2 && pos != ui->splitter->maximumHeight())
        this->setNextButtonState(true);
    if(index == 2 && pos == ui->splitter->maximumHeight())
        this->setNextButtonState(false);
}

void TextTab::modifySize(int modifier)
{
    int textZoneWidth;

    if(modifier == 0)
        changeWidthSlot();


    if(ui->textZone->width() < 400)
        textZoneWidth = 400;
    if(ui->textZone->width() <= 400 && modifier < 0)
        return;
    if(ui->textZone->width() > this->width())
        textZoneWidth = this->width() - 20 ;
    if(ui->textZone->width() > this->width() - 20 &&  modifier > 0)
        return;
    else{
        textZoneWidth = ui->textZone->width() + modifier;

    }

    changeWidthSlot(textZoneWidth);

    ui->textZone->ensureCursorVisible();
    ui->prevTextZone->ensureCursorVisible();
    ui->nextTextZone->ensureCursorVisible();


}





//-------------------------------------------------------------------------------
void  TextTab::addSlimFindReplaceAction()
{
    QList<QAction*> actions;



    QAction *findAct = new QAction(QIcon(":/pics/edit-find-replace.png"),tr("Find && replace"), this);
    findAct->setShortcuts(QKeySequence::Find);
    findAct->setStatusTip(tr("Find text"));
    connect(findAct, SIGNAL(triggered()), this, SLOT(launchSlimFindReplace()));

    actions.append(findAct);

    ui->textZone->addActions(actions);

}
//-------------------------------------------------------------------------------

void  TextTab::launchSlimFindReplace()
{
    ui->findReplace->show();

}

//-------------------------------------------------------------------------------
void TextTab::applyConfig()
{
    ui->editToolBar->applyConfig();
    ui->textZone->applyConfig();
    ui->prevTextZone->applyConfig();
    ui->nextTextZone->applyConfig();


    QSettings settings;
    settings.beginGroup( "Settings" );
    //    int bottMargin = settings.value("TextArea/bottomMargin", 10).toInt();
    //    int textIndent = settings.value("TextArea/textIndent", 20).toInt();
    //    int textHeight = settings.value("TextArea/textHeight", 12).toInt();
    //    QString fontFamily = settings.value("TextArea/textFontFamily", "Liberation Serif").toString();
    changeWidthSlot();
    settings.endGroup();


    ui->textZone->setFixedWidth(ui->textZone->width() + 1);
    ui->textZone->setFixedWidth(ui->textZone->width() -1);


}



//-------------------------------------------------------------------
void TextTab::giveStyle()
{
    QSettings settings;

    QString  customCss =


            "TextZone {"
            "background-color: " + settings.value("MainWindow/textZoneBackColor", "#ffffff").toString() +";"
            "color: " + settings.value("MainWindow/textZoneTextColor", "#000000").toString() +";"
         "}"
            "SizeHandle {"
            "background-color: " + settings.value("MainWindow/textTabBackColor", "#ffffff").toString() +";"
         "}"
            "SizeHandle:hover {"
            "background-color: " + settings.value("MainWindow/textZoneTextColor", "#888888").toString() +";"
         "}"
            "TextTab {"
               "background-color: " + settings.value("MainWindow/textTabBackColor", "#ffffff").toString() +" ;"
               "}"
 ;

    if(!settings.value("Settings/applyCustomColors", true).toBool())
        customCss =             ""
                ;


    this->setStyleSheet( css + customCss);
}
//-------------------------------------------------------------------

void TextTab::paintEvent(QPaintEvent *)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);



}
//-------------------------------------------------------------------

void TextTab::resizeEvent(QResizeEvent *event)
{
    //    int percent = ui->textZone->width()/this->width();

    //    int w = event->size().width();

    //    ui->textZone->setFixedWidth(w*percent);
    //    ui->prevTextZone->setFixedWidth(w*percent);
    //    ui->nextTextZone->setFixedWidth(w*percent);

    //    ui->textZone->document()->setTextWidth(width - scrollBarWidth);
    //    ui->prevTextZone->document()->setTextWidth(width - scrollBarWidth);
    //    ui->nextTextZone->document()->setTextWidth(width - scrollBarWidth);

    //    qDebug() << "event->size().width() : " + QString::number(event->size().width());

        overlay->resize(event->size());

//        overlay->editZoneMoved(this->mapFromGlobal(ui->textZone->mapToGlobal(ui->textZone->pos())).x(), this->mapFromGlobal(ui->textZone->mapToGlobal(ui->textZone->pos())).y(), ui->textZone->width(), ui->textZone->height());

    if(firstTime){
        firstTime = false;
        return;
    }

    this->modifySize(event->size().width() - event->oldSize().width());



    //    qDebug() << "event->size().width() - event->oldSize().width() : " + QString::number(event->size().width() - event->oldSize().width());

    //    qDebug() << "resizing";
}

//-------------------------------------------------------------------

void TextTab::mouseMoveEvent(QMouseEvent *event)
{
    if(m_leftHoverZone.contains(event->pos()) || m_rightHoverZone.contains(event->pos()))
        ui->editToolBar->openToolBar();

}

//-------------------------------------------------------------------
void TextTab::scrollBar_setRange(int min, int max)
{
    ui->verticalScrollBar->setMinimum(min);
    ui->verticalScrollBar->setMaximum(max);

    if(min == 0 && max == 0)
        ui->verticalScrollBar->hide();
    else
        ui->verticalScrollBar->show();

}


int TextTab::idNumber() const
{
    return m_idNumber;
}

void TextTab::setIdNumber(int idNumber)
{
    m_idNumber = idNumber;
}

void TextTab::activateSpellcheck(bool isActivated)
{

    ui->textZone->activateSpellcheck(isActivated);
    ui->prevTextZone->activateSpellcheck(isActivated);
    ui->nextTextZone->activateSpellcheck(isActivated);
}







void TextTab::editZoneMoved(QRect editZoneRect)
{

    QPoint point = this->mapFromGlobal(QPoint(editZoneRect.x(), editZoneRect.y()));
    editZoneRect.setX(point.x());
    editZoneRect.setY(point.y());
    editZoneRect.setWidth(editZoneRect.width());
    editZoneRect.setHeight(editZoneRect.height());



//    ui->editToolBar->move(editZoneRect.x(), editZoneRect.y());
//    ui->editToolBar->resize(editZoneRect.width(), editToolBar->height());




    // hover zones :

    int hoverZoneSize =  editZoneRect.height() / 15;

    QPoint leftPoint(editZoneRect.x(), editZoneRect.y());
    m_leftHoverZone = QRect(leftPoint.x(), leftPoint.y(), - hoverZoneSize, hoverZoneSize);

    QPoint rightPoint(editZoneRect.x() + editZoneRect.width(), editZoneRect.y());
    m_rightHoverZone = QRect(rightPoint.x(), rightPoint.y(), hoverZoneSize, hoverZoneSize);




}

#include "notezone.h"
//
NoteZone::NoteZone(QWidget *parent) :
    QTextEdit(parent)
{
    this->setAttribute(Qt::WA_KeyCompression, true);


    textDocument = new QTextDocument(this);


    toHtml();

    createActions();

    setContextMenuPolicy(Qt::PreventContextMenu);

    connect(this, SIGNAL(currentCharFormatChanged(QTextCharFormat)), this, SLOT(charFormat(QTextCharFormat)));

    setWordWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);

    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    //temporary config
    //    QFont liberationFont("Liberation Serif", 13);
    //    this->setFont(liberationFont);
    //    this->setFontPointSize(13);

    //    QTextBlockFormat textBlockFormat;
    //    textBlockFormat.setBottomMargin(10);
    //    textBlockFormat.setTextIndent(50);
    //    QTextCharFormat textCharFormat;
    //    textCharFormat.setFontFamily("Liberation Serif");
    //    textCharFormat.setFontPointSize(13);

    //    QTextCursor textCursor;
    //    textCursor = this->textCursor();
    //    textCursor.setBlockFormat(textBlockFormat);
    //    textCursor.setCharFormat(textCharFormat);


}

//-----------------------------------------------------


bool NoteZone::openNote(QTextDocument *noteDoc)
{

    textDocument = noteDoc;

    this->setEnabled(true);






    //    noteFile->open(QFile::ReadOnly | QFile::Text);
    //    QApplication::setOverrideCursor(Qt::WaitCursor);
    //    QTextStream noteFileStream( noteFile );
    //    textDocument->setHtml(noteFileStream.readAll());
    //    //  this->setText( noteFileStream.readAll() );
    //    QApplication::restoreOverrideCursor();
    //    noteFile->close();


    setDocument(textDocument);




    setContextMenuPolicy(Qt::DefaultContextMenu);

    setDocumentTitle("Note");


    applyNoteConfig();

    connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(forceNoteFirstCharFont()));

    textDocument->setTextWidth(this->width() - this->verticalScrollBar()->width() - 2);

    return true;
}

//------------------------------------------------------------------------------


bool NoteZone::openSyn(QTextDocument *synDoc)
{

    textDocument = synDoc;

    this->setEnabled(true);

    //    synFile->open(QFile::ReadOnly | QFile::Text);
    //    QApplication::setOverrideCursor(Qt::WaitCursor);
    //    QTextStream synFileStream( synFile );
    //    textDocument->setHtml(synFileStream.readAll());

    //    QApplication::restoreOverrideCursor();

    //    synFile->close();

    setDocument(textDocument);


    setContextMenuPolicy(Qt::DefaultContextMenu);

    setDocumentTitle("Synopsis");

    applySynConfig();

    connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(forceSynFirstCharFont()));

    textDocument->setTextWidth(this->width() - this->verticalScrollBar()->width() - 2);


    return true;
}

//------------------------------------------------------------------------------

bool NoteZone::openAttendDetail(QTextDocument *attendDoc)
{
    textDocument = attendDoc;

    this->setEnabled(true);
    //    synFile->open(QFile::ReadOnly | QFile::Text);
    //    QApplication::setOverrideCursor(Qt::WaitCursor);
    //    QTextStream synFileStream( synFile );
    //    textDocument->setHtml(synFileStream.readAll());

    //    QApplication::restoreOverrideCursor();

    //    synFile->close();


    setDocument(textDocument);


    setContextMenuPolicy(Qt::DefaultContextMenu);

    setDocumentTitle("Attendance detail");

    applyAttendConfig();

    connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(forceAttendFirstCharFont()));

    textDocument->setTextWidth(this->width() - this->verticalScrollBar()->width() - 2);


    return true;
}

//------------------------------------------------------------------------------


//bool NoteZone::saveSyn(QFile *synFile, QString name)
//{
//    if(name != synStackName)
//        return false;


//    QTextDocumentWriter docWriter(synFile, "HTML");
//    bool written = docWriter.write(textDocument);

//    return written;
//}



//------------------------------------------------------------------------------

//bool NoteZone::saveNote(QFile *noteFile, QString name)
//{
//    if(name != noteStackName)
//        return false;



//    QTextDocumentWriter docWriter(noteFile, "HTML");
//    bool written = docWriter.write(textDocument);

//    return written;
//}



//------------------------------------------------------------------------------

bool NoteZone::closeSyn()
{


    this->setEnabled(false);

    setContextMenuPolicy(Qt::PreventContextMenu);

    return true;
}

//------------------------------------------------------------------------------


bool NoteZone::closeNote()
{


    this->setEnabled(false);

    setContextMenuPolicy(Qt::PreventContextMenu);

    return true;
}

//------------------------------------------------------------------------------

bool NoteZone::closeAttendDetail()
{


    this->setEnabled(false);

    setContextMenuPolicy(Qt::PreventContextMenu);

    return true;
}



void NoteZone::forceSynFirstCharFont()
{
    // force the 1 st line to be the correct font

    if(this->textCursor().atStart() == true || this->textCursor().position() == 1 )
        applySynFontConfig();
}

void NoteZone::forceNoteFirstCharFont()
{
    // force the 1 st line to be the correct font


    if(this->textCursor().atStart() == true || this->textCursor().position() == 1 )
        applyNoteFontConfig();
}

void NoteZone::forceAttendFirstCharFont()
{
    // force the 1 st line to be the correct font


    if(this->textCursor().atStart() == true || this->textCursor().position() == 1 )
        applyAttendFontConfig();
}





















//--------- Context Menu :------------------------------------------------------------------


void NoteZone::createActions()
{
    undoAct = new QAction(tr("&Undo"), this);
    undoAct->setShortcuts(QKeySequence::Undo);
    undoAct->setStatusTip(tr("Undo the last operation"));
    connect(undoAct, SIGNAL(triggered()), this, SLOT(undo()));

    redoAct = new QAction(tr("&Redo"), this);
    redoAct->setShortcuts(QKeySequence::Redo);
    redoAct->setStatusTip(tr("Redo the last operation"));
    connect(redoAct, SIGNAL(triggered()), this, SLOT(redo()));

    cutAct = new QAction(tr("Cu&t"), this);
    cutAct->setShortcuts(QKeySequence::Cut);
    cutAct->setStatusTip(tr("Cut the current selection's contents to the "
                            "clipboard"));
    connect(cutAct, SIGNAL(triggered()), this, SLOT(cut()));

    copyAct = new QAction(tr("&Copy"), this);
    copyAct->setShortcuts(QKeySequence::Copy);
    copyAct->setStatusTip(tr("Copy the current selection's contents to the "
                             "clipboard"));
    connect(copyAct, SIGNAL(triggered()), this, SLOT(copy()));

    pasteAct = new QAction(tr("&Paste"), this);
    pasteAct->setShortcuts(QKeySequence::Paste);
    pasteAct->setStatusTip(tr("Paste the clipboard's contents into the current "
                              "selection"));
    connect(pasteAct, SIGNAL(triggered()), this, SLOT(paste()));

    boldAct = new QAction(tr("&Bold"), this);
    boldAct->setCheckable(true);
    boldAct->setShortcuts(QKeySequence::Bold);
    boldAct->setStatusTip(tr("Make the text bold"));
    connect(boldAct, SIGNAL(triggered(bool)), this, SLOT(bold(bool)));

    QFont boldFont = boldAct->font();
    boldFont.setBold(true);
    boldAct->setFont(boldFont);

    italicAct = new QAction(tr("&Italic"), this);
    italicAct->setCheckable(true);
    italicAct->setShortcuts(QKeySequence::Italic);
    italicAct->setStatusTip(tr("Make the text italic"));
    connect(italicAct, SIGNAL(triggered(bool)), this, SLOT(italic(bool)));

    QFont italicFont = italicAct->font();
    italicFont.setItalic(true);
    italicAct->setFont(italicFont);

    //    setLineSpacingAct = new QAction(tr("Set &Line Spacing..."), this);
    //    setLineSpacingAct->setStatusTip(tr("Change the gap between the lines of a "
    //                                       "paragraph"));
    //    connect(setLineSpacingAct, SIGNAL(triggered()), this, SLOT(setLineSpacing()));

    //    setParagraphSpacingAct = new QAction(tr("Set &Paragraph Spacing..."), this);
    //    setParagraphSpacingAct->setStatusTip(tr("Change the gap between paragraphs"));
    //    connect(setParagraphSpacingAct, SIGNAL(triggered()),
    //            this, SLOT(setParagraphSpacing()));

    leftAlignAct = new QAction(tr("&Left Align"), this);
    leftAlignAct->setCheckable(true);
    leftAlignAct->setShortcut(tr("Ctrl+L"));
    leftAlignAct->setStatusTip(tr("Left align the selected text"));
    connect(leftAlignAct, SIGNAL(triggered(bool)), this, SLOT(leftAlign(bool)));

    rightAlignAct = new QAction(tr("&Right Align"), this);
    rightAlignAct->setCheckable(true);
    rightAlignAct->setShortcut(tr("Ctrl+R"));
    rightAlignAct->setStatusTip(tr("Right align the selected text"));
    connect(rightAlignAct, SIGNAL(triggered(bool)), this, SLOT(rightAlign(bool)));

    justifyAct = new QAction(tr("&Justify"), this);
    justifyAct->setCheckable(true);
    justifyAct->setShortcut(tr("Ctrl+J"));
    justifyAct->setStatusTip(tr("Justify the selected text"));
    connect(justifyAct, SIGNAL(triggered(bool)), this, SLOT(justify(bool)));

    centerAct = new QAction(tr("&Center"), this);
    centerAct->setCheckable(true);
    centerAct->setShortcut(tr("Ctrl+E"));
    centerAct->setStatusTip(tr("Center the selected text"));
    connect(centerAct, SIGNAL(triggered(bool)), this, SLOT(center(bool)));

    alignmentGroup = new QMenu(tr("&Alignment"),this);
    alignmentGroup->addAction(leftAlignAct);
    alignmentGroup->addAction(rightAlignAct);
    alignmentGroup->addAction(justifyAct);
    alignmentGroup->addAction(centerAct);
    leftAlignAct->setChecked(true);




}

//------------------------------------------------------------------

void NoteZone::undo()
{
    textDocument->undo();
    //QTextEdit::undo();
}

void NoteZone::redo()
{
    textDocument->redo();
}

void NoteZone::cut()
{
    QTextEdit::cut();
}

void NoteZone::copy()
{
    QTextEdit::copy();
}

void NoteZone::paste()
{
    QTextEdit::paste();
}

void NoteZone::bold(bool boldBool)
{
    if(boldBool){
        setFontWeight(75);
    }
    else{
        setFontWeight(50);
    }
}

void NoteZone::italic(bool italBool)
{
    if(italBool){
        setFontItalic(true);
    }
    else{
        setFontItalic(false);
    }
}

void NoteZone::leftAlign(bool leftBool)
{
    if(leftBool){
        centerAct->setChecked(false);
        rightAlignAct->setChecked(false);
        justifyAct->setChecked(false);
        setAlignment(Qt::AlignLeft);
    }
    else{
        leftAlignAct->setChecked(true);
        leftAlign(true);
    }
}

void NoteZone::rightAlign(bool rightBool)
{
    if(rightBool){
        centerAct->setChecked(false);
        leftAlignAct->setChecked(false);
        justifyAct->setChecked(false);
        setAlignment(Qt::AlignRight);
    }
    else{
        leftAlignAct->setChecked(true);
        leftAlign(true);
    }

}

void NoteZone::justify(bool justBool)
{
    if(justBool){
        centerAct->setChecked(false);
        rightAlignAct->setChecked(false);
        leftAlignAct->setChecked(false);
        setAlignment(Qt::AlignJustify);
    }
    else{
        leftAlignAct->setChecked(true);
        leftAlign(true);
    }

}

void NoteZone::center(bool centBool)
{
    if(centBool){
        rightAlignAct->setChecked(false);
        leftAlignAct->setChecked(false);
        justifyAct->setChecked(false);
        setAlignment(Qt::AlignHCenter);
    }
    else{
        leftAlignAct->setChecked(true);
        leftAlign(true);
    }
}




//----------------------------------------------------------------------

void NoteZone::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu menu(this);
    menu.addSeparator();
    menu.addAction(boldAct);
    menu.addAction(italicAct);
    menu.addSeparator();
    menu.addMenu(alignmentGroup);
    menu.addSeparator();
    menu.addAction(cutAct);
    menu.addAction(copyAct);
    menu.addAction(pasteAct);
    menu.addSeparator();
    menu.addAction(undoAct);
    menu.addAction(redoAct);

    menu.exec(event->globalPos());
}

//--------------------------------------------------------------------------------

void NoteZone::charFormat(QTextCharFormat cFormat)
{
    QString family = cFormat.fontFamily();
    int weight = cFormat.fontWeight();
    bool italic = cFormat.fontItalic();
    Qt::Alignment align = alignment();

    if(weight > 50)
        boldAct->setChecked(true);
    else
        boldAct->setChecked(false);



    if(italic)
        italicAct->setChecked(true);
    else
        italicAct->setChecked(false);



    if (align & Qt::AlignLeft){
        centerAct->setChecked(false);
        rightAlignAct->setChecked(false);
        justifyAct->setChecked(false);
        leftAlignAct->setChecked(true);
    }
    else if (align & Qt::AlignHCenter){
        rightAlignAct->setChecked(false);
        justifyAct->setChecked(false);
        leftAlignAct->setChecked(false);
        centerAct->setChecked(true);
    }
    else if (align & Qt::AlignRight){
        centerAct->setChecked(false);
        justifyAct->setChecked(false);
        leftAlignAct->setChecked(false);
        rightAlignAct->setChecked(true);
    }
    else if (align & Qt::AlignJustify){
        centerAct->setChecked(false);
        rightAlignAct->setChecked(false);
        leftAlignAct->setChecked(false);
        justifyAct->setChecked(true);
    }



}

//--------------------------------------------------------------------------------

//void NoteZone::keyPressEvent(QKeyEvent *event)
//{
//    /////////////qDebug() << "### e->key()   " << e->key();

//}

//--------------------------------------------------------------------------------




void NoteZone::setTextFont(QFont font)
{
    //    QString currentFormat = settings->value("Settings/Text/textFontFamily", textCursor().charFormat().fontFamily() ).toString();
    QTextCharFormat fmt;
    fmt.setFontFamily(font.family());
    //    QString fontcolor = settings->value("Settings/Text/FontColor", "000000").toString();
    //    QColor color;
    //    color.setNamedColor(fontcolor);
    //    QBrush brush(color, Qt::SolidPattern);
    //    QFont font;
    //    font.fromString(currentFormat);
    //    fmt.setForeground(brush);

    mergeFormatOnWordOrSelection(fmt);
}


void NoteZone::setTextHeight(int height)
{
    QTextCharFormat fmt;
    fmt.setFontPointSize(height);
    mergeFormatOnWordOrSelection(fmt);

}



void NoteZone::mergeFormatOnWordOrSelection(const QTextCharFormat &format)
{
    QTextCursor cursor = textCursor();
    //    if(cursor.charFormat().fontItalic())
    //        format.setFontItalic(true);

    cursor.mergeCharFormat(format);
    mergeCurrentCharFormat(format);
}


//--------------------------------------------------------------------------------

void NoteZone::keyPressEvent(QKeyEvent *event)
{
    if(event->matches(QKeySequence::Italic))
            italic(!italicAct->isChecked());
    else if(event->matches(QKeySequence::Bold))
            bold(!boldAct->isChecked());
    else if(event->modifiers() == (Qt::ControlModifier|Qt::ShiftModifier) && event->key() == QKeySequence(tr("L")))    //: L for Left
            leftAlign(true);
    else if(event->modifiers() == (Qt::ControlModifier|Qt::ShiftModifier) && event->key() == QKeySequence(tr("R"))) //: R for Right
            rightAlign(true);
    else if(event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_E)
            center(true);
    else if(event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_J)
            justify(true);
    else if(event->key() == Qt::Key_Space){
        if(preventDoubleSpaceOption == true)
            preventDoubleSpace();
        else
            QTextEdit::keyPressEvent(event);

    }
    else
        QTextEdit::keyPressEvent(event);
}




//--------------------------------------------------------------------------------
void NoteZone::centerCursor()
{
    //    QRect cursor = cursorRect();
    //    QRect viewport =  this->viewport()->rect();
    //    if (alwaysCenter || (cursor.bottom() >= viewport.bottom()) || (cursor.top() <= viewport.top())) {
    //        QPoint offset = viewport.center() - cursor.center();
    //        QScrollBar* scrollbar = verticalScrollBar();
    //        scrollbar->setValue(scrollbar->value() - offset.y());
    //    }
}

//--------------------------------------------------------------------------------

void NoteZone::cursorPositionChangedSlot()
{
    if (QApplication::mouseButtons() == Qt::NoButton) {
        centerCursor();
    }
}


//--------------------------------------------------------------------------------

void NoteZone::setCursorPos(int pos)
{
    for(int i = 0; i < pos ; i++)
        moveCursor(QTextCursor::NextCharacter, QTextCursor::MoveAnchor);


    ensureCursorVisible();
}

//--------------------------------------------------------------------------------

int NoteZone::saveCursorPos()
{
    QTextCursor cursor = this->textCursor();
    return cursor.position();

}

//--------------------------------------------------------------------------------

void NoteZone::preventDoubleSpace()
{

    QTextCursor *tCursor = new QTextCursor(document());
    int cursorPos = this->textCursor().position();
    QString prevChar;
    QString nextChar;

    if(this->textCursor().atBlockStart() == false){
        do {tCursor->setPosition(cursorPos);
            tCursor->movePosition(QTextCursor::PreviousCharacter, QTextCursor::KeepAnchor,1);
            prevChar = tCursor->selectedText();
            if(prevChar == " "){
                tCursor->removeSelectedText();
                cursorPos -= 1;
            }
        }
        while(prevChar == " ");

    }
    if(this->textCursor().atBlockEnd() == false){
        do {tCursor->setPosition(cursorPos);
            tCursor->movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor,1);
            nextChar = tCursor->selectedText();
            if(nextChar == " "){
                tCursor->removeSelectedText();
            }
        }
        while(nextChar == " ");

    }

    this->textCursor().insertText(" ");
}
//--------------------------------------------------------------------------------


void NoteZone::updateTextZone()
{
    setFixedWidth(width()+1);
    setFixedWidth(width()-1);
}


















//--------------------------------------------------------------------------------


void NoteZone::insertFromMimeData (const QMimeData *source )
{



//    if(this->objectName().mid(0,3) == "syn"){


QTextCursor yCursor = this->textCursor();
        int bottMargin = yCursor.blockFormat().bottomMargin();
        int textIndent = yCursor.blockFormat().textIndent();
        int textHeight = yCursor.charFormat().fontPointSize();
        QString fontFamily = yCursor.charFormat().fontFamily();


        if(source->hasHtml()){


            //htmlText
            QTextDocument *document = new QTextDocument;
            document->setHtml(qvariant_cast<QString>(source->html()));

            QTextBlockFormat blockFormat;
            blockFormat.setBottomMargin(bottMargin);
            blockFormat.setTextIndent(textIndent);
             QTextCharFormat charFormat;
            charFormat.setFontPointSize(textHeight);
            charFormat.setFontFamily(fontFamily);
            charFormat.clearForeground();

            QTextCursor *tCursor = new QTextCursor(document);
            tCursor->movePosition(QTextCursor::Start, QTextCursor::MoveAnchor,1);
            tCursor->movePosition(QTextCursor::End, QTextCursor::KeepAnchor,1);

            tCursor->mergeCharFormat(charFormat);
            tCursor->mergeBlockFormat(blockFormat);


            QTextCursor cursor = this->textCursor();
            cursor.insertHtml(document->toHtml("utf-8"));
                    qDebug() << "insertFromMimeData Html";

        }
        else if(source->hasText()){
            QTextDocument *document = new QTextDocument;
            document->setHtml(qvariant_cast<QString>(source->text()));

            QTextBlockFormat blockFormat;
            blockFormat.setBottomMargin(bottMargin);
            blockFormat.setTextIndent(textIndent);
            QTextCharFormat charFormat;
            charFormat.setFontPointSize(textHeight);
            charFormat.setFontFamily(fontFamily);
            charFormat.clearForeground();
            QTextCursor *tCursor = new QTextCursor(document);
            tCursor->movePosition(QTextCursor::Start, QTextCursor::MoveAnchor,1);
            tCursor->movePosition(QTextCursor::End, QTextCursor::KeepAnchor,1);

            tCursor->mergeCharFormat(charFormat);
            tCursor->mergeBlockFormat(blockFormat);

            QTextCursor cursor = this->textCursor();
            cursor.insertHtml(document->toHtml("utf-8"));
                    qDebug() << "insertFromMimeData plainText";

        }





//    }

//    if(this->objectName().mid(0,4) == "note"){


//        QSettings settings;
//        settings.beginGroup( "Settings" );
//        int bottMargin = settings.value("noteArea/bottomMargin", 10).toInt();
//        int textIndent = settings.value("noteArea/textIndent", 20).toInt();
//        int textHeight = settings.value("noteArea/textHeight", 12).toInt();
//        QString fontFamily = settings.value("noteArea/textFontFamily", "Liberation Serif").toString();
//        settings.endGroup();


//        if(source->hasHtml()){


//            //htmlText
//            QTextDocument *document = new QTextDocument;
//            document->setHtml(qvariant_cast<QString>(source->html()));

//            QTextBlockFormat blockFormat;
//            blockFormat.setBottomMargin(bottMargin);
//            blockFormat.setTextIndent(textIndent);
//             QTextCharFormat charFormat;
//            charFormat.setFontPointSize(textHeight);
//            charFormat.setFontFamily(fontFamily);
//            charFormat.setForeground(QBrush(Qt::black));

//            QTextCursor *tCursor = new QTextCursor(document);
//            tCursor->movePosition(QTextCursor::Start, QTextCursor::MoveAnchor,1);
//            tCursor->movePosition(QTextCursor::End, QTextCursor::KeepAnchor,1);

//            tCursor->mergeCharFormat(charFormat);
//            tCursor->mergeBlockFormat(blockFormat);


//            QTextCursor cursor = this->textCursor();
//            cursor.insertHtml(document->toHtml("utf-8"));
//                    qDebug() << "insertFromMimeData Html";

//        }
//        else if(source->hasText()){
//            QTextDocument *document = new QTextDocument;
//            document->setHtml(qvariant_cast<QString>(source->text()));

//            QTextBlockFormat blockFormat;
//            blockFormat.setBottomMargin(bottMargin);
//            blockFormat.setTextIndent(textIndent);
//            QTextCharFormat charFormat;
//            charFormat.setFontPointSize(textHeight);
//            charFormat.setFontFamily(fontFamily);
//            QTextCursor *tCursor = new QTextCursor(document);
//            tCursor->movePosition(QTextCursor::Start, QTextCursor::MoveAnchor,1);
//            tCursor->movePosition(QTextCursor::End, QTextCursor::KeepAnchor,1);

//            tCursor->mergeCharFormat(charFormat);
//            tCursor->mergeBlockFormat(blockFormat);

//            QTextCursor cursor = this->textCursor();
//            cursor.insertHtml(document->toHtml("utf-8"));
//                    qDebug() << "insertFromMimeData plainText";

//        }
//    }

//    if(this->document()->objectName().mid(0,9) == "attendDoc"){


//        QSettings settings;
//        settings.beginGroup( "Settings" );
//        int bottMargin = settings.value("attendArea/bottomMargin", 10).toInt();
//        int textIndent = settings.value("attendArea/textIndent", 20).toInt();
//        int textHeight = settings.value("attendArea/textHeight", 12).toInt();
//        QString fontFamily = settings.value("attendArea/textFontFamily", "Liberation Serif").toString();
//        settings.endGroup();


//        if(source->hasHtml()){


//            //htmlText
//            QTextDocument *document = new QTextDocument;
//            document->setHtml(qvariant_cast<QString>(source->html()));

//            QTextBlockFormat blockFormat;
//            blockFormat.setBottomMargin(bottMargin);
//            blockFormat.setTextIndent(textIndent);
//             QTextCharFormat charFormat;
//            charFormat.setFontPointSize(textHeight);
//            charFormat.setFontFamily(fontFamily);
//            charFormat.setForeground(QBrush(Qt::black));

//            QTextCursor *tCursor = new QTextCursor(document);
//            tCursor->movePosition(QTextCursor::Start, QTextCursor::MoveAnchor,1);
//            tCursor->movePosition(QTextCursor::End, QTextCursor::KeepAnchor,1);

//            tCursor->mergeCharFormat(charFormat);
//            tCursor->mergeBlockFormat(blockFormat);


//            QTextCursor cursor = this->textCursor();
//            cursor.insertHtml(document->toHtml("utf-8"));
//                    qDebug() << "insertFromMimeData Html";

//        }
//        else if(source->hasText()){
//            QTextDocument *document = new QTextDocument;
//            document->setHtml(qvariant_cast<QString>(source->text()));

//            QTextBlockFormat blockFormat;
//            blockFormat.setBottomMargin(bottMargin);
//            blockFormat.setTextIndent(textIndent);
//            QTextCharFormat charFormat;
//            charFormat.setFontPointSize(textHeight);
//            charFormat.setFontFamily(fontFamily);
//            QTextCursor *tCursor = new QTextCursor(document);
//            tCursor->movePosition(QTextCursor::Start, QTextCursor::MoveAnchor,1);
//            tCursor->movePosition(QTextCursor::End, QTextCursor::KeepAnchor,1);

//            tCursor->mergeCharFormat(charFormat);
//            tCursor->mergeBlockFormat(blockFormat);

//            QTextCursor cursor = this->textCursor();
//            cursor.insertHtml(document->toHtml("utf-8"));
//                    qDebug() << "insertFromMimeData plainText";

//        }




//    }







}

//--------------------------------------------------------------------------------


bool NoteZone::canInsertFromMimeData (const QMimeData *source) const
{

    if (source->hasHtml() || source->hasText())
        return true;

    else
        return QTextEdit::canInsertFromMimeData(source);


    //    qDebug() << "canInsertFromMimeData";
}
//--------------------------------------------------------------------------------
void NoteZone::resizeEvent(QResizeEvent* event)
{
    centerCursor();
    textDocument->setTextWidth(this->width() - this->verticalScrollBar()->width() - 2);
    QWidget::resizeEvent(event);
}











//--------------------------------------------------------------------------------
//-----------Apply Config-------------------------------------------------------
//-------------------------------------------------------------------------------

void NoteZone::applyNoteConfig()
{


    QSettings settings;
    settings.beginGroup( "Settings" );
    alwaysCenter = settings.value("NoteArea/alwaysCenter", true).toBool();
    bool noteShowScrollbar = settings.value("NoteArea/showScrollbar", true).toBool();
    preventDoubleSpaceOption = settings.value("preventDoubleSpace", false).toBool();
    settings.endGroup();


    centerCursor();

    if(noteShowScrollbar)
        setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    else
        setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);








    applyNoteFontConfig();
}

void NoteZone::applyNoteFontConfig()
{
    emit disconnectUpdateTextsSignal();


    QSettings settings;
    settings.beginGroup( "Settings" );
    int noteBottMargin = settings.value("NoteArea/bottomMargin", 20).toInt();
    int noteTextIndent = settings.value("NoteArea/textIndent", 20).toInt();
    int noteTextHeight = settings.value("NoteArea/textHeight", 12).toInt();
    QString noteFontFamily = settings.value("NoteArea/textFontFamily", "Liberation Serif").toString();
    settings.endGroup();

    QTextBlockFormat blockFormat;
    blockFormat.setBottomMargin(noteBottMargin);
    blockFormat.setTextIndent(noteTextIndent);
    QTextCharFormat charFormat;
    charFormat.setFontPointSize(noteTextHeight);
    charFormat.setFontFamily(noteFontFamily);
    charFormat.clearForeground();

    QTextCursor *tCursor = new QTextCursor(document());
    tCursor->movePosition(QTextCursor::Start, QTextCursor::MoveAnchor,1);
    tCursor->movePosition(QTextCursor::End, QTextCursor::KeepAnchor,1);

    tCursor->mergeCharFormat(charFormat);
    tCursor->mergeBlockFormat(blockFormat);

    QFont font;
    font.setFamily(noteFontFamily);
    font.setPointSize(noteTextHeight);
    this->document()->setDefaultFont(font);

    emit connectUpdateTextsSignal();

}


//-------------------------------------------------------------------------------
void NoteZone::applySynConfig()
{


    QSettings settings;
    settings.beginGroup( "Settings" );
    alwaysCenter = settings.value("SynArea/alwaysCenter", true).toBool();
    bool synShowScrollbar = settings.value("SynArea/showScrollbar", true).toBool();
    preventDoubleSpaceOption = settings.value("preventDoubleSpace", false).toBool();
    settings.endGroup();


    centerCursor();

    if(synShowScrollbar)
        setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    else
        setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);


    applySynFontConfig();

}


void NoteZone::applySynFontConfig()
{


    emit disconnectUpdateTextsSignal();

    QSettings settings;
    settings.beginGroup( "Settings" );
    int synBottMargin = settings.value("SynArea/bottomMargin", 20).toInt();
    int synTextIndent = settings.value("SynArea/textIndent", 20).toInt();
    int synTextHeight = settings.value("SynArea/textHeight", 12).toInt();
    QString synFontFamily = settings.value("SynArea/textFontFamily", "Liberation Serif").toString();
    settings.endGroup();




    QTextBlockFormat blockFormat;
    blockFormat.setBottomMargin(synBottMargin);
    blockFormat.setTextIndent(synTextIndent);
    QTextCharFormat charFormat;
    charFormat.setFontPointSize(synTextHeight);
    charFormat.setFontFamily(synFontFamily);
    charFormat.clearForeground();
    QTextCursor *tCursor = new QTextCursor(document());
    tCursor->movePosition(QTextCursor::Start, QTextCursor::MoveAnchor,1);
    tCursor->movePosition(QTextCursor::End, QTextCursor::KeepAnchor,1);

    tCursor->mergeCharFormat(charFormat);
    tCursor->mergeBlockFormat(blockFormat);

    QFont font;
    font.setFamily(synFontFamily);
    font.setPointSize(synTextHeight);
    this->document()->setDefaultFont(font);

    emit connectUpdateTextsSignal();

 //   qDebug() << "applySynFontConfig";

}


//-------------------------------------------------------------------------------



void NoteZone::applyAttendConfig()
{


    QSettings settings;
    settings.beginGroup( "Settings" );
    alwaysCenter = settings.value("AttendArea/alwaysCenter", true).toBool();
    bool synShowScrollbar = settings.value("AttendArea/showScrollbar", true).toBool();
    settings.endGroup();


    centerCursor();

    if(synShowScrollbar)
        setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    else
        setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);


    applyAttendFontConfig();

}


void NoteZone::applyAttendFontConfig()
{

   QSettings settings;
   settings.beginGroup( "AttendManager" );
    int attendBottMargin = settings.value("detailTextMargin", 20).toInt();
    int attendTextIndent = settings.value("detailTextIndent", 20).toInt();
    int attendTextHeight = settings.value("detailTextHeight", 12).toInt();
    QString attendFontFamily = settings.value("detailFontFamily", "Liberation Serif").toString();
    settings.endGroup();




    QTextBlockFormat blockFormat;
    blockFormat.setBottomMargin(attendBottMargin);
    blockFormat.setTextIndent(attendTextIndent);
    QTextCharFormat charFormat;
    charFormat.setFontPointSize(attendTextHeight);
    charFormat.setFontFamily(attendFontFamily);
    charFormat.clearForeground();
    QTextCursor *tCursor = new QTextCursor(document());
    tCursor->movePosition(QTextCursor::Start, QTextCursor::MoveAnchor,1);
    tCursor->movePosition(QTextCursor::End, QTextCursor::KeepAnchor,1);

    tCursor->mergeCharFormat(charFormat);
    tCursor->mergeBlockFormat(blockFormat);

    QFont font;
    font.setFamily(attendFontFamily);
    font.setPointSize(attendTextHeight);
    this->document()->setDefaultFont(font);

}

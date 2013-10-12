

#include "fullscreen/fulltextzone.h"
//
FullTextZone::FullTextZone(QWidget *parent) :
    QTextEdit(parent)
{
    this->setMouseTracking(true);
    this->viewport()->setMouseTracking(true);

    createActions();
    setContextMenuPolicy(Qt::DefaultContextMenu);
    connect(this, SIGNAL(currentCharFormatChanged(QTextCharFormat)), this, SLOT(charFormat(QTextCharFormat)));
    connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(cursorPositionChangedSlot()));

    setWordWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
    //setMinimumWidth(600);
    toHtml();


    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    textDocument = new MainTextDocument;
}

void FullTextZone::setDoc(MainTextDocument *doc)
{

setDocument(doc);
textDocument = doc;
}

//--------- Context Menu :------------------------------------------------------------------


void FullTextZone::createActions()
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

//    leftAlignAct = new QAction(tr("&Left Align"), this);
//    leftAlignAct->setCheckable(true);
//    leftAlignAct->setShortcut(tr("Ctrl+L"));
//    leftAlignAct->setStatusTip(tr("Left align the selected text"));
//    connect(leftAlignAct, SIGNAL(triggered(bool)), this, SLOT(leftAlign(bool)));

//    rightAlignAct = new QAction(tr("&Right Align"), this);
//    rightAlignAct->setCheckable(true);
//    rightAlignAct->setShortcut(tr("Ctrl+R"));
//    rightAlignAct->setStatusTip(tr("Right align the selected text"));
//    connect(rightAlignAct, SIGNAL(triggered(bool)), this, SLOT(rightAlign(bool)));

//    justifyAct = new QAction(tr("&Justify"), this);
//    justifyAct->setCheckable(true);
//    justifyAct->setShortcut(tr("Ctrl+J"));
//    justifyAct->setStatusTip(tr("Justify the selected text"));
//    connect(justifyAct, SIGNAL(triggered(bool)), this, SLOT(justify(bool)));

//    centerAct = new QAction(tr("&Center"), this);
//    centerAct->setCheckable(true);
//    centerAct->setShortcut(tr("Ctrl+E"));
//    centerAct->setStatusTip(tr("Center the selected text"));
//    connect(centerAct, SIGNAL(triggered(bool)), this, SLOT(center(bool)));

//    alignmentGroup = new QMenu(tr("&Alignment"),this);
//    alignmentGroup->addAction(leftAlignAct);
//    alignmentGroup->addAction(rightAlignAct);
//    alignmentGroup->addAction(justifyAct);
//    alignmentGroup->addAction(centerAct);
//    leftAlignAct->setChecked(true);


//    setWidthAct = new QAction(tr("&Width"), this);
//    //   setWidthAct->setShortcuts(QKeySequence::Copy);
//    setWidthAct->setStatusTip(tr("Set text area width"));
//    connect(setWidthAct, SIGNAL(triggered()), this, SLOT(setWidth()));

//    setColorsAct = new QAction(tr("&Colors"), this);
//    //    setColorsAct->setShortcuts(QKeySequence::Copy);
//    setColorsAct->setStatusTip(tr("Set text and background colors"));
//    connect(setColorsAct, SIGNAL(triggered()), this, SLOT(setColors()));

//    setZoomAct = new QAction(tr("&Zoom"), this);
//    //    setColorsAct->setShortcuts(QKeySequence::Copy);
//    setZoomAct->setStatusTip(tr("Set text zoom"));
//    connect(setZoomAct, SIGNAL(triggered()), this, SLOT(setZoom()));

//    optionGroup = new QMenu(tr("&Options"),this);
//    optionGroup->addAction(setWidthAct);
//    optionGroup->addAction(setColorsAct);
//    optionGroup->addAction(setZoomAct);



}

//------------------------------------------------------------------

void FullTextZone::undo()
{
    textDocument->undo();
    //QTextEdit::undo();
}

void FullTextZone::redo()
{
    textDocument->redo();
}

void FullTextZone::cut()
{
    QTextEdit::cut();
}

void FullTextZone::copy()
{
    QTextEdit::copy();

}

void FullTextZone::paste()
{
    QTextEdit::paste();
}


void FullTextZone::bold(bool boldBool)
{
    if(boldBool){
        setFontWeight(75);
    }
    else{
        setFontWeight(50);
    }



}

void FullTextZone::italic(bool italBool)
{
    if(italBool){
        setFontItalic(true);
    }
    else{
        setFontItalic(false);
    }
}

//void FullTextZone::leftAlign(bool leftBool)
//{
//    if(leftBool){
//        centerAct->setChecked(false);
//        rightAlignAct->setChecked(false);
//        justifyAct->setChecked(false);
//        setAlignment(Qt::AlignLeft);
//    }
//    else{
//        leftAlignAct->setChecked(true);
//        leftAlign(true);
//    }
//}

//void FullTextZone::rightAlign(bool rightBool)
//{
//    if(rightBool){
//        centerAct->setChecked(false);
//        leftAlignAct->setChecked(false);
//        justifyAct->setChecked(false);
//        setAlignment(Qt::AlignRight);
//    }
//    else{
//        leftAlignAct->setChecked(true);
//        leftAlign(true);
//    }

//}

//void FullTextZone::justify(bool justBool)
//{
//    if(justBool){
//        centerAct->setChecked(false);
//        rightAlignAct->setChecked(false);
//        leftAlignAct->setChecked(false);
//        setAlignment(Qt::AlignJustify);
//    }
//    else{
//        leftAlignAct->setChecked(true);
//        leftAlign(true);
//    }

//}

//void FullTextZone::center(bool centBool)
//{
//    if(centBool){
//        rightAlignAct->setChecked(false);
//        leftAlignAct->setChecked(false);
//        justifyAct->setChecked(false);
//        setAlignment(Qt::AlignHCenter);
//    }
//    else{
//        leftAlignAct->setChecked(true);
//        leftAlign(true);
//    }
//}




//----------------------------------------------------------------------

void FullTextZone::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu menu(this);
    menu.addSeparator();
    menu.addAction(boldAct);
    menu.addAction(italicAct);
    menu.addSeparator();
//    menu.addMenu(alignmentGroup); // styles do that already
//    menu.addSeparator();
    menu.addAction(cutAct);
    menu.addAction(copyAct);
    menu.addAction(pasteAct);
    menu.addSeparator();
    menu.addAction(undoAct);
    menu.addAction(redoAct);
//    menu.addMenu(optionGroup);

    menu.exec(event->globalPos());
}

//--------------------------------------------------------------------------------

void FullTextZone::charFormat(QTextCharFormat cFormat)
{
    emit charFormatChangedSignal(cFormat);

    //  QString family = cFormat.fontFamily();
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



//    if (align & Qt::AlignLeft){
//        centerAct->setChecked(false);
//        rightAlignAct->setChecked(false);
//        justifyAct->setChecked(false);
//        leftAlignAct->setChecked(true);
//    }
//    else if (align & Qt::AlignHCenter){
//        rightAlignAct->setChecked(false);
//        justifyAct->setChecked(false);
//        leftAlignAct->setChecked(false);
//        centerAct->setChecked(true);
//    }
//    else if (align & Qt::AlignRight){
//        centerAct->setChecked(false);
//        justifyAct->setChecked(false);
//        leftAlignAct->setChecked(false);
//        rightAlignAct->setChecked(true);
//    }
//    else if (align & Qt::AlignJustify){
//        centerAct->setChecked(false);
//        rightAlignAct->setChecked(false);
//        leftAlignAct->setChecked(false);
//        justifyAct->setChecked(true);
//    }


    setFocus();
}

//--------------------------------------------------------------------------------




void FullTextZone::setTextFont(QFont font)
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


void FullTextZone::setTextHeight(int height)
{
    QTextCharFormat fmt;
    fmt.setFontPointSize(height);
    mergeFormatOnWordOrSelection(fmt);

}



void FullTextZone::mergeFormatOnWordOrSelection(const QTextCharFormat &format)


{
    QTextCursor cursor = textCursor();
    //    if(cursor.charFormat().fontItalic())
    //        format.setFontItalic(true);

    cursor.mergeCharFormat(format);
    mergeCurrentCharFormat(format);
}


//--------------------------------------------------------------------------------

void FullTextZone::keyPressEvent(QKeyEvent *event)
{
    if(event->matches(QKeySequence::Italic))
        italic(!italicAct->isChecked());
    else if(event->matches(QKeySequence::Bold))
        bold(!boldAct->isChecked());
//    else if(event->modifiers() == (Qt::ControlModifier|Qt::ShiftModifier) && event->key() == QKeySequence(tr("L")))    //: L for Left
//        leftAlign(true);
//    else if(event->modifiers() == (Qt::ControlModifier|Qt::ShiftModifier) && event->key() == QKeySequence(tr("R"))) //: R for Right
//        rightAlign(true);
//    else if(event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_E)
//        center(true);
//    else if(event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_J)
//        justify(true);
    else if(event->key() == Qt::Key_Space){
        if(preventDoubleSpaceOption == true)
            preventDoubleSpace();
        else
            QTextEdit::keyPressEvent(event);

    }
    else if(event->key() == Qt::Key_Escape || event->key() == Qt::Key_F11)
        emit quitFullScreen();
    else
        QTextEdit::keyPressEvent(event);
}



//--------------------------------------------------------------------------------
void FullTextZone::centerCursor()
{
    QRect cursor = cursorRect();
    QRect viewport =  this->viewport()->rect();
    if (alwaysCenter || (cursor.bottom() >= viewport.bottom()) || (cursor.top() <= viewport.top())) {
        QPoint offset = viewport.center() - cursor.center();
        QScrollBar* scrollbar = verticalScrollBar();
        scrollbar->setValue(scrollbar->value() - offset.y());
    }
}

//--------------------------------------------------------------------------------

void FullTextZone::cursorPositionChangedSlot()
{
    if (QApplication::mouseButtons() == Qt::NoButton) {
        centerCursor();
    }
    emit cursorPositionChangedSignal(this->textCursor().position());
}










//--------------------------------------------------------------------------------


void FullTextZone::insertFromMimeData (const QMimeData *source )
{

    QTextCursor cursor = this->textCursor();

    int bottMargin;
    int textIndent;
    int leftMargin;
    Qt::Alignment textAlignment;
    int textHeight;
    QString fontFamily;

    if(cursor.atStart() == true
            || cursor.position() == 1
            || cursor.position() == 0){
        int defaultIndex = textStyles->defaultStyleIndex();
        bottMargin = textStyles->blockBottomMarginAt(defaultIndex);
        textIndent = textStyles->blockFirstLineIndentAt(defaultIndex);
        leftMargin = textStyles->blockLeftMarginAt(defaultIndex);
        textAlignment = textStyles->blockAlignmentTrueNameAt(defaultIndex);
        textHeight = textStyles->fontSizeAt(defaultIndex) + textStyles->zoomModifier();
        fontFamily = textStyles->fontFamilyAt(defaultIndex);
    }
    else{

    bottMargin = cursor.blockFormat().bottomMargin();
    textIndent = cursor.blockFormat().textIndent();
    leftMargin = cursor.blockFormat().leftMargin();
    textAlignment = cursor.blockFormat().alignment();
    textHeight = cursor.charFormat().fontPointSize();
    fontFamily = cursor.charFormat().fontFamily();

}

    if(source->hasHtml()){
        QString sourceString = qvariant_cast<QString>(source->html());
        //        qDebug() << "                         sourceString  :  " << sourceString;
        //        sourceString.replace( QRegExp("<?(script|embed|object|frameset|frame|iframe|meta|link|style|div|a)"), "<div" );
        //qDebug() << "                       regExpedIs Valid  : " << QRegExp("<?(script|embed|object|frameset|frame|iframe|meta|link|style|div|a)").isValid();
        ////        sourceString.replace( QRegExp("/<(.|\n)*?>/g"), "" );




        //        qDebug() << "                       regExpedString  :  " << sourceString;

        ////
        //         /<\s*\w.*?>/g

        //htmlText
        QTextDocument *document = new QTextDocument;
        document->setHtml(sourceString);
        QTextBlockFormat blockFormat;
        blockFormat.setBottomMargin(bottMargin);
        blockFormat.setTextIndent(textIndent);
        blockFormat.setLeftMargin(leftMargin);
        blockFormat.setAlignment(textAlignment);
        blockFormat.setBottomMargin(0);
        blockFormat.setRightMargin(0);
        QTextCharFormat charFormat;
        charFormat.setFontPointSize(textHeight);
        charFormat.setFontFamily(fontFamily);
        charFormat.clearProperty(QTextFormat::ForegroundBrush);
        charFormat.clearProperty(QTextFormat::IsAnchor);
        charFormat.clearProperty(QTextFormat::AnchorHref);
        charFormat.clearProperty(QTextFormat::AnchorName);
        //        charFormat.clearProperty(QTextFormat::TextUnderlineStyle);
        //        charFormat.clearProperty(QTextFormat::FontStrikeOut);
        QTextCursor *tCursor = new QTextCursor(document);
        tCursor->movePosition(QTextCursor::Start, QTextCursor::MoveAnchor,1);
        tCursor->movePosition(QTextCursor::End, QTextCursor::KeepAnchor,1);

        tCursor->mergeCharFormat(charFormat);
        tCursor->mergeBlockFormat(blockFormat);


        QTextCursor cursor = this->textCursor();
        cursor.insertHtml(document->toHtml("utf-8"));
//        qDebug() << "insertFromMimeData Html";

    }
    else if(source->hasText()){
        QTextDocument *document = new QTextDocument;
        document->setPlainText(qvariant_cast<QString>(source->text()));

        QTextBlockFormat blockFormat;
        blockFormat.setBottomMargin(bottMargin);
        blockFormat.setTextIndent(textIndent);
        blockFormat.setBottomMargin(0);
        blockFormat.setRightMargin(0);
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
//        qDebug() << "insertFromMimeData plainText";

    }



}

//--------------------------------------------------------------------------------


bool FullTextZone::canInsertFromMimeData (const QMimeData *source) const
{

    if (source->hasHtml() || source->hasText())
        return true;

    else
        return QTextEdit::canInsertFromMimeData(source);


    //    qDebug() << "canInsertFromMimeData";
}
//--------------------------------------------------------------------------------
void FullTextZone::resizeEvent(QResizeEvent* event)
{
    centerCursor();
    textDocument->setTextWidth(this->width() - this->verticalScrollBar()->width() - 2);

    QWidget::resizeEvent(event);
}












void FullTextZone::mouseMoveEvent(QMouseEvent* event)
{
    if( event->buttons() == Qt::RightButton && !mDragging ){
        this->unsetCursor();
        mDragging = true;
    }


    if(mDragging)
        //    mDragEndPosition = event->pos();
        QTextEdit::mouseMoveEvent(event);

    else
        event->ignore();





}

void FullTextZone::mousePressEvent( QMouseEvent* event)
{
    mDragStartPosition = event->pos();
    QTextEdit::mousePressEvent( event );
}


void FullTextZone::mouseReleaseEvent( QMouseEvent* event)
{
    mDragging = false;
    QTextEdit::mouseReleaseEvent( event );
}


//--------------------------------------------------------------------------------

void FullTextZone::preventDoubleSpace()
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

//---------------------------------------------------------------------------
//----------Apply Config---------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------


void FullTextZone::applyTextConfig()
{
    QSettings settings;
    settings.beginGroup( "Settings" );
    alwaysCenter = settings.value("FullTextArea/alwaysCenter", true).toBool();
    preventDoubleSpaceOption = settings.value("preventDoubleSpace", false).toBool();
    settings.endGroup();
    setFixedWidth(settings.value("FullTextArea/areaWidth", 400).toInt());


    centerCursor();



}

//--------------------------------------------------------------------------------

void FullTextZone::applySynConfig()
{
    QSettings settings;
    settings.beginGroup( "Settings" );
    alwaysCenter = settings.value("FullSynArea/alwaysCenter", true).toBool();
    bool showScrollbar = settings.value("FullSynArea/showScrollbar", false).toBool();
    settings.endGroup();
    //   setFixedWidth(settings.value("FullTextArea/areaWidth", 400).toInt());


    centerCursor();

    if(showScrollbar)
        setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    else
        setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

}

//--------------------------------------------------------------------------------

void FullTextZone::applyNoteConfig()
{
    QSettings settings;
    settings.beginGroup( "Settings" );
    alwaysCenter = settings.value("FullNoteArea/alwaysCenter", true).toBool();
    bool showScrollbar = settings.value("FullNoteArea/showScrollbar", false).toBool();
    settings.endGroup();
    //   setFixedWidth(settings.value("FullTextArea/areaWidth", 400).toInt());


    centerCursor();

    if(showScrollbar)
        setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    else
        setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

}

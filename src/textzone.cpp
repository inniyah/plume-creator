#if QT_VERSION >= 0x050000
#include <QtWidgets>
#endif
#include <QtGui>   

#include "textzone.h"
//
TextZone::TextZone(QWidget *parent) :
    QTextEdit(parent), forceCopyWithoutFormatting(false), m_idNumber(-1),
    m_spellcheckBool(false)

{



}

TextZone::~TextZone()
{
    textDocument->deactivateSpellChecker();

    delete undoAct;
    delete redoAct;
    delete cutAct;
    delete copyAct;
    delete pasteAct;
    delete boldAct;
    delete italicAct;



}


void TextZone::createContent()
{
    //    sounds = new Sounds(this);


    this->setAttribute(Qt::WA_KeyCompression, true);

    textDocument = new MainTextDocument;

    createActions();
    setContextMenuPolicy(Qt::DefaultContextMenu);
    connect(this, SIGNAL(currentCharFormatChanged(QTextCharFormat)), this, SLOT(charFormat(QTextCharFormat)));
    connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(cursorPositionChangedSlot()));

    setWordWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);

    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);


    applyConfig();
}





void TextZone::setDoc(MainTextDocument *doc)
{
    textDocument = doc;
    this->setDocument(doc);

    if(this->objectName().mid(0,7) == "textDoc"
            || this->objectName().mid(0,6) == "synDoc"
            || this->objectName().mid(0,7) == "noteDoc")
        this->setIdNumber(this->objectName().mid(this->objectName().indexOf("_") + 1).toInt());

    connect(this, SIGNAL(cursorPositionChanged(int)), doc, SLOT(setCursorPos(int)));


    connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(cursorPositionChangedSlot()));






    // activate spell check at start

    QSettings settings;
    this->activateSpellcheck(settings.value("SpellChecking/globalSpellCheckActivated", true).toBool());


}


//--------- Context Menu :------------------------------------------------------------------


void TextZone::createActions()
{
    undoAct = new QAction(QIcon(":/pics/edit-undo.png"),tr("&Undo"), this);
    undoAct->setShortcuts(QKeySequence::Undo);
    undoAct->setStatusTip(tr("Undo the last operation"));
    connect(undoAct, SIGNAL(triggered()), this, SLOT(undo()));

    redoAct = new QAction(QIcon(":/pics/edit-redo.png"),tr("&Redo"), this);
    redoAct->setShortcuts(QKeySequence::Redo);
    redoAct->setStatusTip(tr("Redo the last operation"));
    connect(redoAct, SIGNAL(triggered()), this, SLOT(redo()));

    cutAct = new QAction(QIcon(":/pics/edit-cut.png"),tr("Cu&t"), this);
    cutAct->setShortcuts(QKeySequence::Cut);
    cutAct->setStatusTip(tr("Cut the current selection's contents to the "
                            "clipboard"));
    connect(cutAct, SIGNAL(triggered()), this, SLOT(cut()));

    copyAct = new QAction(QIcon(":/pics/edit-copy.png"),tr("&Copy"), this);
    copyAct->setShortcuts(QKeySequence::Copy);
    copyAct->setStatusTip(tr("Copy the current selection's contents to the "
                             "clipboard"));
    connect(copyAct, SIGNAL(triggered()), this, SLOT(copy()));

    pasteAct = new QAction(QIcon(":/pics/edit-paste.png"),tr("&Paste"), this);
    pasteAct->setShortcuts(QKeySequence::Paste);
    pasteAct->setStatusTip(tr("Paste the clipboard's contents into the current "
                              "selection"));
    connect(pasteAct, SIGNAL(triggered()), this, SLOT(paste()));

    pasteWithoutFormattingAct = new QAction(QIcon(":/pics/edit-paste.png"),tr("&Paste Unformatted"), this);
    pasteWithoutFormattingAct->setStatusTip(tr("Paste without formatting the clipboard's contents into the current "
                                               "selection"));
    QList<QKeySequence> klist;
    klist.append(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_V));
    pasteWithoutFormattingAct->setShortcuts(klist);
    connect(pasteWithoutFormattingAct, SIGNAL(triggered()), this, SLOT(pasteWithoutFormatting()));

    boldAct = new QAction(QIcon(":/pics/format-text-bold.png"),tr("&Bold"), this);
    boldAct->setCheckable(true);
    boldAct->setShortcuts(QKeySequence::Bold);
    boldAct->setStatusTip(tr("Make the text bold"));
    connect(boldAct, SIGNAL(triggered(bool)), this, SLOT(bold(bool)));

    QFont boldFont = boldAct->font();
    boldFont.setBold(true);
    boldAct->setFont(boldFont);

    italicAct = new QAction(QIcon(":/pics/format-text-italic.png"),tr("&Italic"), this);
    italicAct->setCheckable(true);
    italicAct->setShortcuts(QKeySequence::Italic);
    italicAct->setStatusTip(tr("Make the text italic"));
    connect(italicAct, SIGNAL(triggered(bool)), this, SLOT(italic(bool)));

    QFont italicFont = italicAct->font();
    italicFont.setItalic(true);
    italicAct->setFont(italicFont);

    spellcheckAct = new QAction(QIcon(""),tr("&Check Spelling"), this);
    spellcheckAct->setCheckable(true);
    //    spellcheckAct->setShortcuts(QKeySequence::Italic);
    spellcheckAct->setStatusTip(tr("Verify your spelling"));
    connect(spellcheckAct, SIGNAL(triggered(bool)), this, SIGNAL(activateSpellcheckSignal(bool)));

    addToUserDictAct = new QAction(QIcon(""),tr("&Add to Dictionary"), this);
    //    addToUserDictAct->setShortcuts(QKeySequence::Paste);
    addToUserDictAct->setStatusTip(tr("Add the current word selection to the project dictionary"));
    connect(addToUserDictAct, SIGNAL(triggered()), this, SLOT(addToUserDictionary()));

    addHyphenToUserDictAct = new QAction(QIcon(""),tr("Add &Hyphen to Dictionary"), this);
    //    addToUserDictAct->setShortcuts(QKeySequence::Paste);
    addHyphenToUserDictAct->setStatusTip(tr("Add the selected hyphenated word to the project dictionary"));
    connect(addHyphenToUserDictAct, SIGNAL(triggered()), this, SLOT(addHyphenToUserDictionary()));

    removeFromUserDictAct = new QAction(QIcon(""),tr("&Remove from Dictionary"), this);
    //    addToUserDictAct->setShortcuts(QKeySequence::Paste);
    removeFromUserDictAct->setStatusTip(tr("Remove the current word selection from the project dictionary"));
    connect(removeFromUserDictAct, SIGNAL(triggered()), this, SLOT(removeFromUserDictionary()));


    createEditWidget();
    QWidgetAction *editWidgetAct = new QWidgetAction(this);
    editWidgetAct->setDefaultWidget(editWidget);

    manageStylesAct = new QAction(/*QIcon(":/pics/edit-find-replace.png"),*/tr("Manage &Styles"),this);
    // aboutAct->setShortcut(QKeySequence::Quit);
    manageStylesAct->setToolTip(tr("Manage the styles"));
    connect(manageStylesAct, SIGNAL(triggered()), this, SIGNAL(manageStylesSignal()));

    stylesGroup = new QMenu(tr("&Styles"),this);
    stylesGroup->addAction(editWidgetAct);
    stylesGroup->addAction(manageStylesAct);


    //    setLineSpacingAct = new QAction(tr("Set &Line Spacing..."), this);
    //    setLineSpacingAct->setStatusTip(tr("Change the gap between the lines of a "
    //                                       "paragraph"));
    //    connect(setLineSpacingAct, SIGNAL(triggered()), this, SLOT(setLineSpacing()));

    //    setParagraphSpacingAct = new QAction(tr("Set &Paragraph Spacing..."), this);
    //    setParagraphSpacingAct->setStatusTip(tr("Change the gap between paragraphs"));
    //    connect(setParagraphSpacingAct, SIGNAL(triggered()),
    //            this, SLOT(setParagraphSpacing()));

    //    leftAlignAct = new QAction(QIcon(":/pics/format-justify-left.png"),tr("&Left Align"), this);
    //    leftAlignAct->setCheckable(true);
    //    leftAlignAct->setShortcut(tr("Ctrl+L"));
    //    leftAlignAct->setStatusTip(tr("Left align the selected text"));
    //    connect(leftAlignAct, SIGNAL(triggered(bool)), this, SLOT(leftAlign(bool)));

    //    rightAlignAct = new QAction(QIcon(":/pics/format-justify-right.png"),tr("&Right Align"), this);
    //    rightAlignAct->setCheckable(true);
    //    rightAlignAct->setShortcut(tr("Ctrl+R"));
    //    rightAlignAct->setStatusTip(tr("Right align the selected text"));
    //    connect(rightAlignAct, SIGNAL(triggered(bool)), this, SLOT(rightAlign(bool)));

    //    justifyAct = new QAction(QIcon(":/pics/format-justify-fill.png"),tr("&Justify"), this);
    //    justifyAct->setCheckable(true);
    //    justifyAct->setShortcut(tr("Ctrl+J"));
    //    justifyAct->setStatusTip(tr("Justify the selected text"));
    //    connect(justifyAct, SIGNAL(triggered(bool)), this, SLOT(justify(bool)));

    //    centerAct = new QAction(QIcon(":/pics/format-justify-center.png"),tr("&Center"), this);
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




}

void TextZone::addActions(QList<QAction*> actions)
{
    m_actions = actions;
}


//------------------------------------------------------------------

void TextZone::undo()
{
    textDocument->undo();
    //QTextEdit::undo();
}

void TextZone::redo()
{
    textDocument->redo();
}

void TextZone::cut()
{
    QTextEdit::cut();
}

void TextZone::copy()
{
    QTextEdit::copy();
}

void TextZone::paste()
{
    QTextEdit::paste();
}
void TextZone::pasteWithoutFormatting()
{
    forceCopyWithoutFormatting = true;
    QTextEdit::paste();
    forceCopyWithoutFormatting = false;

}
void TextZone::bold(bool boldBool)
{
    if(boldBool){
        setFontWeight(75);
    }
    else{
        setFontWeight(50);
    }
}

void TextZone::italic(bool italBool)
{
    if(italBool){
        setFontItalic(true);
    }
    else{
        setFontItalic(false);
    }
}

void TextZone::activateSpellcheck(bool spellcheckBool)
{


if(m_spellcheckBool == spellcheckBool){
    return;
}

    if(spellcheckBool){
        if(!textDocument->activateSpellChecker()){
//                        QMessageBox::warning(this, tr("Plume Creator Spell Checker"), tr("No dictionary is selected. Please select one in "));
            spellcheckAct->setChecked(false);
            m_spellcheckBool = false;
            //hub->showStatusBarMessage(tr("No dictionary is selected. Please select one in Settings/Spelling."));
            qDebug() << "TextZone : !textDocument->activateSpellChecker()";
            return;
        }
        spellcheckAct->setChecked(true);
        m_spellcheckBool = true;

    }
    else{

        textDocument->deactivateSpellChecker();
        spellcheckAct->setChecked(false);
        m_spellcheckBool = false;
    }



}

void TextZone::addToUserDictionary()
{
    if(selectedWord.isEmpty())
        return;

    textDocument->spellChecker()->addToUserWordlist(selectedWord);

}
void TextZone::addHyphenToUserDictionary()
{
    if(selectedWord.isEmpty())
        return;



    textDocument->spellChecker()->addToUserWordlist(selectedHyphenWord);

}
void TextZone::removeFromUserDictionary()
{
    if(selectedWord.isEmpty())
        return;

    textDocument->spellChecker()->removeFromUserWordlist(selectedWord);

}

//void TextZone::leftAlign(bool leftBool)
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

//void TextZone::rightAlign(bool rightBool)
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

//void TextZone::justify(bool justBool)
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

//void TextZone::center(bool centBool)
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

void TextZone::contextMenuEvent(QContextMenuEvent *event)
{
    QTextCursor tCursor = this->textCursor();
    int tCursorPos = tCursor.position();
    int tCursorAnchor = tCursor.anchor();

    int minSelect = qMin(tCursorPos, tCursorAnchor);
    int maxSelect = qMax(tCursorPos, tCursorAnchor);

    QTextCursor cursor= this->cursorForPosition(event->pos());

    if(cursor.position() < minSelect || cursor.position() > maxSelect){
        this->setTextCursor(cursor);

    }


    QMenu menu(this);

    if(m_spellcheckBool){
        selectedWord.clear();


        cursor.select(QTextCursor::WordUnderCursor);


        selectedWord=cursor.selection().toPlainText();


        menu.addSeparator();

        bool isWellSpelled;
        if(textDocument->spellChecker()->spell(selectedWord) != 0)
            isWellSpelled = true;
        else
            isWellSpelled = false;


        if(!isWellSpelled){

            QStringList suggestions = textDocument->spellChecker()->suggest(selectedWord);

            QList<QAction *> suggestionWordsList;

            if(!suggestions.isEmpty())
                for(int i=0;i<suggestions.count();i++)
                {
                    QAction *suggestedWord = new QAction(this);
                    suggestedWord->setText(suggestions.at(i));
                    connect(suggestedWord, SIGNAL(triggered()),this, SLOT(replaceWord()));
                    suggestionWordsList.append(suggestedWord);
                }
            if(suggestions.isEmpty()){
                QAction *suggestedWord = new QAction(this);
                suggestedWord->setText(tr("No suggestion"));
                suggestedWord->setDisabled(true);
                suggestionWordsList.append(suggestedWord);
            }
            menu.addActions(suggestionWordsList);
        }
        if(!cursor.selection().isEmpty()){

            QTextCursor hyphenCursor = this->textCursor();
            int cursorMax = qMax(cursor.anchor(), cursor.position());
            hyphenCursor.setPosition(cursorMax, QTextCursor::MoveAnchor);
            hyphenCursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor);

            //            qDebug() << "anchor : "   + QString::number(cursor.anchor());
            //            qDebug() << "pos : "   + QString::number(cursor.position());

            //            qDebug() << "hyphenCursorSelection : " + hyphenCursor.selection().toPlainText();

            if(!isWellSpelled && !textDocument->spellChecker()->isInUserWordlist(selectedWord))
            {
                menu.addSeparator();
                menu.addAction(addToUserDictAct);

                if(!isWellSpelled && !textDocument->spellChecker()->isInUserWordlist(selectedWord)
                        && hyphenCursor.selection().toPlainText() == "-")
                {
                    int hyphenCursorMin = qMin(hyphenCursor.anchor(), hyphenCursor.position());
                    hyphenCursor.setPosition(hyphenCursorMin);
                    hyphenCursor.movePosition(QTextCursor::NextWord, QTextCursor::KeepAnchor, 2);
                    selectedHyphenWord = selectedWord + hyphenCursor.selection().toPlainText();
                    if(selectedHyphenWord.right(1) == " ")
                        selectedHyphenWord.chop(1);
                    //                                    qDebug() << "selectedHyphenWord : " + selectedHyphenWord;
                    menu.addSeparator();
                    menu.addAction(addHyphenToUserDictAct);
                }
            }
            else if(textDocument->spellChecker()->isInUserWordlist(selectedWord)){
                menu.addSeparator();
                menu.addAction(removeFromUserDictAct);
            }
        }

        menu.addSeparator();
    }



    menu.addAction(boldAct);
    menu.addAction(italicAct);
    menu.addSeparator();
    //    menu.addMenu(alignmentGroup); // styles do that already
    menu.addMenu(stylesGroup);
    menu.addAction(spellcheckAct);
    menu.addSeparator();
    menu.addAction(cutAct);
    menu.addAction(copyAct);
    menu.addAction(pasteAct);
    menu.addAction(pasteWithoutFormattingAct);
    menu.addSeparator();
    menu.addAction(undoAct);
    menu.addAction(redoAct);
    menu.addSeparator();
    menu.addActions(m_actions);

    menu.exec(event->globalPos());
}

//--------------------------------------------------------------------------------

void TextZone::charFormat(QTextCharFormat cFormat)
{
    emit charFormatChangedSignal(cFormat);

    //  QString family = cFormat.fontFamily();
    int weight = cFormat.fontWeight();
    bool italic = cFormat.fontItalic();
    //    Qt::Alignment align = alignment();

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




void TextZone::setTextFont(QFont font)
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
//--------------------------------------------------------------------------------

void TextZone::setTextHeight(int height)
{
    QTextCharFormat fmt;
    fmt.setFontPointSize(height);
    mergeFormatOnWordOrSelection(fmt);
}

//--------------------------------------------------------------------------------

void TextZone::mergeFormatOnWordOrSelection(const QTextCharFormat &format)


{
    QTextCursor cursor = this->textCursor();
    //    if(cursor.charFormat().fontItalic())
    //        format.setFontItalic(true);

    cursor.mergeCharFormat(format);
    mergeCurrentCharFormat(format);
}

//--------------------------------------------------------------------------------

void TextZone::createEditWidget()
{

    editWidget = new EditMenu;
    editWidget->setHub(hub);
    editWidget->setTextStyles(textStyles);
    editWidget->createContent();

    QStringList widgetToHideList;
    widgetToHideList << "zoomBox" << "textWidthBox" << "fontComboBox" << "fontSizeSpinBox";
    editWidget->hideWidgetsByName(widgetToHideList);

    //editWidget->setFrameStyle(QFrame::Panel);
    //editWidget->setLineWidth(2);
    //editWidget->setMidLineWidth(3);

    // repeater to join editWidget to MainWindow :

    //    connect(editWidget, SIGNAL(widthChangedSignal(int)), this, SIGNAL(widthChangedSignal(int)));

    //    connect(editWidget,SIGNAL(textFontChangedSignal(QFont)),this,SIGNAL(textFontChangedSignal(QFont)));
    //    connect(editWidget,SIGNAL(textHeightChangedSignal(int)),this,SIGNAL(textHeightChangedSignal(int)));

    //    connect(this,SIGNAL(charFormatChangedSlotSignal(QTextCharFormat)),editWidget,SLOT(charFormatChangedSlot(QTextCharFormat)));

    connect(editWidget,SIGNAL(styleSelectedSignal(int)), this, SLOT(changeTextStyleSlot(int)));
    connect(this,SIGNAL(setStyleSelectionSignal(int)), editWidget, SLOT(setStyleSelectionSlot(int)));
}

//--------------------------------------------------------------------------------

void TextZone::keyPressEvent(QKeyEvent *event)
{
    //    sounds->startSoundKeyAny();


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

#if QT_VERSION >= 0x050000
    else if(event->modifiers() == (Qt::ShiftModifier|Qt::ControlModifier) && event->key() == QVariant(QKeySequence(tr("V", "paste unformated"))).toInt())
        pasteWithoutFormatting();

#else
    else if(event->modifiers() == (Qt::ControlModifier | Qt::ShiftModifier) && event->key() == QKeySequence(tr("V", "paste unformated")))
        pasteWithoutFormatting();

#endif


    else if(event->key() == Qt::Key_Space){
        if(preventDoubleSpaceOption == true)
            preventDoubleSpace();
        else
            QTextEdit::keyPressEvent(event);

    }
    else{
        QTextEdit::keyPressEvent(event);
    }

}



//--------------------------------------------------------------------------------
void TextZone::centerCursor()
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

void TextZone::cursorPositionChangedSlot()
{
    if (QApplication::mouseButtons() == Qt::NoButton) {
        centerCursor();
    }


    emit cursorPositionChanged(this->textCursor().position());



    // for textstyle :
    QTextCursor tCursor = this->textCursor();

    if((tCursor.atStart() == true
        || tCursor.position() == 1
        || tCursor.position() == 0) && tCursor.hasSelection() == false){
        this->changeTextStyleSlot(textStyles->defaultStyleIndex());
    }

    int currentStyleIndex = textStyles->compareStylesWithText(tCursor.charFormat(), tCursor.blockFormat());

    emit setStyleSelectionSignal(currentStyleIndex);


}








//--------------------------------------------------------------------------------


void TextZone::insertFromMimeData (const QMimeData *source )
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
        textHeight = textStyles->fontSizeAt(defaultIndex);
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

    if(source->hasHtml() && !forceCopyWithoutFormatting){
        QString sourceString = qvariant_cast<QString>(source->html());


        //htmlText
        QTextDocument *document = new QTextDocument;
        document->setHtml(Utils::parseHtmlText(sourceString));
        QTextBlockFormat blockFormat;
        blockFormat.setBottomMargin(bottMargin);
        blockFormat.setTextIndent(textIndent);
        blockFormat.setLeftMargin(leftMargin);
        blockFormat.setAlignment(textAlignment);
        blockFormat.setRightMargin(0);
        QTextCharFormat charFormat;
        charFormat.setFontPointSize(textHeight);
        charFormat.setFontFamily(fontFamily);

        charFormat.setBackground(QBrush(Qt::NoBrush));
        charFormat.setForeground(QBrush(Qt::NoBrush));
        charFormat.setAnchor(false);
        charFormat.setUnderlineStyle(QTextCharFormat::NoUnderline);
        charFormat.setFontStrikeOut(false);

        QTextCursor *tCursor = new QTextCursor(document);
        tCursor->movePosition(QTextCursor::Start, QTextCursor::MoveAnchor,1);
        tCursor->movePosition(QTextCursor::End, QTextCursor::KeepAnchor,1);

        tCursor->mergeCharFormat(charFormat);
        tCursor->mergeBlockFormat(blockFormat);

        QTextCursor cursor = this->textCursor();
        cursor.insertHtml(document->toHtml("utf-8"));
        qDebug() << "insertFromMimeData Html";

    }
    else if(source->hasText() || forceCopyWithoutFormatting){
        QTextDocument *document = new QTextDocument;
        document->setPlainText(qvariant_cast<QString>(source->text()));

        QTextBlockFormat blockFormat;
        blockFormat.setBottomMargin(bottMargin);
        blockFormat.setTextIndent(textIndent);
        blockFormat.setLeftMargin(leftMargin);
        blockFormat.setAlignment(textAlignment);
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
        qDebug() << "insertFromMimeData plainText";

    }


}

//--------------------------------------------------------------------------------


bool TextZone::canInsertFromMimeData (const QMimeData *source) const
{

    if (source->hasHtml() || source->hasText())
        return true;

    else
        return QTextEdit::canInsertFromMimeData(source);


    //    qDebug() << "canInsertFromMimeData";
}
//--------------------------------------------------------------------------------
void TextZone::resizeEvent(QResizeEvent* event)
{
    centerCursor();
    textDocument->setTextWidth(this->width());
    QWidget::resizeEvent(event);

}



//--------------------------------------------------------------------------------


void TextZone::scrollBy(QPoint viewportPoint)
{
    this->scrollContentsBy(viewportPoint.x(), viewportPoint.y());
}

//--------------------------------------------------------------------------------

void TextZone::preventDoubleSpace()
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
//-----------Apply Config-------------------------------------------------------
//-------------------------------------------------------------------------------

void TextZone::applyConfig()
{
    QSettings settings;
    settings.beginGroup( "Settings" );
    alwaysCenter = settings.value("TextArea/alwaysCenter", true).toBool();
    showScrollbar = settings.value("TextArea/showScrollbar", true).toBool();
    preventDoubleSpaceOption = settings.value("preventDoubleSpace", false).toBool();
    settings.endGroup();

    centerCursor();

    if(showScrollbar)
        setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    else
        setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);


    editWidget->applyConfig();
}

int TextZone::idNumber() const
{
    return m_idNumber;
}

void TextZone::setIdNumber(int idNumber)
{
    m_idNumber = idNumber;
}


void TextZone::replaceWord()
{
    QAction *action = qobject_cast<QAction *>(sender());
    if (action){
        QTextCursor cursor = this->textCursor();
        cursor.select(QTextCursor::WordUnderCursor);
        this->setTextCursor(cursor);
        insertPlainText(action->text());
    }
}


//void TextZone::addToDictionary()
//{
//    textDocument->spellChecker()->addToUserWordlist(selectedWord);
//    insertPlainText(selectedWord);
//}










//-------------------------------------------------------------------------------



void TextZone::changeTextStyleSlot(int styleIndex)
{


    QTextBlockFormat blockFormat;
    blockFormat.setBottomMargin(textStyles->blockBottomMarginAt(styleIndex));
    blockFormat.setTextIndent(textStyles->blockFirstLineIndentAt(styleIndex));
    blockFormat.setLeftMargin(textStyles->blockLeftMarginAt(styleIndex));
    blockFormat.setAlignment(textStyles->blockAlignmentTrueNameAt(styleIndex));
    blockFormat.setTopMargin(0);
    blockFormat.setRightMargin(0);
    QTextCharFormat charFormat;
    charFormat.setFontPointSize(textStyles->fontSizeAt(styleIndex));
    charFormat.setFontFamily(textStyles->fontFamilyAt(styleIndex));
    //    charFormat.setFontItalic(textStyles->fontItalicAt(styleIndex));
    //    if (textStyles->fontBoldAt(styleIndex) == true)
    //        charFormat.setFontWeight(75);
    //    else
    //        charFormat.setFontWeight(50);
    //    charFormat.setFontUnderline(textStyles->fontUnderlineAt(styleIndex));
    //    charFormat.setFontStrikeOut(textStyles->fontStrikeOutAt(styleIndex));

    //    charFormat.clearForeground();


    QTextCursor tCursor = this->textCursor();

    // select all of the blocks selected :

    QTextCursor tStartCursor = this->textCursor();
    tStartCursor.setPosition(tCursor.selectionStart());
    tStartCursor.movePosition(QTextCursor::StartOfBlock);
    int startFirstBlock = tStartCursor.position();

    QTextCursor tEndCursor =  this->textCursor();
    tEndCursor.setPosition(tCursor.selectionEnd());
    tEndCursor.movePosition(QTextCursor::EndOfBlock);
    int endLastBlock = tEndCursor.position();

    tCursor.setPosition(startFirstBlock);
    tCursor.setPosition(endLastBlock, QTextCursor::KeepAnchor);


    // merge :
    tCursor.mergeBlockFormat(blockFormat);
    tCursor.mergeCharFormat(charFormat);
    this->mergeCurrentCharFormat(charFormat);

}

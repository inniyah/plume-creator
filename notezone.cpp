#include "notezone.h"

NoteZone::NoteZone(QWidget *parent) :
    QTextEdit(parent)
{
    textDocument = new QTextDocument(this);


    toHtml();

    createActions();

    setContextMenuPolicy(Qt::PreventContextMenu);

    connect(this, SIGNAL(currentCharFormatChanged(QTextCharFormat)), this, SLOT(charFormat(QTextCharFormat)));



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


bool NoteZone::openNote(QFile *noteFile, QString name)
{

    noteStackName = name;


    this->setEnabled(true);






    noteFile->open(QFile::ReadOnly | QFile::Text);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    QTextStream noteFileStream( noteFile );
    textDocument->setHtml(noteFileStream.readAll());
    //  this->setText( noteFileStream.readAll() );
    QApplication::restoreOverrideCursor();
    noteFile->close();


    setDocument(textDocument);

setContextMenuPolicy(Qt::DefaultContextMenu);

    return true;
}

//------------------------------------------------------------------------------


bool NoteZone::openSyn(QFile *synFile, QString name)
{

    synStackName = name;


    this->setEnabled(true);



    //    QFile *tempNoteFile = new QFile(noteFile->fileName() + ".temp");
    //    if(tempNoteFile->exists()){
    //        tempNoteFile->open(QFile::ReadOnly | QFile::Text);
    //        QApplication::setOverrideCursor(Qt::WaitCursor);
    //        QTextStream tempNoteFileStream( tempNoteFile );
    //        this->setText( tempNoteFileStream.readAll() );
    //        QApplication::restoreOverrideCursor();

    //        //        this->setHtml(tempNoteFileArray);
    //        tempNoteFile->close();
    //    }
    //    else{
    synFile->open(QFile::ReadOnly | QFile::Text);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    QTextStream synFileStream( synFile );
    textDocument->setHtml(synFileStream.readAll());
    //this->setText( synFileStream.readAll() );
    QApplication::restoreOverrideCursor();

    //        this->setHtml(noteFileArray);
    synFile->close();
    //    }
    //    noteOpened = true;

    //    noteFileOpened = noteFile;

    setDocument(textDocument);


    setContextMenuPolicy(Qt::DefaultContextMenu);

    return true;
}

//------------------------------------------------------------------------------


bool NoteZone::saveSyn(QFile *synFile, QString name)
{
    if(name != synStackName)
        return false;


    //    synFile->setFileName(synFile->fileName());
    //    synFile->open(QFile::WriteOnly | QFile::Text);

    //    if(synFile->isWritable())
    //    {


    //        QTextStream stream(synFile);
    //        stream << this->toHtml();
    //        stream.flush();
    //        synFile->close();

    //        return true;

    //    }
    //    else{


    //        qDebug() << synFile->fileName() << " isn't Writtable.";
    //        return false;
    //    }

    QTextDocumentWriter docWriter(synFile, "HTML");
    bool written = docWriter.write(textDocument);

    return written;
}



//------------------------------------------------------------------------------

bool NoteZone::saveNote(QFile *noteFile, QString name)
{
    if(name != noteStackName)
        return false;


    //    QFile file;
    //    file.setFileName(noteFile->fileName());
    //    file.open(QFile::WriteOnly | QFile::Text);

    //    if(file.isWritable())
    //    {

    //        QTextStream stream(&file);
    //        stream << this->toHtml();
    //        stream.flush();
    //        file.close();

    //        return true;
    //    }
    //    else{
    //        qDebug() << noteFile->fileName() << " isn't Writtable.";

    //        return false;
    //    }

    QTextDocumentWriter docWriter(noteFile, "HTML");
    bool written = docWriter.write(textDocument);

    return written;
}



//------------------------------------------------------------------------------

bool NoteZone::closeSyn(QFile *synFile, QString name)
{

    if(name != synStackName)
        return false;

    this->clear();
    this->setEnabled(false);

    setContextMenuPolicy(Qt::PreventContextMenu);

    return true;
}

//------------------------------------------------------------------------------


bool NoteZone::closeNote(QFile *noteFile, QString name)
{
    if(name != noteStackName)
        return false;


    this->clear();
    this->setEnabled(false);

    setContextMenuPolicy(Qt::PreventContextMenu);

    return true;
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



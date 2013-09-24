#include "exporter/exporter.h"
#include "ui_exporter.h"

//
Exporter::Exporter(QString mode, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Exporter)
{
    ui->setupUi(this);


    dialogMode = mode;


    if(dialogMode == "export"){
        setWindowTitle(tr("Export Dialog"));
    }
        if(dialogMode == "print"){
        setWindowTitle(tr("Print Dialog"));
ui->stackedWidget->setCurrentIndex(0);
}

    ui->fileNameLineEdit->setValidator(new QRegExpValidator(QRegExp("[^\x002F\\\\:\x002A\?\x0022<>|]+"), ui->fileNameLineEdit));

    //    directoryLabel = new QLabel(tr("Path : "));


    connect(ui->pathButton, SIGNAL(clicked()), this, SLOT(setExistingDirectory()));


    if(dialogMode == "print")
    {
        ui->fileNameLabel->hide();
        ui->fileNameLineEdit->hide();
        ui->pathButton->hide();
        ui->pathLineEdit->hide();
        ui->fileTypeLabel->hide();
        ui->fileTypeComboBox->hide();
    }





    if(dialogMode == "print")
    {
        QWidget *widget = new QWidget;
        QVBoxLayout *layout = new QVBoxLayout;
        layout->addWidget(ui->buttonBox);
        widget->setLayout(layout);
        delete widget;

        QDialogButtonBox *box = new QDialogButtonBox(this);
        QPushButton *cancelBut = box->addButton(QDialogButtonBox::Cancel);
        QPushButton *acceptBut = box->addButton(tr("Print"), QDialogButtonBox::AcceptRole);
    ui->verticalLayout->addWidget(box);


    connect(cancelBut, SIGNAL(clicked()), this, SLOT(close()));
    connect(acceptBut, SIGNAL(clicked()), this, SLOT(accept()));


    }

    connect(ui->previewPushButton, SIGNAL(clicked()), this, SLOT(seePreview()));



}



void Exporter::postConstructor()
{

    createTree();

    applyConfig();

}

Exporter::~Exporter()
{
    delete ui;


}




//-----------------------------------------------------------------------------------

void Exporter::accept()
{
    if(dialogMode == "export")
    {
        if(ui->pathLineEdit->text() == "" || ui->fileNameLineEdit->text() == "" ){
            QMessageBox::information(this, tr("Project Exporter"), tr("The destination fields must be completed !"), QMessageBox::Ok);
            return;
        }

        QApplication::setOverrideCursor(Qt::WaitCursor);

        if(ui->fileTypeComboBox->currentIndex() == 3){
            format = "csv";
            this->exportInCSV();
        }
        if(ui->fileTypeComboBox->currentIndex() == 4){
            format = "pdf";
            this->exportInPDF();
        }
        else
            this->exportDoc();
        QApplication::restoreOverrideCursor();



    }
    else if(dialogMode == "print")
    {


        QApplication::setOverrideCursor(Qt::WaitCursor);
        this->print();
        QApplication::restoreOverrideCursor();



    }

    this->saveConfig();

    QDialog::accept();



}
//-----------------------------------------------------------------------------------

void Exporter::setExistingDirectory()
{
    QFileDialog::Options options = QFileDialog::DontResolveSymlinks | QFileDialog::ShowDirsOnly;
    QString directory = QFileDialog::getExistingDirectory(this, tr("Select a directory"),
                                                          ui->pathLineEdit->text(),
                                                          options);
    if (!directory.isEmpty())
        ui->pathLineEdit->setText(directory);
}









//-----------------------------------------------------------------------------------
//--------Tree-----------------------------------------------------------------
//-----------------------------------------------------------------------------------



void Exporter::createTree()
{
    proxy = new CheckableTreeProxy(this, "exporterExpanded", "exporterCheckState", MainTreeItem::Exporter);
    proxy->setHub(hub);
    proxy->setSourceModel(absTreeModel);
    proxy->setTreeFlags(CheckableTreeProxy::SeparatorCheckable);
    ui->tree->setHub(hub);
    ui->tree->setMainTreeAbstractModel(absTreeModel);
    ui->tree->setModel(proxy);
    ui->tree->applySettingsFromData();



}




//---------------------------------------------------------------------------------------

void Exporter::closeTree()
{



}







QTextDocument * Exporter::buildFinalDoc()
{
    //search for checked items :

    QDomDocument domDoc = hub->mainTreeDomDoc();
    QDomElement root = domDoc.documentElement();

    QList<QDomElement> itemList = searchForCheckedItems(root);

    if(itemList.size() == 0)
        return new QTextDocument();


    // set up the progress bar :
    QWidget *progressWidget = new QWidget(this, Qt::Dialog | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    QHBoxLayout *progressLayout = new QHBoxLayout(progressWidget);
    QProgressBar *progressBar = new QProgressBar(progressWidget);
    int progressValue = 0;

    progressLayout->addWidget(progressBar);
    progressWidget->setLayout(progressLayout);

    progressBar->setMaximum(itemList.size());
    progressBar->setValue(progressValue);
    progressWidget->show();



    //    QString debug;
    //    qDebug() << "itemList" << debug.setNum(itemList->size());

    QTextDocument *textDocument = new QTextDocument;
    QTextEdit *edit = new QTextEdit;

    textDocument->setDefaultStyleSheet("p, li { white-space: pre-wrap; } p{line-height: 2em; font-family:'Liberation Serif'; font-size:12pt;margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:72px;}");



    for(int i = 0; i < itemList.size(); ++i){
        QDomElement element = itemList.at(i);
        QTextCursor *tCursor = new QTextCursor(textDocument);

        QTextBlockFormat blockFormatLeft;
        blockFormatLeft.setBottomMargin(0);
        blockFormatLeft.setTopMargin(0);
        blockFormatLeft.setTextIndent(72);
        blockFormatLeft.setLineHeight(200, QTextBlockFormat::ProportionalHeight);
        blockFormatLeft.setAlignment(Qt::AlignJustify);
        QTextCharFormat charFormatLeft;
        charFormatLeft.setFontPointSize(12);
        charFormatLeft.setFontFamily("Courrier");

        QTextBlockFormat blockFormatCenter;
        blockFormatCenter.setAlignment(Qt::AlignCenter);


        if(element.tagName() != "separator"){

            qDebug() << "element name : "+ element.attribute("name");
            MainTextDocument *textDoc = hub->findChild<MainTextDocument *>("textDoc_" + element.attribute("number"));
            MainTextDocument *synDoc = hub->findChild<MainTextDocument *>("synDoc_" + element.attribute("number"));
            MainTextDocument *noteDoc = hub->findChild<MainTextDocument *>("noteDoc_" + element.attribute("number"));

            QTextDocumentFragment textFrag(prepareTextDoc(textDoc));
            QTextDocumentFragment synFrag(prepareSynDoc(synDoc));
            QTextDocumentFragment noteFrag(prepareNoteDoc(noteDoc));

            edit->setDocument(textDocument);




            if(element.tagName() == "book"){
                textDocument->setMetaInformation(QTextDocument::DocumentTitle,element.attribute("name", ""));
                edit->append("<h1>" + element.attribute("name", "") + "</h1>");
                tCursor->movePosition(QTextCursor::End, QTextCursor::MoveAnchor,1);
                tCursor->mergeBlockFormat(blockFormatCenter);
                edit->append("<h4>" + QDateTime::currentDateTime().toString() + "</h4>");
                tCursor->movePosition(QTextCursor::End, QTextCursor::MoveAnchor,1);
                tCursor->mergeBlockFormat(blockFormatCenter);
                edit->append("<br>");
                edit->append("<br>");

            }
            if(element.tagName() == "act"){
                edit->append("<br>");
                edit->append("<br>");
                edit->append("<h2>" + element.attribute("name", "") + "</h2>");
                tCursor->movePosition(QTextCursor::End, QTextCursor::MoveAnchor,1);
                tCursor->mergeBlockFormat(blockFormatCenter);
                edit->append("<br>");

            }
            if(element.tagName() == "chapter"){
                edit->append("<br>");
                edit->append("<br>");
                edit->append("<h2>" + element.attribute("name", "") + "</h2>");
                tCursor->movePosition(QTextCursor::End, QTextCursor::MoveAnchor,1);
                tCursor->mergeBlockFormat(blockFormatCenter);
                edit->append("<br>");

            }

            if(element.tagName() == "scene" && ui->setSceneTitlesComboBox->currentIndex() != 0){
                QString sceneTitle;
                switch (ui->setSceneTitlesComboBox->currentIndex()){
                case 1:
                    sceneTitle = element.attribute("name", "");
                    break;
                case 2:
                    sceneTitle = "###";
                    break;
                case 3:
                    sceneTitle = "***";
                    break;
                default:
                    sceneTitle = element.attribute("name", "");
                    break;

                }

                edit->append("<br>");
                edit->append("<h3>" + sceneTitle + "</h3>");
                tCursor->movePosition(QTextCursor::End, QTextCursor::MoveAnchor,1);
                tCursor->mergeBlockFormat(blockFormatCenter);
                edit->append("<br>");

            }

            if(ui->synopsisCheckBox->isChecked() && !synFrag.isEmpty()){
                edit->append("<br>");
                edit->append("<h4>" + tr("Synopsis") + "</h4>");
                tCursor->movePosition(QTextCursor::End, QTextCursor::MoveAnchor,1);
                tCursor->mergeBlockFormat(blockFormatCenter);
                edit->append("<br>");
                tCursor->movePosition(QTextCursor::End, QTextCursor::MoveAnchor,1);
                tCursor->insertBlock(blockFormatLeft, charFormatLeft);
                tCursor->insertFragment(synFrag);
            }

            if(ui->notesCheckBox->isChecked() && !noteFrag.isEmpty()){
                edit->append("<br>");
                edit->append("<h4>" + tr("Note") + "</h4>");
                tCursor->movePosition(QTextCursor::End, QTextCursor::MoveAnchor,1);
                tCursor->mergeBlockFormat(blockFormatCenter);
                edit->append("<br>");
                tCursor->movePosition(QTextCursor::End, QTextCursor::MoveAnchor,1);
                tCursor->insertBlock(blockFormatLeft, charFormatLeft);
                tCursor->insertFragment(noteFrag);
            }

            if(ui->storyCheckBox->isChecked()){
                if((ui->synopsisCheckBox->isChecked() || ui->notesCheckBox->isChecked()) && !textFrag.isEmpty()){
                    tCursor->insertBlock();
                    tCursor->insertHtml("<h4>" + tr("Story") + "</h4>");
                    tCursor->mergeBlockFormat(blockFormatCenter);
                    tCursor->insertBlock();

                }
                tCursor->insertHtml("<br>");
                //                tCursor->movePosition(QTextCursor::End, QTextCursor::MoveAnchor,1);
                tCursor->insertBlock(blockFormatLeft, charFormatLeft);
                tCursor->insertFragment(textFrag);
                //                edit->append(textFrag->toHtml());
            }
        }
        else if(element.tagName() == "separator"){
            edit->append("<br>");
            edit->append("<h3>#</h3>");
            tCursor->movePosition(QTextCursor::End, QTextCursor::MoveAnchor,1);
            tCursor->mergeBlockFormat(blockFormatCenter);
            edit->append("<br>");
            tCursor->movePosition(QTextCursor::End, QTextCursor::MoveAnchor,1);
            tCursor->mergeBlockFormat(blockFormatLeft);
        }




        progressValue += 1;
        progressBar->setValue(progressValue);

    }
    QRegExp reg("-qt-paragraph-type:.*;|margin-top:.*;|margin-bottom:.*;|margin-left:.*;|margin-right:.*;|-qt-block-indent:.*;|text-indent:.*;|font-family:.*;|font-size:.*;");
    reg.setMinimal(true);
    textDocument->setHtml(textDocument->toHtml().remove(reg));

    //find and change final page css style :

    //textDocument->setDefaultStyleSheet("p, li { white-space: pre-wrap; } p{line-height: 2em; font-family:'Liberation Serif'; font-size:14pt;margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:72px;}");

    //            <style type="text/css">
    //            p, li { white-space: pre-wrap; }
    //            </style>

    //            tCursor
    //            qDebug() << textDocument->toHtml();


    progressWidget->close();

    return textDocument;
}

//-----------------------------------------------------------------

QList<QDomElement> Exporter::searchForCheckedItems(QDomElement element)
{
    QList<QDomElement> list;

    if(element.tagName() == "plume-tree"){
        for(int i = 0 ; i < element.childNodes().size() ; ++i)
            list.append(searchForCheckedItems(element.childNodes().at(i).toElement()));

    }
    else if(element.attribute("exporterCheckState", "2").toInt() != 0){



        if(element.tagName() == "trash")
            return list;




        list.append(element);

        if(!element.hasChildNodes())
            return list;


        for(int i = 0 ; i < element.childNodes().size() ; ++i)
            list.append(searchForCheckedItems(element.childNodes().at(i).toElement()));



    }
    return list;
}




//-----------------------------------------------------------------
void Exporter::exportDoc()
{

    QTextDocument *document = buildFinalDoc();

    if(ui->fileTypeComboBox->currentIndex() == 0)
        format = "html";
    if(ui->fileTypeComboBox->currentIndex() == 1)
        format = "odt";
    if(ui->fileTypeComboBox->currentIndex() == 2){
        format = "txt";
        QMessageBox::information(this, tr("Project Exporter"), tr("You have selected the .txt format. There is no formatting !"), QMessageBox::Ok);

    }

    QTextDocumentWriter writer;
    QByteArray array;
    QString fileName(ui->pathLineEdit->text() + "/" + ui->fileNameLineEdit->text() + "." + format);
    writer.setFormat(array.append(format));
    writer.setFileName(fileName);
    writer.setCodec(QTextCodec::codecForName("UTF-8"));
    writer.write(document);

    if(format=="html"){

        QFile *textFile = new QFile;
        textFile->setFileName(fileName);
        bool opened = textFile->open(QFile::ReadWrite | QFile::Text| QFile::Truncate);
        //                qDebug() << opened ;
        QTextStream textFileStream( textFile );
        QString textString(textFileStream.readAll());
        QRegExp regExpCss("<style type=\x0022text/css\x0022>.*</style>");
        regExpCss.setMinimal(true);
        textString = textString.replace( regExpCss, "<style type=\x0022text/css\x0022>p, li { white-space: pre-wrap; } p{line-height: 2em; font-family:'Liberation Serif'; font-size:12pt;margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:72px;}</style>");
        //                qDebug() << textString;
        QByteArray array;
        array.append(textString);
        textFile->write(array);

        textFile->close();

    }




    QApplication::restoreOverrideCursor();

    QMessageBox::information(this, tr("Project exported"), tr("This project was successfully exported !"), QMessageBox::Ok);
}

//---------------------------------------------------------------------------------

QTextDocument *Exporter::prepareTextDoc(QTextDocument *textDoc)
{



    QTextDocument *textDocument = textDoc->clone(this);

    //    textDocument->setDefaultStyleSheet("p, li { white-space: pre-wrap; } p{line-height: 2em; font-family:'Liberation Serif'; font-size:19pt;margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:72px;}");

    //cut blank spaces at the begining and end :



    QTextCursor *tCursor = new QTextCursor(textDocument);
    tCursor->movePosition(QTextCursor::Start, QTextCursor::MoveAnchor,1);
    tCursor->movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor,1);

    while(tCursor->selectedText() == " "){
        tCursor->deleteChar();
        tCursor->movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor,1);
    }

    tCursor->movePosition(QTextCursor::End, QTextCursor::MoveAnchor,1);
    tCursor->movePosition(QTextCursor::PreviousCharacter, QTextCursor::KeepAnchor,1);

    while(tCursor->selectedText() == " "){
        tCursor->deleteChar();
        tCursor->movePosition(QTextCursor::PreviousCharacter, QTextCursor::KeepAnchor,1);
    }

    //set text config :

    QTextBlockFormat blockFormat;
    //    blockFormat.setBottomMargin(0);
    //    blockFormat.setTopMargin(0);
    //    blockFormat.setTextIndent(72);
    blockFormat.setLineHeight(200, QTextBlockFormat::ProportionalHeight);
    blockFormat.setAlignment(Qt::AlignJustify);
    //    QTextCharFormat charFormat;
    //    charFormat.setFontPointSize(12);
    //    charFormat.setFontFamily("Courrier");

    tCursor->select(QTextCursor::Document);
    tCursor->mergeBlockFormat(blockFormat);
    //    tCursor->mergeBlockCharFormat(charFormat);


    QRegExp reg("-qt-paragraph-type:.*;|margin-top:.*;|margin-bottom:.*;|margin-left:.*;|margin-right:.*;|-qt-block-indent:.*;|text-indent:.*;|font-family:.*;|font-size:.*;");
    reg.setMinimal(true);
    textDocument->setHtml(textDocument->toHtml().remove(reg));



    return textDocument;
}

//---------------------------------------------------------------------------------


QTextDocument *Exporter::prepareSynDoc(QTextDocument *synDoc)
{
    QTextDocument *textDocument = synDoc->clone(this);



    //cut blank spaces at the begining and end :



    QTextCursor *tCursor = new QTextCursor(textDocument);
    tCursor->movePosition(QTextCursor::Start, QTextCursor::MoveAnchor,1);
    tCursor->movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor,1);

    while(tCursor->selectedText() == " "){
        tCursor->deleteChar();
        tCursor->movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor,1);
    }

    tCursor->movePosition(QTextCursor::End, QTextCursor::MoveAnchor,1);
    tCursor->movePosition(QTextCursor::PreviousCharacter, QTextCursor::KeepAnchor,1);

    while(tCursor->selectedText() == " "){
        tCursor->deleteChar();
        tCursor->movePosition(QTextCursor::PreviousCharacter, QTextCursor::KeepAnchor,1);
    }

    //set text config :

    QTextBlockFormat blockFormat;
    blockFormat.setBottomMargin(0);
    blockFormat.setTopMargin(0);
    blockFormat.setTextIndent(72);
    blockFormat.setLineHeight(200, QTextBlockFormat::ProportionalHeight);
    blockFormat.setAlignment(Qt::AlignJustify);
    QTextCharFormat charFormat;
    charFormat.setFontPointSize(12);
    charFormat.setFontFamily("Courrier");

    tCursor->select(QTextCursor::Document);
    tCursor->mergeBlockCharFormat(charFormat);
    tCursor->mergeBlockFormat(blockFormat);

    return textDocument;
}

//---------------------------------------------------------------------------------

QTextDocument *Exporter::prepareNoteDoc(QTextDocument *noteDoc)
{
    QTextDocument *textDocument = noteDoc->clone(this);




    //cut blank spaces at the begining and end :



    QTextCursor *tCursor = new QTextCursor(textDocument);
    tCursor->movePosition(QTextCursor::Start, QTextCursor::MoveAnchor,1);
    tCursor->movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor,1);

    while(tCursor->selectedText() == " "){
        tCursor->deleteChar();
        tCursor->movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor,1);
    }

    tCursor->movePosition(QTextCursor::End, QTextCursor::MoveAnchor,1);
    tCursor->movePosition(QTextCursor::PreviousCharacter, QTextCursor::KeepAnchor,1);

    while(tCursor->selectedText() == " "){
        tCursor->deleteChar();
        tCursor->movePosition(QTextCursor::PreviousCharacter, QTextCursor::KeepAnchor,1);
    }

    //set text config :

    QTextBlockFormat blockFormat;
    blockFormat.setBottomMargin(0);
    blockFormat.setTopMargin(0);
    blockFormat.setTextIndent(72);
    blockFormat.setLineHeight(200, QTextBlockFormat::ProportionalHeight);
    blockFormat.setAlignment(Qt::AlignJustify);
    QTextCharFormat charFormat;
    charFormat.setFontPointSize(12);
    charFormat.setFontFamily("Courrier");

    tCursor->select(QTextCursor::Document);
    tCursor->mergeBlockCharFormat(charFormat);
    tCursor->mergeBlockFormat(blockFormat);


    return textDocument;
}

//-------------------------------------------------------------------------------------------------------

void Exporter::seePreview()
{
    QTextDocument *document = buildFinalDoc();

    if(dialogMode == "export")
    {

        previewDialog = new QDialog(this);
        previewDialog->setAttribute(Qt::WA_DeleteOnClose);
        previewDialog->setWindowTitle(document->metaInformation(QTextDocument::DocumentTitle));
        previewDialog->setMinimumSize(500,500);

        QVBoxLayout *layout = new QVBoxLayout;


        QTextBrowser *browser = new QTextBrowser;
        if(ui->fileTypeComboBox->currentIndex() == 2){ // if format is txt (plaintext)
            browser->setPlainText(document->toPlainText());
        }
        else{
            browser->setDocument(document);

        }



        QDialogButtonBox *buttons = new QDialogButtonBox((QDialogButtonBox::Ok
                                                          | QDialogButtonBox::Cancel), Qt::Horizontal);
        connect(buttons, SIGNAL(accepted()), this, SLOT(accept()));
        connect(buttons, SIGNAL(rejected()), this, SLOT(closePreview()));

        layout->addWidget(browser);
        layout->addWidget(buttons);
        previewDialog->setLayout(layout);
        previewDialog->show();


    }


    if(dialogMode == "print")
    {

        QPrinter printer;

        QPrintPreviewDialog printPreviewDialog(&printer, this, Qt::Dialog);
        connect(&printPreviewDialog, SIGNAL(paintRequested(QPrinter*)), this, SLOT(previewPrint(QPrinter*))) ;
        printPreviewDialog.exec();
    }



}







void Exporter::previewPrint(QPrinter *printer)
{
    QTextDocument *document = buildFinalDoc();
    document->print(printer);

}



//-------------------------------------------------------------------------------------------------------

void Exporter::print()
{

    QTextDocument *document = buildFinalDoc();

    QPrinter printer;

    QPrintDialog *dialog = new QPrintDialog(&printer, this);
    dialog->setWindowTitle(tr("Print Document"));
    if (dialog->exec() == QDialog::Accepted)
        document->print(&printer);




}
void Exporter::exportInPDF()
{

    QTextDocument *document = buildFinalDoc();


    QString fileName(ui->pathLineEdit->text() + "/" + ui->fileNameLineEdit->text() + "." + format);
    if (!fileName.isEmpty()) {
        if (QFileInfo(fileName).suffix().isEmpty())
            fileName.append(".pdf");
        QPrinter printer(QPrinter::HighResolution);
        printer.setOutputFormat(QPrinter::PdfFormat);
        printer.setOutputFileName(fileName);
        printer.setDocName(ui->fileNameLineEdit->text());
        printer.setPageMargins(10,10,10,10,QPrinter::Millimeter);
        document->print(&printer);

    }


}



void Exporter::exportInCSV()
{

    //search for checked items :

    QDomDocument domDoc = hub->mainTreeDomDoc();
    QDomElement root = domDoc.documentElement();

    QList<QDomElement> itemList = searchForCheckedItems(root);

    if(itemList.size() == 0)
        return;

    // set up the progress bar :
    QWidget *progressWidget = new QWidget(this, Qt::Dialog | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    QHBoxLayout *progressLayout = new QHBoxLayout(progressWidget);
    QProgressBar *progressBar = new QProgressBar(progressWidget);
    int progressValue = 0;

    progressLayout->addWidget(progressBar);
    progressWidget->setLayout(progressLayout);

    progressBar->setMaximum(itemList.size());
    progressBar->setValue(progressValue);
    progressWidget->show();


    QList<QStringList> bigList;


    QStringList headers;
    if(ui->titleCheckBox_2->isChecked())
        headers << "\x0022" << tr("Title") << "\x0022"<<";";
    if(ui->typeOfSheetCheckBox_2->isChecked())
        headers << "\x0022" <<tr("Type")<< "\x0022"<<";";
    if(ui->synopsisCheckBox_2->isChecked())
        headers << "\x0022" <<tr("Synopsis")<< "\x0022"<<";";
    if(ui->notesCheckBox_2->isChecked())
        headers << "\x0022" <<tr("Notes")<< "\x0022"<<";";
    if(ui->wordCountCheckBox_2->isChecked())
        headers << "\x0022" <<tr("Word count")<< "\x0022"<<";";
    if(ui->pointOfViewCheckBox_2->isChecked())
        headers << "\x0022" <<tr("Point of view")<< "\x0022" <<";";

    headers << "\n";

    bigList.append(headers);

    while(!itemList.isEmpty()){
        QStringList line;
        QString title, type, syn, note, wordCount, pov;

        QDomElement element = itemList.takeFirst();
        title = element.attribute("name", "error");
        if(element.tagName() == "book")
            type = tr("book");
        else if(element.tagName() == "act")
            type = tr("act");
        else if(element.tagName() == "chapter")
            type = tr("chapter");
        else if(element.tagName() == "scene")
            type = tr("scene");
        else if(element.tagName() == "separator")
            type = tr("scene break");


        if(element.tagName() == "separator"){
            progressValue += 1;
            progressBar->setValue(progressValue);
            bigList.append(line);
            continue;
        }


        MainTextDocument *textDoc = hub->findChild<MainTextDocument *>("textDoc_" + element.attribute("number"));
        MainTextDocument *synDoc = hub->findChild<MainTextDocument *>("synDoc_" + element.attribute("number"));
        MainTextDocument *noteDoc = hub->findChild<MainTextDocument *>("noteDoc_" + element.attribute("number"));
        syn = synDoc->toPlainText();
        note = noteDoc->toPlainText();

        wordCount = QString::number(textDoc->wordCount());


        QString numbersString = element.attribute("pov", "0");
        QStringList list = numbersString.split("-", QString::SkipEmptyParts);
        QList<int> objectsList;
        foreach(const QString &string, list)
            objectsList.append(string.toInt());

        objectsList.removeOne(0);

        foreach(const int &number, objectsList)
            pov += hub->attendTree_domElementForNumberHash().value(number).attribute("name", "error") + ", ";
        pov.chop(2);



        title.replace("\x0022", "\x0022\x0022");
        syn.replace("\x0022", "\x0022\x0022");
        note.replace("\x0022", "\x0022\x0022");
        pov.replace("\x0022", "\x0022\x0022");



        if(ui->titleCheckBox_2->isChecked())
            line << "\x0022" <<title<< "\x0022" << ";";
        if(ui->typeOfSheetCheckBox_2->isChecked())
            line << "\x0022" <<type<< "\x0022" << ";";
        if(ui->synopsisCheckBox_2->isChecked())
            line << "\x0022" <<syn<< "\x0022" << ";";
        if(ui->notesCheckBox_2->isChecked())
            line << "\x0022" <<note<< "\x0022" << ";";
        if(ui->wordCountCheckBox_2->isChecked())
            line <<wordCount << ";";
        if(ui->pointOfViewCheckBox_2->isChecked())
            line << "\x0022" <<pov<< "\x0022" << ";";


        line << "\n";

        bigList.append(line);



        progressValue += 1;
        progressBar->setValue(progressValue);


    }


    QString finalString;


    foreach(const QStringList &line, bigList)
        foreach(const QString &cell, line)
            finalString += cell;

    progressWidget->close();










    // write file :

    QFile *textFile = new QFile;
    QString fileName(ui->pathLineEdit->text() + "/" + ui->fileNameLineEdit->text() + "." + format);
    textFile->setFileName(fileName);
    bool opened = textFile->open(QFile::ReadWrite | QFile::Text| QFile::Truncate);

    QByteArray array;
    //    array.append(finalString);
    textFile->write(finalString.toUtf8());

    textFile->close();

}




















void Exporter::on_fileTypeComboBox_currentIndexChanged(int index)
{
    if(index == 3)
        ui->stackedWidget->setCurrentIndex(1);
    else
        ui->stackedWidget->setCurrentIndex(0);
}


void Exporter::applyConfig()
{
    ui->fileNameLineEdit->setText(hub->projectName());


    QSettings settings;
    settings.beginGroup("Exporter");
    ui->pathLineEdit->setText(settings.value("path", QDir::toNativeSeparators(QDir::homePath())).toString());
    if(dialogMode == "export"){
    ui->fileTypeComboBox->setCurrentIndex(0); // shake
    ui->fileTypeComboBox->setCurrentIndex(1);// shake
    ui->fileTypeComboBox->setCurrentIndex(settings.value("fileType", 0).toInt());
    }
    ui->storyCheckBox->setChecked(settings.value("story", true).toBool());
    ui->synopsisCheckBox->setChecked(settings.value("syn", true).toBool());
    ui->notesCheckBox->setChecked(settings.value("notes", true).toBool());
    ui->setSceneTitlesComboBox->setCurrentIndex(settings.value("sceneTitles", 1).toInt());

    ui->titleCheckBox_2->setChecked(settings.value("title_2", true).toBool());
    ui->typeOfSheetCheckBox_2->setChecked(settings.value("typeOfSheet_2", true).toBool());
    ui->synopsisCheckBox_2->setChecked(settings.value("syn_2", true).toBool());
    ui->notesCheckBox_2->setChecked(settings.value("notes_2", true).toBool());
    ui->wordCountCheckBox_2->setChecked(settings.value("wordCount_2", true).toBool());
    ui->pointOfViewCheckBox_2->setChecked(settings.value("pointOfView_2", true).toBool());

    settings.endGroup();

}

void Exporter::saveConfig()
{
    QSettings settings;
    settings.beginGroup("Exporter");
    settings.setValue("path", ui->pathLineEdit->text());
    settings.setValue("fileType", ui->fileTypeComboBox->currentIndex());
    settings.setValue("story",ui->storyCheckBox->isChecked());
    settings.setValue("syn",ui->synopsisCheckBox->isChecked());
    settings.setValue("notes",ui->notesCheckBox->isChecked());
    settings.setValue("sceneTitles",ui->setSceneTitlesComboBox->currentIndex());

    settings.setValue("title_2",ui->titleCheckBox_2->isChecked());
    settings.setValue("typeOfSheet_2",ui->typeOfSheetCheckBox_2->isChecked());
    settings.setValue("syn_2",ui->synopsisCheckBox_2->isChecked());
    settings.setValue("notes_2",ui->notesCheckBox_2->isChecked());
    settings.setValue("wordCount_2",ui->wordCountCheckBox_2->isChecked());
    settings.setValue("pointOfView_2",ui->pointOfViewCheckBox_2->isChecked());

    settings.endGroup();
}

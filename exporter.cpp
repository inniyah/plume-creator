#include "exporter.h"
//
Exporter::Exporter(QFile *device, QWidget *parent) :
    QDialog(parent)
{

    //dialog :

    //tree with check

    //name
    //path
    // odt or html
    // story ?
    // notes ?
    // synopsis ?

    // add export characters/items/places

    //    setTitle(tr("New Project"));
    //    setSubTitle(tr("Specify the path of your new project. "
    //                   "Please note you must have writing rights in it. "
    //                   "Plume Creator will create a basic skeleton."));


    //setPixmap(QWizard::LogoPixmap, QPixmap(":/images/logo1.png"));



    setWindowTitle(tr("Export Dialog"));
    setMinimumSize(600,600);


    QLabel *label = new QLabel(tr("Please complete the fields below: "));
    label->setWordWrap(true);

    QLabel *projectNameLabel = new QLabel(tr("File name :"));
    projectNameLabelLineEdit = new QLineEdit;
    projectNameLabelLineEdit->setValidator(new QRegExpValidator(QRegExp("[^\x002F\\\\:\x002A\?\x0022<>|]+"), projectNameLabelLineEdit));

    //    directoryLabel = new QLabel(tr("Path : "));
    directoryLabelLineEdit = new QLineEdit;
    directoryLabelLineEdit->setText(QDir::convertSeparators(QDir::homePath()));

    QPushButton *directoryButton = new QPushButton(tr("Select path"));
    directoryButton->setMaximumWidth(100);

    connect(directoryButton, SIGNAL(clicked()), this, SLOT(setExistingDirectory()));

    QGroupBox *checkGroupBox = new QGroupBox(tr("Options :"), this);
    QGridLayout *checkLayout = new QGridLayout;
    textCheckBox = new QCheckBox(tr("Insert story"), this);
    synCheckBox = new QCheckBox(tr("Insert synopsies"), this);
    noteCheckBox = new QCheckBox(tr("Insert notes"), this);
    sceneTitleCheckBox = new QCheckBox(tr("Insert scene titles"), this);

    textCheckBox->setChecked(true);
    synCheckBox->setChecked(true);
    noteCheckBox->setChecked(true);
    sceneTitleCheckBox->setChecked(true);

    checkLayout->addWidget(textCheckBox, 1,0);
    checkLayout->addWidget(synCheckBox, 2,0);
    checkLayout->addWidget(noteCheckBox, 3,0);
    checkLayout->addWidget(sceneTitleCheckBox, 4,0);
    checkGroupBox->setLayout(checkLayout);


    fileTypeCombo = new QComboBox;
    QStringList list;
    list << tr(".html") << tr(".odt") << tr(".txt");
    fileTypeCombo->insertItems(0, list);

    QWidget *stretcher = new QWidget;
    stretcher->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);

    QPushButton *previewButton = new QPushButton(tr("Preview"), this);

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(label, 0, 0, 1, 2);
    //    layout->addWidget(directoryLabel, 1, 0);
    layout->addWidget(projectNameLabel, 1, 0);
    layout->addWidget(projectNameLabelLineEdit, 1, 1);
    layout->addWidget(directoryButton, 2, 0);
    layout->addWidget(directoryLabelLineEdit, 2, 1);
    layout->addWidget(fileTypeCombo, 3,1);
    layout->addWidget(checkGroupBox,4,0,1,2);
    layout->addWidget(stretcher,5,0,1,2);
layout->addWidget(previewButton,6,0,1,2, Qt::AlignCenter);
    tree = new QTreeWidget;
    tree->setFixedWidth(width()/3);

    QHBoxLayout *treeLayout = new QHBoxLayout;
    treeLayout->addWidget(tree);
    treeLayout->addLayout(layout);


    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
                                     | QDialogButtonBox::Cancel);

    connect(previewButton, SIGNAL(clicked()), this, SLOT(seePreview()));
    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));


    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(treeLayout);
    mainLayout->addWidget(buttonBox);
    setLayout(mainLayout);



    createTree(device);
    targetDevice = device;
}

//-----------------------------------------------------------------------------------

void Exporter::accept()
{
    if(directoryLabelLineEdit->text() == "" || projectNameLabelLineEdit->text() == "" ){
        QMessageBox::information(this, tr("Project Exporter"), tr("The destination fields must be completed !"), QMessageBox::Ok);
return;
            }

    QApplication::setOverrideCursor(Qt::WaitCursor);
    exportDoc();
    QApplication::restoreOverrideCursor();



    QDialog::accept();
}

//-----------------------------------------------------------------------------------

void Exporter::setExistingDirectory()
{
    QFileDialog::Options options = QFileDialog::DontResolveSymlinks | QFileDialog::ShowDirsOnly;
    QString directory = QFileDialog::getExistingDirectory(this, tr("Select a directory"),
                                                          directoryLabelLineEdit->text(),
                                                          options);
    if (!directory.isEmpty())
        directoryLabelLineEdit->setText(directory);
}









//-----------------------------------------------------------------------------------
//--------Tree-----------------------------------------------------------------
//-----------------------------------------------------------------------------------



void Exporter::createTree(QFile *device)
{
    tree->setHeaderHidden(true);
    tree->setExpandsOnDoubleClick(false);
    tree->setAutoExpandDelay(1000);
    tree->setAnimated(true);

    folderIcon.addPixmap(style()->standardPixmap(QStyle::SP_DirClosedIcon),
                         QIcon::Normal, QIcon::Off);
    folderIcon.addPixmap(style()->standardPixmap(QStyle::SP_DirOpenIcon),
                         QIcon::Normal, QIcon::On);
    sceneIcon.addPixmap(style()->standardPixmap(QStyle::SP_FileIcon));



    tree->setContextMenuPolicy(Qt::PreventContextMenu);

    read(device);

    connect(tree, SIGNAL(itemClicked(QTreeWidgetItem*,int)), this, SLOT(itemClickedSlot(QTreeWidgetItem*,int)));

}

//-----------------------------------------------------------------------------------

bool Exporter::read(QFile *device)
{

    if(device->isReadable())
        QMessageBox::information(this, tr("Plume Creator Tree"),
                                 tr("The file is not readable."));
    QString errorStr;
    int errorLine;
    int errorColumn;

    if (!domDocument.setContent(device, true, &errorStr, &errorLine,
                                &errorColumn)) {
        QMessageBox::information(this, tr("Plume Creator Tree"),
                                 tr("Parse error at line %1, column %2:\n%3\n")
                                 .arg(errorLine)
                                 .arg(errorColumn)
                                 .arg(errorStr));

        //        qDebug() << "File path:" << device->fileName();
        //        qDebug() << "File readable:" << device->isReadable();
        //        qDebug() << "File open:" << device->isOpen();


        return false;
    }


    QFileInfo *dirInfo = new QFileInfo(*device);


    root = domDocument.documentElement();
    if (root.tagName() != "plume") {
        QMessageBox::information(this, tr("Plume Creator Tree"),
                                 tr("The file is not a a Plume Creator project file."));
        return false;
    } else if (root.hasAttribute("version")
               && root.attribute("version") != "0.2") {
        QMessageBox::information(this, tr("Plume Creator Tree"),
                                 tr("The file is not an Plume Creator project file version 1.0 "
                                    "file."));
        return false;
    }



    buildTree();



    device->close();


    setEnabled(true);




    return true;
}



//---------------------------------------------------------------------------------------

void Exporter::closeTree()
{

    tree->clear();


}

//---------------------------------------------------------------------------------------



void Exporter::parseFolderElement(const QDomElement &element,
                                  QTreeWidgetItem *parentItem)
{
    QTreeWidgetItem *item = createItem(element, parentItem);

    QString title = element.attribute("name");
    if (title.isEmpty())
        title = QObject::tr("XML problem : parseFolderElement(const QDomElement &element, QTreeWidgetItem *parentItem)");

    tree->setItemExpanded(item, true);
    item->setFlags( Qt::ItemIsSelectable /*| Qt::ItemIsEditable*/ | Qt::ItemIsUserCheckable | Qt::ItemIsTristate | Qt::ItemIsEnabled | Qt::ItemIsDragEnabled);
    item->setIcon(0, folderIcon);
    item->setText(0, title);
    item->setCheckState(0, Qt::Checked);


    QDomElement child = element.firstChildElement();
    while (!child.isNull()) {
        if (child.tagName() == "book") {
            item->flags();
            parseFolderElement(child, item);

        } else if (child.tagName() == "chapter") {
            item->setFlags( item->flags());
            parseFolderElement(child, item);


        } else if (child.tagName() == "scene") {
            QTreeWidgetItem *childItem = createItem(child, item);

            QString title = child.attribute("name");
            if (title.isEmpty())
                title = QObject::tr("XML problem");


            childItem->setFlags(item->flags());
            childItem->setIcon(0, sceneIcon);
            childItem->setText(0, title);
            childItem->setCheckState(0, Qt::Checked);

        }

        else if (child.tagName() == "separator") {
            QTreeWidgetItem *childItem = createItem(child, item);
            childItem->setFlags(item->flags());
            childItem->setText(0, "             " + QString(10, 0xB7));
            childItem->setCheckState(0, Qt::Checked);
        }
        child = child.nextSiblingElement();
    }


}


//-----------------------------------------------------------------------------------

QTreeWidgetItem *Exporter::createItem(const QDomElement &element,
                                      QTreeWidgetItem *parentItem)
{
    QTreeWidgetItem *item;
    if (parentItem) {
        item = new QTreeWidgetItem(parentItem);
    } else {
        item = new QTreeWidgetItem(tree);
    }
    domElementForItem.insert(item, element);

    int number = element.attribute("number").toInt();
    domElementForNumber[number] = element;
    return item;
}


//------------------------------------------------------------------------------------

void Exporter::buildTree()
{




    tree->clear();


    QDomElement child = root.firstChildElement("book");
    while (!child.isNull()) {
        parseFolderElement(child);
        child = child.nextSiblingElement("book");
    }


}

//------------------------------------------------------------------------------------

void Exporter::itemClickedSlot(QTreeWidgetItem* item, int column)
{
    Qt::CheckState state = item->checkState(0);

    if(state == Qt::Unchecked){
        //       qDebug() << "Unchecked ----------------";



        int i = item->childCount()-1;
        int j;
        while(i != -1 && item->childCount() != 0){
            item->child(i)->setCheckState(0, Qt::Unchecked);

            j = item->child(i)->childCount()-1;
            while(j != -1 && item->childCount() != 0){
                item->child(i)->child(j)->setCheckState(0, Qt::Unchecked);
                j -= 1;
            }

            i -= 1;
        }



    }

    if(state == Qt::Checked){
        //       qDebug() << "Checked ----------------";


        int i = item->childCount()-1;
        int j;
        while(i != -1 && item->childCount() != 0){
            item->child(i)->setCheckState(0, Qt::Checked);

            j = item->child(i)->childCount()-1;
            while(j != -1 && item->childCount() != 0){
                item->child(i)->child(j)->setCheckState(0, Qt::Checked);
                j -= 1;
            }

            i -= 1;
        }


    }
}










QTextDocument * Exporter::buildFinalDoc()
{
    //search for checked QTreeWidgetItems :

    QTreeWidgetItemIterator *iterator = new QTreeWidgetItemIterator(tree, QTreeWidgetItemIterator::Checked);
    QList<QTreeWidgetItem *> *itemList = new QList<QTreeWidgetItem *>;


    while(iterator->operator *() != 0){
        itemList->append(iterator->operator *());
        iterator->operator ++();

    }

    // set up the progress bar :
    QWidget *progressWidget = new QWidget(this, Qt::Dialog | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    QHBoxLayout *progressLayout = new QHBoxLayout(progressWidget);
    QProgressBar *progressBar = new QProgressBar(progressWidget);
    int progressValue = 0;

    progressLayout->addWidget(progressBar);
    progressWidget->setLayout(progressLayout);

    progressBar->setMaximum(itemList->size());
    progressBar->setValue(progressValue);
    progressWidget->show();



    //    QString debug;
    //    qDebug() << "itemList" << debug.setNum(itemList->size());

    QFileInfo *dirInfo = new QFileInfo(*targetDevice);
    QString devicePath = dirInfo->path();

    QTextDocument *textDocument = new QTextDocument;
    QTextEdit *edit = new QTextEdit;



    for(int i = 0; i < itemList->size(); ++i){
        QDomElement element = domElementForItem.value(itemList->at(i));
        QTextCursor *tCursor = new QTextCursor(textDocument);
        QTextBlockFormat blockFormatLeft;
        QTextBlockFormat blockFormatCenter;

        blockFormatLeft.setAlignment(Qt::AlignLeft);
        blockFormatCenter.setAlignment(Qt::AlignCenter);

        if(element.tagName() != "separator"){



            QString textPath = element.attribute("textPath");
            QString synPath = element.attribute("synPath");
            QString notePath = element.attribute("notePath");

            QFile *textFile = new QFile(devicePath + textPath);
            QFile *synFile = new QFile(devicePath + synPath);
            QFile *noteFile = new QFile(devicePath + notePath);

            QTextDocumentFragment *textFrag = new QTextDocumentFragment(prepareTextDoc(textFile));
            QTextDocumentFragment *synFrag = new QTextDocumentFragment(prepareSynDoc(synFile));
            QTextDocumentFragment *noteFrag = new QTextDocumentFragment(prepareNoteDoc(noteFile));



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
                tCursor->movePosition(QTextCursor::End, QTextCursor::MoveAnchor,1);
                tCursor->mergeBlockFormat(blockFormatLeft);
            }
            if(element.tagName() == "chapter"){
                edit->append("<br>");
                edit->append("<br>");
                edit->append("<h2>" + element.attribute("name", "") + "</h2>");
                tCursor->movePosition(QTextCursor::End, QTextCursor::MoveAnchor,1);
                tCursor->mergeBlockFormat(blockFormatCenter);
                edit->append("<br>");
                tCursor->movePosition(QTextCursor::End, QTextCursor::MoveAnchor,1);
                tCursor->mergeBlockFormat(blockFormatLeft);
            }

            if(element.tagName() == "scene" && sceneTitleCheckBox->isChecked()){
                edit->append("<br>");
                edit->append("<h3>" + element.attribute("name", "") + "</h3>");
                tCursor->movePosition(QTextCursor::End, QTextCursor::MoveAnchor,1);
                tCursor->mergeBlockFormat(blockFormatCenter);
                edit->append("<br>");
                tCursor->movePosition(QTextCursor::End, QTextCursor::MoveAnchor,1);
                tCursor->mergeBlockFormat(blockFormatLeft);
            }

            if(synCheckBox->isChecked() && !synFrag->isEmpty()){
                edit->append("<br>");
                edit->append("<h4>" + tr("Synopsis") + "</h4>");
                tCursor->movePosition(QTextCursor::End, QTextCursor::MoveAnchor,1);
                tCursor->mergeBlockFormat(blockFormatCenter);
                edit->append("<br>");
                tCursor->movePosition(QTextCursor::End, QTextCursor::MoveAnchor,1);
                tCursor->mergeBlockFormat(blockFormatLeft);
                edit->append(synFrag->toHtml());
            }

            if(noteCheckBox->isChecked() && !noteFrag->isEmpty()){
                edit->append("<br>");
                edit->append("<h4>" + tr("Note") + "</h4>");
                tCursor->movePosition(QTextCursor::End, QTextCursor::MoveAnchor,1);
                tCursor->mergeBlockFormat(blockFormatCenter);
                edit->append("<br>");
                tCursor->movePosition(QTextCursor::End, QTextCursor::MoveAnchor,1);
                tCursor->mergeBlockFormat(blockFormatLeft);
                edit->append(noteFrag->toHtml());
            }

            if(textCheckBox->isChecked()){
                if((synCheckBox->isChecked() || noteCheckBox->isChecked()) && !textFrag->isEmpty()){
                    edit->append("<br>");
                    edit->append("<h4>" + tr("Story") + "</h4>");
                    tCursor->movePosition(QTextCursor::End, QTextCursor::MoveAnchor,1);
                    tCursor->mergeBlockFormat(blockFormatCenter);
                    edit->append("<br>");
                    tCursor->movePosition(QTextCursor::End, QTextCursor::MoveAnchor,1);
                    tCursor->mergeBlockFormat(blockFormatLeft);
                }
                edit->append(textFrag->toHtml());
            }
        }
        else if(element.tagName() == "separator"){
            edit->append("<br>");
            edit->append("<h3>~~~~~~</h3>");
            tCursor->movePosition(QTextCursor::End, QTextCursor::MoveAnchor,1);
            tCursor->mergeBlockFormat(blockFormatCenter);
            edit->append("<br>");
            tCursor->movePosition(QTextCursor::End, QTextCursor::MoveAnchor,1);
            tCursor->mergeBlockFormat(blockFormatLeft);
        }

        progressValue += 1;
        progressBar->setValue(progressValue);

    }
    progressWidget->close();

    return textDocument;
}

void Exporter::exportDoc()
{

    QTextDocument *document = buildFinalDoc();

    QString format;
    if(fileTypeCombo->currentIndex() == 0)
        format = "html";
    if(fileTypeCombo->currentIndex() == 1)
        format = "odt";
    if(fileTypeCombo->currentIndex() == 2){
        format = "txt";
        QMessageBox::information(this, "Project Exporter", "You have selected the .txt format. There is no formatting !", QMessageBox::Ok);
    }

    QTextDocumentWriter writer;
    QByteArray array;
    writer.setFormat(array.append(format));
    writer.setFileName(directoryLabelLineEdit->text() + "/" + projectNameLabelLineEdit->text() + "." + format);
    writer.setCodec(QTextCodec::codecForName("UTF-8"));
    writer.write(document);



    QMessageBox::information(this, "Project exported", "This project was successfully exported !", QMessageBox::Ok);
}

//---------------------------------------------------------------------------------

QTextDocument *Exporter::prepareTextDoc(QFile *textFile)
{



    QTextDocument *textDocument = new QTextDocument;

    textFile->open(QFile::ReadOnly | QFile::Text);
    QTextStream textFileStream( textFile );
    textDocument->setHtml(textFileStream.readAll());

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

    //    QTextBlockFormat blockFormat;
    //    blockFormat.setBottomMargin(noteBottMargin);
    //    blockFormat.setTextIndent(noteTextIndent);
    //    QTextCharFormat charFormat;
    //            charFormat.setFontPointSize(noteTextHeight);
    //    charFormat.setFontFamily(noteFontFamily);

    //    tCursor->movePosition(QTextCursor::Start, QTextCursor::MoveAnchor,1);
    //    tCursor->movePosition(QTextCursor::End, QTextCursor::KeepAnchor,1);
    //    tCursor->mergeCharFormat(charFormat);
    //    tCursor->mergeBlockFormat(blockFormat);





    return textDocument;
}

//---------------------------------------------------------------------------------


QTextDocument *Exporter::prepareSynDoc(QFile *synFile)
{
    QTextDocument *textDocument = new QTextDocument;

    synFile->open(QFile::ReadOnly | QFile::Text);
    QTextStream textFileStream( synFile );
    textDocument->setHtml(textFileStream.readAll());



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

    //    QTextBlockFormat blockFormat;
    //    blockFormat.setBottomMargin(noteBottMargin);
    //    blockFormat.setTextIndent(noteTextIndent);
    //    QTextCharFormat charFormat;
    //            charFormat.setFontPointSize(noteTextHeight);
    //    charFormat.setFontFamily(noteFontFamily);

    //    tCursor->movePosition(QTextCursor::Start, QTextCursor::MoveAnchor,1);
    //    tCursor->movePosition(QTextCursor::End, QTextCursor::KeepAnchor,1);
    //    tCursor->mergeCharFormat(charFormat);
    //    tCursor->mergeBlockFormat(blockFormat);

    return textDocument;
}

//---------------------------------------------------------------------------------

QTextDocument *Exporter::prepareNoteDoc(QFile *noteFile)
{
    QTextDocument *textDocument = new QTextDocument;

    noteFile->open(QFile::ReadOnly | QFile::Text);
    QTextStream textFileStream( noteFile );
    textDocument->setHtml(textFileStream.readAll());




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

    //    QTextBlockFormat blockFormat;
    //    blockFormat.setBottomMargin(noteBottMargin);
    //    blockFormat.setTextIndent(noteTextIndent);
    //    QTextCharFormat charFormat;
    //            charFormat.setFontPointSize(noteTextHeight);
    //    charFormat.setFontFamily(noteFontFamily);

    //    tCursor->movePosition(QTextCursor::Start, QTextCursor::MoveAnchor,1);
    //    tCursor->movePosition(QTextCursor::End, QTextCursor::KeepAnchor,1);
    //    tCursor->mergeCharFormat(charFormat);
    //    tCursor->mergeBlockFormat(blockFormat);

    return textDocument;
}


void Exporter::seePreview()
{
    QTextDocument *document = buildFinalDoc();



    previewDialog = new QDialog(this);
    previewDialog->setAttribute(Qt::WA_DeleteOnClose);
    previewDialog->setWindowTitle(document->metaInformation(QTextDocument::DocumentTitle));
previewDialog->setMinimumSize(600,800);

QVBoxLayout *layout = new QVBoxLayout;


    QTextBrowser *browser = new QTextBrowser;
        if(fileTypeCombo->currentIndex() == 2){ // if format is txt (plaintext)
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

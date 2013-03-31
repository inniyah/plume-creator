#include "exporter.h"

//
Exporter::Exporter(QString mode, QFile *device, QWidget *parent) :
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


    dialogMode = mode;


    if(dialogMode == "export")
        setWindowTitle(tr("Export Dialog"));
    if(dialogMode == "print")
        setWindowTitle(tr("Print Dialog"));


    setMinimumSize(600,300);


    QLabel *label = new QLabel(tr("Please complete the fields below: "));
    label->setWordWrap(true);

    QLabel *projectNameLabel = new QLabel(tr("File name :"));
    projectNameLabelLineEdit = new QLineEdit;
    projectNameLabelLineEdit->setValidator(new QRegExpValidator(QRegExp("[^\x002F\\\\:\x002A\?\x0022<>|]+"), projectNameLabelLineEdit));

    //    directoryLabel = new QLabel(tr("Path : "));
    directoryLabelLineEdit = new QLineEdit;
    directoryLabelLineEdit->setText(QDir::toNativeSeparators(QDir::homePath()));

    QPushButton *directoryButton = new QPushButton(tr("Select path"));
    directoryButton->setMaximumWidth(100);

    connect(directoryButton, SIGNAL(clicked()), this, SLOT(setExistingDirectory()));

    fileTypeCombo = new QComboBox;
    QStringList list;
    list << tr(".html") << tr(".odt") << tr(".txt");
    fileTypeCombo->insertItems(0, list);


    if(dialogMode == "print")
    {
        label->hide();
        projectNameLabel->hide();
        projectNameLabelLineEdit->hide();
        directoryLabelLineEdit->hide();
        fileTypeCombo->hide();
        directoryButton->hide();
    }




    QGroupBox *checkGroupBox = new QGroupBox(tr("Options :"), this);
    QGridLayout *checkLayout = new QGridLayout;
    textCheckBox = new QCheckBox(tr("Insert story"), this);
    synCheckBox = new QCheckBox(tr("Insert synopses"), this);
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


    if(dialogMode == "print")
    {
        buttonBox = new QDialogButtonBox(this);
        buttonBox->addButton(QDialogButtonBox::Cancel);
        buttonBox->addButton(tr("Print"), QDialogButtonBox::AcceptRole);
    }

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
    if(dialogMode == "export")
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

    if(dialogMode == "print")
    {


        QApplication::setOverrideCursor(Qt::WaitCursor);
        print();
        QApplication::restoreOverrideCursor();



        QDialog::accept();
    }






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

    textDocument->setDefaultStyleSheet("p, li { white-space: pre-wrap; } p{line-height: 2em; font-family:'Liberation Serif'; font-size:12pt;margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:72px;}");



    for(int i = 0; i < itemList->size(); ++i){
        QDomElement element = domElementForItem.value(itemList->at(i));
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



            QString textPath = element.attribute("textPath");
            QString synPath = element.attribute("synPath");
            QString notePath = element.attribute("notePath");

            QFile *textFile = new QFile(devicePath + textPath);
            QFile *synFile = new QFile(devicePath + synPath);
            QFile *noteFile = new QFile(devicePath + notePath);

            QTextDocumentFragment textFrag(prepareTextDoc(textFile));
            QTextDocumentFragment synFrag(prepareSynDoc(synFile));
            QTextDocumentFragment noteFrag(prepareNoteDoc(noteFile));

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
            if(element.tagName() == "chapter"){
                edit->append("<br>");
                edit->append("<br>");
                edit->append("<h2>" + element.attribute("name", "") + "</h2>");
                tCursor->movePosition(QTextCursor::End, QTextCursor::MoveAnchor,1);
                tCursor->mergeBlockFormat(blockFormatCenter);
                edit->append("<br>");

            }

            if(element.tagName() == "scene" && sceneTitleCheckBox->isChecked()){
                edit->append("<br>");
                edit->append("<h3>" + element.attribute("name", "") + "</h3>");
                tCursor->movePosition(QTextCursor::End, QTextCursor::MoveAnchor,1);
                tCursor->mergeBlockFormat(blockFormatCenter);
                edit->append("<br>");

            }

            if(synCheckBox->isChecked() && !synFrag.isEmpty()){
                edit->append("<br>");
                edit->append("<h4>" + tr("Synopsis") + "</h4>");
                tCursor->movePosition(QTextCursor::End, QTextCursor::MoveAnchor,1);
                tCursor->mergeBlockFormat(blockFormatCenter);
                edit->append("<br>");
                tCursor->movePosition(QTextCursor::End, QTextCursor::MoveAnchor,1);
                tCursor->insertBlock(blockFormatLeft, charFormatLeft);
                tCursor->insertFragment(synFrag);
            }

            if(noteCheckBox->isChecked() && !noteFrag.isEmpty()){
                edit->append("<br>");
                edit->append("<h4>" + tr("Note") + "</h4>");
                tCursor->movePosition(QTextCursor::End, QTextCursor::MoveAnchor,1);
                tCursor->mergeBlockFormat(blockFormatCenter);
                edit->append("<br>");
                tCursor->movePosition(QTextCursor::End, QTextCursor::MoveAnchor,1);
                tCursor->insertBlock(blockFormatLeft, charFormatLeft);
                tCursor->insertFragment(noteFrag);
            }

            if(textCheckBox->isChecked()){
                if((synCheckBox->isChecked() || noteCheckBox->isChecked()) && !textFrag.isEmpty()){
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

void Exporter::exportDoc()
{

    QTextDocument *document = buildFinalDoc();

    if(fileTypeCombo->currentIndex() == 0)
        format = "html";
    if(fileTypeCombo->currentIndex() == 1)
        format = "odt";
    if(fileTypeCombo->currentIndex() == 2){
        format = "txt";
        QMessageBox::information(this, tr("Project Exporter"), tr("You have selected the .txt format. There is no formatting !"), QMessageBox::Ok);
    }

    QTextDocumentWriter writer;
    QByteArray array;
    QString fileName(directoryLabelLineEdit->text() + "/" + projectNameLabelLineEdit->text() + "." + format);
    writer.setFormat(array.append(format));
    writer.setFileName(fileName);
    writer.setCodec(QTextCodec::codecForName("UTF-8"));
    writer.write(document);

    if(format=="html"){

                QFile *textFile = new QFile;
                textFile->setFileName(fileName);
                bool opened = textFile->open(QFile::ReadWrite | QFile::Text);
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

QTextDocument *Exporter::prepareTextDoc(QFile *textFile)
{



    QTextDocument *textDocument = new QTextDocument;

    textFile->open(QFile::ReadOnly | QFile::Text);
    QTextStream textFileStream( textFile );
    textDocument->setHtml(textFileStream.readAll());

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

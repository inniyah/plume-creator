#include "exporter.h"

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
    // synopsys ?

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

    textCheckBox = new QCheckBox("Insert story :", this);
    synCheckBox = new QCheckBox("Insert synopsies :", this);
    noteCheckBox = new QCheckBox("Insert notes :", this);

    fileTypeCombo = new QComboBox;
    QStringList list;
    list << tr(".html") << tr(".odt") << tr(".txt");
    fileTypeCombo->insertItems(0, list);



    QGridLayout *layout = new QGridLayout;
    layout->addWidget(label, 0, 0);
    //    layout->addWidget(directoryLabel, 1, 0);
    layout->addWidget(projectNameLabel, 1, 0);
    layout->addWidget(projectNameLabelLineEdit, 1, 1);
    layout->addWidget(directoryButton, 2, 0);
    layout->addWidget(directoryLabelLineEdit, 2, 1);
    layout->addWidget(fileTypeCombo, 3,1);
    layout->addWidget(textCheckBox, 4,0);
    layout->addWidget(synCheckBox, 5,0);
    layout->addWidget(noteCheckBox, 6,0);

    tree = new QTreeWidget;
    tree->setFixedWidth(width()/3);

    QHBoxLayout *treeLayout = new QHBoxLayout;
    treeLayout->addWidget(tree);
    treeLayout->addLayout(layout);


    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
                                     | QDialogButtonBox::Cancel);

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
    QApplication::setOverrideCursor(Qt::WaitCursor);
    exportDoc();
    QApplication::restoreOverrideCursor();



    QDialog::accept();
}

//-----------------------------------------------------------------------------------

void Exporter::setExistingDirectory()
{
    QFileDialog::Options options = QFileDialog::DontResolveSymlinks | QFileDialog::ShowDirsOnly;
    QString directory = QFileDialog::getExistingDirectory(this, tr("QFileDialog::getExistingDirectory()"),
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
            childItem->setFlags(item->flags() | Qt::ItemIsSelectable);
            childItem->setText(0, QString(30, 0xB7));
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













void Exporter::exportDoc()
{
    //search for checked QTreeWidgetItems :

    QTreeWidgetItemIterator *iterator = new QTreeWidgetItemIterator(tree, QTreeWidgetItemIterator::Checked);
    QList<QTreeWidgetItem *> *itemList = new QList<QTreeWidgetItem *>;


    while(iterator->operator *() != 0){
        itemList->append(iterator->operator *());
        iterator->operator ++();

    }
    QString debug;
    qDebug() << "itemList" << debug.setNum(itemList->size());

    QFileInfo *dirInfo = new QFileInfo(*targetDevice);
    QString devicePath = dirInfo->path();

    QTextDocument *textDocument = new QTextDocument;
    QTextEdit *edit = new QTextEdit;

    QString format;
    if(fileTypeCombo->currentIndex() == 0)
        format = "html";
    if(fileTypeCombo->currentIndex() == 1)
        format = "odt";
    if(fileTypeCombo->currentIndex() == 2)
        format = "txt";

    for(int i = 0; i < itemList->size(); ++i){
        QDomElement element = domElementForItem.value(itemList->at(i));
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
edit->append("<br>");
edit->append("<br>");
}
        if(element.tagName() == "chapter"){
        edit->append("<center><h2>" + element.attribute("name", "") + "</h2></center>");
        edit->append("<br>");
        }

        if(element.tagName() == "scene")
        edit->append("<h3>" + element.attribute("name", "") + "</h3>");


        if(synCheckBox->isChecked())
        edit->append(synFrag->toHtml());

        if(noteCheckBox->isChecked())
        edit->append(noteFrag->toHtml());

        if(textCheckBox->isChecked())
       edit->append(textFrag->toHtml());

        qDebug() << synFrag->toHtml() + noteFrag->toHtml() + textFrag->toHtml();
    }

    QTextDocumentWriter writer;
    QByteArray array;
    writer.setFormat(array.append(format));
    writer.setFileName(directoryLabelLineEdit->text() + "/" + projectNameLabelLineEdit->text() + "." + format);
    writer.setCodec(QTextCodec::codecForName("UTF-8"));
    writer.write(textDocument);



    QMessageBox::information(this, "Project exported", "This project was successfully exported !", QMessageBox::Ok);
}

//---------------------------------------------------------------------------------

QTextDocument *Exporter::prepareTextDoc(QFile *textFile)
{
    QTextDocument *textDocument = new QTextDocument;

    textFile->open(QFile::ReadOnly | QFile::Text);
    QTextStream textFileStream( textFile );
    textDocument->setHtml(textFileStream.readAll());


    return textDocument;
}

//---------------------------------------------------------------------------------


QTextDocument *Exporter::prepareSynDoc(QFile *synFile)
{
    QTextDocument *textDocument = new QTextDocument;

    synFile->open(QFile::ReadOnly | QFile::Text);
    QTextStream textFileStream( synFile );
    textDocument->setHtml(textFileStream.readAll());

    return textDocument;
}

//---------------------------------------------------------------------------------

QTextDocument *Exporter::prepareNoteDoc(QFile *noteFile)
{
    QTextDocument *textDocument = new QTextDocument;

    noteFile->open(QFile::ReadOnly | QFile::Text);
    QTextStream textFileStream( noteFile );
    textDocument->setHtml(textFileStream.readAll());

    return textDocument;
}

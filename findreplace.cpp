#include <QtGui>
#include "findreplace.h"

FindReplace::FindReplace(QFile *device, QWidget *parent) :
    QDialog(parent)
{

        setWindowTitle(tr("Find & Replace Dialog"));


    setMinimumSize(600,300);


    QLabel *label = new QLabel(tr("Please complete the fields below: "));
    label->setWordWrap(true);

    QLabel *findLabel = new QLabel(tr("Find :"));
    findLineEdit = new QLineEdit;
    QPushButton *findButton = new QPushButton(tr("&Find"));
//    projectNameLabelLineEdit->setValidator(new QRegExpValidator(QRegExp("[^\x002F\\\\:\x002A\?\x0022<>|]+"), projectNameLabelLineEdit));

       QLabel *replaceLabel = new QLabel(tr("Replace with : "));
    replaceLineEdit = new QLineEdit;
    QPushButton *replaceButton = new QPushButton(tr("&Replace"));
    QPushButton *replaceDocButton = new QPushButton(tr("&Replace current doc"));
    QPushButton *replaceAllButton = new QPushButton(tr("&Replace all"));

    connect(replaceButton, SIGNAL(clicked()), this, SLOT());
    connect(replaceDocButton, SIGNAL(clicked()), this, SLOT());
    connect(replaceAllButton, SIGNAL(clicked()), this, SLOT());



    QGroupBox *checkGroupBox = new QGroupBox(tr("Options :"), this);
    QGridLayout *checkLayout = new QGridLayout;
    textCheckBox = new QCheckBox(tr("Check story"), this);
    synCheckBox = new QCheckBox(tr("Check synopses"), this);
    noteCheckBox = new QCheckBox(tr("Check notes"), this);

    textCheckBox->setChecked(true);
    synCheckBox->setChecked(true);
    noteCheckBox->setChecked(true);

    checkLayout->addWidget(textCheckBox, 1,0);
    checkLayout->addWidget(synCheckBox, 2,0);
    checkLayout->addWidget(noteCheckBox, 3,0);
    checkGroupBox->setLayout(checkLayout);




    QWidget *stretcher1 = new QWidget;
//    stretcher1->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);

    QWidget *stretcher2 = new QWidget;
//    stretcher2->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);

    QPushButton *previousButton = new QPushButton(tr("Previous"), this);
    QPushButton *nextButton = new QPushButton(tr("Next"), this);
    QPushButton *replaceAndNextButton = new QPushButton(tr("Replace && Next"), this);
    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok);

    connect(previousButton, SIGNAL(clicked()), this, SLOT());
    connect(nextButton, SIGNAL(clicked()), this, SLOT());
    connect(replaceAndNextButton, SIGNAL(clicked()), this, SLOT());
    connect(buttonBox, SIGNAL(accepted()), this, SLOT(close()));

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(label, 0, 0, 1, 3);
    layout->addWidget(findLabel, 1, 0);
    layout->addWidget(findLineEdit, 2, 0, 1,3);
    layout->addWidget(findButton, 2, 3);
    layout->addWidget(replaceLabel, 3, 0);
    layout->addWidget(replaceLineEdit, 4, 0, 1,3);
    layout->addWidget(replaceButton, 4, 3);
    layout->addWidget(replaceDocButton, 5, 3);
    layout->addWidget(replaceAllButton, 6, 3);
    layout->addWidget(stretcher1, 7, 3);
    layout->addWidget(buttonBox, 8, 3);
    layout->addWidget(checkGroupBox,6,0,2,3);
//    layout->addWidget(stretcher2,7,0,1,4);
    layout->addWidget(previousButton,8,0);
    layout->addWidget(nextButton,8,1);
    layout->addWidget(replaceAndNextButton,8,2);

    tree = new QTreeWidget;
    tree->setFixedWidth(width()/3);


    QHBoxLayout *treeLayout = new QHBoxLayout;
    treeLayout->addWidget(tree);
    treeLayout->addLayout(layout);



QTextBrowser *docViewer = new QTextBrowser;

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addLayout(treeLayout,0,0);
    mainLayout->addWidget(docViewer,1,0);
    setLayout(mainLayout);



    createTree(device);
    targetDevice = device;



    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Warning);
    msgBox.setText(tr("The Find & Replace feature isn't implemented !<br>For now, it's only a mock-up GUI. Please wait for a future release !"));
    msgBox.exec();


}









//-----------------------------------------------------------------------------------
//--------Tree-----------------------------------------------------------------
//-----------------------------------------------------------------------------------



void FindReplace::createTree(QFile *device)
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

bool FindReplace::read(QFile *device)
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

void FindReplace::closeTree()
{

    tree->clear();


}

//---------------------------------------------------------------------------------------



void FindReplace::parseFolderElement(const QDomElement &element,
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

QTreeWidgetItem *FindReplace::createItem(const QDomElement &element,
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

void FindReplace::buildTree()
{




    tree->clear();


    QDomElement child = root.firstChildElement("book");
    while (!child.isNull()) {
        parseFolderElement(child);
        child = child.nextSiblingElement("book");
    }


}

//------------------------------------------------------------------------------------

void FindReplace::itemClickedSlot(QTreeWidgetItem* item, int column)
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










QTextDocument * FindReplace::buildFinalDoc()
{
//    //search for checked QTreeWidgetItems :

//    QTreeWidgetItemIterator *iterator = new QTreeWidgetItemIterator(tree, QTreeWidgetItemIterator::Checked);
//    QList<QTreeWidgetItem *> *itemList = new QList<QTreeWidgetItem *>;


//    while(iterator->operator *() != 0){
//        itemList->append(iterator->operator *());
//        iterator->operator ++();

//    }





//    //    QString debug;
//    //    qDebug() << "itemList" << debug.setNum(itemList->size());

//    QFileInfo *dirInfo = new QFileInfo(*targetDevice);
//    QString devicePath = dirInfo->path();

    QTextDocument *textDocument = new QTextDocument;
//    QTextEdit *edit = new QTextEdit;

//    textDocument->setDefaultStyleSheet("p, li { white-space: pre-wrap; } p{line-height: 2em; font-family:'Liberation Serif'; font-size:12pt;margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:72px;}");



//    for(int i = 0; i < itemList->size(); ++i){
//        QDomElement element = domElementForItem.value(itemList->at(i));
//        QTextCursor *tCursor = new QTextCursor(textDocument);

//        QTextBlockFormat blockFormatLeft;
//        blockFormatLeft.setBottomMargin(0);
//        blockFormatLeft.setTopMargin(0);
//        blockFormatLeft.setTextIndent(72);
//        blockFormatLeft.setLineHeight(200, QTextBlockFormat::ProportionalHeight);
//        blockFormatLeft.setAlignment(Qt::AlignJustify);
//        QTextCharFormat charFormatLeft;
//        charFormatLeft.setFontPointSize(12);
//        charFormatLeft.setFontFamily("Courrier");

//        QTextBlockFormat blockFormatCenter;
//        blockFormatCenter.setAlignment(Qt::AlignCenter);


//        if(element.tagName() != "separator"){



//            QString textPath = element.attribute("textPath");
//            QString synPath = element.attribute("synPath");
//            QString notePath = element.attribute("notePath");

//            QFile *textFile = new QFile(devicePath + textPath);
//            QFile *synFile = new QFile(devicePath + synPath);
//            QFile *noteFile = new QFile(devicePath + notePath);

//            QTextDocumentFragment textFrag(prepareTextDoc(textFile));
//            QTextDocumentFragment synFrag(prepareSynDoc(synFile));
//            QTextDocumentFragment noteFrag(prepareNoteDoc(noteFile));

//            edit->setDocument(textDocument);




//            if(element.tagName() == "book"){
//                textDocument->setMetaInformation(QTextDocument::DocumentTitle,element.attribute("name", ""));
//                edit->append("<h1>" + element.attribute("name", "") + "</h1>");
//                tCursor->movePosition(QTextCursor::End, QTextCursor::MoveAnchor,1);
//                tCursor->mergeBlockFormat(blockFormatCenter);
//                edit->append("<h4>" + QDateTime::currentDateTime().toString() + "</h4>");
//                tCursor->movePosition(QTextCursor::End, QTextCursor::MoveAnchor,1);
//                tCursor->mergeBlockFormat(blockFormatCenter);
//                edit->append("<br>");
//                edit->append("<br>");

//            }
//            if(element.tagName() == "chapter"){
//                edit->append("<br>");
//                edit->append("<br>");
//                edit->append("<h2>" + element.attribute("name", "") + "</h2>");
//                tCursor->movePosition(QTextCursor::End, QTextCursor::MoveAnchor,1);
//                tCursor->mergeBlockFormat(blockFormatCenter);
//                edit->append("<br>");

//            }

//            if(element.tagName() == "scene" && sceneTitleCheckBox->isChecked()){
//                edit->append("<br>");
//                edit->append("<h3>" + element.attribute("name", "") + "</h3>");
//                tCursor->movePosition(QTextCursor::End, QTextCursor::MoveAnchor,1);
//                tCursor->mergeBlockFormat(blockFormatCenter);
//                edit->append("<br>");

//            }

//            if(synCheckBox->isChecked() && !synFrag.isEmpty()){
//                edit->append("<br>");
//                edit->append("<h4>" + tr("Synopsis") + "</h4>");
//                tCursor->movePosition(QTextCursor::End, QTextCursor::MoveAnchor,1);
//                tCursor->mergeBlockFormat(blockFormatCenter);
//                edit->append("<br>");
//                tCursor->movePosition(QTextCursor::End, QTextCursor::MoveAnchor,1);
//                tCursor->insertBlock(blockFormatLeft, charFormatLeft);
//                tCursor->insertFragment(synFrag);
//            }

//            if(noteCheckBox->isChecked() && !noteFrag.isEmpty()){
//                edit->append("<br>");
//                edit->append("<h4>" + tr("Note") + "</h4>");
//                tCursor->movePosition(QTextCursor::End, QTextCursor::MoveAnchor,1);
//                tCursor->mergeBlockFormat(blockFormatCenter);
//                edit->append("<br>");
//                tCursor->movePosition(QTextCursor::End, QTextCursor::MoveAnchor,1);
//                tCursor->insertBlock(blockFormatLeft, charFormatLeft);
//                tCursor->insertFragment(noteFrag);
//            }

//            if(textCheckBox->isChecked()){
//                if((synCheckBox->isChecked() || noteCheckBox->isChecked()) && !textFrag.isEmpty()){
//                    tCursor->insertBlock();
//                    tCursor->insertHtml("<h4>" + tr("Story") + "</h4>");
//                    tCursor->mergeBlockFormat(blockFormatCenter);
//                    tCursor->insertBlock();

//                }
//                tCursor->insertHtml("<br>");
//                //                tCursor->movePosition(QTextCursor::End, QTextCursor::MoveAnchor,1);
//                tCursor->insertBlock(blockFormatLeft, charFormatLeft);
//                tCursor->insertFragment(textFrag);
//                //                edit->append(textFrag->toHtml());
//            }
//        }
//        else if(element.tagName() == "separator"){
//            edit->append("<br>");
//            edit->append("<h3>#</h3>");
//            tCursor->movePosition(QTextCursor::End, QTextCursor::MoveAnchor,1);
//            tCursor->mergeBlockFormat(blockFormatCenter);
//            edit->append("<br>");
//            tCursor->movePosition(QTextCursor::End, QTextCursor::MoveAnchor,1);
//            tCursor->mergeBlockFormat(blockFormatLeft);
//        }



//    }

    return textDocument;
}

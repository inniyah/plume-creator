#include "findreplace.h"
#include "ui_findreplace.h"

FindReplace::FindReplace(QWidget *parent) :
    QDialog(parent),allowRestartFromBeginning(false),allowRestartFromEnd(false),
    ui(new Ui::FindReplace), currentTextOpened(-1),currentWordToFind(""), startingOrClosing(true), configApplying(true)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_DeleteOnClose);

}

void FindReplace::postConstructor()
{



    //        QMessageBox msgBox;
    //        msgBox.setIcon(QMessageBox::Warning);
    //        msgBox.setText("The Find & Replace feature isn't implemented !<br>For now, it's only a mock-up GUI. Please wait for a future release !"
    //                          " Update : it will be available for 0.63");
    //        msgBox.exec();

    ui->textEdit->setTextStyles(textStyles);
    ui->textEdit->createContent();

    createTree();

    applyConfig();


    cloneAllDocs();

    connect(ui->tree, SIGNAL(clickCheckBoxSignal(QModelIndex)), this, SLOT(resetSearch()));


    ui->littleOptionsWidget->setEnabled(false);
    ui->treeWidget->setEnabled(false);
    ui->tabWidget->setEnabled(false);

    startingOrClosing = false;

}

FindReplace::~FindReplace()
{

    saveConfig();

    delete ui;
}












//-----------------------------------------------------------------------------------
//--------Tree-----------------------------------------------------------------
//-----------------------------------------------------------------------------------



void FindReplace::createTree()
{
    domDocument = hub->mainTreeDomDoc();
    root = domDocument.documentElement();


    proxy = new CheckableTreeProxy(this, "findReplaceExpanded", "findReplaceCheckState", MainTreeItem::FindReplace);
    proxy->setHub(hub);
    proxy->setColumnCount(2);
    proxy->setSourceModel(absTreeModel);
    ui->tree->setHub(hub);
    ui->tree->setMainTreeAbstractModel(absTreeModel);
    ui->tree->setModel(proxy);
    ui->tree->applySettingsFromData();


    QHeaderView *header = ui->tree->header();

#if QT_VERSION >= 0x050000
    header->setSectionResizeMode(QHeaderView::ResizeToContents);
    header->setSectionsMovable(false);
    header->setSectionsClickable(false);
#else
    header->setResizeMode(QHeaderView::ResizeToContents);
    header->setMovable(false);
    header->setClickable(false);
#endif

    connect(ui->tree, SIGNAL(textAndNoteSignal(int,QString)), this, SLOT(openText(int)));
    connect(this, SIGNAL(currentOpenedSheetSignal(int)), proxy, SLOT(setSecondaryOpenedSheetSlot(int)));

}

//------------------------------------------------------------------------------------


void FindReplace::resetSearch()
{
    //search for checked items :

    QDomDocument domDoc = hub->mainTreeDomDoc();
    QDomElement root = domDoc.documentElement();

    QList<QDomElement> checkedElementList = searchForCheckedItems(root);

    if(checkedElementList.size() == 0 && !startingOrClosing){
        QMessageBox::information(this, tr("Find && Replace"),
                                 tr("No document has been selected in the tree.\n"
                                    "Have you checked the one(s) you want to look through ?"),
                                 QMessageBox::Ok,
                                 QMessageBox::Ok);
        return;

    }
    if(ui->findEdit->text().size() == 0 && !startingOrClosing){
        QMessageBox::information(this, tr("Find && Replace"),
                                 tr("No text has been entered to find."),
                                 QMessageBox::Ok,
                                 QMessageBox::Ok);
        return;

    }





    // search in these docs and list them if there are answers :

    objectNamesListOfFoundDocs.clear();
    listOfPositionsForDocObjectName.clear();
    QHash<int,int> numberForSheetIdHash;
    progressTotal = 0;


    foreach(QDomElement element, checkedElementList){
        int idNumber = element.attribute("number", "").toInt();

        if(element.tagName() == "trash" || element.tagName() == "separator")
            continue;

        QList<int> listOfTextPositions, listOfSynPositions, listOfNotePositions;

        if(m_textCheckBox){
            MainTextDocument *textDoc = this->findChild<MainTextDocument *>("cloned-textDoc_" + QString::number(idNumber));
            listOfTextPositions = findTextPos(ui->findEdit->text(), textDoc);
            progressTotal += listOfTextPositions.size();
            listOfPositionsForDocObjectName.insert(textDoc->objectName(), listOfTextPositions);
            if(!listOfTextPositions.isEmpty()){
                objectNamesListOfFoundDocs.append(textDoc->objectName());
            }
        }
        if(m_synCheckBox){
            MainTextDocument *synDoc = this->findChild<MainTextDocument *>("cloned-synDoc_" + QString::number(idNumber));
            listOfSynPositions = findTextPos(ui->findEdit->text(), synDoc);
            progressTotal += listOfSynPositions.size();
            listOfPositionsForDocObjectName.insert(synDoc->objectName(), listOfSynPositions);
            if(!listOfSynPositions.isEmpty()){
                objectNamesListOfFoundDocs.append(synDoc->objectName());
            }
        }
        if(m_noteCheckBox){
            MainTextDocument *noteDoc = this->findChild<MainTextDocument *>("cloned-noteDoc_" + QString::number(idNumber));
            listOfNotePositions = findTextPos(ui->findEdit->text(), noteDoc);
            progressTotal += listOfNotePositions.size();
            listOfPositionsForDocObjectName.insert(noteDoc->objectName(), listOfNotePositions);
            if(!listOfNotePositions.isEmpty()){
                objectNamesListOfFoundDocs.append(noteDoc->objectName());
            }
        }





        if(listOfTextPositions.size() > 0 || listOfSynPositions.size() > 0 || listOfNotePositions.size() > 0 )
            numberForSheetIdHash.insert(idNumber, listOfTextPositions.size() + listOfSynPositions.size() + listOfNotePositions.size());




    }


    proxy->setNumberForSheetIdHash(numberForSheetIdHash);





    currentWordToFind = ui->findEdit->text();
    ui->progressLabel->setText( "" + QString::number(progressTotal));

}

//--------------------------------------------------------------------------------

void FindReplace::on_findButton_clicked()
{

    resetSearch();




    if(objectNamesListOfFoundDocs.isEmpty())
        return;

    QString firstObjectName = objectNamesListOfFoundDocs.first();
    int num = firstObjectName.mid(firstObjectName.indexOf("_") + 1).toInt();

    //    qDebug() << "eeee : " + QString::number(num);

    if(num > 0)
        this->openText(num);


    ui->littleOptionsWidget->setEnabled(true);
    ui->treeWidget->setEnabled(true);
    ui->tabWidget->setEnabled(true);





    on_nextButton_clicked();

}

//-----------------------------------------------------------------

QList<QDomElement> FindReplace::searchForCheckedItems(QDomElement element)
{
    QList<QDomElement> list;

    if(element.tagName() == "plume-tree"){
        for(int i = 0 ; i < element.childNodes().size() ; ++i)
            list.append(searchForCheckedItems(element.childNodes().at(i).toElement()));

    }
    else if(element.attribute("findReplaceCheckState", "2").toInt() != 0){



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

//--------------------------------------------------------------------------------------

QList<int> FindReplace::findTextPos(QString text, MainTextDocument *document)
{
    QList<int> list;
    QTextCursor cursor;
    cursor = document->find(text, 0, findFlags);
    while(!cursor.isNull()){
        list.append(cursor.anchor());
        cursor = document->find(text, cursor.position(), findFlags);
    }
    return list;
}

void FindReplace::applyConfig()
{
    configApplying = true;

    QSettings settings;

    settings.beginGroup( "FindReplace" );
    ui->caseSensitiveCheckBox->setChecked(settings.value("caseSensitive", false).toBool());
    ui->caseSensitiveCheckBox->setChecked(settings.value("caseSensitive", false).toBool());
    ui->textCheckBox->setChecked(settings.value("textCheck", true).toBool());
    ui->synCheckBox->setChecked(settings.value("synCheck", false).toBool());
    ui->noteCheckBox->setChecked(settings.value("noteCheck", false).toBool());

    settings.endGroup();

    configApplying = false;

}

//---------------------------------------------------------------------------------------


void FindReplace::saveConfig()
{
    QSettings settings;

    settings.beginGroup( "FindReplace" );
    settings.setValue("caseSensitive", ui->caseSensitiveCheckBox->isChecked());
    settings.setValue("textCheck", ui->textCheckBox->isChecked());
    settings.setValue("synCheck", ui->synCheckBox->isChecked());
    settings.setValue("noteCheck", ui->noteCheckBox->isChecked());
    settings.endGroup();

}


//---------------------------------------------------------------------------------------

void FindReplace::openText(int id)
{
    if(id > 10000)
        return;


    this->closeText(currentTextOpened);


    MainTextDocument *textDoc = this->findChild<MainTextDocument *>("cloned-textDoc_" + QString::number(id));
    ui->textEdit->setDoc(textDoc);
    textDoc->textHighlighter()->setTextToHighlight(currentWordToFind);

    MainTextDocument *synDoc = this->findChild<MainTextDocument *>("cloned-synDoc_" + QString::number(id));
    ui->synEdit->openSyn(synDoc);
    synDoc->textHighlighter()->setTextToHighlight(currentWordToFind);

    MainTextDocument *noteDoc = this->findChild<MainTextDocument *>("cloned-noteDoc_" + QString::number(id));
    ui->noteEdit->openNote(noteDoc);
    noteDoc->textHighlighter()->setTextToHighlight(currentWordToFind);


    currentTextOpened = id;
    emit currentOpenedSheetSignal(id);

}
//---------------------------------------------------------------------------------------

void FindReplace::closeText(int id)
{
    if(id == -1)
        return;



    MainTextDocument *textDoc = this->findChild<MainTextDocument *>("cloned-textDoc_" + QString::number(id));
    textDoc->textHighlighter()->setTextToHighlight("");

    MainTextDocument *synDoc = this->findChild<MainTextDocument *>("cloned-synDoc_" + QString::number(id));
    synDoc->textHighlighter()->setTextToHighlight("");


    MainTextDocument *noteDoc = this->findChild<MainTextDocument *>("cloned-noteDoc_" + QString::number(id));
    noteDoc->textHighlighter()->setTextToHighlight("");




}

//---------------------------------------------------------------------------------------

void FindReplace::on_caseSensitiveCheckBox_toggled(bool checked)
{
    if(checked){

        findFlags |= QTextDocument::FindCaseSensitively;
       caseSensitive = true;
    }
    else{
        findFlags &= ~QTextDocument::FindCaseSensitively;
        caseSensitive = false;

    }



    if(!configApplying)
    this->resetSearch();



}





//---------------------------------------------------------------------------------------

void FindReplace::on_replaceAndNextButton_clicked()
{
    if(currentTextOpened == -1 || currentWordToFind.isEmpty() || ui->replaceEdit->text().isEmpty())
        return;

    TextZone *tEdit;
    NoteZone *nEdit;
    QTextCursor cursor;
    MainTextDocument *doc;
    QString type;

    if(ui->tabWidget->currentWidget()->objectName() == "textTab"){
        tEdit = ui->textEdit;
        doc = this->findChild<MainTextDocument *>("cloned-textDoc_" + QString::number(currentTextOpened));
        cursor = tEdit->textCursor();
        type = "text";
    }
    if(ui->tabWidget->currentWidget()->objectName() == "synTab"){
        nEdit = ui->synEdit;
        doc = this->findChild<MainTextDocument *>("cloned-synDoc_" + QString::number(currentTextOpened));
        cursor = nEdit->textCursor();
        type = "syn";
    }
    if(ui->tabWidget->currentWidget()->objectName() == "noteTab"){
        nEdit = ui->noteEdit;
        doc = this->findChild<MainTextDocument *>("cloned-noteDoc_" + QString::number(currentTextOpened));
        cursor = nEdit->textCursor();
        type = "note";
    }








    bool isTextGood;
    QTextCursor checkCursor(doc);


    if(cursor.selectedText() == currentWordToFind && !cursor.selectedText().isEmpty())
        isTextGood = true;
    else
    {


        checkCursor.setPosition(doc->find(currentWordToFind, cursor.position(), findFlags).position());
        //        qDebug() << "checkCursor.position() : "<< QString::number(checkCursor.position());

        checkCursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, currentWordToFind.size());
        //        qDebug() << "checkCursor.selectedText() : "<< checkCursor.selectedText();
        //        qDebug() << "cursor.selectedText() : "<< cursor.selectedText();

        if(cursor.selectedText() == checkCursor.selectedText() && !cursor.selectedText().isEmpty())
            isTextGood = true;
        else
            isTextGood = false;
    }

    if(ui->replaceEdit->text().isEmpty())
        return;

    if(isTextGood){
        cursor.insertText(ui->replaceEdit->text());
        //        cursor.setPosition(cursor.position() - 1);
        if(type == "syn" || type == "note")
            nEdit->setTextCursor(cursor);
        if(type == "text")
            tEdit->setTextCursor(cursor);
    }
    on_nextButton_clicked();

}



//---------------------------------------------------------------------------------------

void FindReplace::on_replaceDocButton_clicked()
{
    if(currentTextOpened == -1 || currentWordToFind.isEmpty()|| ui->replaceEdit->text().isEmpty())
        return;


    QString name;

    if(ui->tabWidget->currentWidget()->objectName() == "textTab"){
        name = "cloned-textDoc_";
    }
    if(ui->tabWidget->currentWidget()->objectName() == "synTab"){
        name = "cloned-synDoc_";
    }
    if(ui->tabWidget->currentWidget()->objectName() == "noteTab"){
        name = "cloned-noteDoc_";
    }

    MainTextDocument *clonedDoc = this->findChild<MainTextDocument *>(name + QString::number(currentTextOpened));

    QTextCursor *tCursor = new QTextCursor(clonedDoc);
    QList<int>list = findTextPos(currentWordToFind, clonedDoc);
    while(list.size() != 0){
        //            qDebug() << "list size : "+ QString::number(list.size());
        tCursor->clearSelection();
        tCursor->setPosition(list.takeLast(), QTextCursor::MoveAnchor);
        //            qDebug() << "tCursor : "+ QString::number(tCursor->position());
        tCursor->movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor, currentWordToFind.size());
        tCursor->insertText(ui->replaceEdit->text());

    }
}




//---------------------------------------------------------------------------------------

void FindReplace::on_replaceAllButton_clicked()
{
    //    qDebug() <<  "aa";
    if(currentTextOpened == -1 || currentWordToFind.isEmpty()|| ui->replaceEdit->text().isEmpty())
        return;
    //qDebug() << "bb";
    //    int openedSheet = currentTextOpened;
    //int currentTabIndex = ui->tabWidget->currentIndex();

    // set up the progress bar :
    QProgressDialog progress("Replacing...", "Abort replacing", 0, listOfPositionsForDocObjectName.size(), this, Qt::FramelessWindowHint | Qt::WindowStaysOnBottomHint);
    progress.setWindowModality(Qt::WindowModal);
    progress.show();

    //qDebug() << "cc";

    int j = 0;


    QHash<QString, QList<int> >::iterator i = listOfPositionsForDocObjectName.begin();
    while (i != listOfPositionsForDocObjectName.end()) {

        if (progress.wasCanceled())
            break;






        QString objName = i.key();
        MainTextDocument *clonedDoc = this->findChild<MainTextDocument *>(objName);


        //        this->openText(clonedDoc->idNumber());

        //        if(clonedDoc->docType() == "text")
        //ui->tabWidget->setCurrentWidget(ui->textTab);
        //        if(clonedDoc->docType() == "synopsis")
        //ui->tabWidget->setCurrentWidget(ui->synTab);
        //        if(clonedDoc->docType() == "note")
        //ui->tabWidget->setCurrentWidget(ui->noteTab);

        QTextCursor *tCursor = new QTextCursor(clonedDoc);
        QList<int>list = findTextPos(currentWordToFind, clonedDoc);
        while(list.size() != 0){
            //            qDebug() << "list size : "+ QString::number(list.size());
            tCursor->clearSelection();
            tCursor->setPosition(list.takeLast(), QTextCursor::MoveAnchor);
            //            qDebug() << "tCursor : "+ QString::number(tCursor->position());
            tCursor->movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor, currentWordToFind.size());
            tCursor->insertText(ui->replaceEdit->text());

        }



        j += 1;
        progress.setValue(j);


        ++i;
    }

    progress.setValue(listOfPositionsForDocObjectName.size());

    //    this->openText(openedSheet);
    //ui->tabWidget->setCurrentIndex(currentTabIndex);




}



//---------------------------------------------------------------------------------------

void FindReplace::on_prevDocButton_clicked()
{
    if(currentTextOpened == -1 || currentWordToFind.isEmpty())
        return;


    QString name;

    if(ui->tabWidget->currentWidget()->objectName() == "textTab"){
        name = "cloned-textDoc_";
    }
    if(ui->tabWidget->currentWidget()->objectName() == "synTab"){
        name = "cloned-synDoc_";
    }
    if(ui->tabWidget->currentWidget()->objectName() == "noteTab"){
        name = "cloned-noteDoc_";
    }
    if(objectNamesListOfFoundDocs.isEmpty())
        return;

    int index = objectNamesListOfFoundDocs.indexOf(name + QString::number(currentTextOpened));
    if(index == 0)
        return;

    QString prevObjectName = objectNamesListOfFoundDocs.at(index - 1);

    int prevNum = prevObjectName.mid(prevObjectName.indexOf("_") + 1).toInt();
    prevObjectName.truncate(prevObjectName.indexOf("_") + 1);
    QString prevType = prevObjectName;


    if(prevNum > 0)
        this->openText(prevNum );


    if(prevType == "cloned-textDoc_")
        ui->tabWidget->setCurrentIndex(0);
    if(prevType == "cloned-synDoc_")
        ui->tabWidget->setCurrentIndex(1);
    if(prevType == "cloned-noteDoc_")
        ui->tabWidget->setCurrentIndex(2);




    on_nextButton_clicked();
}



//---------------------------------------------------------------------------------------

void FindReplace::on_nextDocButton_clicked()
{
    //    docProgressTotal = 0;

    if(currentTextOpened == -1 || currentWordToFind.isEmpty())
        return;


    QString name;

    if(ui->tabWidget->currentWidget()->objectName() == "textTab"){
        name = "cloned-textDoc_";
    }
    if(ui->tabWidget->currentWidget()->objectName() == "synTab"){
        name = "cloned-synDoc_";
    }
    if(ui->tabWidget->currentWidget()->objectName() == "noteTab"){
        name = "cloned-noteDoc_";
    }
    if(objectNamesListOfFoundDocs.isEmpty())
        return;

    int index = objectNamesListOfFoundDocs.indexOf(name + QString::number(currentTextOpened));
    if(index == objectNamesListOfFoundDocs.size() - 1)
        return;

    QString nextObjectName = objectNamesListOfFoundDocs.at(index + 1);

    int nextNum = nextObjectName.mid(nextObjectName.indexOf("_") + 1).toInt();
    nextObjectName.truncate(nextObjectName.indexOf("_") + 1);
    QString nextType = nextObjectName;


    if(nextNum > 0)
        this->openText(nextNum );


    if(nextType == "cloned-textDoc_")
        ui->tabWidget->setCurrentIndex(0);
    if(nextType == "cloned-synDoc_")
        ui->tabWidget->setCurrentIndex(1);
    if(nextType == "cloned-noteDoc_")
        ui->tabWidget->setCurrentIndex(2);




    on_nextButton_clicked();

}



//---------------------------------------------------------------------------------------

void FindReplace::on_prevButton_clicked()
{
    if(currentTextOpened == -1 || currentWordToFind.isEmpty())
        return;


    TextZone *tEdit;
    NoteZone *nEdit;
    QTextCursor cursor;
    MainTextDocument *doc;
    QString type;

    if(ui->tabWidget->currentWidget()->objectName() == "textTab"){
        tEdit = ui->textEdit;
        doc = this->findChild<MainTextDocument *>("cloned-textDoc_" + QString::number(currentTextOpened));
        cursor = tEdit->textCursor();
        type = "text";
    }
    if(ui->tabWidget->currentWidget()->objectName() == "synTab"){
        nEdit = ui->synEdit;
        doc = this->findChild<MainTextDocument *>("cloned-synDoc_" + QString::number(currentTextOpened));
        cursor = nEdit->textCursor();
        type = "syn";
    }
    if(ui->tabWidget->currentWidget()->objectName() == "noteTab"){
        nEdit = ui->noteEdit;
        doc = this->findChild<MainTextDocument *>("cloned-noteDoc_" + QString::number(currentTextOpened));
        cursor = nEdit->textCursor();
        type = "note";
    }


    oldPosition = cursor.position();

    if(allowRestartFromEnd){
        cursor.movePosition(QTextCursor::End, QTextCursor::MoveAnchor);
        allowRestartFromEnd = false;
    }

    positionsList = findTextPos(currentWordToFind, doc);
    if(positionsList.last() == -1)
        positionsList.takeLast();

    if(!positionsList.isEmpty()){

        int maxPrevPos = -1;
        for(int i = positionsList.size() ; i > 0; --i){
            int textPos = positionsList.at(i - 1);
            if(cursor.anchor() > textPos){
                maxPrevPos = textPos;
                break;
            }
        }
        if(maxPrevPos != -1){
            cursor.setPosition(maxPrevPos);
            cursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, currentWordToFind.size());

            if(type == "text"){
                tEdit->setFocus();
                tEdit->setTextCursor(cursor);
                tEdit->ensureCursorVisible();
            }
            if(type == "syn" || type == "note"){
                nEdit->setFocus();
                nEdit->setTextCursor(cursor);
                nEdit->ensureCursorVisible();
            }
        }
        else
            allowRestartFromEnd = true;
    }
    else {
        cursor.setPosition(oldPosition);
        if(type == "text"){
            tEdit->setTextCursor(cursor);
            tEdit->ensureCursorVisible();
        }
        if(type == "syn" || type == "note"){
            nEdit->setTextCursor(cursor);
            nEdit->ensureCursorVisible();
        }

        qDebug() << "no match";


    }

}



//---------------------------------------------------------------------------------------

void FindReplace::on_nextButton_clicked()
{
    if(currentTextOpened == -1|| currentWordToFind.isEmpty())
        return;


    TextZone *tEdit;
    NoteZone *nEdit;
    QTextCursor cursor;
    MainTextDocument *doc;
    QString type;

    if(ui->tabWidget->currentWidget()->objectName() == "textTab"){
        tEdit = ui->textEdit;
        doc = this->findChild<MainTextDocument *>("cloned-textDoc_" + QString::number(currentTextOpened));
        cursor = tEdit->textCursor();
        type = "text";
    }
    if(ui->tabWidget->currentWidget()->objectName() == "synTab"){
        nEdit = ui->synEdit;
        doc = this->findChild<MainTextDocument *>("cloned-synDoc_" + QString::number(currentTextOpened));
        cursor = nEdit->textCursor();
        type = "syn";
    }
    if(ui->tabWidget->currentWidget()->objectName() == "noteTab"){
        nEdit = ui->noteEdit;
        doc = this->findChild<MainTextDocument *>("cloned-noteDoc_" + QString::number(currentTextOpened));
        cursor = nEdit->textCursor();
        type = "note";
    }





    oldPosition = cursor.position();

    if(allowRestartFromBeginning){
        cursor.movePosition(QTextCursor::Start, QTextCursor::MoveAnchor);
        allowRestartFromBeginning = false;
    }

    positionsList = findTextPos(currentWordToFind, doc);

    if(!positionsList.isEmpty()){

        int maxNextPos = -1;
        foreach(const int &textPos, positionsList){

            if(cursor.position() <= textPos){

                maxNextPos = textPos;
                break;
            }

        }
        if(maxNextPos != -1){
            cursor.setPosition(maxNextPos);
            cursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, currentWordToFind.size());

            if(type == "text"){
                tEdit->setFocus();
                tEdit->setTextCursor(cursor);
                tEdit->ensureCursorVisible();
            }
            if(type == "syn" || type == "note"){
                nEdit->setFocus();
                nEdit->setTextCursor(cursor);
                nEdit->ensureCursorVisible();
            }
        }
        else
            allowRestartFromBeginning = true; // must click once again to do restart from  beginning
    }
    else {
        cursor.setPosition(oldPosition);
        if(type == "text"){
            tEdit->setTextCursor(cursor);
            tEdit->ensureCursorVisible();
        }
        if(type == "syn" || type == "note"){
            nEdit->setTextCursor(cursor);
            nEdit->ensureCursorVisible();
        }

        qDebug() << "no match";

    }






}
//--------------------------------------------------------------------------------
void FindReplace::accept()
{
    startingOrClosing = true;
    // restore all docs :


    QHash<QString, QList<int> >::iterator i = listOfPositionsForDocObjectName.begin();
    while (i != listOfPositionsForDocObjectName.end()) {
        QString objName = i.key();
        MainTextDocument *clonedDoc = this->findChild<MainTextDocument *>(objName);
        QString originalObjectName = objName.mid(7);
        MainTextDocument *originalDoc = hub->findChild<MainTextDocument *>(originalObjectName);


        if(clonedDoc->docType() == originalDoc->docType() && clonedDoc->idNumber() == originalDoc->idNumber())
            originalDoc->setHtml(clonedDoc->toHtml());
        else
            qWarning() << "FindReplace::accept() : no match for doc number : " + objName.mid(objName.indexOf("_")+ 1);

        ++i;
    }






    QDialog::accept();

}
//---------------------------------------------------------------------------------------
void FindReplace::reject()
{
    startingOrClosing = true;
    this->close();

    QDialog::reject();
}
//--------------------------------------------------------------------------------
void FindReplace::on_textCheckBox_toggled(bool checked)
{
    m_textCheckBox = checked;
    if(!configApplying)
    this->resetSearch();
}
//--------------------------------------------------------------------------------
void FindReplace::on_synCheckBox_toggled(bool checked)
{
    m_synCheckBox = checked;
    if(!configApplying)
    this->resetSearch();
}
//--------------------------------------------------------------------------------
void FindReplace::on_noteCheckBox_toggled(bool checked)
{
    m_noteCheckBox = checked;
    if(!configApplying)
    this->resetSearch();
}

//--------------------------------------------------------------------------------
void FindReplace::cloneAllDocs()
{
    QString name;
    QHash<MainTextDocument *, QFile *> fileForDoc = hub->mainTree_fileForDocHash();


    QHash<MainTextDocument *, QFile *>::iterator i = fileForDoc.begin();
    while (i != fileForDoc.end()) {
        MainTextDocument *doc = i.key();
        if(doc->docType() == "text")
            name = "cloned-textDoc_";
        if(doc->docType() == "synopsis")
            name = "cloned-synDoc_";
        if(doc->docType() == "note")
            name = "cloned-noteDoc_";



        MainTextDocument *clonedDoc = new MainTextDocument(this);
        clonedDoc->setHtml(doc->toHtml());
        clonedDoc->setObjectName(name + QString::number(doc->idNumber()));
        clonedDoc->setDocType(doc->docType());
        clonedDoc->setIdNumber(doc->idNumber());

        ++i;
    }

}

//--------------------------------------------------------------------------------
void FindReplace::on_findEdit_textChanged(const QString &arg1)
{


    currentWordToFind = arg1;

    ui->littleOptionsWidget->setEnabled(true);
    ui->treeWidget->setEnabled(true);
    ui->tabWidget->setEnabled(true);

    resetSearch();


}
//--------------------------------------------------------------------------------

void FindReplace::on_findEdit_editingFinished()
{
    on_findButton_clicked();

}

//--------------------------------------------------------------------------------


void FindReplace::keyPressEvent(QKeyEvent *event)
{
    //    sounds->startSoundKeyAny();




    if(ui->findEdit->isModified() && (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return)){
        on_findEdit_editingFinished();
        return;
    }
    if(ui->replaceEdit->isModified() && (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return)){
        on_replaceAndNextButton_clicked();
        return;
    }




    else
        QDialog::keyPressEvent(event);


}



//--------------------------------------------------------------------------------

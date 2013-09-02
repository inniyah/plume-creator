#include "maintreecontextmenu.h"

MainTreeContextMenu::MainTreeContextMenu(QObject *parent, QWidget *baseWidget):
    QObject(parent), base(baseWidget), numberOfStatusActions(0)
{


    prepareContextMenu();



}

void MainTreeContextMenu::postConstructor()
{
    connect(this, SIGNAL(actionSignal(QString,int)), absTreeModel, SLOT(actionSlot(QString,int)), Qt::UniqueConnection);
    connect(this, SIGNAL(actionSignal(QString,int, QVariant)), absTreeModel, SLOT(actionSlot(QString,int,QVariant)), Qt::UniqueConnection);
}

QMenu *MainTreeContextMenu::menu(MainTreeContextMenu::Actions flags)
{


    targetedElement = hub->mainTree_domElementForNumberHash().value(m_id);

    if(targetedElement.isNull())
        return 0;

    QSettings settings;
    QString tagName = targetedElement.tagName();

    QMenu *m_menu = new QMenu;

    if(flags.testFlag(MainTreeContextMenu::Rename)){
        if(tagName != "separator")
            m_menu->addAction(renameAct);

        m_menu->addSeparator();
    }
    if(flags.testFlag(MainTreeContextMenu::AddSheet)){

        if(tagName == "book"){
            m_menu->addAction(addBookNextAct);
            m_menu->addAction(addChildActAct);
            m_menu->addAction(addChildChapterAct);
        }
        if(tagName == "act"){
            m_menu->addAction(addActNextAct);
            m_menu->addAction(addChildChapterAct);
        }
        if(tagName == "chapter"){
            m_menu->addAction(addChapterNextAct);
            m_menu->addAction(addChildSceneAct);
            m_menu->addAction(addChildSeparatorAct);
        }
        if(tagName == "scene" || tagName == "separator"){
            m_menu->addAction(addSceneNextAct);
            m_menu->addAction(addSeparatorNextAct);
        }


        m_menu->addSeparator();
    }
    if(flags.testFlag(MainTreeContextMenu::Badge)){

        m_menu->addAction(setBadgeTextAct);
        m_menu->addAction(displayBadgeAct);
        displayBadgeAct->setChecked(settings.value("MainTree/badgeDisplayed", false).toBool());
        m_menu->addSeparator();
    }
    if(flags.testFlag(MainTreeContextMenu::Status)){

        QMenu *statusMenu = m_menu->addMenu(tr("&Status"));
        numberOfStatusActions = 0;
        for(int i = 0 ; i < MainTreeAbstractModel::giveStatusList().size() ; ++i){
            QAction *statusAct = new QAction(MainTreeAbstractModel::giveStatus(i), this);
            statusAct->setCheckable(true);
            statusAct->setObjectName("statusAction_" + QString::number(i));
            if(targetedElement.attribute("status", "0").toInt() == i)
            statusAct->setChecked(true);
            else
                statusAct->setChecked(false);

            connect(statusAct, SIGNAL(toggled(bool)), this, SLOT(setStatus()), Qt::UniqueConnection);
            statusMenu->addAction(statusAct);
            numberOfStatusActions += 1;

        }
//        m_menu->addAction(displayStatusAct);
//        displayStatusAct->setChecked(settings.value("MainTree/statusDisplayed", false).toBool());
        m_menu->addSeparator();
    }

    if(flags.testFlag(MainTreeContextMenu::Move)){

        m_menu->addAction(moveUpAct);
        m_menu->addAction(moveDownAct);

        m_menu->addSeparator();
    }
    if(flags.testFlag(MainTreeContextMenu::Delete)){

        QMenu *delItemMenu = m_menu->addMenu(tr("&Delete"));
        delItemMenu->addAction(sendToTrashAct);
    }
    if(flags.testFlag(MainTreeContextMenu::EmptyTrash)){

        m_menu->addAction(emptyTrashAct);

    }

    if(flags.testFlag(MainTreeContextMenu::Advanced)){

        m_menu->addSeparator();

        QMenu *advancedMenu = m_menu->addMenu(tr("&Advanced"));
        advancedMenu->addAction(autoRenameChildrenAct);
        advancedMenu->addAction(addMultiAct);
        advancedMenu->addAction(splitAct);

    }
    return m_menu;
}




void MainTreeContextMenu::prepareContextMenu()
{

    renameAct = new QAction(QIcon(":/pics/edit-rename.png"),tr("&Rename"), this);
    connect(renameAct, SIGNAL(triggered()), this, SLOT(rename()));


    moveUpAct = new QAction(QIcon(":/pics/arrow-up.png"),tr("Move Up"), this);
    connect(moveUpAct, SIGNAL(triggered()), this, SLOT(moveUp()));

    moveDownAct = new QAction(QIcon(":/pics/arrow-down.png"),tr("Move Down"), this);
    connect(moveDownAct, SIGNAL(triggered()), this, SLOT(moveDown()));

    sendToTrashAct = new QAction(QIcon(":/pics/knotes_delete.png"),tr("C&onfirm"), this);
    connect(sendToTrashAct, SIGNAL(triggered()), this, SLOT(sendToTrash()));

    autoRenameChildrenAct = new QAction(QIcon(":/pics/edit-rename.png"),tr("&Auto Rename Children"), this);
    connect(autoRenameChildrenAct, SIGNAL(triggered()), this, SLOT(autoRenameChildren()));

    splitAct = new QAction(tr("&Split"), this);
    connect(splitAct, SIGNAL(triggered()), this, SLOT(split()));

    addMultiAct = new QAction(QIcon(":/pics/document-multiple.png"),tr("Add &X Children"), this);
    connect(addMultiAct, SIGNAL(triggered()), this, SLOT(addMulti()));



    addBookNextAct = new QAction(QIcon(":/pics/tree/tree-book.png"),tr("Add &book"), this);
    connect(addBookNextAct, SIGNAL(triggered()), this, SLOT(addBookNext()));

    addActNextAct = new QAction(QIcon(":/pics/tree/tree-act.png"),tr("Add &act"), this);
    connect(addActNextAct, SIGNAL(triggered()), this, SLOT(addActNext()));

    addChapterNextAct = new QAction(QIcon(":/pics/tree/tree-chapter.png"),tr("Add &chapter"), this);
    connect(addChapterNextAct, SIGNAL(triggered()), this, SLOT(addChapterNext()));

    addSceneNextAct = new QAction(QIcon(":/pics/tree/tree-scene.png"),tr("Add &scene"), this);
    connect(addSceneNextAct, SIGNAL(triggered()), this, SLOT(addSceneNext()));

    addSeparatorNextAct = new QAction(QIcon(":/pics/insert-horizontal-rule.png"),tr("Add &separator"), this);
    connect(addSeparatorNextAct, SIGNAL(triggered()), this, SLOT(addSeparatorNext()));

    addChildActAct = new QAction(QIcon(":/pics/tree/tree-act.png"),tr("Add &act"), this);
    connect(addChildActAct, SIGNAL(triggered()), this, SLOT(addChildAct()));

    addChildChapterAct = new QAction(QIcon(":/pics/tree/tree-chapter.png"),tr("Add &chapter"), this);
    connect(addChildChapterAct, SIGNAL(triggered()), this, SLOT(addChildChapter()));

    addChildSceneAct = new QAction(QIcon(":/pics/tree/tree-scene.png"),tr("Add &scene"), this);
    connect(addChildSceneAct, SIGNAL(triggered()), this, SLOT(addChildScene()));

    addChildSeparatorAct = new QAction(QIcon(":/pics/insert-horizontal-rule.png"),tr("Add &separator"), this);
    connect(addChildSeparatorAct, SIGNAL(triggered()), this, SLOT(addChildSeparator()));


    emptyTrashAct = new QAction(QIcon(""),tr("Empty Trash"), this);
    connect(emptyTrashAct, SIGNAL(triggered()), this, SLOT(displayEmptyTrashDialog()));

    displayBadgeAct = new QAction(QIcon(""),tr("Show badges"), this);
    displayBadgeAct->setCheckable(true);
    connect(displayBadgeAct, SIGNAL(toggled(bool)), this, SLOT(displayBadge(bool)));


    setBadgeTextAct = new QAction(QIcon(""),tr("Set badge"), this);
    connect(setBadgeTextAct, SIGNAL(triggered()), this, SLOT(setBadgeText()));

}


void MainTreeContextMenu::displaySplitDialog()
{

    QDialog *splitDialog = new QDialog(base);
    splitDialog->setWindowTitle(tr("Split Dialog"));
    QVBoxLayout *layout = new QVBoxLayout;

    QLabel *titleText = new QLabel(tr("<p>With this tool, you can split a text in several parts."
                                      "For each part, Plume will create a chapter or a scene.</p><br>"));
    QLabel *mainText = new QLabel(tr("<center><b>Each sheet type has different behaviour !</b></center>"
                                     "<br>"
                                     "<p>"
                                     "<blockquote>In a scene sheet : split <b>only</b> into scenes with *** .</blockquote>"
                                     "<blockquote>In a chapter sheet : split into scenes with *** and into chapters with ### .</blockquote>"
                                     "<blockquote>In a book sheet : split into scenes with *** and into chapters with ### .</blockquote></p>"
                                     ));

    QFormLayout *formLayout = new QFormLayout;

    QComboBox *textChoiceBox = new QComboBox;
    QStringList textChoices;
    textChoices << tr("Text") << tr("Synopsys") << tr("Note");
    textChoiceBox->addItems(textChoices);

    connect(textChoiceBox, SIGNAL(currentIndexChanged(int)), this, SLOT(splitChoiceChanged(int)));
    textChoiceBox->setCurrentIndex(0);
    splitChoiceChanged(0);

    formLayout->addRow(tr("Choose the type of document that you want to split :"), textChoiceBox);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Cancel | QDialogButtonBox::Ok);

    connect(buttonBox, SIGNAL(accepted()), splitDialog, SLOT(close()));
    connect(buttonBox, SIGNAL(accepted()), this, SLOT(split()));
    connect(buttonBox, SIGNAL(rejected()), splitDialog, SLOT(close()));

    layout->addWidget(titleText);
    layout->addSpacing(30);
    layout->addWidget(mainText);
    layout->addSpacing(30);
    layout->addLayout(formLayout);
    layout->addWidget(buttonBox);

    splitDialog->setLayout(layout);

    splitDialog->exec();



}

//-------------------------------------------------------------------------------------
void MainTreeContextMenu::splitChoiceChanged(int choice)
{
    switch (choice) {
    case 0:
        splitChoice = "textDoc_";
        break;

    case 1:
        splitChoice = "synDoc_";
        break;
    case 2:
        splitChoice = "noteDoc_";
        break;
    default:
        splitChoice = "textDoc_";
        break;

        //        qDebug() << "splitChoice : " << splitChoice;
    }
}

//-------------------------------------------------------------------------------------



void MainTreeContextMenu::rename()
{

    if(targetedElement.tagName() == "separator")
        return;

    bool ok;
    QString text = QInputDialog::getText(base, tr("Rename sheet"),
                                         tr("Sheet name :"), QLineEdit::Normal,
                                         targetedElement.attribute("name", ""), &ok);
    if (ok && !text.isEmpty()){
        emit actionSignal("rename", m_id, text);
    }
}

void MainTreeContextMenu::addMulti()
{
    bool ok;
    int numSheets = QInputDialog::getInt(base, tr("Add X Children"),
                                         tr("Please enter a number :"),
                                         2, 1, 100, 1, &ok);

    if (!ok || numSheets == 0)
        return;

    emit actionSignal("addMulti", m_id, numSheets);

}

void MainTreeContextMenu::displayBadge(bool value)
{
    QSettings settings;
    settings.setValue("MainTree/badgeDisplayed", value);

    emit actionSignal("displayBadge", m_id, value);
}

void MainTreeContextMenu::setBadgeText()
{
    QString defaultText = targetedElement.attribute("badge", "");

    bool ok;
    QString text = QInputDialog::getText(base, tr("Set badge text"),
                                         tr("Badge :"), QLineEdit::Normal,
                                         defaultText, &ok);
    if (!ok || text == defaultText)
        return;

    emit actionSignal("setBadgeText", m_id, text);
}

void MainTreeContextMenu::setStatus()
{
    int elementStatus = targetedElement.attribute("status", "0").toInt();


    for(int i = 0; i < numberOfStatusActions; ++i ){



        QAction *action = this->findChild<QAction *>("statusAction_" + QString::number(i));
        if(action->objectName() == "statusAction_" + QString::number(elementStatus)
                && action->isChecked() == false){
            continue;
        }
        if(action->isChecked() == true){
            emit actionSignal("setStatus", m_id, QString::number(i));
        }



    }
}

void MainTreeContextMenu::displayEmptyTrashDialog()
{
    int ret = QMessageBox::warning(base, tr("Empty the trash"),
                                   tr("<p>The sheets in the trash "
                                      "will be permanently deleted.</p>\n"
                                      "<br>"
                                      "<p>Do you really want to continue ?</p>"),
                                   QMessageBox::Yes | QMessageBox::Cancel,
                                   QMessageBox::Cancel);

    switch (ret) {
    case QMessageBox::Yes:
        emit actionSignal("emptyTrash", m_id);
        break;
    case QMessageBox::Cancel:
        return;
        break;
    default:
        return;
        break;
    }
}

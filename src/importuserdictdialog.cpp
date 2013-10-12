#include "importuserdictdialog.h"
#include "ui_importuserdictdialog.h"
#include "common/utils.h"
#include "JlCompress.h"

ImportUserDictDialog::ImportUserDictDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ImportUserDictDialog)
{
    ui->setupUi(this);

    projectsListHash = Utils::listAllProjectsInSettingsArray();

    QList<QString> list = projectsListHash.values();



    foreach(QString projectName, list){
        QString path = projectsListHash.key(projectName);
        QListWidgetItem *item = new QListWidgetItem(Utils::projectRealName(path + "/" + projectName + ".plume"), ui->projectsListWidget);
        item->setData(33, path + "/" + projectName + ".plume");
        item->setData(35, projectName);

        item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    }

    connect(ui->projectsListWidget, SIGNAL(itemActivated(QListWidgetItem*)), this, SLOT(populateWordsList(QListWidgetItem*)));
    connect(ui->projectsListWidget, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(populateWordsList(QListWidgetItem*)));

}

ImportUserDictDialog::~ImportUserDictDialog()
{
    delete ui;
}

QStringList ImportUserDictDialog::importedList()
{
    return m_importedList;

}

void ImportUserDictDialog::populateWordsList(QListWidgetItem *projectItem)
{

    m_importedList.clear();
    ui->userDictListWidget->clear();

    QString tempFileName = QDir::tempPath() + "/Plume/importUserDict";
    QString userDictFileName = JlCompress::extractFile(projectItem->data(33).toString(), "dicts/userDict.dict_plume", tempFileName );
    QFile *userDictFile = new QFile(userDictFileName);

    if(!userDictFile->exists()){
        qDebug() << "ERROR ImportUserDictDialog::populateWordsList(...) -> userDictFile";
        return;
    }
    userDictFile->open(QIODevice::ReadOnly | QIODevice::Text);

    QTextStream stream(userDictFile);
    for(QString word = stream.readLine(); !word.isEmpty(); word = stream.readLine()){
        new QListWidgetItem(word, ui->userDictListWidget);
        m_importedList.append(word.toUtf8());
    }


    userDictFile->close();

    userDictFile->remove();

    delete userDictFile;


}

void ImportUserDictDialog::on_browseButton_clicked()
{
    QString projectFileName;
    projectFileName = QFileDialog::getOpenFileName(this,
                                            tr("Open Plume Project Or Exported Dictionary"), QDir::homePath(),
                                                   tr("Plume Project File (*.plume);;Plume Backup Project File (*.plume_backup);;Plume Exported Dictionary (*.plume_dict)"));

    if(projectFileName.isNull())
        return;

    QFile file(projectFileName);
    if(!file.exists())
        return;

    if(Utils::isProjectFromOldSystem(projectFileName))
        projectFileName = Utils::updateProjectIfOldSystem(projectFileName);

QString projectName = projectFileName.split("/").last().remove(".plume");
QListWidgetItem *item = new QListWidgetItem(/*Utils::projectRealName(path + "/" + projectName + ".plume")*/ "", 0);
    item->setData(33, projectFileName);
    item->setData(35, projectName);

    populateWordsList(item);
}

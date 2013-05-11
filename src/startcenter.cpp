#include "startcenter.h"
#include "ui_startcenter.h"

StartCenter::StartCenter(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StartCenter)
{
    ui->setupUi(this);
}

StartCenter::~StartCenter()
{
    delete ui;
}

void StartCenter::postConstructor()
{
    ui->versionabel->setText(QApplication::applicationVersion());

    Utils::sortSettingsArray();

    QSettings settings;


    settings.beginGroup("Manager/projects");

    QStringList groups = settings.childGroups();
    int size = groups.size();
    settings.setValue("size", size);

    settings.endGroup();


    QString name, path, workPath, creationDate, lastModifiedDate;
    settings.beginReadArray("Manager/projects");

    for (int i = 0; i < size; ++i) {
        settings.setArrayIndex(i);

        name = settings.value("name").toString();
        path = settings.value("path").toString();
        workPath = settings.value("workPath").toString();
        lastModifiedDate = settings.value("lastModified").toString();
        creationDate = settings.value("creationDate").toString();

        if(name.isEmpty() && path.isEmpty())
            continue;

        QString time;
        QDateTime *modTime = new QDateTime(QDateTime::fromString(lastModifiedDate, Qt::ISODate));
        if(!modTime->isValid())
            modTime = new QDateTime(QDateTime::fromString(lastModifiedDate, Qt::TextDate));
        if(!modTime->isValid())
            time = lastModifiedDate;
        if(modTime->isValid())
            time = modTime->toString(Qt::TextDate);
        lastModifiedDate = time;


        QDateTime *crTime = new QDateTime(QDateTime::fromString(creationDate, Qt::ISODate));
        if(!crTime->isValid())
            crTime = new QDateTime(QDateTime::fromString(creationDate, Qt::TextDate));
        if(!crTime->isValid())
            time = creationDate;
        if(crTime->isValid())
            time = crTime->toString(Qt::TextDate);
        creationDate = time;



        QListWidgetItem *item = new QListWidgetItem(ui->listWidget);
        item->setFont(QFont("Liberation Serif", 12, 3));
        item->setTextAlignment(Qt::AlignCenter);
        item->setSizeHint(QSize(40,40));


        QFile file(path + "/" + name + ".plume");
        QFile secondFile(workPath + "/" + name + ".plume");
        if(file.exists() && !secondFile.exists()){ // new system
            item->setText(Utils::projectRealName(path + "/" +name + ".plume"));
            if(item->text() == "ERROR real name")
                item->setText(name);
            item->setData(33, path + "/" + name + ".plume");

        }
        else if(!file.exists() && secondFile.exists()){
            item->setText(name);
            item->setData(33, workPath + "/" + name + ".plume");


        }
        item->setData(Qt::UserRole, i);
        item->setData(34, name);
        item->setData(35, modTime->toString(Qt::ISODate));


        item->setToolTip(tr("Path : ") + path + "\n"
                         + tr("Created : ") + creationDate + "\n"
                         + tr("Modified : ") + lastModifiedDate);


        // sort items :

        if(ui->listWidget->row(item) != 0){
            QDateTime previousItemDate;
            QString prevModDate = ui->listWidget->item(0)->data(35).toString();
            previousItemDate = previousItemDate.fromString(prevModDate, Qt::ISODate );
            if(modTime->operator >(previousItemDate))
                ui->listWidget->insertItem(0, ui->listWidget->takeItem(ui->listWidget->row(item)));
        }
    }
    settings.endArray();


    this->setWindowState(Qt::WindowActive);

}

void StartCenter::on_newButton_clicked()
{
    emit newPrjSignal();
    this->close();

}

void StartCenter::on_openButton_clicked()
{

    QString fileName;
    fileName = QFileDialog::getOpenFileName(this,
                                            tr("Open Plume Project"), defaultOpenPath,
                                            tr("Plume Project File (*.plume);;Plume Backup Project File (*.plume_backup)"));

    if(fileName.isNull())
        return;

    QFile file(fileName);
    if(!file.exists())
        return;

    if(Utils::isProjectFromOldSystem(fileName))
        fileName = Utils::updateProjectIfOldSystem(fileName);


    Utils::adaptSettingArrayToProject(fileName);

    hub->startProject(fileName);

    this->close();

}

void StartCenter::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{

    hub->startProject(item->data(33).toString());
    this->close();
}

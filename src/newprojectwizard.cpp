#if QT_VERSION >= 0x050000
#include <QtWidgets>
#endif 
#include <QtGui>   
#include <QDir>


#include "newprojectwizard.h"
//
NewProjectWizard::NewProjectWizard(QWidget *parent)
    : QWizard(parent)
{
    addPage(new IntroPage);
    addPage(new SelectPage);

    setWindowTitle(tr("Project Creator"));

}


void NewProjectWizard::accept()

{
    QString creationDate(QDateTime::currentDateTime().toString(Qt::ISODate));

    QString projectNameFinish = field("projectNameField").toString();
    QString projectDirectoryFinish = field("projectDirectoryField").toString().toUtf8();
    QString workingPath = (projectDirectoryFinish + "/" + projectNameFinish);


    // Creation of the doc using DOM



    QDir path = projectDirectoryFinish + "/" + projectNameFinish;
    path.mkpath(workingPath);
    path.setCurrent(workingPath);

    path.mkdir("objects");
    path.mkdir("text");
    path.mkdir("attend");

    QStringList firstFiles;
    firstFiles << "/text/T1.html"
               << "/text/N1.html"
               << "/text/S1.html"
               << "/text/T2.html"
               << "/text/N2.html"
               << "/text/S2.html"
               << "/text/T3.html"
               << "/text/N3.html"
               << "/text/S3.html" ;

    for (int i = 0; i < firstFiles.size(); ++i){
        QFile firstFile(path.path() + firstFiles.at(i).toLocal8Bit().constData());
        firstFile.open(QFile::WriteOnly | QFile::Text);
        QTextStream out(&firstFile);
        out << "";
        firstFile.close();
        //qDebug() << "File written :" << firstFile.fileName();
    }


    QDomDocument domDoc("plume");


    //    QDomNode xmlNode = domDoc.createProcessingInstruction("xml","version=\"1.0\" encoding=\"UTF-8\"");
    //    domDoc.insertBefore(xmlNode, domDoc.firstChild());


    QFile file(field("projectNameField").toString() + ".plume");
    file.open(QIODevice::ReadWrite | QIODevice::Text);

    QDomElement root = domDoc.createElement("root");
    root.setTagName("plume");
    root.setAttribute("projectName", projectNameFinish);
    root.setAttribute("creationDate", creationDate);
    root.setAttribute("theme","");
    root.setAttribute("version","0.2");
    domDoc.appendChild(root);

    QDomElement bookElem = domDoc.createElement("book");
    bookElem.setAttribute("theme","");
    bookElem.setAttribute("folded","no");
    bookElem.setAttribute("textPath","/text/T1.html");
    bookElem.setAttribute("notePath","/text/N1.html");
    bookElem.setAttribute("synPath","/text/S1.html");
    bookElem.setAttribute("name", projectNameFinish);
    bookElem.setAttribute("number", "1");
    bookElem.setAttribute("textCursorPos", "0");
    bookElem.setAttribute("synCursorPos", "0");
    bookElem.setAttribute("noteCursorPos", "0");

    root.appendChild(bookElem);

    // Here we append the very first chapter element and its attributes, and its Note

    QDomElement chapterElem = domDoc.createElement("chapter");
    chapterElem.setAttribute("theme","");
    chapterElem.setAttribute("folded","no");
    chapterElem.setAttribute("notePath","/text/N2.html");
    chapterElem.setAttribute("textPath","/text/T2.html");
    chapterElem.setAttribute("synPath","/text/S2.html");
    chapterElem.setAttribute("name",tr("Chapter 1"));
    chapterElem.setAttribute("number", "2");
    chapterElem.setAttribute("textCursorPos", "0");
    chapterElem.setAttribute("synCursorPos", "0");
    chapterElem.setAttribute("noteCursorPos", "0");

    bookElem.appendChild(chapterElem);

    //Here we append the very first scene element and its attributes, and its Note


    QDomElement sceneElem = domDoc.createElement("scene");
    sceneElem.setAttribute("theme","");
    sceneElem.setAttribute("notePath","/text/N3.html");
    sceneElem.setAttribute("textPath","/text/T3.html");
    sceneElem.setAttribute("synPath","/text/S3.html");
    sceneElem.setAttribute("name",tr("Scene 1"));
    sceneElem.setAttribute("number", "3");
    sceneElem.setAttribute("textCursorPos", "0");
    sceneElem.setAttribute("synCursorPos", "0");
    sceneElem.setAttribute("noteCursorPos", "0");

    chapterElem.appendChild(sceneElem);










    file.write(domDoc.toByteArray(3));
    file.close();




    //Saving to .conf the new project :

    QSettings settings;

    int size = settings.value("Manager/projects/size", 0).toInt();


    settings.beginWriteArray("Manager/projects");

    settings.setArrayIndex(size);
    settings.setValue("name", projectNameFinish);
    settings.setValue("path", projectDirectoryFinish);
    settings.setValue("workPath", workingPath);
    settings.setValue("lastModified", QDateTime::currentDateTime().toString(Qt::ISODate));
    settings.setValue("creationDate", creationDate);

    settings.endArray();

    emit openProjectSignal(workingPath + "/" + projectNameFinish +".plume");
    QDialog::accept();


}

IntroPage::IntroPage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle(tr("New Project"));
    setSubTitle(tr("Welcome to Plume Creator !"));

    label = new QLabel(tr("This assistant will help you create a new project ."));
    label->setWordWrap(true);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(label);
    setLayout(layout);
}

void IntroPage::initializePage()
{

}


SelectPage::SelectPage(QWidget *parent)
    : QWizardPage(parent)
{


    setTitle(tr("New Project"));
    setSubTitle(tr("Specify the path of your new project. "
                   "Please note you must have writing rights in it. "
                   "Plume Creator will create a basic skeleton."));


    //setPixmap(QWizard::LogoPixmap, QPixmap(":/images/logo1.png"));

    label = new QLabel(tr("Please complete the fields below: "));
    label->setWordWrap(true);

    projectNameLabel = new QLabel(tr("Project name :"));
    projectNameLabelLineEdit = new QLineEdit;
    projectNameLabelLineEdit->setValidator(new QRegExpValidator(QRegExp("[^\x002F\\\\:\x002A\?\x0022<>|]+"), projectNameLabelLineEdit));

    //    directoryLabel = new QLabel(tr("Path : "));
    directoryLabelLineEdit = new QLineEdit;
    QPushButton *directoryButton = new QPushButton(tr("Select path"));
    directoryButton->setMaximumWidth(100);

    connect(directoryButton, SIGNAL(clicked()), this, SLOT(setExistingDirectory()));

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(label, 0, 1);
    //    layout->addWidget(directoryLabel, 1, 0);
    layout->addWidget(projectNameLabel, 1, 0);
    layout->addWidget(projectNameLabelLineEdit, 1, 1);
    layout->addWidget(directoryButton, 2, 0);
    layout->addWidget(directoryLabelLineEdit, 2, 1);
    setLayout(layout);




    registerField("projectNameField*", projectNameLabelLineEdit);
    registerField("projectDirectoryField", directoryLabelLineEdit);
}
void SelectPage::initializePage()
{
    directoryLabelLineEdit->setText(QDir::toNativeSeparators(QDir::homePath()));
}


void SelectPage::setExistingDirectory()
{
    QFileDialog::Options options = QFileDialog::DontResolveSymlinks | QFileDialog::ShowDirsOnly;
    QString directory = QFileDialog::getExistingDirectory(this, tr("QFileDialog::getExistingDirectory()"),
                                                          directoryLabelLineEdit->text(),
                                                          options);
    if (!directory.isEmpty())
        directoryLabelLineEdit->setText(directory);
}

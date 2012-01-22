#include <QtGui/QApplication>
#include <QSettings>


// for translator
#include <QTranslator>
#include <QLocale>
#include <QLibraryInfo>
#include <QTextCodec>
#include <QPixmap>
#include <QSplashScreen>
//#include <QMacStyle>

#include "mainwindow.h"
#include "qtsingleapplication.h"
//
int main(int argc, char *argv[])
{
// QtSingleApplication is here to allow only one instance of the application :
    QtSingleApplication instance(argc, argv);

   QString message=argv[1];

  if (instance.sendMessage(message))
         return 0;




//  QFileInfo appFileInfo(argv[0]);
//  QDir appPath(appFileInfo.path());
//  qDebug() << "log path : " << appPath.path();

//  QFile *logFile = new QFile;
//  logFile->setFileName(QDir::toNativeSeparators(appPath.path() + "/plume.log"));
//  qDebug() << "log file : " << logFile->fileName();
//  logFile->waitForBytesWritten(500);
//  logFile->close();
//  logFile->open(QFile::ReadWrite | QFile::Text | QFile::Append);
//  if(logFile->isWritable())
//  {
//      QTextStream logStream(logFile);

//      for(int i = 0; i < argc; ++i){
//          logStream << QDateTime::currentDateTime().toString() << " :   app arguments : " << argv[i];
//          logStream << "\n";
//      }
//  }
//  logFile->close();





    Q_INIT_RESOURCE(pics);
    Q_INIT_RESOURCE(langs);


    instance.setWindowIcon(QIcon(":/pics/plume-creator.png"));


    // style :

    QApplication::setStyle(new QPlastiqueStyle);
//    QApplication::setStyle(new QMacStyle);

    // splashscreen :

    QPixmap pixmap(":/pics/plume-creator-splash.png");
    QSplashScreen splash(pixmap);
    splash.show();
    instance.processEvents();


    // UTF-8 codec

    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));





    //Names for the QSettings

    QCoreApplication::setOrganizationName( "PlumeSoft" );
    QCoreApplication::setOrganizationDomain( "PlumeSoft.com" );
    QCoreApplication::setApplicationName( "Plume-Creator" );


    // Translator (temporary)

    QString translatorFileName = QLatin1String("qt_");
    translatorFileName += QLocale::system().name();
    QTranslator *translator = new QTranslator(&instance);
    if (translator->load(translatorFileName, QLibraryInfo::location(QLibraryInfo::TranslationsPath)))
        instance.installTranslator(translator);

    QString plumeTranslatorFileName = QLatin1String(":/langs/plume-creator_");
    plumeTranslatorFileName += QLocale::system().name();
    QTranslator *plumeTranslator = new QTranslator(&instance);
    plumeTranslator->load(plumeTranslatorFileName);
    instance.installTranslator(plumeTranslator);


//    qDebug() << "locale : " << "plume-creator_" + QLocale::system().name();


    MainWindow mw;

    // install enventfilter (for Mac)
        QApplication::instance()->installEventFilter(&mw);

    if(argc > 1){
        QFile file(argv[argc - 1]); //pick the last argument
//        if(file.exists() && file.isReadable() && file.isWritable()){
            mw.openExternalProject(&file);
//        }
    }

    splash.finish(&mw);
    mw.show();
    mw.setWindowState(Qt::WindowActive);




//    QObject::connect(&instance, SIGNAL(messageReceived(const QString&)),
//                     &mw, SLOT(handleMessage(const QString&)));




    return instance.exec();
}

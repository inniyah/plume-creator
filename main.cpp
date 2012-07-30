#include <QtGui/QApplication>
#include <QSettings>


// for translator
#include <QTranslator>
#include <QLocale>
#include <QLibraryInfo>
#include <QTextCodec>
#include <QPixmap>
#include <QSplashScreen>

#ifdef Q_OS_WIN32
#include <QWindowsVistaStyle>
#include <QWindowsXPStyle>
#endif
#ifdef Q_OS_MAC
#include <QMacStyle>
#endif


#include "mainwindow.h"
#include "qtsingleapplication.h"
//
int main(int argc, char *argv[])
{
    // QtSingleApplication is here to allow only one instance of the application :
    QtSingleApplication instance(argc, argv);

    instance.setApplicationVersion("0.52");

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


    // style :
    QSettings settings;
QString plumeStyle = settings.value("MainWindow/style", "default").toString();

    if (plumeStyle == "plastique" ){
        QApplication::setStyle(new QPlastiqueStyle);
    }
    else if(plumeStyle == "cleanlooks" ){
        QApplication::setStyle(new QCleanlooksStyle);
    }
#ifdef Q_OS_LINUX
    else if(plumeStyle == "gtk" ){
        QApplication::setStyle(new QGtkStyle);
    }
#endif
#ifdef Q_OS_WIN32
    else if(plumeStyle == "vista" ){
        QApplication::setStyle(new QWindowsVistaStyle);
    }
    else if(plumeStyle == "xp" ){
        QApplication::setStyle(new QWindowsXPStyle);
    }
#endif

#ifdef Q_OS_MAC
    else if (plumeStyle == "osx" ){
        QApplication::setStyle(new QMacStyle);
    }
#endif
    else if (plumeStyle == "default" ){

        QApplication::setStyle(new QPlastiqueStyle);

#ifdef Q_OS_LINUX


        // Checking the current Desktop Environment on Linux :

        QString program = "/bin/pidof";
        QStringList arguments;
        arguments << "ksmserver";
        QProcess *myProcess = new QProcess();
        myProcess->start(program, arguments);
        myProcess->waitForFinished();
        if(myProcess->readAllStandardOutput().size() > 0){
            QApplication::setStyle(new QPlastiqueStyle);
            qDebug() << "Running on KDE";
        }
        myProcess->close();
        arguments.clear();
        arguments << "gnome-session";
        myProcess->start(program, arguments);
        myProcess->waitForFinished();
        if(myProcess->readAllStandardOutput().size() > 0){
            QApplication::setStyle(new QGtkStyle);
            qDebug() << "Running on Gnome";
        }
        myProcess->close();
        arguments.clear();
        arguments << "xfce-mcs-manage";
        myProcess->start(program, arguments);
        myProcess->waitForFinished();
        if(myProcess->readAllStandardOutput().size() > 0){
            QApplication::setStyle(new QGtkStyle);
            qDebug() << "Running on XFCE";
            myProcess->close();
        }
        //    if [ "$(pidof ksmserver)" ]; then
        //       echo "KDE running."
        //       # KDE-specific stuff here
        //    elif [ "$(pidof gnome-session)" ]; then
        //       echo "GNOME running."
        //       # GNOME-specific stuff here
        //    elif [ "$(pidof xfce-mcs-manage)" ]; then
        //       echo "Xfce running."
        //       # Xfce-specific stuff here
        //    fi
#endif






#ifdef Q_OS_MAC
        QApplication::setStyle(new QMacStyle);
#endif

    }

    // Lang menu :


    QString translatorFileName = QLatin1String("qt_");
    QString plumeTranslatorFileName = QLatin1String(":/langs/plume-creator_");



    if (settings.value("MainWindow/firstStart", true).toBool() || settings.value("MainWindow/lang", "none").toString() == "none" ){

        QStringList langs;
        langs << "Français" << "English";
        QStringList langCodes;
        langCodes << "fr_FR" << "en_US";

        bool ok;
        QString selectedLang = QInputDialog::getItem(0, "Language Selector",
                                                     "Please select your language : <br> "
                                                     "Veuillez sélectionner votre langue : ", langs, 0, false, &ok);
        if (ok && !selectedLang.isEmpty()){
            translatorFileName += langCodes.at(langs.indexOf(selectedLang));
            plumeTranslatorFileName += langCodes.at(langs.indexOf(selectedLang));

            QTranslator *translator = new QTranslator(&instance);
            if (translator->load(translatorFileName, QLibraryInfo::location(QLibraryInfo::TranslationsPath)))
                instance.installTranslator(translator);

            QTranslator *plumeTranslator = new QTranslator(&instance);
            plumeTranslator->load(plumeTranslatorFileName);
            instance.installTranslator(plumeTranslator);

            settings.setValue("MainWindow/lang", langCodes.at(langs.indexOf(selectedLang)));
        }
        else{
            translatorFileName += QLocale::system().name();
            QTranslator *translator = new QTranslator(&instance);
            if (translator->load(translatorFileName, QLibraryInfo::location(QLibraryInfo::TranslationsPath)))
                instance.installTranslator(translator);

            plumeTranslatorFileName += QLocale::system().name();
            QTranslator *plumeTranslator = new QTranslator(&instance);
            plumeTranslator->load(plumeTranslatorFileName);
            instance.installTranslator(plumeTranslator);

            settings.setValue("MainWindow/lang", QLocale::system().name());

        }


    }
    else{

        translatorFileName += settings.value("MainWindow/lang", "none").toString();
        QTranslator *translator = new QTranslator(&instance);
        if (translator->load(translatorFileName, QLibraryInfo::location(QLibraryInfo::TranslationsPath)))
            instance.installTranslator(translator);

        plumeTranslatorFileName += settings.value("MainWindow/lang", "none").toString();
        QTranslator *plumeTranslator = new QTranslator(&instance);
        plumeTranslator->load(plumeTranslatorFileName);
        instance.installTranslator(plumeTranslator);

    }
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

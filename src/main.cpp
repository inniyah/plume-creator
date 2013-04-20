#include <QApplication>
#include <QSettings>


// for translator
#include <QTranslator>
#include <QLocale>
#include <QLibraryInfo>
#include <QTextCodec>
#include <QPixmap>
#include <QSplashScreen>
#include <QStyleFactory>

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

    instance.setApplicationVersion(VERSIONSTR);

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

#if QT_VERSION < 0x050000
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8")); // delete for QT5
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8")); // delete for QT5
#endif
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));




    //Names for the QSettings

    QCoreApplication::setOrganizationName( "PlumeSoft" );
    QCoreApplication::setOrganizationDomain( "PlumeSoft.com" );
    QCoreApplication::setApplicationName( "Plume-Creator" );




    // update from native settings to settings in .ini file

    QSettings iniSettings(QSettings::IniFormat, QSettings::UserScope, "PlumeSoft","Plume-Creator" );
    if(iniSettings.value("Settings/isMovedtoIniFile", false).toBool() == false){
        QSettings nativeSettings(QSettings::NativeFormat, QSettings::UserScope, "PlumeSoft","Plume-Creator" );

        QStringList keys = nativeSettings.allKeys();
        for( QStringList::iterator i = keys.begin(); i != keys.end(); i++ )
        {
            iniSettings.setValue( *i, nativeSettings.value( *i ) );
        }

        iniSettings.setValue("Settings/isMovedtoIniFile", true);
    }
    QSettings::setDefaultFormat(QSettings::IniFormat);





    // ----------------------------------------------------
    //move .ini file to app path for portable app (Microsoft Windows only)
    //- not finished !

#ifdef Q_OS_WIN32

    if (iniSettings.value("Settings/isPortableToBeCreated", false).toBool() == true
            && iniSettings.value("Settings/isPortableToBeUnset", false).toBool() == false){

        QSettings::setPath(QSettings::IniFormat, QSettings::UserScope, QCoreApplication::applicationDirPath ());
        iniSettings.setValue("Settings/isPortableToBeCreated", false);
        iniSettings.setValue("Settings/isPortable", true);

        QSettings portableSettings;

        QStringList keys = iniSettings.allKeys();
        for( QStringList::iterator i = keys.begin(); i != keys.end(); i++ )
        {
            portableSettings.setValue( *i, iniSettings.value( *i ) );
        }




    }

    if (iniSettings.value("Settings/isPortable", false).toBool() == true){

        QSettings::setPath(QSettings::IniFormat, QSettings::UserScope, QCoreApplication::applicationDirPath ());
    }

    if (iniSettings.value("Settings/isPortableToBeCreated", false).toBool() == false
            && iniSettings.value("Settings/isPortable", false).toBool() == false
            && iniSettings.value("Settings/isPortableToBeUnset", false).toBool() == true){


        iniSettings.setValue("Settings/isPortableToBeCreated", false);
        iniSettings.setValue("Settings/isPortable", false);
        iniSettings.setValue("Settings/isPortableToBeUnset", false);

        QSettings::setPath(QSettings::IniFormat, QSettings::UserScope, getenv("APPDATA"));
        QSettings localSettings;

        QStringList keys = iniSettings.allKeys();
        for( QStringList::iterator i = keys.begin(); i != keys.end(); i++ )
        {
            localSettings.setValue( *i, iniSettings.value( *i ) );
        }


    }


#endif


    // ---------------------------------------style :

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
        QApplication::setStyle(new QCleanlooksStyle);
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
            QApplication::setStyle(new QCleanlooksStyle);
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
        }
        myProcess->close();
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
    QString plumeTranslatorFileName = QLatin1String(":/translations/plume-creator_");



    if (settings.value("MainWindow/firstStart", true).toBool() || settings.value("MainWindow/lang", "none").toString() == "none" ){

        QStringList langs;
        langs << "Français" << "English" << "Italiano" << "Deutsch";
        QStringList langCodes;
        langCodes << "fr_FR" << "en_US" << "it_IT" << "de_DE";


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

        //        qDebug() << "locale : " << QLocale::system().name();
        //        qDebug() << "plumeTranslatorFileName : " << plumeTranslatorFileName;

    }

    QApplication::setWheelScrollLines(1);



    MainWindow mw;

    // install enventfilter (for Mac)
    QApplication::instance()->installEventFilter(&mw);

    if(argc > 1){
        QTextCodec *codec = QTextCodec::codecForUtfText(argv[argc - 1]);
    QString argument = argv[argc - 1];
#ifdef Q_OS_WIN32
        argument = codec->toUnicode(argv[argc - 1]);
#endif
//        qDebug() << "codec : " << codec->name();

        argument = QDir::fromNativeSeparators(argument);
//        argument = ;
//        qDebug() << "argument : " << argument;
        QFile file(argument); //pick the last argument
//                if(file.exists() && file.isReadable() && file.isWritable()){
//                    qDebug() << "OPENING";
                    mw.openExternalProject(&file);
//                }
    }

    splash.finish(&mw);
    mw.show();
    mw.setWindowState(Qt::WindowActive);
    mw.postConstructor();



    //    QObject::connect(&instance, SIGNAL(messageReceived(const QString&)),
    //                     &mw, SLOT(handleMessage(const QString&)));




    return instance.exec();
}

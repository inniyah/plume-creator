#include <QtGui/QApplication>
#include <QSettings>


// for translator
#include <QTranslator>
#include <QLocale>
#include <QLibraryInfo>
#include <QTextCodec>
#include <QPixmap>
#include <QSplashScreen>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    Q_INIT_RESOURCE(pics);


    app.setWindowIcon(QIcon(":/pics/plume-creator.png"));


    // splashscreen :

    QPixmap pixmap(":/pics/Plume_pen_w.png");
    QSplashScreen splash(pixmap);
    splash.show();
    app.processEvents();


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
        QTranslator *translator = new QTranslator(&app);
        if (translator->load(translatorFileName, QLibraryInfo::location(QLibraryInfo::TranslationsPath)))
            app.installTranslator(translator);

        QString plumeTranslatorFileName = QLatin1String(":/langs/plume-creator_");
        plumeTranslatorFileName += QLocale::system().name();
        QTranslator *plumeTranslator = new QTranslator(&app);
        plumeTranslator->load(plumeTranslatorFileName);
        app.installTranslator(plumeTranslator);


    qDebug() << "locale : " << "plume-creator_" + QLocale::system().name();


    MainWindow w;
    w.show();
    w.setWindowState(Qt::WindowActive);


    splash.finish(&w);
    //    delete splash;


    return app.exec();
}

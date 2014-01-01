#include "spellchecker.h"

#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QDebug>
#include <QDir>
#include <QCoreApplication>


#include "externals/hunspell/hunspell.hxx"
#include "common/utils.h"

SpellChecker::SpellChecker(QObject *parent):
    QObject(parent), m_isActive(false), encodingFix("utf8"), hunspellLaunched(false), m_dictionaryPath("")
{



}


SpellChecker::~SpellChecker()
{
    if(hunspellLaunched)
            delete _hunspell;
}

void SpellChecker::setDict(const QString &dictionaryPath, const QStringList &userDictionary, const QStringList &attendTree_names)
{
    if(hunspellLaunched)
            delete _hunspell;

    m_dictionaryPath = dictionaryPath;

    QString dictFile = dictionaryPath + ".dic";
    QString affixFile = dictionaryPath + ".aff";

    QFile testFile(dictFile);
    if(!testFile.exists())
        this->deactivate();




    QByteArray dictFilePathBA = dictFile.toUtf8();
    QByteArray affixFilePathBA = affixFile.toUtf8();
    _hunspell = new Hunspell(affixFilePathBA.constData(), dictFilePathBA.constData());


    userDict = userDictionary;
    encodingFix = this->testHunspellForEncoding();

    foreach (const QString name, userDict) {
        put_word(name);
    }

    foreach (const QString name, attendTree_names) {
        ignoreWord(name);
    }

    hunspellLaunched = true;

    //test :


//    qWarning() << "encodingFix : " + encodingFix;




    //    m_isActive = true;
}


bool SpellChecker::spell(const QString &word)
{
    //    qWarning() << "word  : " << word;
    bool f_ignore_numbers = false;
    bool f_ignore_uppercase = true;


    int index = -1;
    int length = 0;
    int chars = 1;
    bool is_number = false;
    bool is_uppercase = f_ignore_uppercase;
    bool is_word = false;



    int count = word.length() - 1;
    for (int i = 0; i <= count; ++i) {
        QChar c = word.at(i);
        switch (c.category()) {
        case QChar::Separator_Space:
            return 2;
        case QChar::Number_DecimalDigit:
        case QChar::Number_Letter:
        case QChar::Number_Other:
            is_number = f_ignore_numbers;
            goto Letter;
        case QChar::Letter_Lowercase:
            is_uppercase = false;
Letter:
        case QChar::Letter_Uppercase:
        case QChar::Letter_Titlecase:
        case QChar::Letter_Modifier:
        case QChar::Letter_Other:
        case QChar::Mark_NonSpacing:
        case QChar::Mark_SpacingCombining:
        case QChar::Mark_Enclosing:
            if (index == -1) {
                index = i;
                chars = 1;
                length = 0;
            }
            length += chars;
            chars = 1;
            break;


        case QChar::Punctuation_Other:
            if (c == 0x0027 || c == 0x2019) {
                ++chars;
                break;
            }

        default:
            if (index != -1) {
                is_word = true;
            }
            break;
        }

        if (is_word || (i == count && index != -1)) {
            if (!is_uppercase && !is_number) {

                if(encodingFix == "latin1")
                    return _hunspell->spell(word.toLatin1().constData()) != 0;
                if(encodingFix == "utf8")
                    return _hunspell->spell(word.toUtf8().constData()) != 0;

            }
            index = -1;
            is_word = false;
            is_number = false;
            is_uppercase = f_ignore_uppercase;
        }
    }
    return 2;





}


QStringList SpellChecker::suggest(const QString &word)
{
    if(!m_isActive)
        return QStringList();


    char **suggestWordList;


    int numSuggestions;
    if(encodingFix == "latin1")
        numSuggestions = _hunspell->suggest(&suggestWordList, word.toLatin1().constData());
    if(encodingFix == "utf8")
        numSuggestions = _hunspell->suggest(&suggestWordList, word.toUtf8().constData());
    QStringList suggestions;
    for(int i=0; i < numSuggestions; ++i) {
        QString string;

        if(encodingFix == "latin1")
            string = QString::fromLatin1(suggestWordList[i]);
        if(encodingFix == "utf8")
            string = QString::fromUtf8(suggestWordList[i]);

        suggestions << string;
        free(suggestWordList[i]);
    }
    return suggestions;
}


void SpellChecker::ignoreWord(const QString &word)
{
    put_word(word);
}


void SpellChecker::put_word(const QString &word)
{
    if(word == "")
        return;

    if(encodingFix == "latin1")
        _hunspell->add(word.toLatin1().constData());
    if(encodingFix == "utf8")
        _hunspell->add(word.toUtf8().constData());
}


void SpellChecker::addToUserWordlist(const QString &word)
{
    put_word(word);

    userDict.append(word);



    emit userDictSignal(userDict);

}

bool SpellChecker::isActive()
{

            return m_isActive;



}

void SpellChecker::removeFromUserWordlist(const QString &word)
{

    if(encodingFix == "latin1")
        _hunspell->remove(word.toLatin1().constData());
    if(encodingFix == "utf8")
        _hunspell->remove(word.toUtf8().constData());

    userDict.removeAll(word);



    emit userDictSignal(userDict);

}
bool SpellChecker::isInUserWordlist( QString &word)
{

    return userDict.contains(word);

}
bool SpellChecker::activate()
{

    if(m_dictionaryPath.isEmpty()){


            m_isActive = false;
    return false;
    }

    m_isActive = true;

    return true;
}

void SpellChecker::deactivate()
{
    m_isActive = false;

}



QStringList SpellChecker::dictsPaths()
{

    QStringList list;


#ifdef Q_OS_LINUX

    QDir dir;
    dir.setPath("/usr/share/hunspell/");
    list.append(dir.path());

#endif

    QStringList addonsList = Utils::addonsPathsList();

    foreach (QString string, addonsList) {
        list.append(string + "/dicts/");
    }


    return list;
}


QHash<QString, QString> SpellChecker::dictsList()
{
    QHash<QString, QString> hash;

    QDir dir;
    QStringList filters;
    filters << "*.dic";
    foreach (QString path, SpellChecker::dictsPaths()) {
        dir.setPath(path);


        foreach(QString dict, dir.entryList(filters, QDir::Files)){

            dict.chop(4);
            if(QFile(path + "/" + dict + ".aff").exists())
                hash.insert(path +"/" + dict, dict);

        }
    }


    return hash;
}
//--------------------------------------------------------------------


QString SpellChecker::testHunspellForEncoding()
{
QString encoding(_hunspell->get_dic_encoding());
//qWarning() << "_hunspell->get_dic_encoding() : " + encoding;


    if(encoding == "ISO8859-1")
        return "latin1";
    else
        return "utf8";

    return "utf8";


}

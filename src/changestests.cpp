#include "changestests.h"



//--------------------------------------------------------------------------------

bool ChangesTests::test(int oldInt, int newInt)
{
    if(oldInt != newInt)
        return  false;
    else
        return  true;
}
//--------------------------------------------------------------------------------

bool ChangesTests::test(QString oldString, QString newString)
{
    if(oldString != newString)
        return  false;
    else
        return  true;
}

//--------------------------------------------------------------------------------

bool ChangesTests::test(QHash<QTextDocument *, QFile *> oldHash, QHash<QTextDocument *, QFile *> newHash)
{
    if(oldHash != newHash)
        return  false;
    else
        return  true;
}

//--------------------------------------------------------------------------------

bool ChangesTests::test(QHash<QTextDocument *,int> oldHash, QHash<QTextDocument *, int> newHash)
{
    if(oldHash != newHash)
        return  false;
    else
        return  true;
}

//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------

bool ChangesTests::test(QHash<MainTextDocument *, QFile *> oldHash, QHash<MainTextDocument *, QFile *> newHash)
{
    if(oldHash != newHash)
        return  false;
    else
        return  true;
}

//--------------------------------------------------------------------------------

bool ChangesTests::test(QHash<MainTextDocument *,int> oldHash, QHash<MainTextDocument *, int> newHash)
{
    if(oldHash != newHash)
        return  false;
    else
        return  true;
}

//--------------------------------------------------------------------------------
bool  ChangesTests::test(QHash<int, QDomElement> oldHash, QHash<int, QDomElement> newHash)
{
    if(oldHash != newHash)
        return  false;
    else
        return  true;
}

#include "wordcountthread.h"

WordCountThread::WordCountThread(QObject *parent)
{



}

//-----------------------------------------------------------------------------------------------------------
void WordCountThread::setDocs(QHash<QTextDocument *, QFile *> fileForDocs)
{
    QList<QTextDocument *> docList = fileForDocs.keys();

    textDocList.clear();

    while(!docList.isEmpty()){
        QTextDocument *doc = docList.takeFirst();
        if(doc->objectName().left(8) == "textDoc_"){
            textDocList.append(doc);
        }
    }


}

//-----------------------------------------------------------------------------------------------------------

void WordCountThread::setDom(QDomDocument domDoc)
{
    domDocument = domDoc;
}

//-----------------------------------------------------------------------------------------------------------
void WordCountThread::setCurrentNumber(int number)
{
    currNumber = number;
}
//-----------------------------------------------------------------------------------------------------------
int WordCountThread::countWords(QTextDocument *doc)
{
    QString docText = doc->toPlainText();
    QStringList wordsList;

    wordsList = docText.split(" ", QString::SkipEmptyParts);

    int finalWordCount = wordsList.size();
    return finalWordCount;
}
//-----------------------------------------------------------------------------------------------------------
void WordCountThread::run()
{



    QHash<int,QTextDocument *> docForNumber;

    while(!textDocList.isEmpty()){

        int number = textDocList.first()->objectName().mid( textDocList.first()->objectName().indexOf("_") + 1).toInt();
        docForNumber.insert(number,textDocList.takeFirst());

    }



    // pinpoint current domElement :
    QString string;
    QDomNodeList bookNodeList = domDocument.elementsByTagName("book");
    QDomNodeList chapterNodeList = domDocument.elementsByTagName("chapter");
    QDomNodeList sceneNodeList = domDocument.elementsByTagName("scene");
    QDomNode currNode;

    int i = 0;
    while(i < bookNodeList.length()){
        if(bookNodeList.at(i).toElement().attribute("number") == string.setNum(currNumber))
            currNode = bookNodeList.at(i);
        ++i;
    }
    i = 0;
    while(i < chapterNodeList.length()){
        if(chapterNodeList.at(i).toElement().attribute("number") == string.setNum(currNumber))
            currNode = chapterNodeList.at(i);
        ++i;
    }
    i = 0;
    while(i < sceneNodeList.length()){
        if(sceneNodeList.at(i).toElement().attribute("number") == string.setNum(currNumber))
            currNode = sceneNodeList.at(i);
        ++i;
    }


    // gather domElement numbers and calculate current chapter wordcount :

    if(currNode.toElement().tagName() == "scene"){
        currNode = currNode.parentNode();
    }

    QDomNodeList childNodeList;
    QList<int> nodesNumberList;
    if(currNode.toElement().tagName() == "chapter"){

        nodesNumberList.append(currNode.toElement().attribute("number").toInt());

        childNodeList = currNode.childNodes();
        i = 0;
        while(i < childNodeList.length()){
            if(childNodeList.at(i).toElement().tagName() == "scene")
                nodesNumberList.append(childNodeList.at(i).toElement().attribute("number").toInt());
            ++i;
        }
        QString string;
        qDebug() << "nodesNumberList : " + string.setNum(nodesNumberList.size());
    }

    int chapterWordCount = 0;
    i = 0;
    while(i < nodesNumberList.size()){
        chapterWordCount += countWords(docForNumber.value(nodesNumberList.at(i)));
        QString string;
        qDebug() << "chapterWordCount : " + string.setNum(chapterWordCount);
        ++i;
    }

    emit chapterWordCountSignal(chapterWordCount);


    // gather domElement numbers and calculate current book wordcount :
    QDomNodeList prjNodeList;
    nodesNumberList.clear();

    currNode = currNode.parentNode();

    if(currNode.toElement().tagName() == "book"){

        nodesNumberList.append(currNode.toElement().attribute("number").toInt());

        prjNodeList = currNode.childNodes();

                i = 0;
                while(i < prjNodeList.length()){
                    currNode = prjNodeList.at(i);
                    nodesNumberList.append(currNode.toElement().attribute("number").toInt());

                    QDomNodeList prjChildNodeList;
                    prjChildNodeList = currNode.childNodes();
                    int j = 0;
                    while(j < prjChildNodeList.length()){
                        if(prjChildNodeList.at(j).toElement().tagName() == "scene")
                            nodesNumberList.append(prjChildNodeList.at(j).toElement().attribute("number").toInt());
                        ++j;
                    }
                    ++i;
                }

            }

            int bookWordCount = 0;
            i = 0;
            while(i < nodesNumberList.size()){
                bookWordCount += countWords(docForNumber.value(nodesNumberList.at(i)));
                QString string;
                qDebug() << "bookWordCount : " + string.setNum(bookWordCount);
                ++i;
            }

            emit bookWordCountSignal(bookWordCount);



            // calculate current book wordcount (all doc) :
            int prjWordCount = 0;
            QHashIterator<int,QTextDocument *> e(docForNumber);
            while (e.hasNext()) {
                e.next();
                prjWordCount += countWords(e.value());
            }
            emit projectWordCountSignal(prjWordCount);

        }

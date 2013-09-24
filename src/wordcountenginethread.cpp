#include "wordcountenginethread.h"

WordCountEngineThread::WordCountEngineThread(QObject *parent) :
    QThread(parent), currentSheetNumber(-999), m_projectWordCount(0)
{
}




void WordCountEngineThread::resetTree()
{



    domElementForNumber.clear();



    QDomDocument treeDomDoc = m_domDoc;
    QDomElement root = treeDomDoc.documentElement();

    QDomNode m = root.firstChild();
    while(!m.isNull()) { // book level
        QDomElement e = m.toElement(); // try to convert the node to an element.
        if(!e.isNull()) {
            domElementForNumber.insert(e.attribute("number", "0").toInt(), e);

            QDomNode n = m.firstChild();
            while(!n.isNull()) { // act level
                QDomElement f = n.toElement(); // try to convert the node to an element.
                if(!f.isNull()) {
                    domElementForNumber.insert(f.attribute("number", "0").toInt(), f);

                    QDomNode o = n.firstChild();
                    while(!o.isNull()) { // chapter level
                        QDomElement g = o.toElement(); // try to convert the node to an element.
                        if(!g.isNull()) {
                            domElementForNumber.insert(g.attribute("number", "0").toInt(), g);


                            QDomNode p = o.firstChild();
                            while(!p.isNull()) { // scene level
                                QDomElement q = p.toElement(); // try to convert the node to an element.
                                if(!q.isNull()) {
                                    domElementForNumber.insert(q.attribute("number", "0").toInt(), q);

                                }
                                p = p.nextSibling();

                            }
                        }
                        o = o.nextSibling();

                    }

                }
                n = n.nextSibling();
            }
        }
        m = m.nextSibling();
    }



    treeString = m_domDoc.toString();



}




QList<QDomElement> WordCountEngineThread::allChildNodes(QDomElement node)
{
    QList<QDomElement> list;

    list.append(node.toElement());

    QDomNode m = node.firstChild();
    while(!m.isNull()) { // book level
        QDomElement e = m.toElement(); // try to convert the node to an element.
        if(!e.isNull()) {
            list.append(e);
            QDomNode n = m.firstChild();
            while(!n.isNull()) { // act level
                QDomElement f = n.toElement(); // try to convert the node to an element.
                if(!f.isNull()) {
                    list.append(f);
                    QDomNode o = n.firstChild();
                    while(!o.isNull()) { // chapter level
                        QDomElement g = o.toElement(); // try to convert the node to an element.
                        if(!g.isNull()) {
                            list.append(g);
                            QDomNode p = o.firstChild();
                            while(!p.isNull()) { // scene level
                                QDomElement q = p.toElement(); // try to convert the node to an element.
                                if(!q.isNull()) {
                                    list.append(q);

                                }
                                p = p.nextSibling();

                            }
                        }
                        o = o.nextSibling();

                    }

                }
                n = n.nextSibling();
            }
        }
        m = m.nextSibling();
    }


    return list;
}


int WordCountEngineThread::size(QDomElement element)
{

    QList<QDomElement> list = this->allChildNodes(element);
    int count = 0;
    foreach(const QDomElement &element, list){

        QList<MainTextDocument *> sheetsList = m_numForDoc.keys(element.attribute("number").toInt());
        MainTextDocument *textDoc;
        while(!sheetsList.isEmpty()){
            MainTextDocument *doc = sheetsList.takeFirst();
            if(doc->docType() == "text"){
                textDoc = doc;
                count += textDoc->wordCount();
            }
        }

    }

    return count;

}

//-----------------------------------------------------------

int WordCountEngineThread::parentSize(QDomElement element)
{

    QDomElement parentElement = element.parentNode().toElement();

    return this->size(parentElement);
}

//-----------------------------------------------------------

void WordCountEngineThread::run()
{
    if(currentSheetNumber == -999) // security
        return;



    if(treeString != m_domDoc.toString())
        this->resetTree();


    currentElement = domElementForNumber.value(currentSheetNumber);





    MainTextDocument *currentSheet;
    QList<MainTextDocument *> currentSheetsList = m_numForDoc.keys(currentSheetNumber);

    if(!currentSheetsList.isEmpty())
        if(currentSheetNumber != currentSheetsList.first()->idNumber())
            return;
    if(currentSheetsList.isEmpty())
        return;
    while(!currentSheetsList.isEmpty()){
        MainTextDocument *doc = currentSheetsList.takeFirst();
        if(doc->docType() == "text")
            currentSheet = doc;
    }





    emit currentSheetWordCount(currentSheet->wordCount());


    if(currentElement.tagName() == "book"){

        m_projectWordCount = this->parentSize(currentElement);

        emit projectWordCount(m_projectWordCount);
        emit bookWordCount(this->size(currentElement));
        emit actWordCount(-1);
        emit chapterWordCount(-1);
        emit sceneWordCount(-1);
    }
    else if(currentElement.tagName() == "act"){

        m_projectWordCount = this->parentSize(parentElement(currentElement));
        emit projectWordCount(m_projectWordCount);
        emit bookWordCount(this->parentSize(currentElement));
        emit actWordCount(this->size(currentElement));
        emit chapterWordCount(-1);
        emit sceneWordCount(-1);
    }
    else if(currentElement.tagName() == "chapter"){

        if(parentElement(currentElement).tagName() == "book"){
        m_projectWordCount = this->parentSize(parentElement(currentElement));
        emit projectWordCount(m_projectWordCount);
        emit bookWordCount(this->parentSize(currentElement));
        emit actWordCount(-1);
        }
        else if(parentElement(currentElement).tagName() == "act"){
            m_projectWordCount = this->parentSize(parentElement(parentElement(currentElement)));
            emit projectWordCount(m_projectWordCount);
            emit bookWordCount(this->parentSize(parentElement(currentElement)));
        emit actWordCount(this->parentSize(currentElement));
        }
        emit chapterWordCount(this->size(currentElement));
        emit sceneWordCount(-1);

    }
    else if(currentElement.tagName() == "scene"){

        if(parentElement(parentElement(currentElement)).tagName() == "book"){
        m_projectWordCount = this->parentSize(parentElement(parentElement(currentElement)));

        emit projectWordCount(m_projectWordCount);
        emit bookWordCount(this->parentSize(parentElement(currentElement)));
        emit actWordCount(-1);
        }
        else  if(parentElement(parentElement(currentElement)).tagName() == "act"){
            m_projectWordCount = this->parentSize(parentElement(parentElement(parentElement(currentElement))));
            emit projectWordCount(m_projectWordCount);

            emit bookWordCount(this->parentSize(parentElement(parentElement(currentElement))));
            emit actWordCount(this->parentSize(parentElement(currentElement)));
        }

        emit chapterWordCount(this->parentSize(currentElement));
        emit sceneWordCount(-1);

    }
    else if(currentElement.tagName() == "separator"){

        if(parentElement(parentElement(currentElement)).tagName() == "book"){
        m_projectWordCount = this->parentSize(parentElement(parentElement(currentElement)));

        emit projectWordCount(m_projectWordCount);
        emit bookWordCount(this->parentSize(parentElement(currentElement)));
        emit actWordCount(-1);
        }
        else  if(parentElement(parentElement(currentElement)).tagName() == "act"){
            m_projectWordCount = this->parentSize(parentElement(parentElement(parentElement(currentElement))));
            emit projectWordCount(m_projectWordCount);

            emit bookWordCount(this->parentSize(parentElement(parentElement(currentElement))));
            emit actWordCount(this->parentSize(parentElement(currentElement)));
        }

        emit chapterWordCount(this->parentSize(currentElement));
        emit sceneWordCount(-1);

    }





}



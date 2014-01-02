#include "maintree.h"
//
MainTree::MainTree(QObject *parent) :
    QObject(parent)
{

    mainTreeAbsModel = new MainTreeAbstractModel(this);


}
bool MainTree::startTree()
{


    // load mainTree model :

    mainTreeAbsModel->setDomDocument(hub->mainTreeDomDoc());
    mainTreeAbsModel->setHub(hub);
    connect(hub, SIGNAL(mainTree_numForDocHashChanged(QHash<MainTextDocument*,int>)), mainTreeAbsModel, SLOT(mtoO_setNumForDoc(QHash<MainTextDocument *, int>)));
    mainTreeAbsModel->mtoO_setNumForDoc(hub->mainTree_numForDocHash());

    mainTreeAbsModel->resetAbsModel();

    connect(mainTreeAbsModel, SIGNAL(textAndNoteSignal(int,QString)), this, SIGNAL(textAndNoteSignal(int,QString)), Qt::UniqueConnection);
    connect(hub, SIGNAL(textAndNoteSignal(int,QString)), SIGNAL(textAndNoteSignal(int,QString)), Qt::UniqueConnection);


    return true;
}

//---------------------------------------------------------------------------------------

MainTreeAbstractModel *MainTree::mainTreeAbstractModel()
{
    return mainTreeAbsModel;
}


//---------------------------------------------------------------------------------------

void MainTree::closeTree()
{

    QDomDocument emptyDomDoc;

    this->mainTreeAbstractModel()->setDomDocument(emptyDomDoc);
    this->mainTreeAbstractModel()->resetAbsModel();

    this->mainTreeAbstractModel()->modifyDataForOpenedSheetMarker(0);
}

//---------------------------------------------------------------------------------------

void MainTree::createNewStructure(QHash<QString, int> newStructureHash)
{
   this->mainTreeAbstractModel()->createNewStructure(newStructureHash);
}


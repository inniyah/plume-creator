#include "overviewbase.h"
#include "ui_overviewbase.h"

OverviewBase::OverviewBase(QWidget *parent, Hub *hub, int id) :
    QWidget(parent),
    ui(new Ui::OverviewBase)
{
    ui->setupUi(this);


    setWindowTitle(tr("Overview"));
    setFocusPolicy(Qt::WheelFocus);


    QSettings settings;

    int width = settings.value("Settings/TextArea/textWidth").toInt();
    this->setMinimumWidth(width + 100);






    this->setAttribute(Qt::WA_DeleteOnClose);

    this->show();



    if(id == -1)
        return;

    QDomElement targetedElement = hub->project()->mainTree_domElementForNumberHash().value(id);

    QList<QDomElement> childElementsList = Utils::allChildElements(targetedElement);

    //    while(!childElementsList.isEmpty())
    //        ui->listWidget->addItem(childElementsList.takeFirst().attribute("name", "error"));

    qDomElementForTextCellHash.clear();




    QVBoxLayout *vLayout = new QVBoxLayout(this);
    vLayout->setSpacing(0);
    vLayout->setContentsMargins(2,0,2,0);
    ui->scrollAreaWidgetContents->setLayout(vLayout);

    if(!childElementsList.isEmpty())
        for(int i = 0; i < childElementsList.count(); ++i) {
            QDomElement element = childElementsList.at(i);
            TextCell *cell = new TextCell(this);

            MainTextDocument *textDoc = hub->project()->findChild<MainTextDocument *>("textDoc_" + element.attribute("number"));
         if(textDoc != 0){
             MainTextDocument *tempText = new MainTextDocument(this, hub->spellChecker());
            tempText->setHtml(textDoc->toHtml());
            cell->setTextDocument(tempText );
        }
            cell->setTitle(element.attribute("name", ""));
            cell->setId(element.attribute("number").toInt());
            cell->setType(element.tagName());
            vLayout->addWidget(cell);


            connect(cell, SIGNAL(openSheet(int)), hub, SLOT(openSheet(int)), Qt::UniqueConnection);

            qDomElementForTextCellHash.insert(cell, element);
        }


}

OverviewBase::~OverviewBase()
{
    delete ui;
}

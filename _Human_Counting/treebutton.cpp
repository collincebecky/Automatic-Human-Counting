#include "treebutton.h"

#include <QDebug>
QTreeButton::QTreeButton(QString parent_key)
{

    //->Connecting the button signals with corresponding slots



    //connect(this, SIGNAL(clicked()), this, SLOT(MeClicked()));
    //connect(topLevelButton, SIGNAL(clicked(bool)), this, SLOT(onTopLevelButtonClicked(bool)));

    this->setStyleSheet("border-radius:2px;\
                        Text-align:left;\
                        margin-bottom:5px;\
                        color:#000;\
                        background-color:white;\
                        ");
   this->setMaximumWidth(170);

   this->setText(parent_key);

   this->setMinimumHeight(20);
}

QTreeButton::QTreeButton(QString name_source,MyLabel * front_screen):
    screen_label(front_screen)
{

    //->Connecting the button signals with corresponding slots

    QStringList name_list = name_source.split( "=" );
    QString name = name_list.value(1);

    //-> get the source and convert it to standard string

    source = name_list.value(2);
    qDebug()<<"SOURCES -----------"<<source<<endl;
    //connect(this, SIGNAL(clicked()), this, SLOT(MeClicked()));

    connect(this, SIGNAL (released()),this, SLOT (MeClicked()));
    //connect(topLevelButton, SIGNAL(clicked(bool)), this, SLOT(onTopLevelButtonClicked(bool)));

    this->setStyleSheet("border-radius:2px;\
                        Text-align:left;\
                        margin-bottom:5px;\
                        color:#000;\
                        background-color:white;");

    QPixmap pixmap("./Resources/covers/cctv2.png");
    QIcon ButtonIcon(pixmap);
    this->setIcon(ButtonIcon);
    this->setIconSize(QSize(20,22));



   this->setMaximumWidth(150);
   this->setText(name);

   this->setMinimumHeight(30);
}
void QTreeButton::MeClicked(){

    screen_label->set_source(source);
    qDebug()<<"clicked ...................."<<source<<endl;



}

#include "mylabel.h"
#include <QDebug>
using namespace std;
MyLabel::MyLabel(QWidget *parent):QLabel(parent)
{
    deamon = new Mythread();

    QObject::connect(deamon, SIGNAL(returntomain(QImage)),
    this, SLOT(updatewithframe(QImage)));

    QPixmap map("./Resources/covers/lost.png");

    this->setPixmap(map);
}
void MyLabel::startdeamon(string src,QSqlDatabase &db,int id){

    deamon->strsource=src;
    deamon->Db=db;
    deamon->ID=id;

   if (deamon->connectToCamera()){
        //-> Timeout for when to update database
       QObject::connect(deamon->timer, SIGNAL(timeout()), deamon, SLOT(_update()));
       deamon->timer->start(100000);

       deamon->start();

    }

}
void  MyLabel::startdeamon(int src){

    deamon->intsource=src;
   if (deamon->connectToCamera()){

        deamon->start();

    }

}

void  MyLabel::set_source(QString source){
    deamon->update_data=false;
    deamon->strsource=source.toStdString();

    qDebug()<<"in set source ---------------------"<<endl;
    //cout<<source<<endl;
     qDebug()<<"Mapped ............"<<source<<endl;
    if (deamon->connectToCamera()){

         deamon->start();

     }

}

void MyLabel::updatewithframe(const QImage &image){

    this->setPixmap(QPixmap::fromImage(image).scaled(this->size(),
    Qt::KeepAspectRatio, Qt::FastTransformation));

}



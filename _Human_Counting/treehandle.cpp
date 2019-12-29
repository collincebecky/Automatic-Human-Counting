#include "treehandle.h"
#include <QMultiMap>
#include "handling_db.h"



TreeHandle::TreeHandle()
{
}
void TreeHandle::populate_tree(QTreeWidget * tree,MyLabel * front_screen,QSqlDatabase &db){

    //db::DBhandle                    Handle_db;


    //->get relevant sources from map then  populate accordingly

    auto  map= return_sources(db);


    qDebug()<<"map for return resources "<<map<<endl;


    //->Loop through each sources

    foreach (QString key, map.uniqueKeys()) {
        QTreeButton *topLevelButton = new QTreeButton(key);
        QTreeWidgetItem *topLevelItem = new QTreeWidgetItem();
        tree->addTopLevelItem(topLevelItem);
        tree->setItemWidget(topLevelItem, 0, topLevelButton);

        foreach (QString value, map.values(key)) {

            QTreeButton *childButton = new QTreeButton(value,front_screen);
            QTreeWidgetItem *childItem = new QTreeWidgetItem();
            topLevelItem ->addChild(childItem);
            tree->setItemWidget(childItem, 0, childButton);

        }

  }

}

QMultiMap<QString,QString> TreeHandle::return_sources(QSqlDatabase &db){


    printf("connection to resources successfull");
    QSqlQuery *query= new QSqlQuery(db);

    QString queryString("SELECT * FROM Cameras");
    printf("selection successfull");
    //int idRegion = query.record().indexOf("Region");
    //int idCamera = query.record().indexOf("camera");
    query->exec(queryString);

        //QSqlQuery q = db.exec("SELECT * FROM Table");
      //if (q.last()) {
    //     do {
  //         // Do something with row...
//      } while (q.previous());
//      }



    while (query->next())
     {
        QSqlRecord data = query->record();

        //->get data from every column in current row
        QString Region =data.value(0).toString();
        QString Camera =data.value(1).toString();
        QString source =data.value(2).toString();



        //->concatinate camera and source

        QString camera_source=Camera+":"+source;

         qDebug()<<"camera_source "<<camera_source;


        mapped.insert(Region,camera_source);
     }
    qDebug()<<"Mapped ............"<<mapped<<endl;
    return mapped;
}

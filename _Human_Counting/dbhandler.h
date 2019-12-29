#ifndef DBHANDLER_H
#define DBHANDLER_H
#include <QMultiMap>
#include <QString>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QSqlRecord>
#include <QDebug>

//#include <stdio.h>


class DBhandle
{

private:
    QSqlDatabase m_db;
    QMultiMap<QString,QString> mapped;
public:
    DBhandle(){}
    void close_db(){
        m_db.close();
    }
    void open_db(const QString& path){

        m_db=QSqlDatabase::addDatabase("QSQLITE","count_db");
        m_db.setDatabaseName(path);

        if (!m_db.open())
        {
        qDebug() << "Error: connection with database fail";
        }
        else
        {
        qDebug() << "Database: connection ok";
        }

    }
    DBhandle(){
    m_db=OpenDB("QSQLITE","count_db");
    
    
    
    
    
    
                
                QSqlDatabase::addDatabase("QSQLITE","count_db");
    m_db.setDatabaseName(path);

    if (!m_db.open())
    {
    qDebug() << "Error: connection with database fail";
    }
    else
    {
    qDebug() << "Database: connection ok";
    }
    //return_sources();
    }


    QMultiMap<QString,QString> return_sources(){
    //void DBhandle::return_sources(){

        printf("connection to resources successfull");
        QSqlQuery query;

        QString queryString("SELECT * FROM Cameras");
        printf("selection successfull");
        //int idRegion = query.record().indexOf("Region");
        //int idCamera = query.record().indexOf("camera");
        query.exec(queryString);

            //QSqlQuery q = db.exec("SELECT * FROM Table");
          //if (q.last()) {
        //     do {
      //         // Do something with row...
    //      } while (q.previous());
 //      }



        while (query.next())
         {
            QSqlRecord data = query.record();

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
 };



#endif // DBHANDLER_H

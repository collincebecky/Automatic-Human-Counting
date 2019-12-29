#ifndef HANDLING_DB_H
#define HANDLING_DB_H

#include <QMultiMap>
#include <QString>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QSqlRecord>
#include <QDebug>

namespace db {

class DBhandle
{

private:
    QSqlDatabase m_db,db;
    QMultiMap<QString,QString> mapped;
public:
    DBhandle(){
        //m_db = open_db("QSQLITE","count_db");
    }
    void close_db(){
        m_db.close();
    }
    static QSqlDatabase open_db(const QString & driver,const QString& name){

        QSqlDatabase db;
        if (QSqlDatabase::contains())
        {
        db = QSqlDatabase::database( name );
        }
        else
        {

           QString path="/home/centurine/BRANCE/_Human_Counting/Resources/Database/Human_count.db";
           db = QSqlDatabase::addDatabase(driver,name);
           db.setDatabaseName(path);

        }

        if (!db.isValid())
        {
        qDebug()<<">>>>>>>>>database is invalid ..."<<endl;
        }
        if (!db.open())
        {
            qDebug()<<">>>>>>>>>database open unsuccessful"<<endl;
        // Log error (last error:
        }
    return db;
    }



    QMultiMap<QString,QString> return_sources(){


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

}
#endif // HANDLING_DB_H

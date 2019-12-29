#ifndef TREEHANDLE_H
#define TREEHANDLE_H

#include "treebutton.h"
//#include "dbhandler.h"
#include <QTreeWidget>
#include <QPushButton>
#include <QList>
#include <QString>
#include <QMultiMap>

class TreeHandle
{
public:
    TreeHandle();
    QMultiMap<QString,QString> return_sources(QSqlDatabase &db);
    void populate_tree(QTreeWidget * tree,MyLabel * front_screen,QSqlDatabase &db);
   // DBhandle                    *Handle_db;

    //QTreeButton *item(QString name, QTreeButton* parent=0);
private:
    QMultiMap<QString,QString> mapped;
};

#endif // TREEHANDLE_H

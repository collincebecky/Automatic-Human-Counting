#ifndef MYLABEL_H
#define MYLABEL_H
#include <mythread.h>

#include <QLabel>

class MyLabel:public QLabel
{
    Q_OBJECT
public:
    MyLabel(QWidget *parent);

    void                startdeamon(std::string src,QSqlDatabase &db,int id);
    void                startdeamon(int src);

    void                set_source(QString source);
    Mythread            *deamon;



private slots:
    void                updatewithframe(const QImage & image);
signals:
    void                returntomain(const QImage & image);
};

#endif // MYLABEL_H

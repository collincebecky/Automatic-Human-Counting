#ifndef TREEBUTTON_H
#define TREEBUTTON_H

#include <QPushButton>
#include <QTreeWidgetItem>

#include "mylabel.h"


class QTreeButton:public QPushButton
{
    Q_OBJECT

public:
    QTreeButton(QString parent_key);
    QTreeButton(QString name_source,MyLabel * front_screen);

    QString          source;
    MyLabel         *screen_label;

private slots:

    void MeClicked();

signals:
    void Clicked(bool True);

};

#endif // TREEBUTTON_H

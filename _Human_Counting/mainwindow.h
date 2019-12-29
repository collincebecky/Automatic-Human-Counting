#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QTimer>
#include <QString>
#include <QMultiMap>
#include <QSqlError>
#include <QMainWindow>
#include <QTreeWidgetItem>
#include <QBarCategoryAxis>
#include <QMessageBox>
#include <QStringList>
#include <QRegExp>
#include <QVector>

#include <QtCharts/QValueAxis>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QSplineSeries>
#include <QtCharts/QCategoryAxis>


#include "handling_db.h"
#include "treebutton.h"
#include "mylabel.h"

QT_CHARTS_USE_NAMESPACE

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    VideoCapture                      cap;
    QTimer                           *timer;
    cv::Mat                           q_image;
    int                               counter=0;
    bool                              add_btn_state;

    bool                              filterstate;
    QVector<QStringList>               list_of_lists;
    QStringList                        strings;
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    //TreeHandle *handle_tree;
    QSqlDatabase db_;
    QMultiMap<QString,QString> mapped;
    static const QImage mat8ToImage(const cv::Mat &mat);
    void populate_tree();
    void return_sources();
    void populate_cam_table();
    void populate_camera_table();
    //void update_statistics();
    void render_chart();

    void auto_add_display();

private slots:
    //void on_lineEdit_textChanged(const QString &arg1);

    void on_pushButton_clicked();

    void on_doneBtn_released();
    void edit_row(int row,int column);

    void on_cam_config_btn_2_released();

    void on_del_btn_2_released();

    void on_testBtn_2_released();
    void update_test_label();
    void update_statistics();

    void on_search_cameras_textChanged(const QString &arg1);

signals:
    //void set_tester_image(const QImage & image);


private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H

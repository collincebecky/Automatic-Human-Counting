#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QMap>
#include <QLabel>
#include <QImage>
#include <QObject>
#include <QThread>
#include <QVector>
#include <QDebug>
#include <QTimer>
#include <QTime>
#include <QSqlQuery>

#include <dlib/image_processing.h>
#include <dlib/gui_widgets.h>
#include <dlib/image_io.h>
#include <dlib/dir_nav.h>

#include <opencv2/dnn.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core/utils/trace.hpp>

#include <pd/people_count_test.hpp>

#include <vector>


//#include <trackooi.h>
#include <detectpeople.h>
//#include <dbhandler.h>
//#include <handl0_db.h>
using namespace cv;
using namespace std;
using namespace dlib;

class Mythread:public QThread
{
   Q_OBJECT

public:
    Mythread();
    DetectPeople                       *detect_people;
    std::vector<dlib::correlation_tracker>   trackers;
    Mat                               frame;
    QMap<int,int>                       tracker_map;

    const QImage                      mat8ToImage(const cv::Mat &mat);

    string                             status="";
    string                             source="";
    string                             strsource;

    bool                              connectToCamera();
    bool                              camOpenResult;
    bool                              isOpen;
    bool                              isSend=false;
    bool                              update_data;


    int                                totalframes=0;
    int                                Maxframes=60;
    int                                IN =0;
    int                                OUT=0;
    int                                people_in=0;
    int                                people_out=0;
    int                                ID;
    int                                intsource;

    const int                           height=416;//500;
    const int                           width=416;//400;

    matrix<rgb_pixel>                   dlib_frame;
    VideoCapture                       cap;

    people_count_test                  *Count;
    QVector<int>                       _vector;
    QTimer                           *timer;

    QSqlQuery                         query;
    QSqlDatabase                      Db;
    QString                           Today;
    bool                      create_directory();
    //void                      create_directory();


protected:
    void                              run();

private slots:
    void                              update_data_base(const QVector<int> & in_out);
    void                              _update();

signals:
    void                              returntomain(QImage);
    void                              returnpeoplenumbers(const QVector<int>);



};

#endif // MYTHREAD_H

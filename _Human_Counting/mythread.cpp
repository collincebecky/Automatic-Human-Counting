#include "mythread.h"
//#include "hogdescriptor.h"
#include "handling_db.h"
#include <QSqlError>
#include <QDir>

Mythread::Mythread():QThread()
{
timer=new QTimer(this);
Count = new people_count_test();
//handle_db=new DBhandle();
//create directories
//create_directory();
//
}
bool Mythread::connectToCamera()
{
    // Open camera

    if(strsource==""){

     camOpenResult= cap.open(intsource);
    }
    else{
     camOpenResult= cap.open(strsource);
    }


    // Set resolution
    //if(width != -1)
        //cap.set(CV_CAP_PROP_FRAME_WIDTH, width);
    //if(height != -1)
        //cap.set(CV_CAP_PROP_FRAME_HEIGHT, height);
    // Return result

    return camOpenResult;
}
void Mythread::run(){

    HOGDescriptor hog;
    hog.setSVMDetector(HOGDescriptor::getDefaultPeopleDetector());

   // detect_people=new DetectPeople();
    if (!connectToCamera()){
        std::cerr<<"Unreliable source"<<std::endl;
        throw std::invalid_argument( "Source doesnt exist" );

        }


    if(strsource==""){

      cap.open(intsource);
    }



    QImage frame_;
    //->performance critical operation;
    forever{

        cap>>frame;
        if (!cap.grab()){connectToCamera();continue;}
        (*Count).track(frame);
        //cv::resize(frame, frame, cv::Size(frame.cols * 0.3,frame.rows * 0.3), 0, 0,CV_INTER_LINEAR);
        frame_=mat8ToImage(frame);
        //cvtColor(frame, frame, CV_BGR2BGRA);
        //cvtColor(frame, frame, CV_BGR2RGB);
        //QImage frame_(frame.data, frame.cols, frame.rows, (int) frame.step, QImage::Format_RGB888);

        //QImage frame_((const unsigned char*) (frame.data),frame.cols, frame.rows, QImage::Format_RGB888);


        emit returntomain(frame_);
        if(isSend && update_data){
            QVector<int> ppledata=(*Count).people_in_out();


            ///qDebug()<<"Vector size "<<ppledata.size()<<endl;


            emit update_data_base(ppledata);

            isSend=false;

        }

    }

}


void Mythread::_update(){
    qDebug()<<">>>>>>>>>>>>>In update function now"<<endl;

    isSend=true;
}

const QImage Mythread::mat8ToImage(const cv::Mat &mat)
{
    switch (mat.type()) {
    case CV_8UC1:
    {

        QVector<QRgb> ct;
        for (int i = 0; i < 256; ++i)
            ct.append(qRgb(i, i, i));
        QImage result(mat.data, mat.cols, mat.rows, (int) mat.step, QImage::Format_Indexed8);
        result.setColorTable(ct);
        return result.copy();
    }
    case CV_8UC3:
    {

        cv::Mat tmp;
        cvtColor(mat, tmp, CV_BGR2BGRA);
        return mat8ToImage(tmp);
    }
    case CV_8UC4:
    {
        QImage result(mat.data, mat.cols, mat.rows, (int) mat.step, QImage::Format_RGB32);
        return result.rgbSwapped();
    }
    default:
        qWarning("Unhandled Mat format %d", mat.type());
        return QImage();
    }
}

void Mythread::update_data_base(const QVector<int> & in_out){



    //Temp cam register
    Today=QDate::currentDate().toString();
    QString camera="cam_"+ID;
    qDebug()<<"camera ID ..........."<<ID<<endl;
    //QString current_time=QTime::currentTime().toString("hh:mm:ss");
    QString current_time="00:00:01";


        QSqlQuery *query = new QSqlQuery(Db);
        if(current_time=="00:00:01"){
            /*
            query->exec("create table if '"+QDate::currentDate().toString()+"' "
                      "(id integer primary key, "
                      "firstname varchar(20), "
                      "lastname varchar(30), "
                      "age integer)");
             */
            query->exec("CREATE TABLE IF NOT EXISTS '"+Today+"' (Camera INTEGER PRIMARY KEY,Time TEXT,Person_In INTEGER,Person_Out INTEGER)");

           query->prepare("INSERT INTO '"+Today+"' (Camera, Time, Person_In,Person_Out) "
                          "VALUES (:Camera, :Time, :Person_In, :Person_Out)");

           qDebug()<<"new table created -------successfully"<<endl;
        }
        else{
        qDebug()<<"inserting data into db"<<endl;
         query->exec("CREATE TABLE IF NOT EXIST Mine  (Camera TEXT,Time TEXT,Person_In INTEGER,Person_Out INTEGER);");
         query->prepare("INSERT INTO Mine (Camera, Time, Person_In,Person_Out) "
                        "VALUES (:Camera, :Time, :Person_In, :Person_Out)");
        }
        //query.prepare("INSERT INTO Statistics (Camera, Time, Person_In, Person_Out) VALUES (:Camera, :Time, :Person_In, :Person_Out)");

        query->bindValue(":Camera",            ID);
        query->bindValue(":Time"     , current_time);
        query->bindValue(":Person_In",     in_out[0]);
        query->bindValue(":Person_Out",    in_out[1]);
        //query->exec();
        if(query->exec())
        {
            qDebug() << " success "
            << endl;
        }
        else
        {
        qDebug() << " error: "
        << query->lastError();
        }

}
bool Mythread::create_directory(){

    QString path= "/home/centurine/BRANCE/build-Human_Counting-Desktop-Release/";
    QDir dir(path+"1");
    if(!dir.exists())
        dir.mkdir(path+"1");
    int i= 2;
    while (dir.exists()){
      dir.mkdir(path+QString::number(i));
      ++i;
    }
    return true;
}

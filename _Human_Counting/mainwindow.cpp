#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "mobilessd.h"

QT_CHARTS_USE_NAMESPACE


string src="/home/techbridge/collins/Brance/Human Counting Project/Human_Counting/example_01.mp4";
std::string url ="rtsp://admin:K3Y@H1K.C4M3R4@192.168.100.152:554";
string pple="/home/centurine/BRANCE/_Human_Counting/example_01.mp4";
string pple2="/home/centurine/BRANCE/gogo/people-counting-opencv/videos/example_02.mp4";
string cars2="/home/centurine/BRANCE/gogo/people-counting-opencv/videos/tmp/freewa.mp4";
string cars="/home/centurine/Downloads/Quality Control - Too Hotty by Migos featuring Eurielle.webm";
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    //->setup ui
    //->dectivate add++ button
    ui->doneBtn->setEnabled(false);
    //->start timer slot
    timer = new QTimer(this);
    QPixmap tab(":icon0.png");
    ui->horizontalTabWidget->setTabIcon(4,QIcon(tab));
    QPixmap tab1(":icon1.png");
    ui->horizontalTabWidget->setTabIcon(5,QIcon(tab1));
    QPixmap tab2(":icon4.png");
    ui->horizontalTabWidget->setTabIcon(3,QIcon(tab2));




    //->Create connection to the database

    db_=db::DBhandle::open_db("QSQLITE","count_db");

    //->return camera resources from db

    return_sources();

    //->add camera auto test
    //auto_add_display();

    //->Tree buttons
    populate_tree();
    //->populate cam table
    populate_cam_table();
    //runmobilessd();
    update_statistics();
    //->its
    render_chart();

}


void MainWindow::return_sources(){

    //->clear map to prevent redundacy

    mapped.clear();


    printf("connection to resources successfull");
    QSqlQuery *query= new QSqlQuery(db_);

    QString queryString("SELECT Cam_id,Region,Location,Source FROM Cameras");
    
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
        QString Cam_id=data.value(0).toString();
        QString Region =data.value(1).toString();
        QString Location =data.value(2).toString();
        QString source =data.value(3).toString();



        //->concatinate camera_id ,location and source

        QString camera_source=Cam_id+"="+Location+"="+source;

         qDebug()<<"camera_source "<<camera_source;


        mapped.insert(Region,camera_source);
     }
    qDebug()<<"Mapped ............"<<mapped<<endl;

}

void MainWindow::populate_tree(){


    //->get relevant sources from map then  populate accordingly

    //->clear every cam tree
    ui->cam_tree->clear();



    qDebug()<<"map for return resources "<<mapped<<endl;


    //->Loop through each sources

    foreach (QString key, mapped.uniqueKeys()) {
        QTreeButton *topLevelButton = new QTreeButton(key);
        QTreeWidgetItem *topLevelItem = new QTreeWidgetItem();
        ui->cam_tree->addTopLevelItem(topLevelItem);
        ui->cam_tree->setItemWidget(topLevelItem, 0, topLevelButton);

        foreach (QString value, mapped.values(key)) {

            QTreeButton *childButton = new QTreeButton(value,ui->front_screen);
            QTreeWidgetItem *childItem = new QTreeWidgetItem();
            topLevelItem ->addChild(childItem);
            ui->cam_tree->setItemWidget(childItem, 0, childButton);

        }

  }

}

void MainWindow::on_doneBtn_released()
{
//->verify if the cam_id exists to prevent redundancy when updating

if(ui->cam_id->text()==""){
QSqlQuery *query = new QSqlQuery(db_);

qDebug()<<"adding camera into database"<<endl;

"rtsp://admin:K3Y@H1K.C4M3R4@192.168.100.152:554";

QString source="rtsp://"+ui->username->text()+":"+
ui->password->text()+"@"+ui->cam_IP->text()+":554";
query->prepare("INSERT INTO Cameras (Region,Location,Source)"
                   "VALUES (:Region,:Location,:Source)");
QString none="none";
qDebug()<<"rtsp source .................. "<<source<<endl;

//query->bindValue(":Cam_id", ui->cam_id->text());
query->bindValue(":Region", ui->cam_zone->text());
//query->bindValue(":camera", ui->cam_id->text());
query->bindValue(":Location", ui->cam_link->text());
query->bindValue(":Source", source);
QString empty="";

if (ui->cam_zone->text()==empty
   ||ui->cam_link->text()==empty
   ||source == empty){

    QMessageBox::
    warning(this, "Oooops!",
    "please complete all fields",
    "ok");

return;
}


 if(query->exec())
  {
  qDebug() << " success "
  << endl;

  QMessageBox::
  information(this, "Hurray!",
  "Camera was a database added successfully, ",
  "Thanks for choosing Brance");

  //->TODO CAMERA TEST

  return_sources();
  populate_cam_table();
  populate_tree();

  }
  else
  {
  qDebug() << " error: "
   << query->lastError();

  QMessageBox::
  warning(this, "Oooops!",
  "There was an error while adding camera please try again",
  "ok");
  }

 //->dectivate add++ button
 ui->doneBtn->setEnabled(false);
}
}




void MainWindow::populate_cam_table(){

    //->clear every cam table two
    ui->camera_table_2->clearContents();

    ui->camera_table_2->setRowCount(0);

    int row_count=0;

    qDebug()<<"--------------MAPPED----"<<mapped<<endl;

    foreach (QString key, mapped.keys()) {

        QStringList name_list =  mapped.values(key)[0].split( "=" );

        qDebug()<<"NAMELIST --------"<<name_list<<endl;

        QString Cam_id="cam_"+name_list.value(0);
        QString name = name_list.value(1);
        QString source = name_list.value(2);
        ui->camera_table_2->insertRow(row_count);
        ui->camera_table_2->setItem(row_count, 0, new QTableWidgetItem(Cam_id));
        ui->camera_table_2->setItem(row_count, 1, new QTableWidgetItem(key));
        ui->camera_table_2->setItem(
            row_count, 2, new QTableWidgetItem(name));

        ui->camera_table_2->setItem(
             row_count, 3, new QTableWidgetItem(source));

       ui->camera_table_2->setItem(row_count, 4, new QTableWidgetItem("Edit"));
       ui->camera_table_2->item(row_count,4)->setTextColor(QColor(0, 0, 0));

       connect(ui->camera_table_2,SIGNAL(cellClicked(int,int)), this, SLOT(edit_row(int,int)));

        ui->camera_table_2->item(row_count,4)->setTextAlignment(Qt::AlignCenter);
        ui->camera_table_2->item(row_count,4)->setBackground(QBrush(QColor(227, 234, 234)));

        ++row_count;
    }
    //->Disable ++ button



}
void MainWindow::edit_row(int row,int col){

    qDebug()<<"our row "<<row<<"our column "<<col<<endl;
    //for(int i=0;i<3;++i)
    if(col==4){
       //->TODO ACTIVATE ID LATTER

    QString camid=ui->camera_table_2->item(row,0)->text();
    QString region=ui->camera_table_2->item(row,1)->text();
    QString location=ui->camera_table_2->item(row,2)->text();

    //-> why no source text

    QString source=ui->camera_table_2->item(row,3)->text();


    //->rtsp://admin:K3Y@H1K.C4M3R4@192.168.100.152:554

    //QString split_source=

    QStringList items= source.split( ":" );

    QString name = items.value(1).remove(0,2);
    QString pwd_port= items.value(2);

    //->TODO consider what the user might opt for a totaly different password
    QStringList pwd_port_split=pwd_port.split("@");

    qDebug()<<"PASWORD AND PORT AFTER SPLIT"<<pwd_port_split<<endl;

    QString pwd=pwd_port_split.value(0)+pwd_port_split.value(1);
    QString port=pwd_port_split.value(2);


    qDebug()<<" NAME "<<row<<" PASS "<<pwd<<" PORT "<<port<<endl;


    ui->cam_id->setText(camid);
    ui->cam_zone->setText(region);

     ui->cam_IP->setText(port);

    ui->cam_link->setText(location);

    ui->username->setText(name);

    ui->password->setText(pwd);

    }

}

void MainWindow::update_statistics(){
    //->temporary
    if(ui->search_cameras->text()!=NULL)return;

    ui->camera_table->clearContents();
    printf("connection to resources successfull");
    QSqlQuery *query= new QSqlQuery(db_);

    QString queryString("SELECT * FROM Mine");

    //int idRegion = query.record().indexOf("Region");
    //int idCamera = query.record().indexOf("camera");
    query->exec(queryString);

        //QSqlQuery q = db.exec("SELECT * FROM Table");
      //if (q.last()) {
    //     do {
  //         // Do something with row...
//      } while (q.previous());
//      }

    int Total_pple_in=0;
    int Total_pple_out=0;
    int row_count=0;
    QString Big_time;
    while (query->next())
    {
        QStringList strings;
        QSqlRecord data = query->record();

        //->get data from every column in current row
        QString cameras_id ="cam_"+data.value(0).toString();

        Big_time =data.value(1).toString();
        if(Big_time=="") continue;
        QString pple_in =data.value(2).toString();
        QString pple_0ut =data.value(3).toString();
        //->update total in and total out

        Total_pple_in+=pple_in.toInt();
        Total_pple_out+=pple_0ut.toInt();

        //->append various qstrings to list to QStringList

        strings.append(cameras_id);
        strings.append(Big_time);
        strings.append(pple_in);
        strings.append(pple_0ut);


        //->check if line edit is clean or not to prevent filter and update issues

        ui->camera_table->insertRow(row_count);
        ui->camera_table->setItem(row_count, 0, new QTableWidgetItem(cameras_id));
        ui->camera_table->setItem(
             row_count, 1, new QTableWidgetItem(Big_time));

        ui->camera_table->setItem(
                    row_count, 2, new QTableWidgetItem(pple_in));
        ui->camera_table->setItem(
                    row_count, 3, new QTableWidgetItem(pple_0ut));


        for (int i=0;i<4;++i)

        ui->camera_table->item(row_count,i)->setTextAlignment(Qt::AlignCenter);





        //ui->camera_table->item(row_count,3)->setTextAlignment(Qt::AlignHCenter);
        //ui->camera_table->item(row_count,3)->setBackground(QBrush(QColor(100, 100, 200)));


         ++row_count;
        list_of_lists.append(strings);


     }


    query->finish();

    //->use Big time to query the previous day

    int Total_pple_in_past=0;
    int Total_pple_out_past=0;

     QString querystring("SELECT Person_In,Person_Out FROM'"
    +QDate::currentDate().toString()+"' WHERE Time ='"+Big_time+"'");

     query->exec(querystring);
     qDebug()<<"This the Qt last error "<<query->lastError()<<endl;
     qDebug()<<"Bigtime ........"<<Big_time<<endl;
     while (query->next())
          {
            QSqlRecord data = query->record();

            QString people_in= data.value(0).toString();
            qDebug()<<"PEOPLE IN  "<<people_in<<endl;
            //throw std::invalid_argument("Database locked throw back .......................");
            QString people_out= data.value(1).toString();

            Total_pple_in_past+=people_in.toInt();

            Total_pple_out_past+=people_out.toInt();


            }



    //->show totals in the qlabels
    ui->vis_veh_2->setText(QString::number(Total_pple_in));
    ui->vis_cyc_2->setText(QString::number(Total_pple_out));

    ui->vis_bus_2->setText(QString::number(Total_pple_in_past));
    ui->vis_truck_2->setText(QString::number(Total_pple_out_past));





}
MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::on_search_cameras_textChanged(const QString &arg1)
{

    ui->camera_table->clearContents();
    //ui->camera_table->setRowCount(0);
    QRegExp regex(arg1,Qt::CaseInsensitive,QRegExp::Wildcard);
    int row=0;
    foreach (QStringList list, list_of_lists){
        QStringList filtered=list.filter(regex);
        if (filtered.size()!=0){
            int colmn=0;
            foreach (QString str,list) {

                ui->camera_table->setItem(row,colmn, new QTableWidgetItem(str));
                ui->camera_table->item(row,colmn)->setTextAlignment(Qt::AlignCenter);

                ++colmn;

            }
            ++row;
        }


    }
}

void MainWindow::populate_camera_table(){


}



void MainWindow::on_pushButton_clicked()
{
    ui->screen1->startdeamon(pple,db_,1);
    ui->screen2->startdeamon(pple,db_,2);
    ui->screen3->startdeamon(pple,db_,3);
    ui->screen4->startdeamon(pple,db_,4);
    ui->screen5->startdeamon(pple,db_,5);
    ui->screen6->startdeamon(pple,db_,6);

    ui->front_screen->startdeamon(url,db_,7);
    //->start timer to update ui->camera->table
   QTimer *time_for_update=new QTimer(this);

   //QObject::connect(time_for_update,SIGNAL(timeout()),this,SLOT(update_statistics()));
   //time_for_update->start(1100);

}
void MainWindow::auto_add_display(){


    //->TODO ADD QLABELS TO GRID

    MyLabel *label = new MyLabel(ui->gridFrame);
    label->setText("new");


}
//->TODO ACTIVATE ALL SEARCHES


void MainWindow::render_chart(){

    QChart *chart = new QChart();
    chart->legend()->show();
    chart->setTitle("People Traffic Analysis");

    //QValueAxis *axisX = new QValueAxis;
    //axisX->setTickCount(12);
    //chart->addAxis(axisX, Qt::AlignBottom);

    QSplineSeries *series = new QSplineSeries;
    //*series << QPointF(1, 5) << QPointF(3.5, 18) << QPointF(4.8, 7.5) << QPointF(10, 2.5);

    *series << QPointF(0.00, 150) << QPointF(3.00, 150) << QPointF(6.00, 150)<< QPointF(9.00, 160)
            << QPointF(12.00, 160)
            << QPointF(15.00, 350) << QPointF(18.00, 1650)<< QPointF(21.00, 1650);

    chart->addSeries(series);

    QValueAxis *axisY = new QValueAxis;
    axisY->setLinePenColor(series->pen().color());

    chart->addAxis(axisY, Qt::AlignLeft);
    ////////////////////


    QStringList categories;
    categories << "00:00" << "03:00" << "06:00" << "09:00" << "12:00" << "15:00"
              << "18:00" << "21:00";

    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(categories);
    chart->addAxis(axisX, Qt::AlignBottom);


    //////////////////
    series->attachAxis(axisX);
    series->attachAxis(axisY);

    series = new QSplineSeries;
    /*
    *series << QPointF("00:00", 0.5) << QPointF("02:00", 4.5) << QPointF("04.00", 2.5) << QPointF("06.00", 12.5)
            << QPointF("08:00", 3.5) << QPointF("10:00", 16.5) << QPointF("12:00", 7.5) << QPointF("14:00", 17)
            << QPointF("16:00", 3.5) << QPointF("18:00", 16.5) << QPointF("20:00", 7.5) << QPointF("22:00", 17);*/

    *series << QPointF(0.00, 1000) << QPointF(3.00, 150) << QPointF(6.00, 250) << QPointF(9.00, 1250)
            << QPointF(12.00, 1250)
            << QPointF(15.00, 350) << QPointF(18.00, 1650)<< QPointF(21.00, 1650);


    chart->addSeries(series);
    QCategoryAxis *axisY3 = new QCategoryAxis;
    axisY3->append("Low", 500);
    axisY3->append("Medium", 1000);
    axisY3->append("High", 1600);
    axisY3->setLinePenColor(series->pen().color());
    axisY3->setGridLinePen((series->pen()));

    chart->addAxis(axisY3, Qt::AlignRight);
    series->attachAxis(axisX);
    series->attachAxis(axisY3);

    // NoAnimation, GridAxisAnimations, SeriesAnimations
    chart->setAnimationOptions(QChart::AllAnimations);

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);


    ui->plot_layout_2->addWidget(chartView);


}

void MainWindow::on_cam_config_btn_2_released()
{
    //->update database with cam_id

                //->clean id to get three digit
                 QString camid=ui->cam_id->text();

                 QStringList clean_id=camid.split("_");
                 QString value =clean_id.value(1);

                   QSqlQuery *query = new QSqlQuery(db_);

                   qDebug()<<"adding camera into database"<<endl;

                   QString source="rtsp://"+ui->username->text()+":"+
                   ui->password->text()+"@"+ui->cam_IP->text()+":554";
                   //->preprocess cam_id text
                   //QStringList clean_id=
                   query->prepare("UPDATE Cameras SET \
                                  Region='"+ui->cam_link->text()+"',\
                                  Location='"+ui->cam_zone->text()+"',\
                                  Source ='"+source+"' WHERE \
                                  Cam_id='"+value+"'");
                   //QString none="none";
                   //qDebug()<<"rtsp source .................. "<<source<<endl;




                   if (ui->cam_zone->text()==""
                      ||ui->cam_link->text()==""
                      ||source            ==""){

                       QMessageBox::
                       warning(this, "Oooops!",
                       "please complete all fields",
                       "ok");


                   }
                   else{
                    if(query->exec())
                     {
                     qDebug() << " success "
                     << endl;

                     QMessageBox::
                     information(this, "Hurray!",
                     "Camera was updated successfully, ",
                     "Thanks for choosing Brance");

                     //-> clear the lineedits

                     ui->cam_id->clear();
                     ui->cam_zone->clear();

                     ui->cam_IP->clear();

                     ui->cam_link->clear();

                     ui->username->clear();

                     ui->password->clear();


                     //->TODO CAMERA TEST

                     return_sources();
                     populate_cam_table();
                     populate_tree();

                     }
                     else
                     {
                     qDebug() << " error: "
                      << query->lastError();

                     QMessageBox::
                     warning(this, "Oooops!",
                     "There was an error while adding camera please try again",
                     "ok");
                     }
                    }

}

void MainWindow::on_del_btn_2_released()
{
    //->TODO message box before delete
   QSqlQuery *query = new QSqlQuery(db_);
   //->preprocess ui camid
   QString camid=ui->cam_id->text();

   QStringList clean_id=camid.split("_");
   QString value =clean_id.value(1);


   query->prepare("DELETE FROM Cameras WHERE Cam_id='"+value+"'");

    if( !query->exec() )
         qDebug() << query->lastError();
    else{
        qDebug( "Deleted!" );
        ui->cam_id->clear();
        ui->cam_zone->clear();
        ui->cam_IP->clear();
        ui->cam_link->clear();
        ui->username->clear();
        ui->password->clear();

        return_sources();
        populate_cam_table();
        populate_tree();
    }
    delete query;

}

void MainWindow::on_testBtn_2_released()
{   counter=0;
    //->activate add++ button
    ui->doneBtn->setEnabled(true);
    "rtsp://admin:K3Y@H1K.C4M3R4@192.168.100.152:554";


    //->construct source

    QString source="rtsp://"+ui->username->text()+":"+
    ui->password->text()+"@"+ui->cam_IP->text()+":554";


    //->check if either of the fields is empty

    if (ui->cam_zone->text()==""
       ||ui->cam_link->text()==""
       ||source ==""){

        QMessageBox::
        warning(this, "Oooops!",
        "please complete all fields",
        "ok");

        return;
    }
    cap.open(0);

        if(!cap.isOpened())  // Check if we succeeded
        {
            cout << "camera is not open" << endl;
             QMessageBox::information(this,"Error","Error while accessing camera please check your connection","ok");
        }
        else
        {
            //->deactivate test button
            ui->testBtn_2->setEnabled(false);

            cout << "camera is open" << endl;

            QObject::connect(timer, SIGNAL(timeout()), this, SLOT(update_test_label()));
            timer->start();
        }
}

void MainWindow::update_test_label(){

    if (counter==150){
        disconnect(timer, SIGNAL(timeout()), this, SLOT(update_test_label()));
        cap.release();

        ui->testBtn_2->setEnabled(true);

        //Mat image = Mat::zeros(frame.size(),CV_8UC3);

        //qt_image = QImage((const unsigned char*) (image.data), image.cols, image.rows, QImage::Format_RGB888);
        QPixmap map("./Resources/covers/qcctv.png");
            //map.scaled(150,150);

        ui->test_window_2->setPixmap(map);
        QMessageBox::information(this,"Hurray","Camera Test succesfull","ok");
        //->dectivate add++ button
        ui->doneBtn->setEnabled(true);
    }
    else{
    cv::Mat frame;
    cap >> frame;
    if (!cap.grab()){
        QMessageBox::information(this,"Error while accessing camera please check your connection","ok");
    }
    cvtColor(frame, frame, CV_BGR2RGB);

    QImage qt_image = QImage((const unsigned char*) (frame.data), frame.cols, frame.rows, QImage::Format_RGB888);

    ui->test_window_2->setPixmap(QPixmap::fromImage(qt_image));

     ui->test_window_2->resize(ui->test_window_2->pixmap()->size());
     ++counter;
     cout<<"counted "<<counter<<endl;
    }

}




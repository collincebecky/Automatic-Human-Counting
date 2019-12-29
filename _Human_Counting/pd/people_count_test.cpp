#include "people_count_test.hpp"
#include "bg_subtract_detector.hpp"
#include "opencv_trackers.hpp"


#ifdef HAS_DLIB
#include "dlib_tracker.hpp"
#include <dlib/image_processing.h>
#include <dlib/opencv/cv_image.h>
#endif

#include <pd/contour_utility.hpp>
#include <pd/rect_utility.hpp>

#include <opencv2/core.hpp>
#include <opencv2/bgsegm.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/videoio/videoio.hpp>
#include <opencv2/video/background_segm.hpp>




#include <QDebug>
#include <iostream>
using namespace cv;
namespace{
/*
cv::VideoWriter create_video_writer(std::string const &video)
{
    cv::VideoCapture cap(video);
    int const frame_width = cap.get(cv::CAP_PROP_FRAME_WIDTH);
   //change CAP_PROP_FRAME_WIDTH into CV_CAP_PROP_FRAME_WIDTH
   //change CAP_PROP_FRAME_HEIGHT into CV_CAP_PROP_FRAME_HEIGHT
   //change CAP_PROP_FRAME_COUNT into CV_CAP_PROP_FRAME_COUNT
    int const frame_height = cap.get(cv::CAP_PROP_FRAME_HEIGHT);
    int const fps = cap.get(cv::CAP_PROP_FPS);
    //CV_FOURCC('P','I','M','1') CV_FOURCC('M','J','P','G')
    return cv::VideoWriter("out.avi", cv::CAP_PROP_FOURCC('P','I','M','1'),
                          fps, {frame_width,frame_height});
}
*/
} //nameless namespace

people_count_test::people_count_test()
{
 init_data();
}

people_count_test::~people_count_test()
{
qDebug()<<'destructor called'<<endl;
}

void people_count_test::init_data()
{
    in_blob_size_ = 0;
    out_blob_size_ = 0;
    double constexpr anchor_ratio = 0.5;
    blob_detector_ = std::make_unique<bg_subtract_detector>(cv::createBackgroundSubtractorMOG2(500, 60, true),
                                                            min_human_size_, anchor_ratio, detect_margin_);

    auto legal_roi_strategy = [this](auto const &r)
    {
        return r.area()> min_human_size_ && (r.height > r.width);
    };

    auto lost_track_strategy = [this](auto &&origin, auto &&current)
    {
        if(origin.y > current.y){
            ++in_blob_size_;
        }else{
            ++out_blob_size_;
        }
    };

    tracker_ = std::make_unique<opencv_trackers>(legal_roi_strategy,
                                                 lost_track_strategy, "KCF");
    //tracker_ = std::make_unique<dlib_tracker>(legal_roi_strategy, lost_track_strategy);
}

void people_count_test::draw_measure_line(cv::Mat & img_)
{
    int const ypos = img_.rows / 2;
    //cv::line(img_, {0, ypos - detect_margin_}, {img_.cols, ypos - detect_margin_}, {255, 0, 0});
    //cv::line(img_, {0, ypos}, {img_.cols, ypos}, {0, 255, 0});
    cv::line(img_,cv::Point(0, ypos), cv::Point(img_.cols, ypos), cv::Scalar( 255 ), 2, 8 );
    //cv::line(img_, {0, ypos + detect_margin_}, {img_.cols, ypos + detect_margin_}, {255, 0, 0});
}

void people_count_test::track(cv::Mat & img_)
{

                //img_ = cv::Mat::zeros(img_.size(),CV_8UC3);


                    draw_measure_line(img_);
                    blob_detector_->detect_blob(img_);
                    tracker_->track(img_, blob_detector_->get_roi());
                    tracker_->draw_track_blobs(img_, img_);
                    put_inout_msg(img_);




}

void people_count_test::put_inout_msg(cv::Mat & img_)
{   //IN+=in_blob_size_;
    //OUT+=out_blob_size_;
    cv::putText(img_, "in " + std::to_string(in_blob_size_),
    {0 + 20, img_.rows / 2 - 50}, cv::FONT_HERSHEY_TRIPLEX, 1, {0, 0, 0});
    cv::putText(img_, "out " + std::to_string(out_blob_size_),
    {0 + 20, img_.rows / 2 + 50}, cv::FONT_HERSHEY_TRIPLEX,1, {0,0, 0});
}

QVector<int> people_count_test::people_in_out(){
   people.clear();
   people.append(in_blob_size_);
   people.append(out_blob_size_);
   qDebug()<<"NUMBER OF PEOPLE OUT APPENDED  "<<out_blob_size_<<endl;

   return people;
}


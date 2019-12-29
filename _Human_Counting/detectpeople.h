#ifndef DETECTOBJECTCV_H
#define DETECTOBJECTCV_H
#define DLIB_JPEG_SUPPORT
#define DLIB_PNG_SUPPORT
//#define OPENCV_USE_CUDA

#include <opencv2/dnn.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core/utils/trace.hpp>

#include <dlib/image_processing.h>
#include <dlib/gui_widgets.h>
#include <dlib/image_io.h>
#include <dlib/dir_nav.h>
#include <dlib/opencv.h>

#include <ctime>
#include <cstdio>
#include <fstream>
#include <cstdlib>
#include "stdint.h"
#include <iostream>
#include <cstdlib>
#include "string"
#include <vector>



#include "utils.h"

using namespace dlib;
using namespace std;
using namespace cv;
using namespace cv::dnn;


namespace CO{
class DetectPeople;
}

class DetectPeople
{
private:
     String                      CLASSES[21];
     string                      modelTxt;
     string                      modelBin;
     dlib::matrix<rgb_pixel>     dlib_mat;

     //vector<Rect> boxes;
     dlib::correlation_tracker   tracker;

     Net                         net;
     Mat                         img2;
public:
    DetectPeople() {

        String CLASSES[] = {"background", "aeroplane", "bicycle", "bird", "boat",
              "bottle", "bus", "car", "cat", "chair", "cow", "diningtable",
              "dog", "horse", "motorbike", "person", "pottedplant", "sheep",
              "sofa", "train", "tvmonitor"};


        modelTxt ="/home/nano/Desktop/_Human_Counting/Resources/Models/MobileNetSSD_deploy.prototxt";
        modelBin = "/home/nano/Desktop/_Human_Counting/Resources/Models/MobileNetSSD_deploy.caffemodel";
      net = dnn::readNetFromCaffe(modelTxt, modelBin);





    }


 inline void detectobjectcv(Mat &img,std::vector<dlib::correlation_tracker> & trackers){

     std::cout<<'>>>>>>>>>>>>>>THERE IS AN OBJECT'<<std::endl;

    //cv::resize(img, img, Size(300,300));

    Mat inputBlob = blobFromImage(img, 0.007843, Size(300,300), Scalar(127.5, 127.5, 127.5), false);

    net.setInput(inputBlob, "data");
    Mat detection = net.forward("detection_out");
    Mat detectionMat(detection.size[2], detection.size[3], CV_32F, detection.ptr<float>());

    ostringstream ss;
    float confidenceThreshold = 0.5;
    for (int i = 0; i < detectionMat.rows; i++)
    {
        float confidence = detectionMat.at<float>(i, 2);

        if (confidence > confidenceThreshold)
        {
        int idx = static_cast<int>(detectionMat.at<float>(i, 1));


        if (idx != 15){
            continue;
        }

        int xLeftBottom = static_cast<int>(detectionMat.at<float>(i, 3) * img.cols);
        int yLeftBottom = static_cast<int>(detectionMat.at<float>(i, 4) * img.rows);
        int xRightTop = static_cast<int>(detectionMat.at<float>(i, 5) * img.cols);
        int yRightTop = static_cast<int>(detectionMat.at<float>(i, 6) * img.rows);


        //int W=xRightTop - xLeftBottom;
        //int H=xRightTop - xLeftBottom;


        Rect object((int)xLeftBottom, (int)yLeftBottom,
                    (int)(xRightTop - xLeftBottom),
                    (int)(yRightTop - yLeftBottom));

        cv::rectangle(img, object, Scalar(0, 255, 0), 2);
        //->turn to cv2 Rects turn to a dlib rects
        //tracker_map.insert(i++,(int)(xLeftBottom/2));

        //dlib::assign_image(dlib_mat,cv_image<bgr_pixel>(img));


        //tracker.start_track(dlib_mat,openCVRectToDlib(object));

        //tracker.start_track(img, centered_rect(point(xLeftBottom,yLeftBottom), W, H));
        //trackers.push_back(tracker);




        }


    }

 }
};


#endif // DETECTOBJECTCV_H

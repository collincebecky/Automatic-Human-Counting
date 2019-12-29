#ifndef MOBILESSD_H
#define MOBILESSD_H


#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/dnn.hpp>

#include <ctime>
#include <cstdio>
#include <fstream>
#include <cstdlib>
#include "stdint.h"
#include <iostream>
#include <cstdlib>
#include "string"
#include <vector>

using namespace cv;
using namespace std;
using namespace cv::dnn;

const size_t inWidth = 300;
const size_t inHeight = 300;
//-> OLD

String weights="/home/centurine/BRANCE/gogo/people-counting-opencv/model/frozen_inference_graph.pb";
String prototxt = "/home/centurine/BRANCE/gogo/people-counting-opencv/model/ssd_mobilenet_v1_coco_2017_11_17.pbtxt";
//->LATEST

//String weights = "/home/centurine/BRANCE/LIGHT OBJ/ssdlite_mobilenet_v2_coco_2018_05_09/2018_3/ExploreOpencvDnn-master/models/frozen_inference_graph.pb";
//String weights= "/home/centurine/BRANCE/LIGHT OBJ/ssdlite_mobilenet_v2_coco_2018_05_09/frozen_inference_graph.pb";
//String prototxt ="/home/centurine/BRANCE/LIGHT OBJ/ssdlite_mobilenet_v2_coco_2018_05_09/2018_3/ExploreOpencvDnn-master/models/ssd_mobilenet_v2_coco_2018_03_29.pbtxt";
const float WHRatio = inWidth / (float)inHeight;

const String classNames[] = {"background",
        "person","bicycle","car","motorcycle","airplane", "bus",
      "train","truck", "boat","traffic light", "fire hydrant",
       "stop sign", "parking meter","bench","bird","cat",
       "dog","horse","sheep","cow", "elephant",  "bear",
        "zebra","giraffe","backpack","umbrella","handbag",
       "tie","suitcase","frisbee","skis","snowboard",
        "sports ball", "kite","baseball bat","baseball glove",
       "skateboard","surfboard","tennis racket","bottle",
       "wine glass","cup","fork", "knife","spoon",
       "bowl","banana","apple","sandwich","orange",
       "broccoli","carrot","hot dog","pizza","donut",
       "cake","chair","couch","potted plant","bed",
       "dining table","toilet","tv","laptop","mouse",
        "remote","keyboard","cell phone", "microwave","oven",
      "toaster","sink","refrigerator","book","clock",
       "vase","scissors","teddy bear","hair drier","toothbrush"};



Net net = cv::dnn::readNetFromTensorflow(weights, prototxt);
//VideoCapture cap("rtsp://admin:whxx2017@192.168.168.161:554/Streaming/Channels/001");

void runmobilessd(){
    //VideoCapture cap("/home/centurine/BRANCE/gogo/people-counting-opencv/videos/tmp/surveillance.mp4");
     VideoCapture cap(0);
    Mat frame;
    while(1){
        cap >> frame;
        //Size frame_size = frame.size();
        /*
        Size cropSize;
        if (frame_size.width / (float)frame_size.height > WHRatio)
        {
        cropSize = Size(static_cast<int>(frame_size.height * WHRatio),
            frame_size.height);
        }
        else
        {
        cropSize = Size(frame_size.width,
            static_cast<int>(frame_size.width / WHRatio));
        }
    Rect crop(Point((frame_size.width - cropSize.width) / 2,
        (frame_size.height - cropSize.height) / 2),
        cropSize);
        */

     cv::resize(frame, frame, cv::Size(frame.cols * 0.5,frame.rows * 0.5), 0, 0,INTER_LINEAR);
     //cv::Mat blob = cv::dnn::blobFromImage(frame,1./255,Size(300,300));
     cv::Mat blob= cv::dnn::blobFromImage(frame, 0.007843, Size(300,300), Scalar(127.5, 127.5, 127.5), false);
    //cout << "blob size: " << blob.size << endl;

     net.setInput(blob);
     Mat output = net.forward();
    //cout << "output size: " << output.size << endl;

     Mat detectionMat(output.size[2], output.size[3], CV_32F, output.ptr<float>());

     //frame = frame(crop);
     float confidenceThreshold = 0.5;

    for (int i = 0; i < detectionMat.rows; i++)
    {
        float confidence = detectionMat.at<float>(i, 2);

        if (confidence > confidenceThreshold)
        {
            size_t objectClass = (size_t)(detectionMat.at<float>(i, 1));

            int xLeftBottom = static_cast<int>(detectionMat.at<float>(i, 3) * frame.cols);
            int yLeftBottom = static_cast<int>(detectionMat.at<float>(i, 4) * frame.rows);
            int xRightTop = static_cast<int>(detectionMat.at<float>(i, 5) * frame.cols);
            int yRightTop = static_cast<int>(detectionMat.at<float>(i, 6) * frame.rows);

            ostringstream ss;
            ss << confidence;
            String conf(ss.str());

            Rect object((int)xLeftBottom, (int)yLeftBottom,
                (int)(xRightTop - xLeftBottom),
                (int)(yRightTop - yLeftBottom));

            cv::rectangle(frame, object, Scalar(114, 159, 207),2);

            String label = classNames[objectClass] + ":"+conf;

            int baseLine = 0;
            Size labelSize = getTextSize(label, FONT_HERSHEY_SIMPLEX, 0.5, 1, &baseLine);
            cv::rectangle(frame, Rect(Point(xLeftBottom, yLeftBottom - labelSize.height),
                Size(labelSize.width, labelSize.height + baseLine)),
                Scalar (114, 159, 207), cv::FILLED);
            putText(frame, label, Point(xLeftBottom, yLeftBottom),
                cv::FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 0));
        }
    }

   namedWindow("image", cv::WINDOW_NORMAL);
   imshow("image", frame);
   char c=(char)waitKey(1);
   if(c==27){
       cap.release();
        break;}
    }
}


#endif // MOBILESSD_H

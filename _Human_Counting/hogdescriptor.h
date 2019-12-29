#ifndef HOGDESCRIPTOR_H
#define HOGDESCRIPTOR_H
#include "opencv2/opencv.hpp"
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/videoio.hpp"
#include <opencv2/objdetect.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/video.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>

using namespace cv;
using namespace std;

std::vector<Rect> found, found_filtered;




inline void detectAndDraw(const HOGDescriptor &hog, Mat &img)
{
    found.clear(); found_filtered.clear();
    double time = (double) getTickCount();
    hog.detectMultiScale(img, found, 0, Size(8,8), Size(16,16), 1.07, 2);
    //time = (double) getTickCount() - time;
    //cout << "detection time = " << (time*1000./cv::getTickFrequency()) << " ms" << endl;

    for(size_t i = 0; i < found.size(); i++ )
    {
        Rect r = found[i];
        size_t j;
        // Do not add small detections inside a bigger detection.
        for ( j = 0; j < found.size(); j++ )
            if ( j != i && (r & found[j]) == r )
                break;
        if ( j == found.size() )
            found_filtered.push_back(r);
    }
    for (size_t i = 0; i < found_filtered.size(); i++)
    {
        Rect r = found_filtered[i];
        // The HOG detector returns slightly larger rectangles than the real objects,
        // so we slightly shrink the rectangles to get a nicer output.
        r.x += cvRound(r.width*0.1);
        r.width = cvRound(r.width*0.8);
        r.y += cvRound(r.height*0.07);
        r.height = cvRound(r.height*0.8);
        cv::rectangle(img, r.tl(), r.br(), cv::Scalar(0,255,0), 3);
    }

}






#endif // HOGDESCRIPTOR_H

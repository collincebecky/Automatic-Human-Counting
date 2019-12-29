#ifndef PEOPLE_COUNT_TEST_HPP
#define PEOPLE_COUNT_TEST_HPP

#include <opencv2/core.hpp>
#include <opencv2/bgsegm.hpp>
#include <opencv2/tracking.hpp>

#include <QVector>

#include <memory>

class blob_detector_base;
class tracker_base;

class people_count_test
{
public:
    people_count_test();

    ~people_count_test();

    QVector<int> people_in_out();

    void track(cv::Mat & img_);
private:
    QVector<int> people;

    void draw_measure_line(cv::Mat & img_);
    void init_data();
    void put_inout_msg(cv::Mat & img_);
    void track_people(cv::Mat const &kernel);
    int OUT=0;
    int IN= 0;
    std::unique_ptr<blob_detector_base> blob_detector_;
    cv::Mat gray_img_;
    //cv::Mat img_;
    size_t in_blob_size_ = 0;
    double min_human_size_ = 4000;
    int detect_margin_ = 50;
    size_t out_blob_size_ = 0;
    std::unique_ptr<tracker_base> tracker_;
};

#endif // PEOPLE_COUNT_TEST_HPP

#ifndef BG_SUBTRACT_DETECTOR_HPP
#define BG_SUBTRACT_DETECTOR_HPP

#include "blob_detector_base.hpp"

#include <opencv2/bgsegm.hpp>

#include <memory>

class bg_subtract_detector : public blob_detector_base
{
public:

    bg_subtract_detector(cv::Ptr<cv::BackgroundSubtractor> bg_subtractor,
                         double blob_min_size = 4000,
                         double anchor_ratio = 0.5,
                         int detect_margin = 50,
                         cv::Mat const kernel = cv::getStructuringElement(cv::MORPH_RECT, {7,7}),
                         size_t neglect_frame_size = 10);
    ~bg_subtract_detector();

    void clear() override;

    /**
     * @brief detect interesting blob in the screen
     * @param input image want to detect interesting blob
     */
    void detect_blob(cv::Mat const &input) override;

    /**
     * @brief get interesting regions
     * @return bounding boxes of interesting regions
     */
    std::vector<cv::Rect> const& get_roi() const override;

private:
    struct impl;

    std::unique_ptr<impl> pimpl_;
};

#endif // BG_SUBTRACT_DETECTOR_HPP

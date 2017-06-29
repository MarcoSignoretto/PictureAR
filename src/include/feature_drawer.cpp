//
// Created by Marco Signoretto on 23/06/2017.
//

#include "feature_drawer.h"
#include <iostream>
#include <assert.h>
#include <opencv2/imgproc.hpp>

void mcv::feature_drawer::draw_rect(cv::Mat& dst, const std::vector<cv::Point>& rect) {
    assert(dst.channels() == 3 && "Invalid channel number");

    cv::rectangle(dst,rect[0],rect[1],cv::Scalar(255,0,255));
}

//
// Created by Marco Signoretto on 23/06/2017.
//

#ifndef PROJECT_FEATURE_DRAWER_H
#define PROJECT_FEATURE_DRAWER_H


#include <opencv2/core/mat.hpp>

namespace mcv{
    namespace feature_drawer{
        // TODO implements
        void draw_rect(cv::Mat& dst, const std::vector<cv::Point>& rect);

    }
}


#endif //PROJECT_FEATURE_DRAWER_H

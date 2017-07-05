//
// Created by Marco Signoretto on 23/06/2017.
//

#ifndef PROJECT_FEATURE_DRAWER_H
#define PROJECT_FEATURE_DRAWER_H


#include <opencv2/core/mat.hpp>

namespace mcv{
    namespace feature_drawer{

        /**
         * This function wrap rectancle function of openCV
         * @param dst: colored image where write
         * @param rect: rectangle to write
         */
        void draw_rect(cv::Mat& dst, const std::vector<cv::Point>& rect);

    }
}


#endif //PROJECT_FEATURE_DRAWER_H

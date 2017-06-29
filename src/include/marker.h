//
// Created by Marco Signoretto on 29/06/2017.
//

#ifndef PROJECT_MARKER_H
#define PROJECT_MARKER_H

#include <opencv2/core/mat.hpp>

namespace mcv{
    namespace marker{
        const int WIDTH = 135;
        const int HEIGHT = 55;

        const int OFFSET = 53; // marker border size
        const std::vector< cv::Point > RECT_0 = {
                cv::Point(256-(OFFSET+WIDTH), 256-(OFFSET+HEIGHT)),
                cv::Point(256-OFFSET, 256-OFFSET)
        };
        const std::vector< cv::Point > RECT_90 = {
                cv::Point(256-(OFFSET+HEIGHT), OFFSET),
                cv::Point(256-OFFSET, OFFSET+WIDTH)
        };
        const std::vector< cv::Point > RECT_180 = {
                cv::Point(OFFSET, OFFSET),
                cv::Point(OFFSET+WIDTH, OFFSET+HEIGHT)
        };
        const std::vector< cv::Point > RECT_270 = {
                cv::Point(OFFSET, 256-(OFFSET+WIDTH)),
                cv::Point(OFFSET+HEIGHT, 256-OFFSET)
        };

        const int BLACK = 0;
        const int WHITE = 255;

        //TODO complete
        /**
         * This image must be thresholded and 256x256
         * @param warped_image
         * @return 0,90,180,270 degree of rotation respect to original marker
         */
        int detect_orientation(const cv::Mat& warped_image);

        cv::Mat calculate_rotation_matrix(int rotation_degree);

        void rotate(cv::Mat& img, int rotation_degree);

    }
}

#endif //PROJECT_MARKER_H

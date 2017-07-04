//
// Created by Marco Signoretto on 29/06/2017.
//

#ifndef PROJECT_MARKER_H
#define PROJECT_MARKER_H

#include <opencv2/core/mat.hpp>

namespace mcv{
    namespace marker{

        const int BOUNDARY_MIN_LENGTH = 200;
        const int BOUNDARY_MAX_LENGTH = 1200;

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

        const std::vector<cv::Vec2d> DST_POINTS = {
                cv::Vec2d(0, 0),
                cv::Vec2d(256, 0),
                cv::Vec2d(256, 256),
                cv::Vec2d(0, 256),
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

        void calculate_rotation_matrix(cv::Mat& rotation_matrix, int rotation_degree);

        void calculate_picture_rotation(cv::Mat& rotation_matrix, int rotation_degree);

        void rotate(cv::Mat& img, int rotation_degree);

        float compute_matching(const cv::Mat& marker_extracted, const cv::Mat& marker_candidate);

        /**
         * This function execute the pipeline to apply AR to the original image "frame", the pipeline is the following:
         * 1) Read frame and convert in grayscale
         * 2) Apply Otzu threshold to grayscale image
         * 3) Extract image boundaries
         * 4) Filter the above boundaries with length between BOUNDARY_MIN_LENGTH and BOUNDARY_MAX_LENGTH
         * 5) Create image with filtered boundaries
         * 6) Apply harris corner into the above image
         * 7) Compute which pixels of the boundaries are corners
         * 8) Filter boundaries which have 4 corners
         *
         * @param img_0p: image placeholder 0 ( leo picture )
         * @param img_1p: image placeholder 1 ( van picture )
         * @param img_0m_th: image marker 0 thresholded ( leo marker )
         * @param img_1m_th: image marker 1 thresholded ( van marker )
         * @param frame: original image on which AR will be applied
         * @param debug_info: if true additional images will be shown with deboug pourpose
         */
        void apply_AR(const cv::Mat& img_0p, const cv::Mat& img_1p, const cv::Mat& img_0m_th, const cv::Mat& img_1m_th, cv::Mat& frame, bool debug_info);

    }
}

#endif //PROJECT_MARKER_H

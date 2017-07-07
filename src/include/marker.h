//
// Created by Marco Signoretto on 29/06/2017.
//

#ifndef PROJECT_MARKER_H
#define PROJECT_MARKER_H

#include <opencv2/core/mat.hpp>
#include <MacTypes.h>

namespace mcv{
    namespace marker{

        /// Constants related to boundary length filter
        const int BOUNDARY_MIN_LENGTH = 200;
        const int BOUNDARY_MAX_LENGTH = 1500;//1200;

        /// Threshold value for marker matching
        /* This is a very important parameter:
         * value must be between 0.0 and 1.0 and is the threshold of probability that is a given marker.
         * value close to 1.0:
         *      - easier to reject marker as candidate because we request that marker extracted must be very similar to
         *        the original, in this configuration its difficult to confuse between the two markers
         * value far away for 1.0:
         *      - easier to consider a candidate marker as a marker also if "noised" but in this configuration its more
         *        probable to have confusion between the two markers
         * In this project I have chosen a value close to 1.0 because I would like to see correct marker or not at all,
         * if you prefer to see markers also if there is higer probability to confuse them you are free to change this
         * parameter ( for example 0.85 ensure strong detection also on fast movement or high inclination but
         * maybe you will see the wrong marker )
         *
         * */
        const float MATCH_THRESHOLD = 0.90f;
        /// Constants related to marker orientation detection
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

        const std::vector< cv::Point > RECT_MATCHING = {
                cv::Point(OFFSET+10, 256-(OFFSET+HEIGHT)),
                cv::Point(256-OFFSET-80, 256-OFFSET)
        };

        const std::vector<cv::Vec2d> DST_POINTS = {
                cv::Vec2d(0, 0),
                cv::Vec2d(256, 0),
                cv::Vec2d(256, 256),
                cv::Vec2d(0, 256),
        };

        // Constant for BLACK pixel in thresholded image
        const int BLACK = 0;

        /**
         * Detect orientation of a given marker and return its orientation in degree to obtain the original marker orientation
         * @param warped_image: image that shold be contain a marker to work properly (must be thresholded and 256x256)
         * @param
         * @return 0,90,180,270 degree of rotation respect to original marker
         */
        int detect_orientation(const cv::Mat& warped_image);

        /**
         * This function given the "rotation_degree" obtained from detect_orientation function calculate the rotation
         * matrix which will be saved into rotation_matrix
         * @see detect_orientation
         * @param rotation_matrix: reference of matrix where rotation matrix will be setted
         * @param rotation_degree: rotation in degree to obtain the original marker orientation
         */
        void calculate_rotation_matrix(cv::Mat& rotation_matrix, int rotation_degree);

        /**
         * This function works as calculate_rotation_matrix but compute matrix used to rotate placeholder before warp
         * @see calculate_rotation_matrix
         * @param rotation_matrix: reference of matrix where rotation matrix will be setted
         * @param rotation_degree: rotation in degree to obtain the original marker orientation
         */
        void calculate_picture_rotation(cv::Mat& rotation_matrix, int rotation_degree);

        /**
         * This function compute the probability of a certain marker ( marker_extracted ) to be the "marker_candidate"
         * @param marker_extracted: marker extracted from frame
         * @param marker_candidate: one of the marker for the pictures ( OM or 1M )
         * @return probability that the two markers are the same
         */
        float compute_matching(const cv::Mat& marker_extracted, const cv::Mat& marker_candidate, cv::Point top_left = cv::Point(0,0), cv::Point bottom_right = cv::Point(256,256));



        /**
         * This function execute the pipeline to apply AR to the original image "camera_frame", the pipeline is the following:
         * 0) Convert originale frame and convert in grayscale
         * 1) Apply GaussianBlur to remove noise
         * 2) Apply Otzu threshold to grayscale image
         * 3) Extract image boundaries
         * 4) Filter the above boundaries with length between BOUNDARY_MIN_LENGTH and BOUNDARY_MAX_LENGTH
         * 5) Create image with filtered boundaries
         * 6) Apply harris corner into the above image
         * 7) Compute which pixels of the boundaries are corners
         * 8) Keep only boundaries which have 4 corners
         * For each boundary:
         * 9) find homography and warp image into a 256x256 image
         * 10) detect marker orientation ( in this step also other candidate marker has been rotate because final filtering is applied during matching phase )
         * 11) calculate rotation for placeholder
         * 12) rotate marker in order to perform matching
         * 13) compute matching coefficient
         * 14) warp placeholder with higer probability into original image if matching is above MATCH_THRESHOLD
         *
         * @param img_0p: image placeholder 0 ( leo picture )
         * @param img_1p: image placeholder 1 ( van picture )
         * @param img_0m_th: image marker 0 thresholded ( leo marker )
         * @param img_1m_th: image marker 1 thresholded ( van marker )
         * @param camera_frame: original image on which AR will be applied
         * @param debug_info: if true additional images will be shown with deboug pourpose
         */
        void apply_AR(const cv::Mat& img_0p, const cv::Mat& img_1p, const cv::Mat& img_0m_th, const cv::Mat& img_1m_th, cv::Mat& camera_frame,  bool debug_info);

    }
}

#endif //PROJECT_MARKER_H

//
// Created by Marco Signoretto on 29/06/2017.
//

#include "marker.h"
#include "utils.h"
#include "boundary_extractor.h"
#include <stdexcept>
#include <iostream>
#include <assert.h>
#include <opencv2/imgproc.hpp>
#include <opencv/cv.hpp>


int mcv::marker::detect_orientation(const cv::Mat& warped_image) {
    // TODO clean code using vector of rect and vector of accumulator
    int res = 0;
    int max = 0;
    int accumulator_0 = 0;
    int accumulator_90 = 0;
    int accumulator_180 = 0;
    int accumulator_270 = 0;
    // TODO define rect for accumulators
    int y,x;
    const uchar* p;
    for( y = OFFSET; y < warped_image.rows-OFFSET; ++y) {
        p = warped_image.ptr<uchar>(y);
        for ( x = OFFSET; x < warped_image.cols-OFFSET; ++x) {
            if(p[x] == mcv::marker::BLACK){
                if(x > RECT_0[0].x && x < RECT_0[1].x && y > RECT_0[0].y && y < RECT_0[1].y){
                    ++accumulator_0;
                }
                if(x > RECT_90[0].x && x < RECT_90[1].x && y > RECT_90[0].y && y < RECT_90[1].y){
                    ++accumulator_90;
                }
                if(x > RECT_180[0].x && x < RECT_180[1].x && y > RECT_180[0].y && y < RECT_180[1].y){
                    ++accumulator_180;
                }
                if(x > RECT_270[0].x && x < RECT_270[1].x && y > RECT_270[0].y && y < RECT_270[1].y){
                    ++accumulator_270;
                }
            }
        }
    }

    max = accumulator_0;
    if(accumulator_90 > max){
        max = accumulator_90;
        res = 90;
    }
    if(accumulator_180 > max){
        max = accumulator_180;
        res = 180;
    }
    if(accumulator_270 > max){
        max = accumulator_270; // TODO shold be removed
        res = 270;
    }


    return res;// TODO change
}

void mcv::marker::calculate_rotation_matrix(cv::Mat& rotation_matrix, int rotation_degree) {
    float radiants = 0.0;
    float offset_x = 0.0;
    float offset_y = 0.0;
    switch(rotation_degree){
        case 0:
            break;
        case 90:
            radiants = (float)(CV_PI/2.0);
            offset_x = 256.0;
            offset_y = 0.0;
            break;
        case 180:
            radiants = (float)CV_PI;
            offset_x = 256.0;
            offset_y = 256.0;
            break;
        case 270:
            radiants = (float)((3.0/2.0)*CV_PI);
            offset_x = 0.0;
            offset_y = 256.0;
            break;
        default:
            throw std::invalid_argument("only 0,90,180,270 degree are supported");
    }
    // TODO init matrix in another way, in this way raw_data allocated in stack and mat create only reference no copy ( iterate with pointer to populate) rotation_matrix = cv::Mat(3, 3, CV_32F, raw_data doesn't work);
    float raw_data[] = {
            (float)cos(radiants), (float)-sin(radiants), offset_x,
            (float)sin(radiants), (float)cos(radiants), offset_y,
            0.0, 0.0, 1.0
    };
    rotation_matrix = cv::Mat(3, 3, CV_32F);
    rotation_matrix.at<float>(0,0) = raw_data[0];
    rotation_matrix.at<float>(0,1) = raw_data[1];
    rotation_matrix.at<float>(0,2) = raw_data[2];
    rotation_matrix.at<float>(1,0) = raw_data[3];
    rotation_matrix.at<float>(1,1) = raw_data[4];
    rotation_matrix.at<float>(1,2) = raw_data[5];
    rotation_matrix.at<float>(2,0) = raw_data[6];
    rotation_matrix.at<float>(2,1) = raw_data[7];
    rotation_matrix.at<float>(2,2) = raw_data[8];


}

void mcv::marker::rotate(cv::Mat &img, int rotation_degree) {

    // TODO complete


}

void mcv::marker::calculate_picture_rotation(cv::Mat &rotation_matrix, int rotation_degree) { // TODO understand better because this happens
    if(rotation_degree == 90 || rotation_degree == 270) rotation_degree = (rotation_degree+180)%360;
    calculate_rotation_matrix(rotation_matrix, rotation_degree);// TODO optimize, avoid to calculate new matrix
}

float mcv::marker::compute_matching(const cv::Mat &marker_extracted, const cv::Mat &marker_candidate) {

    assert(marker_extracted.rows == marker_candidate.rows && marker_extracted.cols == marker_candidate.cols && "Dimensions mismatch");
    int sum = 0;
    int max = marker_extracted.rows*marker_extracted.cols;

    const uchar *p_marker_extracted;
    const uchar *p_marker_candidate;
    for(int y = 0; y < marker_extracted.rows; ++y) {
        p_marker_extracted = marker_extracted.ptr<uchar>(y);
        p_marker_candidate = marker_candidate.ptr<uchar>(y);
        for (int x = 0; x < marker_extracted.cols; ++x) {
            if(p_marker_extracted[x] == p_marker_candidate[x])++sum;
        }
    }

    return (float)sum/(float)max;
}

void mcv::marker::apply_AR(const cv::Mat& img_0p, const cv::Mat& img_1p, const cv::Mat& img_0m_th, const cv::Mat& img_1m_th, cv::Mat& frame, bool debug_info) {
    cv::Mat frame_debug;
    cv::Mat grayscale;
    cv::Mat boundaries_img;


    if(debug_info) {
        frame_debug = frame.clone();
    }

    ///=== STEP 1 ===
    // Convert original image into gray scale
    cv::cvtColor(frame, grayscale, CV_RGB2GRAY);

    ///=== STEP 2 ===
    //Calculate threshold image from the gray scale
    int max_value;
    cv::Mat hist_ = mcv::compute_hist(grayscale, max_value);
    cv::Mat normHist = mcv::normalize_hist(hist_, grayscale);
    int threshold = mcv::compute_Otsu_thresholding(normHist);
    cv::Mat frame_th = mcv::image_threshold(threshold, grayscale);

    ///=== STEP 3 ===
    // Boundary extraction
    mcv::boundary_extractor be(grayscale);
    be.find_boundaries();

    ///=== STEP 4 ===
    be.keep_between(BOUNDARY_MIN_LENGTH, BOUNDARY_MAX_LENGTH);

    ///=== STEP 5 ===
    be.create_boundaries_image(boundaries_img);// 1 pixel of padding

    ///=== STEP 6 ===
    //===detect corners of the boundaries with harris corner ===
    cv::Mat img_corners = cv::Mat::zeros(boundaries_img.rows, boundaries_img.cols, CV_32FC1); // float values
    int block_size = 11;//Good 7
    int kernel_size = 7;// Good 5
    float free_parameter = 0.05; // more little more corners will be found
    cv::cornerHarris(boundaries_img, img_corners, block_size, kernel_size, free_parameter,
                     cv::BorderTypes::BORDER_DEFAULT);

    ///=== STEP 7 ===
    //search corners in img_corners
    be.compute_corners(img_corners);
    ///=== STEP 8 ===
    be.keep_between_corners(4, 4);

    //========== HOMOGRAPHY =============
    std::vector<mcv::boundary> &boundaries = be.get_boundaries();
    for (mcv::boundary &boundary : boundaries) {
        cv::Mat warped_img;

        ///=== STEP 9 ===
        // find Homography
        std::vector<cv::Vec2d> corners;
        for (cv::Vec2i &corner : boundary.corners) {
            corners.push_back(cv::Vec2d(corner[0], corner[1]));
        }
        cv::Mat H = cv::findHomography(corners, mcv::marker::DST_POINTS);
        cv::warpPerspective(frame_th, warped_img, H.inv(), cv::Size(256, 256), cv::WARP_INVERSE_MAP, cv::BORDER_DEFAULT);
        ///=== STEP 10 ===
        // Detect orientation
        int orientation = mcv::marker::detect_orientation(warped_img);


        // Calculate rotation matrixes
        cv::Mat rotation_matrix;
        cv::Mat picture_rotation;
        mcv::marker::calculate_rotation_matrix(rotation_matrix, orientation); // TODO optimize this code
        ///=== STEP 11 ===
        mcv::marker::calculate_picture_rotation(picture_rotation, orientation);

        ///=== STEP 12 ===
        // Set marker in correct orientation for matching
        cv::warpPerspective(warped_img, warped_img, rotation_matrix.inv(), cv::Size(256, 256),
                            cv::WARP_INVERSE_MAP, cv::BORDER_DEFAULT);

        // TODO continue doc here

        // ============ MATCHING

        float match_0m = mcv::marker::compute_matching(img_0m_th, warped_img);
        float match_1m = mcv::marker::compute_matching(img_1m_th, warped_img);



        if (match_0m > 0.92 || match_1m > 0.92) {

            const cv::Mat &matched_image = (match_0m > match_1m) ? img_0p : img_1p;

            cv::Mat output_img;

            cv::warpPerspective(matched_image, output_img, picture_rotation, cv::Size(256, 256));
            cv::warpPerspective(output_img, frame, H, cv::Size(frame.cols, frame.rows),
                                cv::WARP_INVERSE_MAP,
                                cv::BORDER_TRANSPARENT);
        }

        if(debug_info){
            cv::imshow("warped_marker", warped_img);
            cout << "LEO: " << match_0m << ", VAN: " << match_1m << endl;
        }

    }

    if (debug_info) {
        be.draw_boundaries(frame_debug);
        be.draw_boundaries_corners(frame_debug);

        cv::imshow("corners", img_corners);
        cv::imshow("live", frame_debug);
    }
}



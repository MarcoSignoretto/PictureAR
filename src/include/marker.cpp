//
// Created by Marco Signoretto on 29/06/2017.
//

#include "marker.h"
#include "utils.h"
#include "boundary_extractor.h"
#include <assert.h>
#include <opencv2/imgproc.hpp>
#include <opencv/cv.hpp>



int mcv::marker::detect_orientation(const cv::Mat& warped_image) {

    int res = 0;
    int max = -1;
    /*
     * accumutators[0] = 0 degree accumulator
     * accumutators[1] = 90 degree accumulator
     * accumutators[2] = 180 degree accumulator
     * accumutators[3] = 270 degree accumulator
     */
    int accumulators[] = {0, 0, 0, 0};
    int y,x;
    const uchar* p;
    for( y = OFFSET; y < warped_image.rows-OFFSET; ++y) {
        p = warped_image.ptr<uchar>(y);
        for ( x = OFFSET; x < warped_image.cols-OFFSET; ++x) {
            // If pixel black add it into accumulator of correct region
            if(p[x] == BLACK){
                if(x > RECT_0[0].x && x < RECT_0[1].x && y > RECT_0[0].y && y < RECT_0[1].y){
                    ++(accumulators[0]);
                }
                if(x > RECT_90[0].x && x < RECT_90[1].x && y > RECT_90[0].y && y < RECT_90[1].y){
                    ++(accumulators[1]);
                }
                if(x > RECT_180[0].x && x < RECT_180[1].x && y > RECT_180[0].y && y < RECT_180[1].y){
                    ++(accumulators[2]);
                }
                if(x > RECT_270[0].x && x < RECT_270[1].x && y > RECT_270[0].y && y < RECT_270[1].y){
                    ++(accumulators[3]);
                }
            }
        }
    }

    // Find orientation considering accumulator with higher value.
    for(int i=0; i<4; ++i){
        if(accumulators[i] > max){
            max = accumulators[i];
            res = 90*i;
        }
    }
    return res;
}

void mcv::marker::calculate_rotation_matrix(cv::Mat& rotation_matrix, int rotation_degree, const bool rotation_update) {
    float radiants = 0.0f;
    float offset_x = 0.0f;
    float offset_y = 0.0f;
    // Convert orientation into radiants and compute offset necessary to have rotation respect to center
    switch(rotation_degree){
        case 0:
            break;
        case 90:
            radiants = (float)(CV_PI/2.0);
            offset_x = 256.0f;
            offset_y = 0.0f;
            break;
        case 180:
            radiants = (float)CV_PI;
            offset_x = 256.0f;
            offset_y = 256.0f;
            break;
        case 270:
            radiants = (float)((3.0f/2.0f)*CV_PI);
            offset_x = 0.0f;
            offset_y = 256.0f;
            break;
        default:
            throw std::invalid_argument("only 0,90,180,270 degree are supported");
    }
    // Init raw
    const float raw_data[] = {
            cos(radiants), -sin(radiants), offset_x,
            sin(radiants), cos(radiants), offset_y,
            0.0f, 0.0f, 1.0f
    };

    // Create matrix if this isn't and update
    if(!rotation_update) {
        rotation_matrix = cv::Mat(3, 3, CV_32F);
    }

    // Iterate on raw data and matrix to populate or update it
    float *p;
    for(int y = 0; y < rotation_matrix.rows; ++y) {
        p = rotation_matrix.ptr<float>(y);
        for (int x = 0; x < rotation_matrix.cols; ++x) {
            p[x] = raw_data[rotation_matrix.cols*y+x]; // copy raw data value into matrix
        }
    }
}

void mcv::marker::calculate_picture_rotation(cv::Mat &rotation_matrix, int rotation_degree) {
    // For picture respect marker we need to flip if degrees are 90 0 270
    if(rotation_degree == 90 || rotation_degree == 270){
        // Calculate orientation of picture given orientation of marker
        rotation_degree = (rotation_degree+180)%360;
        calculate_rotation_matrix(rotation_matrix, rotation_degree, true); // Update rotation matrix
    }
}

float mcv::marker::compute_matching(const cv::Mat &marker_extracted, const cv::Mat &marker_candidate, const cv::Point top_left, const cv::Point bottom_right) {

    assert(marker_extracted.rows == marker_candidate.rows && marker_extracted.cols == marker_candidate.cols && "Dimensions mismatch");

    int sum = 0;
    int max = (bottom_right.x-top_left.x)*(bottom_right.y-top_left.y);

    // For each pixels campare them and add one if they are equals
    const uchar *p_marker_extracted;
    const uchar *p_marker_candidate;
    for(int y = top_left.y; y < bottom_right.y; ++y) {
        p_marker_extracted = marker_extracted.ptr<uchar>(y);
        p_marker_candidate = marker_candidate.ptr<uchar>(y);
        for (int x = top_left.x; x < bottom_right.x; ++x) {
            if(p_marker_extracted[x] == p_marker_candidate[x])++sum;
        }
    }

    // Normalize sum in order to convert into probability
    return (float)sum/(float)max;
}

void mcv::marker::apply_AR(const cv::Mat& img_0p, const cv::Mat& img_1p, const cv::Mat& img_0m_th, const cv::Mat& img_1m_th, cv::Mat& camera_frame, bool debug_info) {
    cv::Mat frame_debug;
    cv::Mat grayscale;
    cv::Mat unblured_grayscale;
    cv::Mat frame_th;
    cv::Mat unblured_frame_th;
    cv::Mat boundaries_img; // 1px larger than camera_frame

    int threshold;
    int max_value;

    ///=== STEP 0 ===
    // Convert original image into gray scale image
    cv::cvtColor(camera_frame, unblured_grayscale, CV_RGB2GRAY);

    ///=== STEP 1 ===
    int sigma = 3;
    int ksize = 7;
    cv::GaussianBlur(unblured_grayscale, grayscale, cv::Size(ksize,ksize), sigma);

    if(debug_info) {
        frame_debug = camera_frame.clone();
    }

    ///=== STEP 2 ===
    //Calculate threshold image from the gray scale
    cv::Mat hist_ = mcv::compute_hist(grayscale, max_value);
    cv::Mat normHist = mcv::normalize_hist(hist_, grayscale);
    threshold = mcv::compute_Otsu_thresholding(normHist);
    frame_th = mcv::image_threshold(threshold, grayscale);
    // Unblured thresholded image ( use same threshold of the previous one )
    unblured_frame_th = mcv::image_threshold(threshold, unblured_grayscale);

    ///=== STEP 3 ===
    // Boundary extraction
    mcv::boundary_extractor be(frame_th, false);
    be.find_boundaries(mcv::BLACK);

    ///=== STEP 4 ===
    be.keep_between(BOUNDARY_MIN_LENGTH, BOUNDARY_MAX_LENGTH);

    ///=== STEP 5 ===
    be.create_boundaries_image(boundaries_img);// 1 pixel of padding

    ///=== STEP 6 ===
    //===detect corners of the boundaries with harris corner (WARNING both images has 1px of padding respect the original one )
    cv::Mat img_corners = cv::Mat::zeros(boundaries_img.rows, boundaries_img.cols, CV_32FC1); // float values
    // FIXME continue from here
    // TODO decide how create corner in boundaries,
    // TODO maybe good idea fill boundaries?? attention if wrong boundary contains markers!!!
    // TODO try to remove gaussian blur ( but maybe can help to remove noise )
    // TODO check draw boundaries exception
    /*int block_size = 11;//11;//Good 7
    int kernel_size = 7;//7;// Good 5
    float free_parameter = 0.05f; // more little more corners will be found
    cv::cornerHarris(boundaries_img, img_corners, block_size, kernel_size, free_parameter, cv::BorderTypes::BORDER_DEFAULT);*/
    int block_size = 7;//11;//Good 7
    int kernel_size = 5;//7;// Good 5
    float free_parameter = 0.05f;
    cv::cornerHarris(unblured_frame_th, img_corners, block_size, kernel_size, free_parameter,
                     cv::BorderTypes::BORDER_DEFAULT);

    cv::Mat corners_th = cv::Mat::zeros(img_corners.rows, img_corners.cols, CV_8UC1);

    for(int y = 0; y < img_corners.rows; ++y ){

        for(int x = 0; x < img_corners.cols; ++x ){
            float f = img_corners.at<float>(y,x);
            if( f > 0.2f) corners_th.at<uchar>(y,x) = 255;
        }
    }




    //img_corners.convertTo(corners_th, CV_8UC1);
    cv::imshow("Conversion", corners_th);
    //cv::waitKey(0);

    /*cv::Mat labels;
    cv::Mat stats;
    cv::Mat centroids;
    cv::connectedComponentsWithStats(corners_th,labels,stats, centroids);
    cout << "centroids: " << centroids.rows << ", " << centroids.cols << endl;
    cout << centroids << endl;

    centroids.convertTo(centroids, CV_32F); // TODO is correct detection ( yes improve!!!) Try to negative of the gray scale image maybe improve
    const cv::TermCriteria criteria = cv::TermCriteria(cv::TermCriteria::EPS + cv::TermCriteria::MAX_ITER, 100, 0.001);
    cv::cornerSubPix(unblured_frame_th,centroids,cv::Size(5,5),cv::Size(-1,-1),criteria);


    cv::Mat centroids_img = cv::Mat::zeros(img_corners.rows, img_corners.cols, CV_8UC1); // Img
    for(int y = 0; y < centroids.rows; ++y ){
        const float* c = centroids.ptr<float>(y); // FIXme double or float??
        centroids_img.at<uchar>((int)c[1],(int)c[0]) = 255;


        int offset = 1; // Used to make corners visible ( bigger than a pixel )
        int j_origin = (int)c[0];
        int i_origin = (int)c[1];

        // Remove all color from blue and green channel and add full color to green channel
        // Corners are drawn with "offset" has extra size to make them more visible
        for (int i = i_origin-offset; i<=i_origin+offset; ++i) {
            if (i >= 0 && i < frame_debug.rows) {
                for (int j = j_origin - offset; j <= j_origin + offset; ++j) {
                    if (j >= 0 && j < frame_debug.cols) {
                        cv::Vec3b &intensity = frame_debug.at<cv::Vec3b>(i, j);

                        intensity[0] = 255;
                        intensity[1] = 0;
                        intensity[2] = 0;
                    }
                }
            }
        }


    }*/





    // TODO end test

    ///=== STEP 7 ===
    //search corners in img_corners
    be.compute_corners(img_corners);
    ///=== STEP 8 ===
    be.keep_between_corners(4, 4);


    // TODO new test
    cv::Mat corner_matrix;
    be.corners_to_matrix(corner_matrix);
    cout << corner_matrix << endl;

    const cv::TermCriteria criteria_new = cv::TermCriteria(cv::TermCriteria::EPS + cv::TermCriteria::MAX_ITER, 100, 0.001);
    cv::cornerSubPix(unblured_frame_th,corner_matrix,cv::Size(5,5),cv::Size(-1,-1),criteria_new);
    //be.matrix_to_corners(corner_matrix);

    cout << corner_matrix << endl;

    cv::Mat centroids_img = cv::Mat::zeros(img_corners.rows, img_corners.cols, CV_8UC1); // Img
    for(int y = 0; y < corner_matrix.rows; ++y ){
        const float* c = corner_matrix.ptr<float>(y); // FIXme double or float??
        centroids_img.at<uchar>((int)c[1],(int)c[0]) = 255;


        int offset = 1; // Used to make corners visible ( bigger than a pixel )
        int j_origin = (int)c[0];
        int i_origin = (int)c[1];

        // Remove all color from blue and green channel and add full color to green channel
        // Corners are drawn with "offset" has extra size to make them more visible
        for (int i = i_origin-offset; i<=i_origin+offset; ++i) {
            if (i >= 0 && i < frame_debug.rows) {
                for (int j = j_origin - offset; j <= j_origin + offset; ++j) {
                    if (j >= 0 && j < frame_debug.cols) {
                        cv::Vec3b &intensity = frame_debug.at<cv::Vec3b>(i, j);

                        intensity[0] = 255;
                        intensity[1] = 0;
                        intensity[2] = 0;
                    }
                }
            }
        }


    }

    cv::imshow("Centroids", centroids_img);
    cv::imshow("live", frame_debug);
    //cv::waitKey(0);

    // TODo end new test

    //========== HOMOGRAPHY =============
    // All homography operation are applied into unblured image
    // warp has benn computed in inverse_map configuration to avoid white hole.
    std::vector<mcv::boundary> &boundaries = be.get_boundaries();
    for (mcv::boundary &boundary : boundaries) {
        cv::Mat warped_img;
        cv::Mat rotation_matrix; // This matrix will be used to find rotation of placeholders and images



        ///=== STEP 9 ===
        // find Homography
        std::vector<cv::Vec2d> corners;
        for (cv::Vec2i &corner : boundary.corners) {
            corners.push_back(cv::Vec2d(corner[0], corner[1]));
        }
        cv::Mat H = cv::findHomography(corners, mcv::marker::DST_POINTS);
        //cv::warpPerspective(frame_th, warped_img, H.inv(), cv::Size(256, 256), cv::WARP_INVERSE_MAP, cv::BORDER_DEFAULT);
        cv::warpPerspective(unblured_frame_th, warped_img, H.inv(), cv::Size(256, 256), cv::WARP_INVERSE_MAP, cv::BORDER_DEFAULT);
        ///=== STEP 10 ===
        // Detect orientation
        int orientation = mcv::marker::detect_orientation(warped_img);

        ///=== STEP 11 ===
        // Calculate rotation matrixes ( and create it )
        mcv::marker::calculate_rotation_matrix(rotation_matrix, orientation);
        cv::warpPerspective(warped_img, warped_img, rotation_matrix.inv(), cv::Size(256, 256), cv::WARP_INVERSE_MAP, cv::BORDER_DEFAULT);

        ///=== STEP 12 ===
        // ============ MATCHING
        float match_0m = mcv::marker::compute_matching(img_0m_th, warped_img);
        float match_1m = mcv::marker::compute_matching(img_1m_th, warped_img);


        // Project placeholder with higher probability in original image if match above certain threshold
        ///=== STEP 13 ===
        if (match_0m > MATCH_THRESHOLD || match_1m > MATCH_THRESHOLD) {

            const cv::Mat &matched_image = (match_0m > match_1m) ? img_0p : img_1p;

            cv::Mat output_img;

            // Updated rotation matrix with picture rotation
            mcv::marker::calculate_picture_rotation(rotation_matrix, orientation);
            cv::warpPerspective(matched_image, output_img, rotation_matrix, cv::Size(256, 256));
            cv::warpPerspective(output_img, camera_frame, H, cv::Size(camera_frame.cols, camera_frame.rows), cv::WARP_INVERSE_MAP, cv::BORDER_TRANSPARENT);
        }

        if(debug_info){
            cv::imshow("warped_marker", warped_img);
            cout << "LEO: " << match_0m << ", VAN: " << match_1m << endl;
        }

    }

    // Shows debug images with filter thresholded and feature
    if (debug_info) {
        be.draw_boundaries(frame_debug);
        be.draw_boundaries_corners(frame_debug);

        cv::imshow("filtered_image", grayscale);
        cv::imshow("thresholded", frame_th);
        cv::imshow("corners", img_corners);
        cv::imshow("live", frame_debug);

    }
}



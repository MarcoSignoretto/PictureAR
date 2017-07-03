#include <iostream>
#include <opencv2/videoio.hpp>
#include <opencv/cv.hpp>
#include <exception>

#include "include/utils.h"
#include "include/speedometer.h"
#include "include/boundary_extractor.h"
#include "include/marker.h"
#include "include/feature_drawer.h"

using namespace std;

const std::string SOURCE_VIDEO = "video";
const std::string SOURCE_IMAGE = "image";
const std::string SOURCE_WEBCAM = "webcam";
const std::string SOURCE_DEFAULT  = SOURCE_WEBCAM;



const double theta = CV_PI / 180;
const double rho = 1;

const int THRESHOLD = 230; // experimental good threshold for current problem


void mouse_callback(int event, int x, int y, int flag, void* userdata){
    if(event==cv::EVENT_LBUTTONDOWN){
        std::vector<cv::Vec2d>* points = reinterpret_cast<std::vector<cv::Vec2d>* >(userdata);// put spaces between >>
        std::cout << "mouse event at: " << x << " " << y << std::endl;

        points -> push_back(cv::Vec2d(x, y));
    }

}

int main( int argc, char* argv[] ) {

    std::string frame_source = SOURCE_DEFAULT;
    std::string filename = "";
    int camera_index = 0;
    bool debug_info = false;

    // Skip program name
    int i = 1;
    while (i < argc) {
        const std::string argv_str(argv[i]);
        if(argv_str == "-s" || argv_str == "--source"){
            const std::string argv_str_1(argv[i+1]);
            if(argv_str_1 == SOURCE_VIDEO || argv_str_1 == SOURCE_IMAGE || argv_str_1 == SOURCE_WEBCAM){
                frame_source = argv_str_1;
                ++i;
            }else{
                throw invalid_argument("This type of source are not allowed, only video, image, webcam are valid sources");
            }
        }
        else if(argv_str == "-f" || argv_str == "--filename"){
            const std::string argv_str_1(argv[i+1]);
            filename = argv_str_1;
            ++i;
        }
        else if(argv_str == "-D" || argv_str == "--Debug"){
            debug_info = true;
        }
        else if(argv_str == "-c" || argv_str == "--camera"){
            const std::string argv_str_1(argv[i+1]);
            camera_index = std::stoi(argv_str_1);
        }
        ++i;
    }

    if(frame_source == SOURCE_IMAGE || frame_source == SOURCE_VIDEO){
        if(filename == "")throw invalid_argument("With source type video or image you must provide a valid filename as resource");
    }


    // Load Marker images and Placeholder
    const cv::Mat img_0p = cv::imread("data/0P.png");
    const cv::Mat img_1p = cv::imread("data/1P.png");
    const cv::Mat img_0m = cv::imread("data/0M.png", cv::IMREAD_GRAYSCALE);
    const cv::Mat img_1m = cv::imread("data/1M.png", cv::IMREAD_GRAYSCALE);

    //Threshold img_0m
    int max_value_img_0m;
    cv::Mat hist_img_0m = mcv::compute_hist(img_0m, max_value_img_0m);
    cv::Mat normHist_img_0m = mcv::normalize_hist(hist_img_0m, img_0m);
    int threshold_img_0m = mcv::compute_Otsu_thresholding(normHist_img_0m);
    const cv::Mat img_0m_th = mcv::image_threshold(threshold_img_0m, img_0m);

    // Threshold img_1m
    int max_value_img_1m;
    cv::Mat hist_img_1m = mcv::compute_hist(img_1m, max_value_img_1m);
    cv::Mat normHist_img_1m = mcv::normalize_hist(hist_img_1m, img_1m);
    int threshold_img_1m = mcv::compute_Otsu_thresholding(normHist_img_1m);
    const cv::Mat img_1m_th = mcv::image_threshold(threshold_img_1m, img_1m);

    //float match = mcv::marker::compute_matching(img_0m_th,img_0m_th); // resutl to 1 correct




    /*cv::imshow("0P",img_0p);
    cv::waitKey(0);

    cv::imshow("1P",img_1p);
    cv::waitKey(0);

    cv::imshow("0M",img_0m);
    cv::waitKey(0);

    cv::imshow("1M",img_1m);
    cv::waitKey(0);*/

    // ============ ROTATION TEST ================

    /*cv::Mat img_0m_color = cv::imread("data/0M.png");
    cv::Mat img_1m_color = cv::imread("data/1M.png");

    mcv::feature_drawer::draw_rect(img_0m_color,mcv::marker::RECT_0);

    cv::imshow("0M_color",img_0m_color);
    cv::waitKey(0);

    mcv::feature_drawer::draw_rect(img_1m_color,mcv::marker::RECT_0);
    mcv::feature_drawer::draw_rect(img_1m_color,mcv::marker::RECT_90);
    mcv::feature_drawer::draw_rect(img_1m_color,mcv::marker::RECT_180);
    mcv::feature_drawer::draw_rect(img_1m_color,mcv::marker::RECT_270);

    cv::imshow("1M_color",img_1m_color);
    cv::waitKey(0);

    int orientation = mcv::marker::detect_orientation(img_0m);

    cv::Mat rotation_matrix;
    mcv::marker::calculate_rotation_matrix(rotation_matrix, orientation);
    cout << rotation_matrix << endl;




    cv::Mat warped_img;
    cv::warpPerspective(img_1m_color, warped_img, rotation_matrix, cv::Size(256,256));

    cv::imshow("1M_color_",img_1m_color);
    cv::waitKey(0);

    cv::imshow("1M_color",warped_img);
    cv::waitKey(0);*/


    /*
     * 1) Identify possible marker
     * 2) Search best match with Models given, cv::warpPerspective can be used to warm images
     * The plane transformation is a homography ( we shold be able to estimate the homography matrix H )
     *
     *
     */

    //const std::string filename = "data/test2.jpg";










    /*


    //=== begin corners on img thresholded ==
    cv::Mat img_corners = cv::Mat::zeros(image.rows, image.cols, CV_32FC1);
    int block_size = 11;//Good 7
    int kernel_size = 7;// Good 5
    float free_parameter = 0.08; // more little more corners will be found
    cv::cornerHarris(image,img_corners,block_size, kernel_size, free_parameter,cv::BorderTypes::BORDER_DEFAULT);

    cv::imshow("image_corners",img_corners);
    cv::waitKey(0);
    //===END

    //=== begin canny on img thresholded ==
    cv::Mat img_canny = cv::Mat::zeros(image.rows, image.cols, CV_8UC1);

    cv::Canny(image, img_canny, 100, 200, 3);
    cv::imshow("image_canny",img_canny);
    cv::waitKey(0);
    //===END

    //=== begin corners on canny ==
    cv::Mat img_corners_canny = cv::Mat::zeros(image.rows, image.cols, CV_32FC1);
    block_size = 7;//Good 7
    kernel_size = 5;// Good 5
    free_parameter = 0.08; // more little more corners will be found
    cv::cornerHarris(image,img_corners,block_size, kernel_size, free_parameter,cv::BorderTypes::BORDER_DEFAULT);

    cv::imshow("image_corners_canny",img_corners_canny);
    cv::waitKey(0);*/
    //===END

    ///=== begin boundary extractor + corners ==
    if( frame_source == SOURCE_IMAGE) {

        cv::Mat input_img;
        input_img = cv::imread(filename);

        cv::imshow("image",input_img);

        cv::waitKey(0);

        const cv::Mat image_gray = cv::imread(filename, cv::IMREAD_GRAYSCALE);

        //Calculate histogram
        int max_value;
        cv::Mat hist_ = mcv::compute_hist(image_gray, max_value);

        // normalize histogram
        cv::Mat normHist = mcv::normalize_hist(hist_, image_gray);

        //compute OtsuThresholding
        int threshold = mcv::compute_Otsu_thresholding(normHist);

        //generate image from threshold
        cv::Mat frame_th = mcv::image_threshold(threshold, image_gray);

        cv::Mat fin_img;
        cv::Mat boundaries_img;

        // TODO extract thresholded image from boundaries
        mcv::boundary_extractor be(image_gray);
        be.find_boundaries();
        be.keep_between(300, 1000);
        //be.compute_corners();
        cout << "Boundaries: " << endl;
        be.print_boundary_lengths();
        be.create_boundaries_image(boundaries_img); // draw boundary image


        //===corners
        cv::Mat img_corners = cv::Mat::zeros(boundaries_img.rows, boundaries_img.cols, CV_32FC1); // float values
        int block_size = 11;//Good 7
        int kernel_size = 7;// Good 5
        float free_parameter = 0.05; // more little more corners will be found
        cv::cornerHarris(boundaries_img, img_corners, block_size, kernel_size, free_parameter,
                         cv::BorderTypes::BORDER_DEFAULT);

        //search corners in img_corners
        be.compute_corners(img_corners);
        be.keep_between_corners(4, 4);

        be.draw_boundaries(image_gray, fin_img);
        be.draw_boundaries_corners(fin_img);

        //cv::imshow("image_corners_boundary",img_corners);
        //cv::waitKey(0);

        //cv::imshow("live_boundaries", boundaries_img);
        //cv::waitKey(0);

        cv::imshow("live", fin_img);
        cv::waitKey(0);

        //===== threshold of input


        std::vector<mcv::boundary> &boundaries = be.get_boundaries();
        for (mcv::boundary &boundary : boundaries) {
            std::vector<cv::Vec2d> corners;
            for (cv::Vec2i &corner : boundary.corners) {
                corners.push_back(cv::Vec2d(corner[0], corner[1]));
            }


            std::vector<cv::Vec2d> dst_points = {
                    cv::Vec2d(0, 0),
                    cv::Vec2d(256, 0),
                    cv::Vec2d(256, 256),
                    cv::Vec2d(0, 256),
            };

            cv::Mat H = cv::findHomography(corners, dst_points);

            std::cout << H << std::endl;
            std::cout << H.inv() << std::endl; // inverse of the matrix




            cv::Mat warped_img;
            cv::warpPerspective(frame_th, warped_img, H.inv(), cv::Size(256, 256), cv::WARP_INVERSE_MAP,
                                cv::BORDER_DEFAULT);

            cv::imshow("warped_img", warped_img);
            cv::waitKey(0);

            // TODO find correct rotation
            int orientation = mcv::marker::detect_orientation(warped_img);

            cv::Mat rotation_matrix;
            cv::Mat picture_rotation;
            mcv::marker::calculate_rotation_matrix(rotation_matrix, orientation);
            mcv::marker::calculate_picture_rotation(picture_rotation, orientation);
            cout << rotation_matrix << endl;


            cv::warpPerspective(warped_img, warped_img, rotation_matrix.inv(), cv::Size(256, 256), cv::WARP_INVERSE_MAP,
                                cv::BORDER_DEFAULT);


            float match_0m = mcv::marker::compute_matching(img_0m_th, warped_img);
            float match_1m = mcv::marker::compute_matching(img_1m_th, warped_img);

            if (match_0m > 0.92 || match_1m > 0.92) {

                const cv::Mat &matched_image = (match_0m > match_1m) ? img_0p : img_1p;

                cv::Mat output_img;

                cv::warpPerspective(matched_image, output_img, picture_rotation, cv::Size(256, 256));
                cv::warpPerspective(output_img, input_img, H, cv::Size(input_img.cols, input_img.rows),
                                    cv::WARP_INVERSE_MAP,
                                    cv::BORDER_TRANSPARENT);

                cv::imshow("rotated", output_img);
                cv::waitKey(0);

            }

            cv::imshow("warped", warped_img);
            cv::waitKey(0);


            cv::imshow("original", input_img);
            cv::waitKey(0);


        }

    }


    //====houghProb

    /*vector<cv::Vec4i> lines;
    cv::HoughLinesP( boundaries_img, lines, 1, CV_PI/180, 60, 30, 10);
    for( size_t i = 0; i < lines.size(); i++ )
    {
        cv::line( fin_img, cv::Point(lines[i][0], lines[i][1]),
              cv::Point(lines[i][2], lines[i][3]), cv::Scalar(255,0,0), 3, 8 );
    }*/

    //==== hough
    /*cv::Mat rho_theta_plane;
    cv::Point2f best_rho_theta;
    std::vector<cv::Point2f> lines;
    int min_points = 60;
    mcv::extract_lines(boundaries_img, rho_theta_plane, lines, min_points);
    //mcv::compute_rho_theta_plane(boundaries_img,rho_theta_plane, best_rho_theta);

    for (cv::Point2f& point: lines) {
        mcv::line(fin_img,point.y,point.x);
    }*/






    //===END
    else if( frame_source == SOURCE_VIDEO || frame_source == SOURCE_WEBCAM) {


        //std::string video_input_name = "data/test_video2.mp4";

        const int width = 256; // experimental good window width for current problem
        const int height = 256; // experimental good window height for current problem

        cv::VideoCapture vc;

        // Init VideoWriter to produce output video
        //int fourcc_type = cv::VideoWriter::fourcc('M','J','P','G');
        //cv::VideoWriter vw("output.mp4",fourcc_type, 30.0, cv::Size2i(640,480));
        if(frame_source == SOURCE_VIDEO) {
            vc.open(filename);  // video input file
        }else{
            vc.open(camera_index);
        }

        bool end = false;
        if (vc.isOpened()) {
            // Setup frame with fixed resolution
            vc.set(CV_CAP_PROP_FRAME_WIDTH,640);
            vc.set(CV_CAP_PROP_FRAME_HEIGHT,480);

            int frame_number = 1;


            cv::Mat grayscale;
            cv::Mat boundaries_img;

            while (!end) {
                cv::Mat frame;

                vc >> frame; // put frame image Mat  , this operation is blocking until a frame is provided
                if (frame.empty()) { // if camera frame are infinite but in video no
                    end = true;
                } else {
                    cv::Mat frame_debug;




                    if(debug_info) {
                        frame_debug = frame.clone();
                    }

                    cv::cvtColor(frame, grayscale, CV_RGB2GRAY);

                    //Calculate threshold image
                    int max_value;
                    cv::Mat hist_ = mcv::compute_hist(grayscale, max_value);
                    cv::Mat normHist = mcv::normalize_hist(hist_, grayscale);
                    int threshold = mcv::compute_Otsu_thresholding(normHist);
                    cv::Mat frame_th = mcv::image_threshold(threshold, grayscale);

                    // Boundary extraction

                    mcv::boundary_extractor be(grayscale);
                    be.find_boundaries();
                    be.keep_between(200, 1200);


                    be.create_boundaries_image(boundaries_img);// 1 pixel of padding

                    //===corners
                    cv::Mat img_corners = cv::Mat::zeros(boundaries_img.rows, boundaries_img.cols,
                                                         CV_32FC1); // float values
                    int block_size = 11;//Good 7
                    int kernel_size = 7;// Good 5
                    float free_parameter = 0.05; // more little more corners will be found
                    cv::cornerHarris(boundaries_img, img_corners, block_size, kernel_size, free_parameter,
                                     cv::BorderTypes::BORDER_DEFAULT);

                    //search corners in img_corners
                    be.compute_corners(img_corners);
                    be.keep_between_corners(4, 4);



                    //========== HOMOGRAPHY =============
                    std::vector<mcv::boundary> &boundaries = be.get_boundaries();
                    for (mcv::boundary &boundary : boundaries) {
                        cv::Mat warped_img;

                        // find Homography
                        std::vector<cv::Vec2d> corners;
                        for (cv::Vec2i &corner : boundary.corners) {
                            corners.push_back(cv::Vec2d(corner[0], corner[1]));
                        }


                        cv::Mat H = cv::findHomography(corners, mcv::marker::DST_POINTS);


                        cv::warpPerspective(frame_th, warped_img, H.inv(), cv::Size(256, 256), cv::WARP_INVERSE_MAP,
                                            cv::BORDER_DEFAULT);

                        // Detect orientation
                        int orientation = mcv::marker::detect_orientation(warped_img);

                        // Calculate rotation matrixes
                        cv::Mat rotation_matrix;
                        cv::Mat picture_rotation;
                        mcv::marker::calculate_rotation_matrix(rotation_matrix, orientation); // TODO optimize this code
                        mcv::marker::calculate_picture_rotation(picture_rotation, orientation);


                        // Set marker in correct orientation for matching
                        cv::warpPerspective(warped_img, warped_img, rotation_matrix.inv(), cv::Size(256, 256),
                                            cv::WARP_INVERSE_MAP, cv::BORDER_DEFAULT);

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


                    cv::imshow("original", frame);
                    if (debug_info) {
                        be.draw_boundaries(frame_debug);
                        be.draw_boundaries_corners(frame_debug);

                        cv::imshow("corners", img_corners);
                        cv::imshow("live", frame_debug);
                    }


                    cv::waitKey(1);  // delay between frame read ( if to long we loose frames )
                    ++frame_number;
                }

            }
        } else {
            std::cout << "Error impossible open the video" << std::endl;
        }

    }
    //vw.release();
    std::cout << "================ END ===============" << std::endl;

    return 0;



    //std::vector<cv::Vec2d> points; // used also from homography
    //cv::setMouseCallback("image", mouse_callback, &points);

    /*int last_n_pts=0;
    while(true){
        cv::imshow("image", input_img);
        cv::waitKey(100);
        if(points.size() != last_n_pts){
            cv::Vec2d pt = points.back();
            cv::drawMarker(input_img, cv::Point2i(pt[0], pt[1]),CV_RGB(255,0,0), cv::MARKER_CROSS);
            last_n_pts = points.size();

            if( points.size() == 4 ){
                std::vector< cv::Vec2d > dst_points;
                dst_points.push_back( cv::Vec2d(0, 0));
                dst_points.push_back( cv::Vec2d(400, 0));
                dst_points.push_back( cv::Vec2d(400, 400));
                dst_points.push_back( cv::Vec2d(0, 400));

                cv::Mat H = cv::findHomography( points, dst_points );

                std::cout << H << std::endl;
                std::cout << H.inv() << std::endl; // inverse of the matrix


                cv::Mat warped_img;
                cv::warpPerspective(input_img, warped_img, H, cv::Size(400,400));

                cv::imshow("warped", warped_img);
                points.clear();
            }
            // findHomography
        }

    }*/


}



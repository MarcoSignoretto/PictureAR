#include <iostream>
#include <opencv2/videoio.hpp>
#include <opencv/cv.hpp>

#include "include/utils.h"
#include "include/speedometer.h"
#include "include/boundary_extractor.h"

using namespace std;



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

    /*
     * 1) Identify possible marker
     * 2) Search best match with Models given, cv::warpPerspective can be used to warm images
     * The plane transformation is a homography ( we shold be able to estimate the homography matrix H )
     *
     *
     */

    /*const std::string filename = "data/test1.jpg";

    cv::Mat input_img;
    input_img = cv::imread("data/test1.jpg");

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
    cv::Mat image = mcv::image_threshold(threshold, image_gray);

    cv::imshow("image_th",image);

    cv::waitKey(0);



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
    cv::waitKey(0);
    //===END

    mcv::boundary_extractor be(filename);
    be.find_boundaries();
    be.keep_between(300,1000);
    cout << "Boundaries: " << endl;
    be.print_boundary_lengths();
    be.draw_boundaries("boundaries.png");

    cout << "================ END ===============" << endl;*/

    std::string video_input_name = "data/test_video2.mp4";

    const int width = 256; // experimental good window width for current problem
    const int height = 256; // experimental good window height for current problem

    cv::VideoCapture vc;

    // Init VideoWriter to produce output video
    //int fourcc_type = cv::VideoWriter::fourcc('M','J','P','G');
    //cv::VideoWriter vw("output.mp4",fourcc_type, 30.0, cv::Size2i(640,480));
    vc.open(video_input_name);  // video input file

    bool end = false;
    if(vc.isOpened()){
        /*
        // experimental good initial window point for current problem
        cv::Point initial_point{180,270};
        cv::Point end_point{initial_point.x + width, initial_point.y + height};

        // some initialization
        cv::Mat channels[3];
        cv::Mat window_mat(width, height, CV_8UC1);
        cv::Mat t_mat(width, height, CV_8UC1);

        cv::Mat rho_theta_plane;

        as3::speedometer sm{320,2000};
        sm.set_output_file("speed.txt");*/
        int frame_number = 1;


        while(!end){
            cv::Mat frame;
            vc >> frame; // put frame image Mat  , this operation is blocking until a frame is provided
            if(frame.empty()){ // if camera frame are infinite but in video no
                end = true;
            }else {

                cv::Mat grayscale;
                cvtColor(frame, grayscale, CV_RGB2GRAY);
                std::cout << grayscale.channels() << std::endl;

                cv::Mat fin_img;

                mcv::boundary_extractor be(grayscale);
                be.find_boundaries();
                be.keep_between(200,1200);
                cout << "Boundaries: " << endl;
                be.print_boundary_lengths();
                be.draw_boundaries(grayscale, fin_img);
                //be.draw_boundaries("boundaries.png");
                std::cout << fin_img.channels() << std::endl;

                cv::imshow("live", fin_img);

                /*// split image in channels ( red is interesting for this problem )
                cv::split(frame, channels);

                // obtain a relevant window of frame and store into window_mat
                channels[2](cv::Range(initial_point.y, end_point.y), cv::Range(initial_point.x, end_point.x))
                        .copyTo(window_mat);

                // threshold on image with high value of red
                cv::threshold(window_mat, t_mat, THRESHOLD, 255, cv::THRESH_BINARY);

                // In order to have a better line (thin) with few observation an erosion are applied
                // the kernel size are found in experimental way
                // In this way also Km/h label will be completly removed
                cv::Mat kernel = cv::getStructuringElement(cv::MorphShapes::MORPH_RECT, cv::Size2i(7,7));
                cv::erode(t_mat,t_mat,kernel);

                // compute rho_theta_plane and return best line params
                cv::Point2f best_rho_theta;
                mcv::compute_rho_theta_plane(t_mat,rho_theta_plane, best_rho_theta);

                // plot line from rho and theta into thresholded window
                mcv::line(t_mat,best_rho_theta.y,best_rho_theta.x);
                // plot line also into original frame
                mcv::line(frame,best_rho_theta.y,best_rho_theta.x, initial_point.x, initial_point.y);

                // print some informations
                std::cout << "theta: " << best_rho_theta.x << std::endl;
                std::cout << "theta (in degree):" << mcv::to_degree(best_rho_theta.x) << std::endl;
                std::cout << "speed: " << sm.add_observation(best_rho_theta.x,frame_number) << std::endl;


                // Draw simple bounds to interesting area
                cv::line( channels[2], initial_point, cv::Point(end_point.x,initial_point.y), cv::Scalar( 255, 0, 0), 2, cv::LINE_AA, 0  );
                cv::line( channels[2], cv::Point(initial_point.x,end_point.y), end_point, cv::Scalar( 255, 0, 0), 2, cv::LINE_AA, 0  );
                cv::line( channels[2], initial_point, cv::Point(initial_point.x,end_point.y), cv::Scalar( 255, 0, 0), 2, cv::LINE_AA, 0  );
                cv::line( channels[2], cv::Point(end_point.x,initial_point.y), end_point, cv::Scalar( 255, 0, 0), 2, cv::LINE_AA, 0  );

                // Show interesting image ad processes
                vw.write(frame);

                cv::imshow("live", frame);
                cv::imshow("red channel",channels[2]);
                cv::imshow("window",t_mat);
                cv::imshow("rho_theta_plane", mcv::to_image(rho_theta_plane));
                cv::imshow("graph", sm.get_graph());*/

                cv::waitKey(1);  // delay between frame read ( if to long we loose frames )
                ++frame_number;
            }

        }
    } else{
        std::cout << "Error impossible open the video" << std::endl;
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



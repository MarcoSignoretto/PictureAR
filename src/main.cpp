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

    const std::string filename = "data/test1.jpg";

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

    cout << "================ END ===============" << endl;



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






    std::cout << "================ END ===============" << std::endl;

    return 0;
}



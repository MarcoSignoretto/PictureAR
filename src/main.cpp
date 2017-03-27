#include <iostream>
#include <opencv2/videoio.hpp>
#include <opencv/cv.hpp>

#include "include/utils.h"
#include "include/speedometer.h"

using namespace std;



const double theta = CV_PI / 180;
const double rho = 1;

const int THRESHOLD = 230; // experimental good threshold for current problem

int main( int argc, char* argv[] ) {

    const int width = 256; // experimental good window width for current problem
    const int height = 256; // experimental good window height for current problem

    cv::VideoCapture vc;

    // Init VideoWriter to produce output video
    int fourcc_type = cv::VideoWriter::fourcc('M','J','P','G');
    cv::VideoWriter vw("output.mp4",fourcc_type, 30.0, cv::Size2i(1280,720));
    vc.open("carspeed.mp4");  // video input file

    bool end = false;
    if(vc.isOpened()){
        // experimental good initial window point for current problem
        cv::Point initial_point{180,270};
        cv::Point end_point{initial_point.x + width, initial_point.y + height};

        // some initialization
        cv::Mat channels[3];
        cv::Mat window_mat(width, height, CV_8UC1);
        cv::Mat t_mat(width, height, CV_8UC1);

        cv::Mat rho_theta_plane;

        as3::speedometer sm{320,2000};
        sm.set_output_file("speed.txt");
        int frame_number = 1;


        while(!end){
            cv::Mat frame;
            vc >> frame; // put frame image Mat  , this operation is blocking until a frame is provided
            if(frame.empty()){ // if camera frame are infinite but in video no
                end = true;
            }else {

                // split image in channels ( red is interesting for this problem )
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
                cv::imshow("graph", sm.get_graph());

                cv::waitKey(1);  // delay between frame read ( if to long we loose frames )
                ++frame_number;
            }

        }
    } else{
        std::cout << "Error impossible open the video" << std::endl;
    }
    vw.release();
    std::cout << "================ END ===============" << std::endl;

    return 0;
}



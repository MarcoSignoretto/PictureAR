#include <iostream>
#include <opencv2/videoio.hpp>
#include <opencv/cv.hpp>
#include <exception>

#include "include/utils.h"
#include "include/boundary_extractor.h"
#include "include/marker.h"

using namespace std;

const std::string SOURCE_VIDEO = "video";
const std::string SOURCE_IMAGE = "image";
const std::string SOURCE_WEBCAM = "webcam";
const std::string SOURCE_DEFAULT  = SOURCE_WEBCAM;

int main( int argc, char* argv[] ) {

    std::string frame_source = SOURCE_DEFAULT;
    std::string filename = "";
    int camera_index = 0;
    bool debug_info = false;

    ///======= HANDLE PARAMETERS OF THE PROGRAM =======

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

    ///======= LOAD MARKERS AND PLACEHOLDERS =======

    // Load Marker images and Placeholder
    const cv::Mat img_0p = cv::imread("data/0P.png");
    const cv::Mat img_1p = cv::imread("data/1P.png");
    const cv::Mat img_0m = cv::imread("data/0M.png", cv::IMREAD_GRAYSCALE);
    const cv::Mat img_1m = cv::imread("data/1M.png", cv::IMREAD_GRAYSCALE);

    //Threshold img_0m
    cv::Mat img_0m_th;
    mcv::image_otsu_thresholding(img_0m,img_0m_th);

    // Threshold img_1m
    cv::Mat img_1m_th;
    mcv::image_otsu_thresholding(img_1m,img_1m_th);


    ///======= HANDLE DIFFERENT SOURCES =======

    if( frame_source == SOURCE_IMAGE) {

        cv::Mat frame = cv::imread(filename);

        mcv::marker::apply_AR(img_0p, img_1p, img_0m_th, img_1m_th, frame, debug_info);

        cv::imshow("original", frame);
        cv::waitKey(0);

    } else if( frame_source == SOURCE_VIDEO || frame_source == SOURCE_WEBCAM) {

        cv::VideoCapture vc;

        if(frame_source == SOURCE_VIDEO) {
            vc.open(filename);
        }else{
            vc.open(camera_index);
        }

        bool end = false;
        if (vc.isOpened()) {
            // Setup frame with fixed resolution in order to be fast and work in the same way for each resolution
            vc.set(CV_CAP_PROP_FRAME_WIDTH,640);
            vc.set(CV_CAP_PROP_FRAME_HEIGHT,480);

            int frame_number = 1;

            while (!end) {
                cv::Mat camera_frame;

                vc >> camera_frame; // put frame image Mat , this operation is blocking until a frame is provided
                if (camera_frame.empty()) { // if camera frame are infinite but in video no
                    end = true;
                } else {

                    mcv::marker::apply_AR(img_0p, img_1p, img_0m_th, img_1m_th, camera_frame, debug_info);
                    cv::imshow("original", camera_frame);

                    cv::waitKey(1);  // delay between frame read ( if to long we loose frames )
                    ++frame_number;
                }

            }
        } else {
            std::cout << "Error impossible open the video" << std::endl;
        }

    }
    std::cout << "================ END ===============" << std::endl;

    return 0;
}



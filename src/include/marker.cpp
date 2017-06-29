//
// Created by Marco Signoretto on 29/06/2017.
//

#include "marker.h"
#include <stdexcept>
#include <iostream>


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

cv::Mat mcv::marker::calculate_rotation_matrix(int rotation_degree) {
    double radiants = 0.0;
    switch(rotation_degree){
        case 0:
            break;
        case 90:
            radiants = CV_PI/2.0;
            break;
        case 180:
            radiants = CV_PI;
            break;
        case 270:
            radiants = (3.0/2.0)*CV_PI;
            break;
        default:
            throw std::invalid_argument("only 0,90,180,270 degree are supported");
    }
    double raw_data[9] = { cos(radiants), -sin(radiants), 0.0, sin(radiants), cos(radiants), 0.0, 0.0, 0.0, 1.0 };
    return cv::Mat(3, 3, CV_64F, raw_data);

}

void mcv::marker::rotate(cv::Mat &img, int rotation_degree) {

    // TODO complete


}

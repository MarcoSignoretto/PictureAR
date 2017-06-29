//
// Created by Marco Signoretto on 29/06/2017.
//

#include "marker.h"


int mcv::marker::detect_orientation(const cv::Mat& warped_image) {
    int accumulator_0,accumulator_90,accumulator_180,accumulator_270 = 0;
    // TODO define rect for accumulators
    int i,j;
    const uchar* p;
    for( i = OFFSET; i < warped_image.rows-OFFSET; ++i) {
        p = warped_image.ptr<uchar>(i);
        for ( j = OFFSET; j < warped_image.cols-OFFSET; ++j) {
            if(p[j] == mcv::marker::BLACK){

                // TODO Controll were increment accumulator
            }
        }
    }

    return -1;// TODO change
}

//
// Created by Marco Signoretto on 06/03/2017.
//

#ifndef ASSIGNMENT2_COMPONENT_H
#define ASSIGNMENT2_COMPONENT_H

#include <iostream>
#include <vector>
#include <opencv2/core/core.hpp>

namespace mcv{
    struct boundary{
    public:
        int min_x = -1;
        int min_y = -1;
        int max_x = -1;
        int max_y = -1;
        int length = 0;
        int corners_number = 0;

        std::vector<cv::Vec2i> points; // clock wise ordered
        std::vector<cv::Vec2i> corners; // clock wise ordered

        void add_item(cv::Vec2i& b){
            ++length;
            // Updated max and min x,y values used for performance improvements  (corner case max = min)
            if(b[0]<min_x || min_x == -1)min_x = b[0];
            if(b[0]>max_x || max_x == -1)max_x = b[0];
            if(b[1]<min_y || min_y == -1)min_y = b[1];
            if(b[1]>max_y || max_y == -1)max_y = b[1];
            points.push_back(b); // add pixel to boundary points
        }

        void print(){
            std::cout << "Boundary:" << std::endl;
            for (cv::Vec2i& v : points) {
                std::cout << "(" << v[0] << "," << v[1] << ")" << std::endl;
            }
        }


        void compute_corners(cv::Mat& img_corners){
            int kernel_size = 3;
            bool on_corner = false;
            cv::Vec2i* corner = nullptr;
            float corner_intensity = 0.0f;
            for(int i = 0; i < points.size(); ++i) {
                cv::Vec2i &point = points[i];

                // Calculate intensity based on neighbourhood
                float current_intensity = 0.0f;
                const float *p;
                for(int y = point[1]+1-kernel_size; y <= point[1]+1+kernel_size; ++y) {
                    if (y >= 0 && y < img_corners.rows) { // Bound checking
                        p = img_corners.ptr<float>(y);
                        for (int x = point[0] + 1 - kernel_size; x <= point[0] + 1 + kernel_size; ++x) {
                            if (x >= 0 && x < img_corners.cols) { // Bound checking
                                current_intensity += p[x];
                            }
                        }
                    }
                }

                // Check if point is a corner or not
                if(current_intensity > 1.0f){
                    if(!on_corner){
                        on_corner = true;
                    }
                    if(current_intensity>corner_intensity){
                        corner_intensity = current_intensity;
                        corner = &point;
                    }
                }else{
                    if(on_corner){
                        corners_number += 1;
                        corners.push_back(*corner);
                        on_corner = false;
                        corner_intensity = 0.0f;
                        corner = nullptr;
                    }
                }

            }
        }


    private:
        /**
         * Normalize corner values ( remove padding )
         */
        void normalizeCorners(){
            for (cv::Vec2i& corner: corners) {
                corner[0]-=1;
                corner[1]-=1;
            }
        }
    };


}

#endif //ASSIGNMENT2_COMPONENT_H




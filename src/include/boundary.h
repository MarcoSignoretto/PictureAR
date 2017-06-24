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
        std::vector<cv::Vec2i> corners = {
                cv::Vec2i(-1,-1),
                cv::Vec2i(-1,-1),
                cv::Vec2i(-1,-1),
                cv::Vec2i(-1,-1),
        }; // clock wise ordered
        std::vector<cv::Vec2i> points; // clock wise ordered

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

        // TODO comment
        void compute_corners(){
            for(cv::Vec2i& point : points){
                // -1 because there is offset of padded image
                if(point[0]==max_x-1 && corners[0][0]==-1){ // first corner that archive max_x
                    corners[0][0] = point[0];
                    corners[0][1] = point[1];
                }
                if(point[1]==max_y-1 && corners[1][1]==-1){ // MAX Y
                    corners[1][0] = point[0];
                    corners[1][1] = point[1];
                }
                if(point[0]==min_x-1 && corners[2][0]==-1){ // MIN X
                    corners[2][0] = point[0];
                    corners[2][1] = point[1];
                }
                if(point[1]==min_y-1 && corners[3][1]==-1){ // MIN Y
                    corners[3][0] = point[0];
                    corners[3][1] = point[1];
                }
            }
        }
    };
}

#endif //ASSIGNMENT2_COMPONENT_H




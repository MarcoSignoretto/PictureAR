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
        /*std::vector<cv::Vec2i> corners = {
                cv::Vec2i(-1,-1),
                cv::Vec2i(-1,-1),
                cv::Vec2i(-1,-1),
                cv::Vec2i(-1,-1),
        }; // clock wise ordered*/
        std::vector<float> intensity = {
                0.0f,
                0.0f,
                0.0f,
                0.0f
        }; // clock wise ordered

        std::vector<cv::Vec2i> points; // clock wise ordered
        std::vector<cv::Vec2i> corners;

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
        // TODO if look at next consider last point of boundaries
        /*void compute_corners(){
            //for(cv::Vec2i& point : points){
            for(int i = 0; i < points.size(); ++i){
                cv::Vec2i& point = points[i];
                cv::Vec2i& next_point = points[(i+1)%points.size()];

                // -1 because there is offset of padded image
                if(point[0]==max_x-1  ){ // first corner that archive max_x
                    if(corners[0][0]==-1 || next_point[0]<corners[0][0]) {
                        corners[0][0] = point[0];
                        corners[0][1] = point[1];
                    }
                }
                if(point[1]==max_y-1){ // MAX Y
                    if(corners[1][1]==-1 || next_point[1]<corners[1][1]) {
                        corners[1][0] = point[0];
                        corners[1][1] = point[1];
                    }
                }
                if(point[0]==min_x-1) { // MIN X
                    if (corners[2][0] == -1 || next_point[0] > corners[2][0]) {
                        corners[2][0] = point[0];
                        corners[2][1] = point[1];
                    }
                }
                if(point[1]==min_y-1){ // MIN Y
                    if(corners[3][1]==-1 || next_point[1] > corners[3][1]) {
                        corners[3][0] = point[0];
                        corners[3][1] = point[1];
                    }
                }
            }
        }*/

        /*void compute_corners(cv::Mat& img_corners){


            for(int i = 0; i < points.size(); ++i){
                cv::Vec2i& point = points[i];
                point[0] += 1;// remove offset because corner image has offset also
                point[1] += 1;
                //cv::Vec2i& next_point = points[(i+1)%points.size()];

                //float current_intensity = img_corners.at<float>(point[0],point[1]);

                // -1 because there is offset of padded image
                if(point[0]==max_x){ // first corner that archive max_x
                    search_corner(0,point,img_corners);

                }
                if(point[1]==max_y){ // MAX Y
                    search_corner(1,point,img_corners);

                }
                if(point[0]==min_x) { // MIN X
                    search_corner(2,point,img_corners);

                }
                if(point[1]==min_y){ // MIN Y
                    search_corner(3,point,img_corners);

                }
            }
            normalizeCorners();
        }*/

        void compute_corners(cv::Mat& img_corners){
            bool on_corner = false;
            cv::Vec2i* corner = nullptr;
            float corner_intensity = 0.0f;
            for(int i = 0; i < points.size(); ++i) {
                cv::Vec2i &point = points[i];

                float current_intensity = img_corners.at<float>(point[1]+1,point[0]+1);// remove offset because corner image has offset also
                if(current_intensity > 2.0f){
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
        void normalizeCorners(){
            for (cv::Vec2i& corner: corners) {
                corner[0]-=1;
                corner[1]-=1;
            }
        }

        // TODO improve performance using pointers
        inline void search_corner(int index, cv::Vec2i& point, cv::Mat& img_corners){
            int neighbour = 0;
            for(int i= point[1] - neighbour; i <= point[1] + neighbour; ++i){
                for(int j=point[0]-neighbour; j<= point[0]+neighbour; ++j) {
                    // core TODO search why also with neightbour there are problems ion corner detection ( maybe wring if below)
                    //if(i>=0 && j>=0 && i<img_corners.cols && j<img_corners.rows) {
                        float current_intensity = img_corners.at<float>(i, j);
                        if (intensity[index] < current_intensity) {
                            corners[index][0] = point[0];
                            corners[index][1] = point[1];
                            intensity[index] = current_intensity;
                        }
                    //}
                }
            }
        }

    };


}

#endif //ASSIGNMENT2_COMPONENT_H




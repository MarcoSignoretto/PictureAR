//
// Created by Marco Signoretto on 06/03/2017.
//

#ifndef ASSIGNMENT2_BOUNDARY_EXTRACTOR_H
#define ASSIGNMENT2_BOUNDARY_EXTRACTOR_H

#include <vector>
#include <opencv2/core/mat.hpp>
#include "boundary.h"

namespace mcv{


    /**
     * Class that allow operations on boundaries, c and b are the common param used in moore algorithm
     * Vec2i is used to store (x,y) Vec2i[0] is x Vec2i[1] is y
     */
    class boundary_extractor {
    public:

        /**
         * Constructor: this perform loading of image with filename given as param then compute otsu thresholding and
         * keep in memory the result of thresholding ( this is target of all class operation )
         * @param filename name of image that we want to analize
         */
        boundary_extractor(const std::string& filename);

        boundary_extractor(const cv::Mat image_gray);

        /**
         * Find al boundaries of the image
         */
        void find_boundaries();

        /**
         * Given a point with coordinate x,y find boundary starting from that point
         * @param x is the column index of image
         * @param y is the row index of the image
         */
        inline boundary moore_algorithm(int x, int y);

        /**
         * Draw boundaries evaluated with find_boundaries and alive after keep_between
         * @param dest filename of the destionation image
         */
        void draw_boundaries(const std::string& dest);

        void draw_boundaries(cv::Mat& image, cv::Mat& fin_img);

        /**
         * This function purge boundaries vector and keep only boundary that has length between
         * min_length and min_length
         * @param min_length lower bound (included)
         * @param max_length upper bound (included)
         */
        void keep_between(int min_length, int max_length);

        /**
         * This function, for each boundary, print index of boundary into boundaries vector and his length
         */
        void print_boundary_lengths();

    private:
        // Name of the target image
        const std::string filename_;
        // Image thresholded with 1 pixel of padding outside
        cv::Mat image_;
        // histogram of the image
        cv::Mat hist_;
        // Vector of all boundaries of the image ( full after calling find_boundaries )
        std::vector<boundary> boundaries_;

        /**
         * Internal function that check position (x,y) is a valid initial boundary point not already found
         * @param x column index in image_
         * @param y row index in image_
         * @return true if that point is starting point for a new boundary
         */
        inline bool is_valid(int x, int y);

        /**
         * Check if point (x,y) is already present into a boundary given as param, this is used to avoid multiple
         * boundary with different starting point but same sequence
         * @param b: boundary where search (x,y)
         * @param x
         * @param y
         * @return true if (x,y) is present into bounddary b
         */
        inline bool check_in(const boundary& b, int x, int y);

        /**
         * Search in clock wise order a new c and b starting from current_c current_b, current_b is the center
         * @param current_c: precedent black value in terms of anti-clockwise respect the previous b
         * @param current_b: center of the clock
         * @param c: used as return param (contain new c value if found = true)
         * @param b: used as return param (contain new b value if found = true)
         * @return found: true if a new b and c are found false otherwise ( single point boundary never found new b,c)
         */
        inline bool search_clockwise(cv::Vec2i& current_c, cv::Vec2i& current_b, cv::Vec2i* c, cv::Vec2i* b);

        /**
         * Find clock index of c given that center is b
         * clock index are defined as:
         *
         * 0  1  2
         * 7  b  3
         * 6  5  4
         *
         * where b is the center (param b)
         *
         * @param c: c in moore algorithm
         * @param b: center of the clock
         * @return clock index
         */
        inline int find_clock_index(cv::Vec2i* c, cv::Vec2i* b);

        /**
         * Internal function that draw in red a boundary on a image divided into channel given.
         * @param b: boundary to draw
         * @param channels: 0 is blue, 1 is green, 2 is red (as standard OpenCV)
         */
        inline void draw_boundary(const boundary& b, std::vector<cv::Mat>& channels);

        /**
         * This function remove offset of padding image for all boundary points of each boundary
         */
        inline void normalize();
    };

}



#endif //ASSIGNMENT2_BOUNDARY_EXTRACTOR_H

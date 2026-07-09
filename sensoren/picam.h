#pragma once

#include <opencv2/opencv.hpp>


class Picam {
    public:
        Picam();
        cv::Mat frame;
        void run();
};

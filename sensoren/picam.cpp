#include "picam.h"


Picam::Picam() {
}


void Picam::run() {
    std::cout << "Drücken Sie 'q', um die Aufnahme zu beenden." << std::endl;
    cv::VideoCapture cap("libcamerasrc ! video/x-raw,width=640,height=480,framerate=30/1 ! videoconvert ! appsink", cv::CAP_GSTREAMER);
    while (true) {
        cap >> frame;

        if (frame.empty()) {
            std::cerr << "Kein Bild aufgenommen." << std::endl;
            break;
        }

        cv::imshow("Pi Camera", frame);

        if (cv::waitKey(1) == 'q') {
            break;
        }
    }

    cap.release();
    cv::destroyAllWindows();
}


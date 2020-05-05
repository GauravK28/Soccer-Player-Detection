//
// Created by Gaurav Krishnan on 4/28/20.
//

#ifndef FINALPROJECT_MYLIBRARY_TRACKERTWO_H
#define FINALPROJECT_MYLIBRARY_TRACKERTWO_H

#include <opencv2/videoio.hpp>
#include "opencv2/core.hpp"
#include "opencv2/tracking.hpp"
#include "cinder/Cinder.h"

namespace mylibrary {
    class Detector {
    public :
        void Detect(cv::VideoCapture& select_cap, bool save_vid);
        int FindPlayer(const cv::Rect& roi,
                const cv::Scalar& lower_color, const cv::Scalar& upper_color);
        void FindBall();
    private:
        cv::VideoCapture cap_;
        cv::Mat frame_;

        // color roi thresholds
        int kteam1thresh = 500; // currently for yellow
        int kteam2thresh = 70; // currently for navy blue
    };

}

#endif //FINALPROJECT_MYLIBRARY_TRACKERTWO_H

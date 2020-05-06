//
// Created by Gaurav Krishnan on 4/28/20.
//

#ifndef FINALPROJECT_MYLIBRARY_UTILS_H
#define FINALPROJECT_MYLIBRARY_UTILS_H

#include <opencv2/videoio.hpp>
#include "opencv2/core.hpp"
#include "opencv2/tracking.hpp"
#include "cinder/Cinder.h"

namespace mylibrary {
    std::string GetTimeStamp();

    void PlayVideo(cv::VideoCapture& cap);
}

#endif //FINALPROJECT_MYLIBRARY_UTILS_H

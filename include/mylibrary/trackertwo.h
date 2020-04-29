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
    class OtherTracker {
    public :
        void SetupTracker(cv::VideoCapture& select_cap);


    private:

    };

}

#endif //FINALPROJECT_MYLIBRARY_TRACKERTWO_H

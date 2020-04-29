//
// Created by Gaurav Krishnan on 4/28/20.
//

#include "mylibrary/trackertwo.h"

#include <opencv2/videoio.hpp>
#include "opencv2/core.hpp"
#include "opencv2/tracking.hpp"
#include "cinder/Cinder.h"

using std::cout;
using std::endl;

namespace mylibrary {
    void OtherTracker::SetupTracker(cv::VideoCapture& select_cap) {
//      vidcap = cv2.VideoCapture('cutvideo.mp4')
//      success,image = vidcap.read()
//      count = 0
//      success = True
//      idx = 0

      cv::VideoCapture cap = select_cap;
      cv::Mat frame;

      if (!cap.isOpened()) {
        cout << "Error opening video file "  << endl;
        return;
      }
      cap >> frame;

//      while(cap.isOpened()) {
//
//      }

      // converts image to hsv
      cvtColor(frame, frame, CV_BGR2HSV, 0);
      //green range (grass)
      auto lower_green = cv::Scalar(40,40,40);
      auto upper_green = cv::Scalar(70, 255, 255);

      //blue range (team 1)


      //Red range (team 2)

      //white range
//      lower_white = np.array([0,0,0])
//      upper_white = np.array([0,0,255])





    }

}

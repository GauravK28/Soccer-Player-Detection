//
// Created by Gaurav Krishnan on 4/28/20.
//

#include <opencv2/highgui.hpp>
#include "mylibrary/utils.h"

using std::string;
using std::to_string;
using std::cout;
using std::endl;

namespace mylibrary {

    string GetTimeStamp() {
      std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
      time_t tt = std::chrono::system_clock::to_time_t(now);
      tm local_tm = *localtime(&tt);

      string hour =  to_string(local_tm.tm_hour);
      string min = to_string(local_tm.tm_min);
      string sec =  to_string(local_tm.tm_sec);

      return hour + ":" + min + ":" + sec;
    }

    void PlayVideo(cv::VideoCapture& cap) {
      // Source: https://www.learnopencv.com/read-write-and-display-a-video-using-opencv-cpp-python/

      if(!cap.isOpened()){
        cout << "Error opening video stream or file" << endl;
        return;
      }
      while (true) {
        cv::Mat frame;
        // Capture frame-by-frame
        cap >> frame;
        // If the frame is empty, break immediately
        if (frame.empty())
          break;

        // Displays frame
        cv::imshow( "Video Frame", frame );

        char c = (char) cv::waitKey(25);
        if(c==27) { // stops playing video with ESCAPE KEY
          break;
        }
      }

      cap.release();
      cv::destroyAllWindows();
    }

};

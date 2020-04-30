//
// Created by Gaurav Krishnan on 4/28/20.
//

#include "mylibrary/trackertwo.h"

#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include "opencv2/core.hpp"
#include "opencv2/tracking.hpp"
#include "cinder/Cinder.h"

using std::cout;
using std::endl;

namespace mylibrary {
    void OtherTracker::SetupTracker(cv::VideoCapture& select_cap) {

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
      cv::Mat hsv; // output image
      cvtColor(frame, hsv, CV_BGR2HSV, 0);

      //green range (grass)
      auto lower_green = cv::Scalar(40,40,40);
      auto upper_green = cv::Scalar(70, 255, 255);

      //blue range (team 1)
      auto lower_blue = cv::Scalar(110,50,50);
      auto upper_blue = cv::Scalar(130,255,255);

      //Red range (team 2)
      auto lower_red = cv::Scalar(0,31,255);
      auto upper_red = cv::Scalar(176,255,255);

      //white range
      auto lower_white = cv::Scalar(0,0,0);
      auto upper_white = cv::Scalar(0,0,255);

      // Source:
      // Define a mask ranging from lower to uppper
      cv::Mat mask_image;
      cv::inRange(hsv, lower_green, upper_green, mask_image);
      // Do masking
      cv::Mat result;
      cv::bitwise_and(hsv, hsv, result, mask_image);


      // convert to hsv to gray
      //cv::cvtColor(result, result, cv::COLOR_HSV2BGR, 0);
      cv::cvtColor(result, result, cv::COLOR_BGR2GRAY, 0); // comment out to show only green

      // Source: https://stackoverflow.com/questions/22965277/opencv-removing-noise-from-image
      // Defining a kernel to do morphological operation in threshold #image to get better output.
      int kMaxNoiseWidth = 12;
      int kMaxNoiseHeight = 12;
      cv::Mat kernel = cv::Mat(cv::Size(kMaxNoiseWidth,kMaxNoiseHeight),CV_8UC1,cv::Scalar(255));

      cv::Mat temp;
      // Using otsu to determine threshold value automatically
      // Source: https://docs.opencv.org/3.4/d7/d4d/tutorial_py_thresholding.html
      cv::threshold(result,temp, 127, 255, cv::THRESH_BINARY_INV + cv::THRESH_TRIANGLE);

      cv::Mat output;
      // Source: https://docs.opencv.org/3.4/dbe/tutorial
      cv::morphologyEx(temp,output,cv::MORPH_CLOSE, kernel);
//      kernel = np.ones((13,13),np.uint8)
//      thresh = cv2.threshold(res_gray,127,255,cv2.THRESH_BINARY_INV |  cv2.THRESH_OTSU)[1]
//      thresh = cv2.morphologyEx(thresh, cv2.MORPH_CLOSE, kernel)

      cv::resize(frame, frame, cv::Size(600,400));
      cv::imshow("Other", frame);
      cv::resize(output, output, cv::Size(600,400));
      cv::imshow("Video Frame", output);
    }

}

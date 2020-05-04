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
using std::vector;

namespace mylibrary {
    void OtherTracker::SetupTracker(cv::VideoCapture& select_cap) {
      //why hsv & masking:
      // https://www.learnopencv.com/color-spaces-in-opencv-cpp-python/

      //image segmentation using color spaces
      // https://realpython.com/python-opencv-color-spaces/#visualizing-nemo-in-hsv-color-space

      //basic workflow
      // https://stackoverflow.com/questions/10948589/choosing-the-correct-upper-and-lower-hsv-boundaries-for-color-detection-withcv

      //opencv morphological operations
      // https://docs.opencv.org/3.4/d3/dbe/tutorial_opening_closing_hats.html


      cv::VideoCapture cap = select_cap;
      cv::Mat frame;

      if (!cap.isOpened()) {
        cout << "Error opening video file "  << endl;
        return;
      }
      //cap >> frame;

      while(cap.isOpened()) {       // part of vid playing
        cap >> frame;
        if (frame.empty()) {
          break;                // part of vid playing
        }


        // converts image to hsv
        cv::Mat hsv; // output image
        cvtColor(frame, hsv, CV_BGR2HSV, 0);

        //green range (grass)
        auto lower_green = cv::Scalar(40,40,40);
        auto upper_green = cv::Scalar(70, 255, 255);


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

        // Notes: https://stackoverflow.com/questions/22965277/opencv-removing-noise-from-image
        // kernel to do morph op on thresholded image.
        int kMaxNoiseWidth = 12;
        int kMaxNoiseHeight = 12;
        // TODO try diff sizes
        cv::Mat kernel = cv::Mat(cv::Size(kMaxNoiseWidth,kMaxNoiseHeight),CV_8UC1,cv::Scalar(255));

        // thresholding determines players vs non-players (thats wht i used binary)
        // morphologyEx gets rid of noise/ sharpen player contours

        cv::Mat temp;
        // Using otsu to determine threshold value automatically
        // Notes: https://docs.opencv.org/3.4/d7/d4d/tutorial_py_thresholding.html
        // https://stackoverflow.com/questions/17141535/how-to-use-the-otsu-threshold-in-opencv
        // thresh val is inconsequential bc OTSU determines the thresh value
        cv::threshold(result,temp, -1, 255, cv::THRESH_BINARY_INV + cv::THRESH_OTSU);
        // TODO try diff threshold algos
        cv::Mat output;
        // Source: https://docs.opencv.org/trunk/d9/d61/tutorial_py_morphological_ops.html
        cv::morphologyEx(temp,output,cv::MORPH_CLOSE, kernel);
        //cv::morphologyEx(output, output, cv::MORPH_OPEN, kernel);
        // TODO try Morph_open


        // Detect contours
        // Contour notes:
        // https://opencv-python-tutroals.readthedocs.io/en/latest/py_tutorials/py_imgproc/py_contours/py_table_of_contents_contours/py_table_of_contents_contours.html
        cv::Mat canny_output;
        vector<vector<cv::Point> > contours;
        //vector<cv::Vec4i> hierarchy;
        int thresh = 100;

        /// Detect edges using canny
        // Source: https://docs.opencv.org/2.4/doc/tutorials/imgproc/shapedescriptors/find_contours/find_contours.html
        Canny(output, canny_output, thresh, thresh * 2, 3 );
        /// Find contours
        findContours(canny_output, contours,
                CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE,
                cv::Point(0, 0) );

        //vector<cv::Rect> bounding_rects;
        for (auto& contour: contours) {
          // bounding rect notes:
          // https://opencv-python-tutroals.readthedocs.io/en/latest/py_tutorials/py_imgproc/py_contours/py_contour_features/py_contour_features.html#contour-features
          cv::Rect roi = cv::boundingRect(contour); // roi = region of interest
          if (roi.height >= roi.width && roi.height > 15 && roi.width >= 15 &&
          roi.height < 100 && roi.width < 100) {

            // mask for team 1
            cv::Mat team_1_hsv;
            cv::Mat temp1 = frame(roi);
            temp1.copyTo(team_1_hsv);  // forces copy constructor instead of ref
            cv::cvtColor(team_1_hsv, team_1_hsv, CV_BGR2HSV, 0);

            // mask for team 1
            // yellow range
            auto lower_blue = cv::Scalar(21,50,50);
            auto upper_blue = cv::Scalar(36,255,255);

            cv::Mat team_1_mask;
            cv::inRange(team_1_hsv, lower_blue, upper_blue, team_1_mask);
            cv::Mat blue;

            cv::bitwise_and(team_1_hsv, team_1_hsv, blue, team_1_mask);
            cv::cvtColor(blue, blue, CV_HSV2BGR, 0);
            cv::cvtColor(blue, blue, CV_BGR2GRAY, 0);
            int blue_count = cv::countNonZero(blue);


            // mask for team 2
            // navy blue range
            auto lower_red = cv::Scalar(96,0,0);
            auto upper_red = cv::Scalar(114,150,150);

            cv::Mat team_2_hsv;
            cv::Mat temp2 = frame(roi);
            temp2.copyTo(team_2_hsv);
            cv::cvtColor(team_2_hsv, team_2_hsv, CV_BGR2HSV, 0);

            cv::Mat team_2_mask;
            cv::inRange(team_2_hsv, lower_red, upper_red, team_2_mask);
            cv::Mat red;
            cv::bitwise_and(team_2_hsv, team_2_hsv, red, team_2_mask);
            cv::cvtColor(red, red, CV_HSV2BGR, 0);
            cv::cvtColor(red, red, CV_BGR2GRAY, 0);
            int red_count = cv::countNonZero(red);

            if (blue_count > 500 && red_count > 70) {
              cv::rectangle(frame, roi, cv::Scalar(0, 255, 0));
            } else if (blue_count > 500) {
              cv::rectangle(frame, roi, cv::Scalar(255, 0, 0));
            } else if (red_count > 70) {
              cv::rectangle(frame, roi, cv::Scalar(0, 0, 255));
            }
          }
        }


//      //blue range (team 1)
//      auto lower_blue = cv::Scalar(110,50,50);
//      auto upper_blue = cv::Scalar(130,255,255);
//
//      //Red range (team 2)
//      auto lower_red = cv::Scalar(0,31,255);
//      auto upper_red = cv::Scalar(176,255,255);
//
//      //white range
//      auto lower_white = cv::Scalar(0,0,0);
//      auto upper_white = cv::Scalar(0,0,255);


        //cv::resize(frame, frame, cv::Size(600,400));
        cv::imshow("Other", frame);
//      cv::resize(output, output, cv::Size(600,400));
//      cv::imshow("Video Frame", output);



        char c = (char) cv::waitKey(25);
        if(c==27) { // stops playing video with ESCAPE KEY
          break;
        }

      }
      cap.release();
      cv::destroyAllWindows();

    }









    void OtherTracker::Test(cv::VideoCapture& select_cap) {
      cv::VideoCapture cap = select_cap;
      cv::Mat frame;

      if (!cap.isOpened()) {
        cout << "Error opening video file "  << endl;
        return;
      }
      //cap >> frame;

      // while(cap.isOpened()) {       // part of vid playing
      cap >> frame;
//        if (frame.empty()) {
//          break;                // part of vid playing
//        }


      cv::Mat temp2;
      frame.copyTo(temp2);
      // converts image to hsv
      cv::Mat hsv; // output image
      cvtColor(frame, hsv, CV_BGR2HSV, 0);

      //green range (grass)
      auto lower_green = cv::Scalar(40,40,40);
      auto upper_green = cv::Scalar(70, 255, 255);

      //blue range (team 1)
      auto lower_blue = cv::Scalar(110,50,50);
      auto upper_blue = cv::Scalar(130,255,255);

//      //Red range (team 2)
//      lower = np.array([155,25,0])
//      upper = np.array([179,255,255])
      auto lower_red = cv::Scalar(0,50,180); //0,31,255
      auto upper_red = cv::Scalar(23,255,255); //179,255,255

      // Define a mask ranging from lower to uppper
      cv::Mat mask_image;
      //cv::inRange(hsv, lower_green, upper_green, mask_image);
      cv::inRange(hsv, lower_green, upper_green, mask_image);


      // Do masking
      cv::Mat result;
      cv::bitwise_and(temp2, temp2, result, mask_image);

      cv::Mat test;
      test = frame - result;
//      cv::approxPolyDP()
//      cv::fillConvexPoly()
      cv::imshow("after first bit", test);

      int kMaxNoiseWidth = 55;
      int kMaxNoiseHeight = 55;
      cv::Mat kernel = cv::Mat(cv::Size(kMaxNoiseWidth,kMaxNoiseHeight),CV_8UC1,cv::Scalar(255));
      cv::morphologyEx(result, result, cv::MORPH_OPEN, kernel);
      cv::morphologyEx(result, result, cv::MORPH_CLOSE, kernel);

//      cv::Mat test;
//      test = frame - result;
//      cv::bitwise_not(frame, test, result);

//      cv::Mat canny_output;
//      vector<vector<cv::Point> > contours;
//      int thresh = 100;
//      /// Detect edges using canny
//      Canny(result, canny_output, thresh, thresh * 2, 3 );
//      /// Find contours
//      findContours( canny_output, contours, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0) );
//      cv::drawContours(result, contours, -1, cv::Scalar(255,0,0));
//      cv::imshow("Contours", result);




      // convert to hsv to gray
      //cv::cvtColor(result, result, cv::COLOR_BGR2GRAY, 0); // comment out to show only green
      //cv::imshow("Before gray", result);

//      // kernel to do morph op on thresholded image.
//      int kMaxNoiseWidth = 12;
//      int kMaxNoiseHeight = 12;
//      cv::Mat kernel = cv::Mat(cv::Size(kMaxNoiseWidth,kMaxNoiseHeight),CV_8UC1,cv::Scalar(255));
//
//      cv::Mat temp;
//      cv::threshold(result,temp, -1, 255, cv::THRESH_BINARY_INV + cv::THRESH_OTSU);
//      cv::Mat output;
//      cv::morphologyEx(temp,output,cv::MORPH_CLOSE, kernel);
//      cv::imshow("After morph", output);



//      // SEPERATING RED PLAYERS
////      //Red range (team 2)
//      auto lower_red = cv::Scalar(0,31,255);
//      auto upper_red = cv::Scalar(176,255,255);
//
//      cv::Mat team_mask;
//      cv::inRange(hsv, lower_red, lower_red, team_mask);
//      // Do masking
//      cv::Mat team_result;
//      cv::bitwise_and(hsv, hsv, team_result, team_mask);
//
//      // Using otsu to determine threshold value automatically
//      cv::threshold(team_result,temp, -1, 255, cv::THRESH_BINARY_INV + cv::THRESH_OTSU);
//      cv::morphologyEx(temp,output,cv::MORPH_CLOSE, kernel);
//
//      cv::imshow("team morph", output);




//    cv::Mat canny_output;
//      vector<vector<cv::Point> > contours;
//      int thresh = 100;
//
//      /// Detect edges using canny
//      Canny(output, canny_output, thresh, thresh * 2, 3 );
//      /// Find contours
//      findContours( canny_output, contours, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0) );
//
//      //vector<cv::Rect> bounding_rects;
//      for (auto& contour: contours) {
//        // bounding rect notes:
//        // https://opencv-python-tutroals.readthedocs.io/en/latest/py_tutorials/py_imgproc/py_contours/py_contour_features/py_contour_features.html#contour-features
//        cv::Rect bound = cv::boundingRect(contour);
//        if (bound.height >= bound.width && bound.height > 15 && bound.width >= 15) {
//          // mask for team 1
////          cv::Mat team_1_hsv;
////          cvtColor(frame, hsv, CV_BGR2HSV, 0);
////
////          auto lower_blue = cv::Scalar(110,50,50);
////          auto upper_blue = cv::Scalar(130,255,255);
////
////
////          cv::Mat team_1_mask;
////          cv::inRange(hsv, lower_green, upper_green, mask_image);
////          cv::Mat result;
////          cv::bitwise_and(hsv, hsv, result, mask_image);
////
////          cv::cvtColor(result, result, cv::COLOR_BGR2GRAY, 0); // convert to gray
////
//
//          cv::rectangle(frame, bound, cv::Scalar(255,0,0));
//        }
//      }
//
//
//      //blue range (team 1)
//      auto lower_blue = cv::Scalar(110,50,50);
//      auto upper_blue = cv::Scalar(130,255,255);
//
//      //Red range (team 2)
//      auto lower_red = cv::Scalar(0,31,255);
//      auto upper_red = cv::Scalar(176,255,255);
////
////      //white range
////      auto lower_white = cv::Scalar(0,0,0);
////      auto upper_white = cv::Scalar(0,0,255);
//
//
//      //cv::resize(frame, frame, cv::Size(600,400));
//      cv::imshow("Other", frame);
////      cv::resize(output, output, cv::Size(600,400));
////      cv::imshow("Video Frame", output);
//
//
//
//      // TODO part of vid playing
////        char c = (char) cv::waitKey(25);
////        if(c==27) { // stops playing video with ESCAPE KEY
////          break;
////        }
////
////      }
////      cap.release();
////      cv::destroyAllWindows();
    }


}

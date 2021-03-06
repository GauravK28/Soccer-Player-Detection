
//
// Created by Gaurav Krishnan on 4/28/20.
//

#include "mylibrary/detector.h"

#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <mylibrary/utils.h>
#include "opencv2/core.hpp"
#include "opencv2/tracking.hpp"
#include "cinder/Cinder.h"

using std::cout;
using std::endl;
using std::string;
using std::vector;


namespace mylibrary {
    void Detector::Detect(cv::VideoCapture& select_cap, bool split_teams, bool save_vid) {
      should_split_teams_ = split_teams;
      should_save_ = save_vid;

      cap_ = select_cap;
      cap_.set(cv::CAP_PROP_FPS, 30);

      // to save detection video
      string path = "/Users/gauravkrishnan/Downloads/"
                    "cinder_0.9.2_mac/my-projects/"
                    "final-project-GauravK28/assets/tracked/detected";
      string time_stamp = mylibrary::GetTimeStamp();
      path = path + time_stamp + ".avi";

      if (should_save_) {
        cv::VideoWriter video(path,
                              cv::VideoWriter::fourcc('M','J','P','G'),20,
                              cv::Size(cap_.get(cv::CAP_PROP_FRAME_WIDTH),
                                       cap_.get(cv::CAP_PROP_FRAME_HEIGHT)));
        video_ = video;
      }

      if (!cap_.isOpened()) {
        cout << "Error opening video file "  << endl;
        return;
      }

      while(cap_.isOpened()) {
        cap_ >> frame_;
        if (frame_.empty()) {
          break;
        }

        DetectFrame();

        char c = (char) cv::waitKey(25);
        if(c==27) { // stops playing video with ESCAPE KEY
          break;
        }
      }// end of while loop

      cap_.release();
      if (should_save_) {
        video_.release();
      }

      cv::destroyAllWindows();
    }

    // seperate method to look at single frame
    void Detector::DetectFrame() {
      box_counts_ = 0;
      team_1_cnt_ = 0;
      team_2_cnt_ = 0;

      // converts image to hsv
      cv::Mat hsv; // output image
      cvtColor(frame_, hsv, CV_BGR2HSV, 0);

      // grass color (green color range)
      auto lower_grass = cv::Scalar(40, 40, 40);
      auto upper_grass = cv::Scalar(70, 255, 255);

      // CREATING MASK ON GRASS
      cv::Mat mask_image;
      cv::inRange(hsv, lower_grass, upper_grass, mask_image);
      cv::Mat result; // result means after mask
      cv::bitwise_and(hsv, hsv, result, mask_image);
      // convert hsv to gray bc threshold only works on single channel images
      cv::cvtColor(result, result, cv::COLOR_BGR2GRAY, 0);

      // CREATING KERNAL FOR THRESHOLD
      // Notes: https://stackoverflow.com/questions/22965277/opencv-removing-noise-from-image
      // kernel to do morph op on thresholded image.
      int kMaxNoiseWidth = 10;
      int kMaxNoiseHeight = 10;
      cv::Mat kernel = cv::Mat(cv::Size(kMaxNoiseWidth,kMaxNoiseHeight),
                               CV_8UC1,cv::Scalar(255));

      // INFO:
      // Thresholding: determines players vs non-players (reason for binary down below)
      // morphologyEx: gets rid of noise/ sharpen player contours

      // CREATING THRESHOLD FOR MORPH
      // Notes: https://docs.opencv.org/3.4/d7/d4d/tutorial_py_thresholding.html
      // https://stackoverflow.com/questions/17141535/how-to-use-the-otsu-threshold-in-opencv
      // Using otsu to determine threshold value automatically
      // thresh val is inconsequential bc OTSU determines the thresh value
      cv::Mat temp; // temp image to do thresholding
      cv::threshold(result,temp, -1, 255,
              cv::THRESH_BINARY_INV + cv::THRESH_OTSU);
      cv::Mat output;
      // Notes: https://docs.opencv.org/trunk/d9/d61/tutorial_py_morphological_ops.html
      cv::morphologyEx(temp,output,cv::MORPH_CLOSE, kernel);
      // Only Morph_close just to focus on contours inside grass area

      // DETECTING CONTOURS
      // Contour notes:
      // https://opencv-python-tutroals.readthedocs.io/en/latest/py_tutorials/py_imgproc/py_contours/py_table_of_contents_contours/py_table_of_contents_contours.html
      cv::Mat canny_output;
      vector<vector<cv::Point> > contours;
      int lower_thresh = 100;
      int ratio = 2;
      // Detect edges using canny
      // Notes: https://docs.opencv.org/2.4/doc/tutorials/imgproc/shapedescriptors/find_contours/find_contours.html
      Canny(output, canny_output, lower_thresh,lower_thresh * ratio,3 );
      // Find contours
      findContours(canny_output, contours,
                   CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE,
                   cv::Point(0, 0) );

      for (auto& contour: contours) {
        // bounding rect notes:
        // https://opencv-python-tutroals.readthedocs.io/en/latest/py_tutorials/py_imgproc/py_contours/py_contour_features/py_contour_features.html#contour-features
        cv::Rect roi = cv::boundingRect(contour); // roi = region of interest

        // tries to filter out non-player ROIs
        // arbitrary values based on guess & check
        if (roi.height >= roi.width && roi.height > 15  && roi.width < 100) {

          // yellow range
          auto lower_color = cv::Scalar(21, 50, 50);
          auto upper_color = cv::Scalar(36, 255, 255);
          int team_1_presence = FindPlayer(roi, lower_color, upper_color);

          // navy blue range
          lower_color = cv::Scalar(96,0,0);
          upper_color = cv::Scalar(114,150,150);
          int team_2_presence = FindPlayer(roi, lower_color, upper_color);

          if (!should_split_teams_) { // no team seperation
            if (team_1_presence > kteam1thresh && team_2_presence > kteam2thresh) { // multiple players
              cv::rectangle(frame_, roi, cv::Scalar(0, 0, 255));
              box_counts_++;
            } else if (team_1_presence > kteam1thresh || team_2_presence > kteam2thresh) { // any other player
              cv::rectangle(frame_, roi, cv::Scalar(255, 0, 0));
              box_counts_++;
            }

          } else if (team_1_presence > kteam1thresh && team_2_presence > kteam2thresh) { // multiple players
            cv::rectangle(frame_, roi, cv::Scalar(0, 0, 255));
            box_counts_++;
          } else if (team_1_presence > kteam1thresh) { // differentiate team 1
            cv::rectangle(frame_, roi, cv::Scalar(0, 255, 255));
            box_counts_++;
            team_1_cnt_++;
          } else if (team_2_presence > kteam2thresh) { // differentiate team 2
            cv::rectangle(frame_, roi, cv::Scalar(255, 0, 0));
            box_counts_++;
            team_2_cnt_++;
          }
        }

      }

      //FindBall();

      if (should_save_) { // saving video
        video_.write(frame_);

      } else { // playing video
        cv::imshow("Detection", frame_);
      }
    }


    int Detector::FindPlayer(const cv::Rect& roi,
                             const cv::Scalar& lower_color, const cv::Scalar& upper_color) {
      // Creating mask for team color
      // crops frame to just the ROI
      cv::Mat team_hsv;
      cv::Mat temp = frame_(roi);
      temp.copyTo(team_hsv); // forces copy constructor instead of ref
      // so primary frame is not modified
      cv::cvtColor(team_hsv, team_hsv, CV_BGR2HSV, 0);

      cv::Mat team_mask;
      cv::inRange(team_hsv, lower_color, upper_color, team_mask);

      cv::Mat team;
      cv::bitwise_and(team_hsv, team_hsv, team, team_mask);
      cv::cvtColor(team, team, CV_HSV2BGR, 0);
      cv::cvtColor(team, team, CV_BGR2GRAY, 0);
      // presence of the color in the ROI
      int team_count = cv::countNonZero(team);
      return team_count;
    }

    int Detector::GetBoxCounts() {
      return box_counts_;
    }

    int Detector::GetTeam1Count() {
      return team_1_cnt_;
    }
    int Detector::GetTeam2Count() {
      return team_2_cnt_;
    }

    void Detector::SetFrame(const cv::Mat& frame) {
      frame_ = frame;
    }

    void Detector::SetShouldSplitTeams(bool set_team) {
      should_split_teams_ = set_team;
    }





    void Detector::FindBall() {
      cv::Mat img_gray;
      cv::cvtColor(frame_, img_gray, cv::COLOR_BGR2GRAY, 0);

      auto lower_white = cv::Scalar(0,0,10);
      auto upper_white = cv::Scalar(255,255,195);

//      // CREATING MASK ON GRASS
      cv::Mat mask_image;
      cv::inRange(img_gray, lower_white, upper_white, mask_image);
      cv::Mat result; // result means after mask
      cv::bitwise_and(img_gray, img_gray, result, mask_image);

      medianBlur(img_gray, img_gray, 5);

      cv::imshow("Test", img_gray);

      int kMaxNoiseWidth = 3;
      int kMaxNoiseHeight = 3;
      cv::Mat kernel = cv::Mat(cv::Size(kMaxNoiseWidth,kMaxNoiseHeight),
                               CV_8UC1,cv::Scalar(255));
      cv::Mat morphed;
      cv::adaptiveThreshold(img_gray, morphed, 255,
                            cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY,5,15);
      cv::threshold(img_gray,morphed, 100, 255, cv::THRESH_BINARY);
      cv::morphologyEx(img_gray, morphed,cv::MORPH_ERODE, kernel, cv::Point(-1,-1), 2);
      cv::morphologyEx(morphed, morphed,cv::MORPH_DILATE, kernel, cv::Point(-1,-1), 3);
      cv::morphologyEx(morphed, morphed,cv::MORPH_OPEN, kernel,cv::Point(-1,-1), 1);

      cv::imshow("between", morphed);
      cv::morphologyEx(morphed, morphed,cv::MORPH_CLOSE, kernel, cv::Point(-1,-1), 1);

      // reduces false circles 9x9 is kernal size recommended by
      // https://docs.opencv.org/2.4/modules/imgproc/doc/feature_detection.html?highlight=houghcircles
      GaussianBlur(img_gray, img_gray, cv::Size(5, 5), 2, 2);

      medianBlur(img_gray, img_gray, 5);
      // Find circles
      vector<cv::Vec3f> circles;
      HoughCircles(img_gray, circles, CV_HOUGH_GRADIENT,
                   2, img_gray.rows / 4, 200, 100, 1, 15 );
      for (auto& circle : circles) {

        cv::Point center(cvRound(circle[0]), cvRound(circle[1]));
        cout << center.x << " " << center.y << endl;
        int radius = cvRound(circle[2]);
        // draw the circle center
        cv::circle( frame_, center, 3,
                cv::Scalar(0,255,0), -1, 8, 0 );
        // draw the circle outline
        cv::circle( frame_, center, radius,
                cv::Scalar(0,0,255), 3, 8, 0 );
      }
      cv::imshow("end", frame_);

      cv::Mat canny_output;
      vector<vector<cv::Point> > contours;
      int lower_thresh = 100;
      int ratio = 2;
      // Detect edges using canny
      // Notes: https://docs.opencv.org/2.4/doc/tutorials/imgproc/shapedescriptors/find_contours/find_contours.html
      Canny(morphed, canny_output, lower_thresh,lower_thresh * ratio,5);
      // Find contours
      findContours(canny_output, contours,
                   CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE,
                   cv::Point(0, 0) );

      cv::imshow("before", frame_);
      for (auto& contour: contours) {
        cv::Rect roi = cv::boundingRect(contour);
        if (roi.width > 3 && roi.width < 20 && roi.height > 3 && roi.height < 20) {
          lower_white = cv::Scalar(0,0,40);
          upper_white = cv::Scalar(179,0,225);
//          //white range
          auto lower_white = cv::Scalar(0,0,0);
          auto upper_white = cv::Scalar(0,0,255);
          int count = FindPlayer(roi, lower_white, upper_white);
          if (count >= 1) {
            cv::rectangle(frame_, roi, cv::Scalar(255, 0, 0));
          }
        }
      }

      cv::imshow("after", frame_);
    }

}

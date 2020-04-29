// Copyright (c) 2020 [Your Name]. All rights reserved.
#ifndef FINALPROJECT_MYLIBRARY_EXAMPLE_H_
#define FINALPROJECT_MYLIBRARY_EXAMPLE_H_

#include <opencv2/videoio.hpp>
#include "opencv2/core.hpp"
#include "opencv2/tracking.hpp"
#include "cinder/Cinder.h"


namespace mylibrary {

  class PlayerTracker {
    public :
      void InitializeTracker(cv::VideoCapture& select_cap);
      void LockTracker();
      void Track();

    private:
      //functions
      cv::Ptr<cv::Tracker> SelectTracker(const std::string& track_type);
      void SetBoxColor(std::vector<cv::Scalar>& colors, const int box_num);

      //vars
      cv::Ptr<cv::Tracker> tracker_;

      cv::VideoCapture cap_;
      cv::Mat first_frame_;

      std::vector<cv::Rect> bounding_boxes_; // tracking boxes for players
      std::vector<cv::Scalar> box_colors_;

      const std::string tracking_type_ = "CSRT";
      // "BOOSTING", "MIL", "KCF", "TLD", "MEDIANFLOW", "GOTURN", "MOSSE", "CSRT"

      cv::Ptr<cv::MultiTracker> multi_tracker_;
  };


}  // namespace mylibrary


#endif // FINALPROJECT_MYLIBRARY_EXAMPLE_H_

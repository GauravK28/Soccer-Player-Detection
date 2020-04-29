// Copyright (c) 2020 [Your Name]. All rights reserved.

#include <mylibrary/tracker.h>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>

#include "opencv2/core.hpp"
#include "opencv2/tracking.hpp"
#include "cinder/Cinder.h"

using cv::Ptr;
using cv::Tracker;
using std::string;
using std::vector;
using std::cout;
using std::endl;

namespace mylibrary {

void PlayerTracker::InitializeTracker(cv::VideoCapture& select_cap) {
  // create a video capture object to read videos
  cap_ = select_cap;


  // quit if unabke to read video file
  if(!cap_.isOpened()) {
    cout << "Error opening video file " << endl;
  }
  cap_ >> first_frame_;  // read first frame

  cv::selectROIs("Select Objects", first_frame_,
          bounding_boxes_, true, false);
  // fromCenter to false allows box to be drawn from top left corner
  // Space or Enter to save a selection, cancel selection with 'c'
  // Escape to play video

  if(bounding_boxes_.empty()) {
    cout << "No bounding boxes selected" << endl;
    return;
  }

  SetBoxColor(box_colors_, bounding_boxes_.size());
  cv::destroyAllWindows();
}

void PlayerTracker::LockTracker() {
  // adds an individual tracker for each bounded box
  // then adds all of those to a multitracker system

  multi_tracker_ = cv::MultiTracker::create();
  for (auto& bounding_box : bounding_boxes_) {
    multi_tracker_->add(SelectTracker(tracking_type_),
                        first_frame_, cv::Rect2d(bounding_box));
  }
}

void PlayerTracker::Track() {
  // could make, playing video a utility function,
  // since it is very similar to the code in my_app.cc

  while(cap_.isOpened()) {
    cv::Mat frame = first_frame_;
    cap_ >> frame;
    if (frame.empty()) {
      break;
    }

    multi_tracker_->update(frame);
    // creates tracking boxes
    for(int i = 0; i < multi_tracker_->getObjects().size(); i++) {
      cv::rectangle(frame, multi_tracker_->getObjects()[i], box_colors_[i], 2, 1);
    }
    imshow("Tracking Video", frame);

    if (cv::waitKey(1) == 27) {
      break;
    }
  }

  cap_.release();
  cv::destroyAllWindows();
}


/**
 * More of a utility function, could go in a seperate file
 * Used to differentiate colors based on team
 *
 * @param colors
 * @param box_num
 */
void PlayerTracker::SetBoxColor(std::vector<cv::Scalar>& colors, const int box_num) {
  for(int i=0; i < box_num; i++) {
    // clang said to use emplace_back instead of push_back
    colors.emplace_back(255, 0, 0);
  }
}

/**
 * Types of trackers opencv has implemented
 * Mainly for debugging to find best tracker type for sports
 *
 * @param track_type tracker type in string format
 * @return tracker ptr object with specified tracking algo
 */
Ptr<Tracker> PlayerTracker::SelectTracker(const string& track_type) {
  // had to use if statements instead of switch_case
  // b/c C++ doesn't allow string switches
  if (track_type ==  "BOOSTING")
    tracker_ = cv::TrackerBoosting::create();
  else if (track_type == "MIL")
    tracker_ = cv::TrackerMIL::create();
  else if (track_type == "KCF")
    tracker_ = cv::TrackerKCF::create();
  else if (track_type == "TLD")
    tracker_ = cv::TrackerTLD::create();
  else if (track_type == "MEDIANFLOW")
    tracker_ = cv::TrackerMedianFlow::create();
  else if (track_type == "GOTURN")
    tracker_ = cv::TrackerGOTURN::create();
  else if (track_type == "MOSSE")
    tracker_ = cv::TrackerMOSSE::create();
  else if (track_type == "CSRT")
    tracker_ = cv::TrackerCSRT::create();
  else {
    cout << "Incorrect tracker name" << endl;
  }
  return tracker_;
}


}  // namespace mylibrary

// Copyright (c) 2020 CS126SP20. All rights reserved.

#ifndef FINALPROJECT_APPS_MYAPP_H_
#define FINALPROJECT_APPS_MYAPP_H_

#include <cinder/app/App.h>

#include <imgui.h>

#include "opencv2/core/core.hpp"
#include "opencv2/highgui.hpp"

namespace myapp {

/**
 * Unable to use cinder's gui display because opencv library is not Cinder-OpenCV3
 * it is the normal OpenCV library because the cinder version would not properly link
 */
class MyApp : public cinder::app::App {
 public:
  MyApp();
  void setup() override;
  void update() override;
  void draw() override;
  void keyDown(cinder::app::KeyEvent) override;


 private:
  // user inputed file path
  char buf_[25]; //ImGui only supports fixed-length input strings

  // video playing
  cv::VideoCapture cap_;
  void PlayVideo();
  std::string file_path_; // user specified video's path
  // default video file path
  const std::string def_file_path_ = "/Users/gauravkrishnan/Downloads/"
                                     "cinder_0.9.2_mac/my-projects/"
                                     "final-project-GauravK28/assets/soccer.mp4";
};

}  // namespace myapp

#endif  // FINALPROJECT_APPS_MYAPP_H_

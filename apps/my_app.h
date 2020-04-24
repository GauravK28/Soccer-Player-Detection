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
  // Button functions & vars
//  void InitializeButton(const char* button_name);
//  const ImVec4 base_color_ = {0.492f, 0.995f, 0.765f, 0.784f };
//  const ImVec4 press_color_ = {0.202f, 0.549f, 0.798f, 0.784f };
//  ImVec4 current_color_;
//  int color_count_;
  // user inputed file path
  char buf_[25]; //ImGui only supports fixed-length input strings

  // video playing
  cv::VideoCapture cap_;
  void PlayVideo();
  std::string file_path_; // user specified video's path
  // default video file path
  const std::string def_file_path_ = "/Users/gauravkrishnan/Downloads/"
                                     "cinder_0.9.2_mac/my-projects/"
                                     "final-project-GauravK28/assets/soccer-game-clip.mp4";
};

}  // namespace myapp

#endif  // FINALPROJECT_APPS_MYAPP_H_

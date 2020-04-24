// Copyright (c) 2020 [Your Name]. All rights reserved.

#include "my_app.h"

#include "mylibrary/example.h"

#include <cinder/app/App.h>
#include <cinder/gl/wrapper.h>

#include "opencv2/core/core.hpp"
#include "opencv2/highgui.hpp"

#include "CinderImGui.h"

using namespace cinder;
using namespace cv;
using std::cout;
using std::endl;
using std::string;


namespace myapp {

using cinder::app::KeyEvent;

MyApp::MyApp() : color_count_{0},
                file_path_{""}{ }

void MyApp::setup() {
  ImGui::initialize();

  cap_.set(CAP_PROP_FRAME_WIDTH, 430);
  cap_.set(CAP_PROP_FRAME_HEIGHT, 315);
}

void MyApp::update() {
}

void MyApp::draw() {
  gl::clear(Color( 0, 0, 0 ));
  ImGui::Begin("Player Tracking"); {
//    color_count_ = 0;
//
//    InitializeButton("Upload");
//    ImGui::SameLine();
//    //Source: https://github.com/ocornut/imgui/blob/master/imgui_demo.cpp
//    string label = "(Enter video file path)";
//    //static char test[25];
//    ImGui::InputText(label.c_str(), buf_, IM_ARRAYSIZE(buf_));
//    ImGui::PushItemWidth(20);
//
//    InitializeButton("Play video");
//    InitializeButton("create tracking video");

      if (ImGui::Button("Upload")) {
          string base_path = "/Users/gauravkrishnan/Downloads/"
                   "cinder_0.9.2_mac/my-projects/"
                   "final-project-GauravK28/assets/";
          file_path_ = buf_;
          // trim the string to get rid of empty characters
          file_path_.erase(std::find_if(
                file_path_.rbegin(), file_path_.rend(), [](int ch) {
                    return !std::isspace(ch);
                }).base(), file_path_.end());
      }
      ImGui::SameLine();
      string label = "(Enter video file path)";
      ImGui::InputText(label.c_str(), buf_, IM_ARRAYSIZE(buf_));
    if (ImGui::Button("Play Video")) {
      cout << "should play video?" << endl;
        if (!cap_.isOpened()) {
          cout << "We gucci?" << endl;
          PlayVideo();
        }
    }
    if (ImGui::Button("create tracking video")) {

    }


  }
  ImGui::End();
}


void MyApp::keyDown(KeyEvent event) {

}

void MyApp::PlayVideo() {
  // Source: https://www.learnopencv.com/read-write-and-display-a-video-using-opencv-cpp-python/
  if (!file_path_.empty()) {
    cap_.open(file_path_);
    cout << "file path empty?" << endl;
  } else {
    cap_.open(def_file_path_);
  }
  if(!cap_.isOpened()){
    cout << "Error opening video stream or file" << endl;
    return;
  }
  cout << "Playing video?" << endl;
  while (true) {
    Mat frame;
    // Capture frame-by-frame
    cap_ >> frame;
    // If the frame is empty, break immediately
    if (frame.empty())
      break;

    // Display the resulting frame
    imshow( "Frame", frame );

    char c = (char)cv::waitKey(25);
    if(c==27)
      break;
  }
  // When everything done, release the video capture object
  cap_.release();
  // Closes all the frames
  destroyAllWindows();
}

}  // namespace myapp

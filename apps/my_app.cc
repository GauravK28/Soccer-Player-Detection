// Copyright (c) 2020 [Your Name]. All rights reserved.

#include "my_app.h"

#include "mylibrary/tracker.h"

#include <cinder/app/App.h>
#include <cinder/gl/wrapper.h>
#include <mylibrary/trackertwo.h>

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

MyApp::MyApp() : file_path_{""}{ }

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
        file_path_ = base_path + file_path_;
        cout << "the new file path" << file_path_ << endl;
    }

    ImGui::SameLine();
    string label = "(Enter video file path)";
    // TODO: Maybe allow user to open file explorer to get a file?
    //  possible with this add on https://github.com/AirGuanZ/imgui-filebrowser
    ImGui::InputText(label.c_str(), buf_, IM_ARRAYSIZE(buf_));
    if (ImGui::Button("Play Video")) {
      cout << "should play video?" << endl;
        if (!cap_.isOpened()) {
          cout << "We gucci?" << endl;
          PlayVideo();
        }
    }
    if (ImGui::Button("Setup tracking video")) {
      mylibrary::PlayerTracker tracker;


      if (file_path_.empty()) {
        VideoCapture cap(def_file_path_);
        tracker.InitializeTracker(cap);

      } else {
        VideoCapture cap(file_path_);
        tracker.InitializeTracker(cap);
      }
      tracker.LockTracker();
      tracker.Track();
    }

    if (ImGui::Button("Test image")) {
      mylibrary::Detector track;
      VideoCapture cap(def_file_path_);
      track.SetupTracker(cap);
    }
    if (ImGui::Button("Testing team sep")) {
      mylibrary::Detector track;
      VideoCapture cap(def_file_path_);
      track.Test(cap);
    }

    // TODO: add gui option to set grass color, and both team colors

    // TODO: add ui option to select if team det. is wanted


  }
  ImGui::End();
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

    // Displays frame
    imshow( "Video Frame", frame );

    char c = (char) cv::waitKey(25);
    if(c==27) { // stops playing video with ESCAPE KEY
      break;
    }
  }

  cap_.release();
  destroyAllWindows();
}


void MyApp::keyDown(KeyEvent event) {}

}  // namespace myapp

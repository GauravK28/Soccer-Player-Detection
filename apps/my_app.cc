// Copyright (c) 2020 [Your Name]. All rights reserved.

#include "my_app.h"

#include <cinder/app/App.h>
#include <cinder/gl/wrapper.h>
#include "opencv2/core.hpp"
#include "opencv2/video.hpp"
#include "opencv2/highgui.hpp"
#include "CinderImGui.h"

using namespace cinder;
using std::cout;
using std::endl;


namespace myapp {

using cinder::app::KeyEvent;

MyApp::MyApp(): color_count_{0},
                should_play_video_{false}
                { }

void MyApp::setup() {
  ImGui::initialize();
}

void MyApp::update() {
  if (should_play_video_) {
    // image loading source: https://www.learnopencv.com/read-write-and-display-a-video-using-opencv-cpp-python/
    cv::VideoCapture cap("soccer-game-clip.mp4");
    if(!cap.isOpened()){
      cout << "Error opening video stream or file" << endl;
      return;
    }
    while (1) {
      cv::Mat frame;
      // Capture frame-by-frame
      cap >> frame;

      // If the frame is empty, break immediately
      if (frame.empty())
        break;

      // Display the resulting frame
      imshow( "Frame", frame );

      char c=(char)cv::waitKey(25);
      if(c==27)
        break;

    }
    // When everything done, release the video capture object
    cap.release();

    // Closes all the frames
    cv::destroyAllWindows();
  }
}

void MyApp::draw() {
  gl::clear(Color( 0, 0, 0 ));
  ImGui::Begin("Player Tracking"); {
    color_count_ = 0;
    DrawButton("Play video");
    DrawButton("Pause Video");
    DrawButton("create tracking video");
  }
  ImGui::PopStyleColor(color_count_);
  ImGui::End();
}

void MyApp::DrawButton(const char* button_name) {
  color_count_++;
  ImGui::PushStyleColor( ImGuiCol_ButtonActive, current_color_);
  ImGui::Button(button_name);
  if (ImGui::IsItemClicked()) {
    current_color_ = press_color_;
    cout << button_name << endl;
    if (strcmp(button_name, "Play video") != 0) {
      should_play_video_ = true;
    }
  } else {
    current_color_ = base_color_;
  }
}

void MyApp::keyDown(KeyEvent event) {

}

}  // namespace myapp

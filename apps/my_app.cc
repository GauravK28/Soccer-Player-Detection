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

MyApp::MyApp(): color_count_{0} { }

void MyApp::setup() {
  ImGui::initialize();
}

void MyApp::update() {
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
  } else {
    current_color_ = base_color_;
  }
}

void MyApp::keyDown(KeyEvent event) {

}

}  // namespace myapp

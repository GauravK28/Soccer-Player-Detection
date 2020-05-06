// Copyright (c) 2020 [Your Name]. All rights reserved.

#include "my_app.h"

#include <cinder/app/App.h>
#include <cinder/gl/wrapper.h>
#include <mylibrary/detector.h>
#include <mylibrary/utils.h>

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

    MyApp::MyApp() : file_path_{""},
                     is_box_checked_{false},
                     should_split_teams_{false}{ }

    void MyApp::setup() {
      ImGui::initialize();

      cap_.set(CAP_PROP_FRAME_WIDTH, 430);
      cap_.set(CAP_PROP_FRAME_HEIGHT, 315);
      cap_.set(CAP_PROP_FPS, 30);
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

          if (file_path_.empty()) {

          }
        }

        ImGui::SameLine();
        string label = "(Enter video file path)";
        // TODO: Maybe allow user to open file explorer to get a file?
        //  possible with this add on https://github.com/AirGuanZ/imgui-filebrowser
        ImGui::InputText(label.c_str(), buf_, IM_ARRAYSIZE(buf_));
        if (ImGui::Button("Play Video")) {
          if (!cap_.isOpened()) {

            if (!file_path_.empty()) {
              cap_.open(file_path_);
              cout << "file path empty?" << endl;
            } else {
              cap_.open(def_file_path_);
            }

            mylibrary::PlayVideo(cap_);


            //PlayVideo();
          }
        }

        if (ImGui::Checkbox("Seperate teams", &is_box_checked_)) {
          should_split_teams_ = is_box_checked_;
        }

        if (ImGui::Button("Play tracked video")) {
          mylibrary::Detector track;
          string file;
          if (file_path_.empty()) {
            file = def_file_path_;
          } else {
            file = file_path_;
          }
          VideoCapture cap(file);
          track.Detect(cap, should_split_teams_, false);
        }


        if (ImGui::Button("Save tracked video")) {
          mylibrary::Detector track;
          string file;
          if (file_path_.empty()) {
            file = def_file_path_;
          } else {
            file = file_path_;
          }
          VideoCapture cap(file);
          track.Detect(cap, should_split_teams_, true);

        }

        // TODO: add gui option to set grass color, and both team colors
        // TODO: add ui option to select if team det. is wanted
      }

      ImGui::End();
    }



    void MyApp::keyDown(KeyEvent event) {}

}  // namespace myapp

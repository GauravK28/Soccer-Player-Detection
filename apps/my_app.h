// Copyright (c) 2020 CS126SP20. All rights reserved.

#ifndef FINALPROJECT_APPS_MYAPP_H_
#define FINALPROJECT_APPS_MYAPP_H_

#include <cinder/app/App.h>
#include <imgui.h>


namespace myapp {

class MyApp : public cinder::app::App {
 public:
  MyApp();
  void setup() override;
  void update() override;
  void draw() override;
  void keyDown(cinder::app::KeyEvent) override;
 private:
  void DrawButton(const char* button_name);
  const ImVec4 base_color_ = {0.492f, 0.995f, 0.765f, 0.784f };
  const ImVec4 press_color_ = {0.202f, 0.549f, 0.798f, 0.784f };
  ImVec4 current_color_;
  int color_count_;
};

}  // namespace myapp

#endif  // FINALPROJECT_APPS_MYAPP_H_

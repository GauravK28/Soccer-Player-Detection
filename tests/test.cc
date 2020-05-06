// Copyright (c) 2020 CS126SP20. All rights reserved.

#define CATCH_CONFIG_MAIN

#include <catch2/catch.hpp>
#include <cinder/Rand.h>

#include <mylibrary/detector.h>

using std::string;


TEST_CASE("Correct number of player boxes", "[player-detection]") {
  mylibrary::Detector detect;
  string file = "/Users/gauravkrishnan/Downloads/"
                "cinder_0.9.2_mac/my-projects/"
                "final-project-GauravK28/assets/soccer.mp4";

  cv::VideoCapture cap(file);
  cv::Mat first_frame;
  cap >> first_frame;
  detect.SetFrame(first_frame);
  detect.SetShouldSplitTeams(true);
  detect.DetectFrame();
  const int players_in_view = 15;

  REQUIRE(detect.GetBoxCounts() > players_in_view * .6);
}

TEST_CASE("Team sepeartion", "[team]") {
  mylibrary::Detector detect;
  string file = "/Users/gauravkrishnan/Downloads/"
                "cinder_0.9.2_mac/my-projects/"
                "final-project-GauravK28/assets/soccer.mp4";

  cv::VideoCapture cap(file);
  cv::Mat first_frame;
  cap >> first_frame;
  detect.SetFrame(first_frame);
  detect.SetShouldSplitTeams(true);
  detect.DetectFrame();

  REQUIRE(detect.GetTeam1Count() >= 4);
  REQUIRE(detect.GetTeam2Count() >= 3);
}
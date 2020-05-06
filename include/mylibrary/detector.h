//
// Created by Gaurav Krishnan on 4/28/20.
//

#ifndef FINALPROJECT_MYLIBRARY_TRACKERTWO_H
#define FINALPROJECT_MYLIBRARY_TRACKERTWO_H

#include <opencv2/videoio.hpp>
#include "opencv2/core.hpp"
#include "opencv2/tracking.hpp"
#include "cinder/Cinder.h"

namespace mylibrary {
    class Detector {
    public :
        void Detect(cv::VideoCapture& select_cap, bool split_teams, bool save_vid);
        void DetectFrame();
        int FindPlayer(const cv::Rect& roi,
                       const cv::Scalar& lower_color, const cv::Scalar& upper_color);
        int GetBoxCounts();
        int GetTeam1Count();
        int GetTeam2Count();
        void SetFrame(const cv::Mat& frame);
        void SetShouldSplitTeams(bool set_team);

        void FindBall();
    private:
        cv::VideoCapture cap_;
        cv::Mat frame_;

        bool should_split_teams_;

        cv::VideoWriter video_;
        bool should_save_;

        // color roi thresholds
        const int kteam1thresh = 500; // currently set for yellow
        const int kteam2thresh = 70; // currently set for navy blue

        // for testing
        int box_counts_;
        int team_1_cnt_;
        int team_2_cnt_;
    };

}

#endif //FINALPROJECT_MYLIBRARY_TRACKERTWO_H

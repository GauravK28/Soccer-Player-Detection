//
// Created by Gaurav Krishnan on 4/28/20.
//

#include "mylibrary/utils.h"


/**
 * More of a utility function, could go in a seperate file
 * Used to differentiate colors based on team
 *
 * @param colors
 * @param box_num
 */
void mylibrary::SetBoxColor(std::vector<cv::Scalar>& colors, const int box_num) {
  for(int i=0; i < box_num; i++) {
    // clang said to use emplace_back instead of push_back
    colors.emplace_back(255, 0, 0);
  }
}

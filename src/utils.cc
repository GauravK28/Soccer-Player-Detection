//
// Created by Gaurav Krishnan on 4/28/20.
//

#include "mylibrary/utils.h"

using std::string;
using std::to_string;

namespace mylibrary {

    string mylibrary::GetTimeStamp() {
      std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
      time_t tt = std::chrono::system_clock::to_time_t(now);
      tm local_tm = *localtime(&tt);

      string hour =  to_string(local_tm.tm_hour);
      string min = to_string(local_tm.tm_min);
      string sec =  to_string(local_tm.tm_sec);

      return hour + ":" + min + ":" + sec;
    }

};

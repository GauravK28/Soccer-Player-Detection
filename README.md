# Soccer Player Tracking

[![license](https://img.shields.io/badge/license-MIT-green)](LICENSE)
[![docs](https://img.shields.io/badge/docs-yes-brightgreen)](docs/README.md)

![Player Detection](/snapshots/detection-snapshot.png)

![GUI Snapshot](/snapshots/gui-snapshot.png)

**Author**: Gaurav Krishnan - [`gauravk2@illinois.edu`](mailto:example@illinois.edu)

Read [this document](https://cliutils.gitlab.io/modern-cmake/chapters/basics/structure.html) to understand the project
layout.



### Project Goal
At first this was meant to be a tracking project, but shifted 
to detection because tracking was too clunky and not accurate enough. So now the
this project aims to read in a video and detect the players on the screen using bounding
rectangles to mark their approximate locations. The detection primarily works by identifiying
contours in each frame. Each frame of the video goes through several manipulations and transformations
that make it easier for the opencv `findContours()` function to detect the players. 

### Quick start
Project should be compatible with Mac/Windows/Linux as
OpenCV and ImGUI are compatible with all three.
Only potential problem could occur when saving the detected video as the video encoding is system specific
 but opencv should be able to handle that.

#### Dependencies
- [OpenCV](https://github.com/opencv/opencv/tree/4.3.0) 
    - To get opencv working with cinder follow this [guide](https://blog.zhajor.com/2016/10/install-opencv-and-make-a-test-project-with-clion/)
    - To add the project to cmake you may need to add this snippet under the project sections in the cmake file
    ```
       find_package(OpenCV REQUIRED) 
       add_library(opencv INTERFACE)
       include_directories(${OpenCV_INCLUDE_DIRS})
       target_link_libraries(opencv INTERFACE ${OpenCV_LIBS})
  ```
- [ImGui](https://github.com/simongeilfus/Cinder-ImGui)
    - First clone the library into the cinder cinder-blocks folder
    - ImGui is already a Cmake project so you can just add the name to the Cmake
    in that is in your apps directory. It should look like this:
    ```
  ci_make_app(
           APP_NAME    cinder-myapp
           CINDER_PATH ${CINDER_PATH}
           SOURCES     ${SOURCE_LIST}
           LIBRARIES   mylibrary
           BLOCKS      Cinder-ImGui
   )
  ```


### Features
- Uses a default video clip of a soccer game
- Users can upload their own videos
- Can play the clip
- Can detect players on the field
- Can toggle on/off team seperation in the detection
    - With team seperation, it detects if several players are in one bounding box
    and marks it differently
- Can download the detected video

### Important Notes
- To exit out of a video just press the escape key
- Uploading files are based from the assets directory (which is git-ignored) and only handles
file names that are 24 characters or less (limitation because of ImGui 
but can be increased if needed)

### Limitations
- Ball detection is not working
- Accurate player detection is still missing
- Overlapping players are hard to detect
- Takes a long time to create a new detected video
- Player detection based on color of the field and the team uniform
colors, which could restrict the detection if they are similar colors
- Goalies are not detected because they wear different color uniforms
- Sometimes the ref gets detected if his shirt is a similar color
- Colors for detected teams are currently hard coded (looking into
creating a tool for user to select team color)

### Useful Documentation
[Why hsv & masking](https://www.learnopencv.com/color-spaces-in-opencv-cpp-python/)

[Image segmentation using color spaces](https://realpython.com/python-opencv-color-spaces/#visualizing-nemo-in-hsv-color-space)

[Basic process](https://stackoverflow.com/questions/10948589/choosing-the-correct-upper-and-lower-hsv-boundaries-for-color-detection-withcv)

[opencv morphological operations](https://docs.opencv.org/3.4/d3/dbe/tutorial_opening_closing_hats.html)


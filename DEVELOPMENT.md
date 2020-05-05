# Development

---
 - **4/21/20**  
    - Got OpenCV3 to work
    - Added imgui library for gui
    - Added basic gui functionality
    - Started using opencv
- **4/22/20**
    - Reconfigured opencv to use default lib instead 
    of cinder-opencv3
- **4/23/20**
    - Implemented basic video playing
    - Integrated imgui with opencv with cinder
    - Can start video through imgui
- **4/26/20**
    - Brainstormed ways to track objects that go 
    out of frame
- **4/28/2020**
    - Implemented simple multi-object tracking
    - Need to come up with a better tracking system
    - Or need to figure out
    - Started working on basic obj detect w/ contours
- **4/28/2020**
    - Started working on object detection
    - Got grass image mask working
    - Started working on removing noise from image
    - Need to mess with kernal size and various noise
     reduction methods
    - Need to create pixel color finder in gui to not hardcode
    colors
- **5/1/2020**
    - Got player detection working
    - Need to implement team seperation
    - Need to implement ball detection
- **5/2/2020**
    - Worked on team detection
- **5/4/2020**
    - Basic team detection working
    - Compartamentalized part of code
    - Could not get ball detection to work, tried:
        - Hough circle transformation w/ adaptive thresh, reg. thresh, 
        & various morphs
        - Canny edge detection w/ adaptive thresh, reg. thresh, 
        & various morphs
    - Maybe add ui button to select if team det. on/off
    - Need to create video playing/creating util file
- **5/5/2020**
    - Refactored detection code
    - Implemented video writing
    - Need to refactor video playing, to not repeat code
    

    
# Soccer Player Tracking 
##### Author: Gaurav Krishnan
 
### Project Goal
Be able to process a clip of a soccer game and
track the players of each team in the frame. The program should also be
able to track the position of the ball. The program will also have a user
interface to allow the user to upload soccer game clips and it will output
the clip with the players and ball outlined with boxes, labelled, and tracked.

### Background Knowledge
I do not have much background in computer vision, so I thought player tracking
would be a great introductory project to learn about CV. Since the OpenCV 
library for C++ is robust, I thought it would use this as my CV framework. I have
some experience with machine learning and large scale data manipulation
using SKLearn in python so that skill could possibly help me when dealing with 
image data.

### External libraries
- [OpenCV](https://github.com/opencv/opencv/tree/4.3.0) 
- GUI (possibly [imgui](https://github.com/ashitani/opencv_imgui_viewer))

### Time Line
##### Week 1:
- Get development environment and libraries set up
- Learn about OpenCV and player tracking
- Start working on tracker, starting with the ball
##### Week 2:
- Finish ball tracking
- Finish player tracking and increase tracking accuracy
##### Week 3:
- Polish player
- Create GUI for user interaction

### Possible Challenges
- Getting the libraries to function properly
- Properly tracking players with sufficient accuracy
- Creating a smooth, usable GUI

### Stretch Goals
- Make GUI look polished
- Ball possesion detection
- Player classification by name
- Detect Scoring
- Detect players of different sports
- Maybe switch a detecting algorithm vs a tracking one to
 identify specific players
- Player heatmaps
- Allow user to draw a box around a player the want to track
- Super stretch: Convert the player's position from 3d to 2d using a
homographic transformation and display the positions on a top-down view map

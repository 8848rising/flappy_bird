# flappy_bird

A Simple flappy bird game on Arduino. Using only the Aidafurit library for graphics other things are built from scratch.

Basic working:
  1. Takes inputs from the buttons on the board which are debounced to rectify glitches
  2. Random pillars are created at the start and are stored in an array, those pillars who have there coordinates in the frame get displayed
  3. if the ball object touches the frame or pillar --> game over
  4. the ball moves using simple physics equations x coordinate is fixed y coordinate is based on s=ut + 1/2(a*t^2);
  5. a ball bounce means giving the ball an upward vilocity 
  6. The game also have a frame accalaration so as the game progresses the speed of the game keeps increasing

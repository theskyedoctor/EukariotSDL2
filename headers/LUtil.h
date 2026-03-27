//
// Created by skyehigh on 3/6/26.
//

#include "LOpenGL.h"

//screen constants
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;
const int SCREEN_FPS = 60;
const int DEBUG = 0;

//Color Modes
const int COLOR_MODE_CYAN = 0;
const int COLOR_MODE_MULTI = 1;

bool initGL();
/*
Pre Condition:
 -A valid OpenGL context
Post Condition:
 -Initializes matrices and clear color
 -Reports to console if there was an OpenGL error
 -Returns false if there was an error in initialization
Side Effects:
 -Projection matrix is set to identity matrix
 -Modelview matrix is set to identity matrix
 -Matrix mode is set to modelview
 -Clear color is set to black
*/

bool loadMedia();
/*
 *Pre condition:
 *-valid OpenGL context
 *post condition:
 *-loads media to use in the program
 *-Reports to console if there was an error in loading the media
 *-returns true if the media loaded successfully
 *Side effects:
 *-none
 */

void update();
/*
Pre Condition:
 -None
Post Condition:
 -Does per frame logic
Side Effects:
 -None
*/

void render(float timeValue);
/*
Pre Condition:
 -A valid OpenGL context
 -Active modelview matrix
Post Condition:
 -Renders the scene
Side Effects:
 -Clears the color buffer
 -Swaps the front/back buffer
*/

//Input handler
void handleKeys( unsigned char key, int x, int y );
/*
Pre Condition:
 -None
Post Condition:
 -Toggles the color mode when the user presses q
 -Cycles through different projection scales when the user presses e
Side Effects:
 -If the user presses e, the matrix mode is set to projection
*/

//Shader loading utility programs. these only log so have no effects
void printProgramLog( GLuint program );
void printShaderLog( GLuint shader );


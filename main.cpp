/*
Graphics Prgm 3 for Sarah Ibsen
Class: CS 445


Architecture Structure:
----------------------
GLUT generation of an aquarium

*/

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <stdio.h>
#include<iostream> // for debugging
#include "OpenGL445Setup-2025.h"

const int frameTime = 0;
bool isAnimating = false;
bool showMessage = true;
unsigned int fishDisplayList;
float fishX = 0.0;
bool movingLeft = true;
bool rotating = false;
float rotationAngle = 0.0f;
bool rotatingLeft = false;

const float TANK_LEFT = -400.0f;
const float TANK_RIGHT = 400.0f;
const float TANK_BOTTOM = -200.0f;
const float FISH_HALF_WIDTH = 75.0f;  // Adjusted fish width for body collision check

// function to check the boundaries where the fish needs to turn
void checkBoundaries() {
    if (movingLeft) {
        if (fishX - FISH_HALF_WIDTH < TANK_LEFT + 4.0f) {
            rotating = true;
            rotatingLeft = true;
        }
    } else {
        if (fishX + FISH_HALF_WIDTH > TANK_RIGHT - 4.0f) {
            rotating = true;
            rotatingLeft = false;
        }
    }
}

void setupCamera() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-400.0f, 400.0f, -400.0f, 400.0f, 900.0f, 100.0f); // Orthographic view
  //  glTranslatef(0.0, 0.0, -400.0); // Camera is placed at (0, 0, -400) -- putting it in the positive view port sinze we are already set up in the neg
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void drawSmallFish() {
    glColor3f(1.0, (126.0/255.0), (4.0/255.0)); // PANTONE Tangelo (orange)

    glPushMatrix();
    glPushMatrix();
    glTranslatef(-82.0, -120.0, -200.0);

    glScalef(50.0, 20.0, 10.0); // small fish dimensions
    glutWireOctahedron();
    glPopMatrix();

    // drawing of the fish tail : )
  //  glPushMatrix();
    glTranslatef(-64.0, -110.0, -210.0); // Slightly behind the fish body (Z direction)
    glBegin(GL_TRIANGLES);
        glVertex3f(25.0, -10.0, 20.0);   // Top of tail
        glVertex3f(50.0, 0.0, 20.0);  // Bottom of tail
        glVertex3f(50.0, -20.0, 20.0);   // Back tip of tail
    glEnd();
    glPopMatrix();


}

void drawDecoration() {
    glColor3f(0.0, 0.5, 0.0); // dark green
    const float boxDepth = -400.0f;
    const float centerX = 75.0f;
    const float baseY = -400.0f;
    const float centerBoxHeight = 175.0f;
    const float halfHeight = centerBoxHeight * 0.5f;
    const float twoThirdsHeight = centerBoxHeight * (2.0f / 3.0f);

    // --- Center vertical box ---
    glPushMatrix();
    glTranslatef(centerX, baseY + centerBoxHeight / 2.0f, -400.0f); // Centered vertically
    glScalef(50.0f, centerBoxHeight, boxDepth);
    glutWireCube(1.0);
    glPopMatrix();

    // --- Left horizontal branch ---

    // 1/2 height
    glPushMatrix();
    glTranslatef(centerX - 50.0f, baseY + halfHeight, -400.0f); // Left of center box
    glScalef(50.0f, 20.0f, boxDepth); // Horizontal branch: wide, shallow
    glutWireCube(1.0);
    glPopMatrix();

    // --- Baby vertical branch sitting on top of that horizontal branch ---
    glPushMatrix();

    // Dimensions of the baby branch
    float babyWidth = 10.0f;
    float babyHeight = 25.0f;  // Make it visibly vertical
    float horizontalBranchHeight = 20.0f;

    // Position:
    // - X: start at center of horizontal branch (centerX - 50), then offset left toward branch edge
    // - Y: baseY + halfHeight (height of horizontal branch center), plus half horizontal + half baby = place on top
    // - Z: same as branch
    float babyX = centerX - 50.0f - 20.0f; // 20 units to the left of center of horizontal branch
    float babyY = baseY + halfHeight + (horizontalBranchHeight / 2.0f) + (babyHeight / 2.0f);
    float babyZ = -400.0f;

    glTranslatef(babyX, babyY, babyZ);
    glScalef(babyWidth, babyHeight, boxDepth);
    glutWireCube(1.0);
    glPopMatrix();
    // --- Right horizontal branch ---


    // 2/3 height
    glPushMatrix();
    glTranslatef(centerX + 50.0f, baseY + twoThirdsHeight, -400.0f);
    glScalef(50.0f, 20.0f, boxDepth);
    glutWireCube(1.0);
    glPopMatrix();

    // --- baby vertical branch on top of right 2/3 height branch ---
    glPushMatrix();

    // Dimensions
    float babyWidth2 = 10.0f;
    float babyHeight2 = 25.0f;
    float horizontalBranchHeight2 = 20.0f;

    // X: move toward the outer (right) edge of the branch
    float babyX2 = centerX + 50.0f + 20.0f;  // nudged rightward from center
    float babyY2 = baseY + twoThirdsHeight + (horizontalBranchHeight2 / 2.0f) + (babyHeight2 / 2.0f);
    float babyZ2 = -400.0f;

    glTranslatef(babyX2, babyY2, babyZ2);
    glScalef(babyWidth2, babyHeight2, boxDepth);
    glutWireCube(1.0);
    glPopMatrix();
}


void createFishDisplayList() {
    fishDisplayList = glGenLists(1);
    if (fishDisplayList == 0) {
        printf("Error creating display list\n");
        return;
    }

    glNewList(fishDisplayList, GL_COMPILE);

    glPushMatrix();

    // Body of the fish
  //  glTranslatef(0.0, 0.0, -400.0);
    glPushMatrix();
    glTranslatef(0.0, 0.0, -400.0);
    glColor3f((191.0 / 255.0), (25.0 / 255.0), (50.0 / 255.0)); // Pantone True Red
    glScalef(150.0, 50.0, 25.0);
    glutWireOctahedron();

    glPopMatrix(); // Finish scaling body

    //  it rotates with the fish
   // glPushMatrix();

    // translate relative to the fish's center
    glTranslatef(95.0, 20.0, -390.0); // based on tail's position behind body

    glBegin(GL_TRIANGLES);
        glVertex3f(50.0, -20.0, 40.0);   // Top of tail
        glVertex3f(100.0, 0.0, 40.0);  // Bottom of tail
        glVertex3f(100.0, -40.0, 40.0);   // Back tip of tail
    glEnd();

   // glPopMatrix();
    glPopMatrix();

    glEndList();
    printf("Display list created\n");
}

void display_func() {
    setupCamera();

    glClearColor((180.0 / 255.0), (220.0 / 255.0), (234.0 / 255.0), 1.0); // Pantone Spun Sugar
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPushMatrix();

    // Step 1: Move to fish position
    glTranslatef(fishX, 0.0f, 0.0f);

    // Step 2: Move pivot to fish center (z = -400), rotate, then undo pivot translation
    glTranslatef(0.0f, 0.0f, -400.0f);
    glRotatef(rotationAngle, 0.0f, 1.0f, 0.0f);  // Rotate around Y axis
    glTranslatef(0.0f, 0.0f, 400.0f);

    // Step 3: Draw fish
    glCallList(fishDisplayList);

    glPopMatrix();

    drawSmallFish(); // Draw the small static fish
    drawDecoration(); // Draw the decorations

    glutSwapBuffers();
}

// Timer function for animation
void timer(int value) {
    if (!rotating) {
        if (movingLeft) {
            fishX -= 5.0;
            checkBoundaries(); // Check for boundary collision and start rotation if necessary
        } else {
            fishX += 5.0;
            checkBoundaries();
        }
    } else {
        rotationAngle += (rotatingLeft ? 5.0 : -5.0);
        if (abs(rotationAngle) >= 180.0) {
            rotating = false;
            rotationAngle = 0.0;
            movingLeft = !movingLeft; // Change direction after rotation
        }
    }

    glutPostRedisplay();
    glutTimerFunc(50, timer, 0); // 20 fps
}

// Keyboard function for quitting
void keyboardFunc(unsigned char key, int x, int y) {
    switch (key) {
        case 'q':
        case 'Q':
            exit(0);
            break;
    }
}

int main(int argc, char **argv)
{
    glEnable(GL_DEPTH_TEST);
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 800);
    glutCreateWindow("Fish Tank");

    glewInit();   // Initialize GLEW before using OpenGL functions

    createFishDisplayList();
    glutDisplayFunc(display_func);
    glutKeyboardFunc(keyboardFunc);
    glutTimerFunc(50, timer, 0); // Start the timer animation

    glutMainLoop();
    return 0;
}

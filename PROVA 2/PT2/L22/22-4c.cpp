#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <cstdlib>
#include <cmath>
#include <iostream>
#include <freeglut.h>
#include <vector>

#define PI 3.14159265

using namespace std;

// Globals.
static int isAnimate = 0; // Animated?
static int animationPeriod = 33; // Time interval between frames.
static float t = 0.0; // Time parameter.
static float t2 = 0.0;
static float t3 = -40.0;
static int mid = 0;
static float h = 1; // Horizontal component of initial velocity.
static float v = 4.0; // Vertical component of initial velocity.
static float g = 0.2;  // Gravitational accelaration.
static char theStringBuffer[10]; // String buffer.
static long font = (long)GLUT_BITMAP_8_BY_13; // Font selection.


// Drawing routine.
void drawScene(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    // Place scene in frustum.
    glTranslatef(-15.0, -15.0, -45.0);

    glPushMatrix();
    glTranslatef(0.0, -5.0, 0.0);
    glColor3f(1.0, 0.0, 0.0);
    glutSolidCube(10);
    glTranslatef(20.0, 0.0, 0.0);
    glutSolidCube(10);
    glTranslatef(20.0, 0.0, 0.0);
    glutSolidCube(10);
    glPopMatrix();

    // Apply equations of motion to transform sphere.
    glTranslatef(h * t, v * t - (g / 2.0) * t * t, 0.0);

    if (t == 20) {
        glTranslatef(0.0, t2, 0.0);
    }
    if (t2 < -40) {
        glTranslatef(0.0, t3, 0.0);
    }

    // Sphere.
    glColor3f(0.0, 0.0, 1.0);
    glutWireSphere(2.0, 10, 10);

    glutSwapBuffers();
}

// Timer function.
void animate(int value)
{
    if (isAnimate)
    {
        if (t < 20) {
            t += 1.0;
        }
        if (t == 20) {
            t2 -= 1.0;
        }
        if (t2 < -40) {
            t3 += 1.0;
            t = 20.1;
        }
        if (t3 > 0) {
            t2 = 0;
            t += 1.0;
        }

        if (t >= 40) {
            isAnimate = 0;
            t3 = 0;
        }

    }
    glutTimerFunc(animationPeriod, animate, 1);
    glutPostRedisplay();
}

// Initialization routine.
void setup(void)
{
    glClearColor(0.2, 0.2, 0.2, 0.0);
}

// OpenGL window reshape routine.
void resize(int w, int h)
{
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-5.0, 5.0, -5.0, 5.0, 5.0, 100.0);

    glMatrixMode(GL_MODELVIEW);
}

// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 27:
        exit(0);
        break;
    case ' ':
        if (isAnimate) isAnimate = 0;
        else {
            isAnimate = 1;
            t = t2= 0;
            t3 = -40;
        }
        glutPostRedisplay();
        break;
    case 'r':
        isAnimate = 0;
        t = 0.0;
        glutPostRedisplay();
        break;
    default:
        break;
    }
}


// Callback routine for non-ASCII key entry.
void specialKeyInput(int key, int x, int y)
{
    if (key == GLUT_KEY_UP) v += 0.05;;
    if (key == GLUT_KEY_DOWN) if (v > 0.1) v -= 0.05;
    if (key == GLUT_KEY_RIGHT) h += 0.05;
    if (key == GLUT_KEY_LEFT) if (h > 0.1) h -= 0.05;
    if (key == GLUT_KEY_PAGE_UP) g += 0.05;
    if (key == GLUT_KEY_PAGE_DOWN) if (g > 0.1) g -= 0.05;

    glutPostRedisplay();
}

// Routine to output interaction instructions to the C++ window.
void printInteraction(void)
{
    cout << "Interaction:" << endl;
    cout << "Press space to toggle between animation on and off." << endl
        << "Press right/left arrow kes to increase/decrease the initial horizontal velocity." << endl
        << "Press up/down arrow keys to increase/decrease the initial vertical velocity." << endl
        << "Press page up/down keys to increase/decrease gravitational acceleration." << endl
        << "Press r to reset." << endl;
}

// Main routine.
int main(int argc, char** argv)
{
    printInteraction();
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("throwBall.cpp");
    setup();
    glutDisplayFunc(drawScene);
    glutReshapeFunc(resize);
    glutKeyboardFunc(keyInput);
    glutTimerFunc(5, animate, 1);
    glutSpecialFunc(specialKeyInput);
    glutMainLoop();

    return 0;
}
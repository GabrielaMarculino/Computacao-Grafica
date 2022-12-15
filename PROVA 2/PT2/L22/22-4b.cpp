#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <cstdlib>
#include <cmath>
#include <iostream>
#include <freeglut.h>
#include <vector>

using namespace std;

static int isAnimate = 0; // Animated?
static int animationPeriod = 33; // Time interval between frames.
static float angle = 0.0;
static float Xangle = 45.0, Yangle = -30.0, Zangle = 0.0;
// Drawing routine.
void drawScene(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    // Modeling transformations.
    glTranslatef(0.0, 0.0, -35.0);

    glRotatef(Zangle, 0.0, 0.0, 1.0);
    glRotatef(Yangle, 0.0, 1.0, 0.0);
    glRotatef(Xangle, 1.0, 0.0, 0.0);

    glPushMatrix();
    glColor3f(1.0, 0.0, 0.0);
    glScalef(1.0, 0.01, 1.0);
    glutSolidCube(20.0);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0, 4.5, 18.0);
    glRotatef(-25, 1.0, 0.0, 0.0);
    glColor3f(0.0, 0.0, 1.0);
    glScalef(1.0, 0.01, 1.0);
    glutSolidCube(20.0);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0, 5.0, 0.0);
    glColor3f(1.0, 1.0, 1.0);
    if (angle < 90) {
        glTranslatef(0.0, 0.0, 0.1 * angle);
    }
    else {
        glTranslatef(0.0, 0.5, 0.1 * angle);
    }
    glRotatef((GLfloat)2*angle, 1.0, 0.0, 0.0);
    glutWireSphere(2.5, 10, 8);
    glPopMatrix();

    
    

    glFlush();
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

void animate(int value)
{
    if (isAnimate)
    {
        angle += 5.0;
        if (angle > 180.0) angle -= 180.0;
    }
    glutTimerFunc(animationPeriod, animate, 1);
    glutPostRedisplay();
}

// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y)
{
    switch (key) {
    case 'x':
        Xangle += 5.0;
        if (Xangle > 360.0) Xangle -= 360.0;
        glutPostRedisplay();
        break;
    case 'X':
        Xangle -= 5.0;
        if (Xangle < 0.0) Xangle += 360.0;
        glutPostRedisplay();
        break;
    case 'y':
        Yangle += 5.0;
        if (Yangle > 360.0) Yangle -= 360.0;
        glutPostRedisplay();
        break;
    case 'Y':
        Yangle -= 5.0;
        if (Yangle < 0.0) Yangle += 360.0;
        glutPostRedisplay();
        break;
    case 'z':
        Zangle += 5.0;
        if (Zangle > 360.0) Zangle -= 360.0;
        glutPostRedisplay();
        break;
    case 'Z':
        Zangle -= 5.0;
        if (Zangle < 0.0) Zangle += 360.0;
        glutPostRedisplay();
        break;
    case ' ':
        if (isAnimate == 0) isAnimate = 1;
        else isAnimate = 0;
        glutPostRedisplay();
        break;
    case 27:
        exit(0);
        break;
    default:
        break;
    }
}

// Main routine.
int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("box.cpp");
    setup();
    glutDisplayFunc(drawScene);
    glutReshapeFunc(resize);
    glutKeyboardFunc(keyInput);
    glutTimerFunc(5, animate, 1);
    glutMainLoop();

    return 0;
}
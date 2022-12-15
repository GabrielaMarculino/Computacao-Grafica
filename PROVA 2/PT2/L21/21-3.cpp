#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <cstdlib>
#include <cmath>
#include <iostream>
#include <freeglut.h>
#include <vector>

using namespace std;

static int isWire = 0;
static float Xangle = 0.0, Yangle = 0.0, Zangle = 0.0;
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


    glColor3f(1.0, 1.0, 1.0);
    if (isWire) {

        glScalef(2.0, 0.2, 1.0);
        glutSolidCube(20.0);

        glTranslatef(-8.5, -65.0, -8.5);
        glScalef(0.2, 10.0, 0.2);
        glutSolidCube(15.0);
        glTranslatef(0.0, 0.0, 83.5);
        glutSolidCube(15.0);
        glTranslatef(85.0, 0.0, 0.0);
        glutSolidCube(15.0);
        glTranslatef(0.0, 0.0, -84.0);
        glutSolidCube(15.0);
    }
    else {

        glScalef(2.0, 0.2, 1.0);
        glutWireCube(20.0);

        glTranslatef(-8.5, -65.0, -8.5);
        glScalef(0.2, 10.0, 0.2);
        glutWireCube(15.0);
        glTranslatef(0.0, 0.0, 83.5);
        glutWireCube(15.0);
        glTranslatef(85.0, 0.0, 0.0);
        glutWireCube(15.0);
        glTranslatef(0.0, 0.0, -84.0);
        glutWireCube(15.0);
    }

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
        if (isWire == 0) isWire = 1;
        else isWire = 0;
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
    glutMainLoop();

    return 0;
}
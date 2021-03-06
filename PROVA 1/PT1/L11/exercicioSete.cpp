// g++ -o exercicioSete exercicioSete.cpp -lglut -lGL -lGLU -lm
#include <cstdlib>
#include <cmath>
#include <iostream>

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

#define PI 3.14159265

using namespace std;

// Globals.
static int M = 3;
static int isWire = 0; // Is wireframe?
static float R = 5.0; // Radius of hemisphere.
static int p = 4; // Number of longitudinal slices.
static int q = 2; // Number of latitudinal slices.
static float Xangle = 0.0, Yangle = 0.0, Zangle = 0.0; // Angles to rotate hemisphere.

// Drawing routine.
void drawScene(void)
{
    int  i, j;

    glClear(GL_COLOR_BUFFER_BIT);

    glLoadIdentity();

    // Command to push the hemisphere, which is drawn centered at the origin, 
    // into the viewing frustum.
    glTranslatef(0.0, 0.0, -20.0);

    // Commands to turn the hemisphere.
    glRotatef(Zangle, 0.0, 0.0, 1.0);
    glRotatef(Yangle, 0.0, 1.0, 0.0);
    glRotatef(Xangle, 1.0, 0.0, 0.0);

    if (isWire) glPolygonMode(GL_FRONT, GL_LINE); else glPolygonMode(GL_FRONT, GL_FILL);
    glColor3f(1.0, 1.0, 1.0);

    glBegin(GL_POLYGON);
    glVertex3f(-4.0, -4.0, 0.0);
    glVertex3f(-4.0, -4.0, -4.0);
    glVertex3f(-4.0, 6.0, -4.0);
    glVertex3f(-4.0, 6.0, 0.0);
    glEnd();
    
    glBegin(GL_TRIANGLE_STRIP);
    for (i = -M; i <= 0; ++i) {
        glVertex3f(6.0 + 10.0 * (float)i / M, 6.0 * (float)(i * i) / (M * M), 0.0);
        glVertex3f(6.0 + 10.0 * (float)i / M, 6.0 * (float)(i * i) / (M * M), -4.0);
    }
    glEnd();
    
    glBegin(GL_POLYGON);
    glVertex3f(6.0, 0.0, 0.0);
    glVertex3f(6.0, 0.0, -4.0);
    glVertex3f(6.0, -4.0, -4.0);
    glVertex3f(6.0, -4.0, 0.0);
    glEnd();


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
    switch (key)
    {
    case 27:
        exit(0);
        break;
    case 'P':
        p += 1;
        glutPostRedisplay();
        break;
    case 'p':
        if (p > 3) p -= 1;
        glutPostRedisplay();
        break;
    case 'Q':
        q += 1;
        glutPostRedisplay();
        break;
    case 'q':
        if (q > 3) q -= 1;
        glutPostRedisplay();
        break;
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
    default:
        break;
    }
}

// Routine to output interaction instructions to the C++ window.
void printInteraction(void)
{
    cout << "Interaction:" << endl;
    cout << "Press P/p to increase/decrease the number of longitudinal slices." << endl
        << "Press Q/q to increase/decrease the number of latitudinal slices." << endl
        << "Press x, X, y, Y, z, Z to turn the hemisphere." << endl;
}

// Main routine.
int main(int argc, char** argv)
{
    printInteraction();
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("hemisphere.cpp");
    setup();
    glutDisplayFunc(drawScene);
    glutReshapeFunc(resize);
    glutKeyboardFunc(keyInput);
    glutMainLoop();

    return 0;
}
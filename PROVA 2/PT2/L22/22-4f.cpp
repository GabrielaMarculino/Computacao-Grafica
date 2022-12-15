#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <cstdlib>
#include <cmath>
#include <iostream>
#include <freeglut.h>
#include <vector>

static int isAnimate = 0; // Animated?
static int animationPeriod = 100; // Time interval between frames.
static float angle = 0.0; // Rotation.angle of hat.
static int year = 0, day = 0;

void init(void)
{
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glShadeModel(GL_FLAT);
}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT);


    glPushMatrix();
    glColor3f(1.0, 0.5, 0.0);
    glutWireSphere(1.0, 20, 16);   /* draw sun */
    glRotatef((GLfloat)angle, 0.0, 1.0, 0.0);
    glTranslatef(2.0, 0.0, 0.0);
    glRotatef((GLfloat)2 * angle, 0.0, 1.0, 0.0);
    glColor3f(0.0, 1.0, 0.0);
    glutWireSphere(0.2, 10, 8);    /* draw smaller planet */
    glTranslatef(0.0, 0.0, 0.5);
    glRotatef((GLfloat)2 * angle, 0.0, 1.0, 0.0);
    glColor3f(1.0, 1.0, 1.0);
    glutWireSphere(0.1, 10, 8);
    glPopMatrix();


    glPushMatrix();
    glRotatef((GLfloat)angle, 0.0, 1.0, 0.0);
    glRotatef((GLfloat)2 * angle, 1.0, 0.0, 0.0);
    glTranslatef(2.0, 1.0, 0.0);
    glColor3f(1.0, 1.0, 1.0);
    glutWireSphere(0.1, 10, 8);
    glPopMatrix();

    glutSwapBuffers();
}

void reshape(int w, int h)
{
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (GLfloat)w / (GLfloat)h, 1.0, 20.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

void animate(int value)
{
    if (isAnimate)
    {
        angle += 5.0;
        if (angle > 360.0) angle -= 360.0;
    }
    glutTimerFunc(animationPeriod, animate, 1);
    glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y)
{
    switch (key) {
    case ' ':
        if (isAnimate) isAnimate = 0;
        else isAnimate = 1;
        glutPostRedisplay();
        break;
    case 27:
        exit(0);
        break;
    default:
        break;
    }
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow(argv[0]);
    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(5, animate, 1);
    glutMainLoop();
    return 0;
}
// g++ -o exercicioQuatro exercicioQuatro.cpp -lglut -lGL -lGLU -lm
#include <cstdlib>
#include <cmath>
#include <iostream>

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

#define PI 3.14159265
#define N 40.0 // Number of vertices on the boundary of the disc.

using namespace std;

// Globals.
static int isWire = 0; // Is wireframe?
static long font = (long)GLUT_BITMAP_8_BY_13; // Font selection.

// Routine to draw a bitmap character string.
void writeBitmapString(void *font, char *string)
{  
   char *c;

   for (c = string; *c != '\0'; c++) glutBitmapCharacter(font, *c);
} 

// Function to draw a disc with center at (X, Y, Z), radius R, parallel to the
// xy-plane.
void drawDisc(float R, float X, float Y, float Z)
{
   float t;
   int i;

   glBegin(GL_TRIANGLE_FAN);
      glVertex3f( X, Y, Z);
      for(i = 0; i <= N; ++i)
	  {
         t = 2 * PI * i / N; 
         glVertex3f(X + cos(t) * R, Y + sin(t) * R, Z);
	  }
   glEnd();
}

// Drawing routine.
void drawScene(void)
{  
   float angle;
   int i;

   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear the buffers including 
                                                       // the depth buffer.

   glPolygonMode(GL_FRONT, GL_FILL);

   glEnable(GL_DEPTH_TEST);
   glColor3f(1.0, 0.0, 0.0);
   drawDisc(10.0, 20.0, 20.0, 0.0);

   glColor3f(0.2, 0.2, 0.2);
   drawDisc(5.0, 20.0, 20.0, 0.5);

   glColor3f(1.0, 0.0, 0.0);
   drawDisc(10.0, 20.0, 35.0, 0.0);

   glColor3f(0.2, 0.2, 0.2);
   drawDisc(5.0, 20.0, 35.0, 0.5);

   glDisable(GL_DEPTH_TEST);

   if(isWire){

       glPolygonMode(GL_FRONT, GL_LINE);

   }else{

       glPolygonMode(GL_FRONT, GL_FILL);
   }
   glColor3f(1.0, 0.0, 0.0);
   glBegin(GL_TRIANGLE_STRIP);

   for(i = 0; i <= N; ++i){
       
       angle = 2 * PI * i / N;
       glVertex3f(70 + cos(angle) * 10.0, 30 + sin(angle) * 10.0, 0.0);
       glVertex3f(70 + cos(angle) * 20.0, 30 + sin(angle) * 20.0, 0.0);
   }
   glEnd();

   glBegin(GL_TRIANGLE_STRIP);

   for(i = 0; i <= N; ++i){

       angle = 2 * PI * i / N;
       glVertex3f(70 + cos(angle) * 10.0, 60 + sin(angle) * 10.0, 0.0);
       glVertex3f(70 + cos(angle) * 20.0, 60 + sin(angle) * 20.0, 0.0);
   }
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
   glOrtho(0.0, 100.0, 0.0, 100.0, -1.0, 1.0);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}

// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y)
{
   switch(key) 
   {
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

// Routine to output interaction instructions to the C++ window.
void printInteraction(void)
{
   cout << "Interaction:" << endl;
   cout << "Press the space bar to toggle between wirefrime and filled for the lower annulus." << endl;  
}

// Main routine.
int main(int argc, char **argv) 
{
   printInteraction();
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH); // Initialize the buffers 
                                                             // including the depth buffer.
   glutInitWindowSize(500, 500);
   glutInitWindowPosition(100, 100); 
   glutCreateWindow("circularAnnuluses.cpp");
   setup(); 
   glutDisplayFunc(drawScene); 
   glutReshapeFunc(resize);  
   glutKeyboardFunc(keyInput);
   glutMainLoop(); 

   return 0;  
}
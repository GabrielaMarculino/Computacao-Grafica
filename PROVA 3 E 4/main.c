/*
 *  Gabriela Marculino 
 *  gcc -o main main.c -lglut -lGL -lm -L/usr/local/lib -lGLU
 *  ./quadricas
 * 
 *  1- elipsoide
 *  2- cone
 *  4- paraboloide
 *  5- cone duplo
 *  	
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>


#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#ifdef GL_VERSION_1_1
GLuint list;
GLint spinx = 0;
GLint spiny = 0;
GLfloat ztrans = 0.0;
GLfloat polyfactor = 1.0;
GLfloat polyunits = 1.0;
GLfloat a = 1.0;
GLfloat b = 1.0;
GLfloat c = 1.0;
GLint theta_div = 32;
GLint alpha_div = 32;
GLint theta_animate = 1;
GLint theta_min = 0;
GLint alpha_min = 0;
GLfloat theta_max = 2 * 3.14159;
GLfloat alpha_max = 2 * 3.14159;
GLint render = 1;
int isAnimate = 0;
unsigned char function = 'e';

// Funções de cálculo das coordenadas
GLfloat xPos(GLfloat theta, GLfloat alpha){
GLfloat result;
	switch (function) {
		case 'e':
		  result = a * cos(theta) * sin(alpha);
	      break;
		case 'c':
		  result = a * alpha * cos(theta);
	      break;
		case 'l':
		  result = a * cos(theta);
	      break;
		case 'p':
		  result = a * alpha * cos(theta);
	      break;
		default:
		   result = a * cos(theta) * sin(alpha);
		   break;
}
return (result);     //coordenada x em função de theta e alpha
}

GLfloat yPos(GLfloat theta, GLfloat alpha){
GLfloat result;
	switch (function) {
		case 'e':
		  result = b * sin(theta) * sin(alpha);
	      break;
		case 'c':
		  result = b * alpha * sin(theta);
	      break;
		case 'l':
		  result = b * sin(theta);
	      break;
		case 'p':
		  result = b * alpha * sin(theta);
	      break;
		default:
		   result = b * sin(theta) * sin(alpha);
		   break;
}
return (result);     //coordenada y em função de theta e alpha
}

GLfloat zPos(GLfloat alpha){
GLfloat result;
	switch (function) {
		case 'e':
		  result = c * cos(alpha);
	      break;
		case 'c':
		  result = c * alpha;
	      break;
		case 'l':
		  result = alpha;
	      break;
		case 'p':
		  result = alpha * alpha;
	      break;
		default:
		   result = c * cos(alpha);
		   break;
}
return (result);                 //coordenada z em função de e alpha
}


GLvoid glDrawQuadric() 
{
GLfloat vertex1_xcoord, vertex1_ycoord, vertex1_zcoord, vertex2_xcoord,
 vertex2_ycoord, vertex2_zcoord, vertex3_xcoord, vertex3_ycoord,
  vertex3_zcoord,vertex4_xcoord, vertex4_ycoord, vertex4_zcoord;
GLfloat theta1, theta2, alpha1, alpha2;
GLint i, j;

glColor3f(0.0f,0.0f,1.0f);        

for(i = theta_min; i < theta_animate; i++){
    theta1 = i * theta_max / theta_div;
    theta2 = (i+1) * theta_max / theta_div;

    for(j = alpha_min; j < alpha_div; j++){
        alpha1 = j * alpha_max / alpha_div;
        alpha2 = (j+1) * alpha_max / alpha_div;

        vertex1_xcoord = xPos(theta1, alpha1);
        vertex1_ycoord = yPos(theta1, alpha1);
        vertex1_zcoord = zPos(alpha1);

        vertex2_xcoord = xPos(theta1, alpha2);
        vertex2_ycoord = yPos(theta1, alpha2);
        vertex2_zcoord = zPos(alpha2);

        vertex3_xcoord = xPos(theta2, alpha1);
        vertex3_ycoord = yPos(theta2, alpha1);
        vertex3_zcoord = zPos(alpha1);

        vertex4_xcoord = xPos(theta2, alpha2);
        vertex4_ycoord = yPos(theta2, alpha2);
        vertex4_zcoord = zPos(alpha2);

        if(render)
            glBegin(GL_QUADS);                 //modelo renderizado
        else
            glBegin(GL_LINE_STRIP);            // modelo wireframe  //vetor normal  

                glNormal3f( (vertex1_xcoord+vertex4_xcoord)/2, (vertex1_ycoord+vertex4_ycoord)/2, (vertex1_zcoord+vertex4_zcoord)/2 ); // Normal de cada quadrilátero
                glVertex3f(vertex1_xcoord,vertex1_ycoord,vertex1_zcoord); 
                glVertex3f(vertex2_xcoord,vertex2_ycoord,vertex2_zcoord);
                glVertex3f(vertex4_xcoord,vertex4_ycoord,vertex4_zcoord);
                glVertex3f(vertex3_xcoord,vertex3_ycoord,vertex3_zcoord);
            glEnd();
        }
    }
}

/*  display() 
 */
void display (void)
{
    GLfloat mat_ambient[] = { 0.8, 0.8, 0.8, 1.0 };
    GLfloat mat_diffuse[] = { 1.0, 0.0, 0.5, 1.0 };
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat gray[] = { 0.8, 0.8, 0.8, 1.0 };
    GLfloat black[] = { 0.0, 0.0, 0.0, 1.0 };

    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPushMatrix ();
    glTranslatef (0.0, 0.0, ztrans);
    glRotatef ((GLfloat) spinx, 1.0, 0.0, 0.0);
    glRotatef ((GLfloat) spiny, 0.0, 1.0, 0.0);
	
	list = glGenLists(1);
    glNewList (list, GL_COMPILE);
	   glDrawQuadric();
    glEndList ();

    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, gray);
    glMaterialfv(GL_FRONT, GL_SPECULAR, black);
    glMaterialf(GL_FRONT, GL_SHININESS, 0.0);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glEnable(GL_POLYGON_OFFSET_FILL);
    glPolygonOffset(polyfactor, polyunits);
    glCallList (list);
    glDisable(GL_POLYGON_OFFSET_FILL);

    glDisable(GL_LIGHTING);
    glDisable(GL_LIGHT0);
    glColor3f (1.0, 1.0, 1.0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glCallList (list);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	

    glPopMatrix ();
    glFlush ();
}

/*  specify initial properties
 *  initialize lighting and depth buffer
 */
void gfxinit (void)
{
    GLfloat light_ambient[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 };

    GLfloat global_ambient[] = { 0.2, 0.2, 0.2, 1.0 };

    glClearColor (0.0, 0.0, 0.0, 1.0);

 

    glEnable(GL_DEPTH_TEST);

    glLightfv (GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv (GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv (GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv (GL_LIGHT0, GL_POSITION, light_position);
    glLightModelfv (GL_LIGHT_MODEL_AMBIENT, global_ambient);

    /*************************************************************/
    //Segundo ponto de luz
    GLfloat lightambient[] = { 1.0, 0.0, 0.0, 0.0 };
    GLfloat lightdiffuse[] = { 1.0, 0.0, 1.0, 0.0 };
    GLfloat lightspecular[] = { 0.0, 1.0, 0.0, 1.0 };
    GLfloat lightposition[] = { 0.0, 0.0, 0.0, 1.0 };

    GLfloat globalambient[] = { 0.1, 0.1, 0.1, 1.0 };

    glClearColor (0.0, 0.0, 0.0, 1.0);

 

    glEnable(GL_DEPTH_TEST);

    glLightfv (GL_LIGHT1, GL_AMBIENT, lightambient);
    glLightfv (GL_LIGHT1, GL_DIFFUSE, lightdiffuse);
    glLightfv (GL_LIGHT1, GL_SPECULAR, lightspecular);
    glLightfv (GL_LIGHT1, GL_POSITION, lightposition);
    glLightModelfv (GL_LIGHT_MODEL_AMBIENT, globalambient);
}

/*  call when window is resized  */
void reshape(int width, int height)
{
    glViewport (0, 0, width, height);
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    gluPerspective(45.0, (GLdouble)width/(GLdouble)height,
	    1.0, 10.0);
    glMatrixMode (GL_MODELVIEW);
    glLoadIdentity ();
    gluLookAt (0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

/*  call when mouse button is pressed  */
void mouse(int button, int state, int x, int y) {
    switch (button) {
	case GLUT_LEFT_BUTTON:
	    switch (state) {
		case GLUT_DOWN:
		    spinx = (spinx + 5) % 360; 
                    glutPostRedisplay();
		    break;
		default:
		    break;
            }
            break;
	case GLUT_RIGHT_BUTTON:
	    switch (state) {
		case GLUT_DOWN:
		    spiny = (spiny + 5) % 360; 
                    glutPostRedisplay();
		    break;
		default:
		    break;
            }
            break;
	case GLUT_MIDDLE_BUTTON:
	    switch (state) {
		case GLUT_DOWN:
		    if (render) render = 0;
			else
				render = 1;
			glutPostRedisplay();
		    break;
		default:
		    break;
            }
            break;		
        default:
            break;
    }
}

void keyboard (unsigned char key, int x, int y)
{
   switch (key) {
      case (27):
	     exit(0);
		 break;
      case 'z':
         if (ztrans < 4.0) {
            ztrans = (ztrans + 0.5);
            glutPostRedisplay();
         }
         break;
      case 'Z':
         if (ztrans > -5.0) {
            ztrans = (ztrans - 0.5);
            glutPostRedisplay();
         }
         break;
      case 'F':
         polyfactor = polyfactor + 0.1;
	 printf ("polyfactor is %f\n", polyfactor);
         glutPostRedisplay();
         break;
      case 'f':
         polyfactor = polyfactor - 0.1;
	 printf ("polyfactor is %f\n", polyfactor);
         glutPostRedisplay();
         break;
      case 'U':
         polyunits = polyunits + 1.0;
	 printf ("polyunits is %f\n", polyunits);
         glutPostRedisplay();
         break;
      case 'u':
         polyunits = polyunits - 1.0;
	 printf ("polyunits is %f\n", polyunits);
         glutPostRedisplay();
         break;
	 case 'A':
         a+= 0.1;
	 printf ("a is %f\n", a);
         glutPostRedisplay();
         break;
      case 'a':
         a+= -0.1;
	 printf ("a is %f\n", a);
         glutPostRedisplay();
         break;
	  case 'B':
         b+= 0.1;
	 printf ("b is %f\n", b);
         glutPostRedisplay();
         break;
      case 'b':
         b+= -0.1;
	 printf ("b is %f\n", b);
         glutPostRedisplay();
         break;
	  case 'C':
         c+= 0.1;
	 printf ("c is %f\n", c);
         glutPostRedisplay();
         break;
      case 'c':
         c+= -0.1;
	 printf ("c is %f\n", c);
         glutPostRedisplay();
         break;
	  case '1': //elipsoide
	     theta_min = 0;
		 alpha_min = 0;
		 theta_max = 2 * 3.14159;
		 alpha_max = 2 * 3.14159;
		 function = 'e';
         theta_animate = 1;
                 
		 break;
	  case '2': //cone eliptico
	     theta_min = 0;
		 alpha_min = 0;
		 theta_max = 2 * 3.14159;
		 alpha_max = 1;
		 function = 'c';
		 theta_animate = 1;
		 break;
	  /*case '3': //cilindro elipico
	     theta_min = 0;
		 alpha_min = 0;
		 theta_max = 2 * 3.14159;
		 alpha_max = 1;
		 function = 'l';
		 theta_animate = 1;
		 break;*/
	  case '4': //paraboloide elipico
	     theta_min = 0;
		 alpha_min = 0;
		 theta_max = 2 * 3.14159;
		 alpha_max = 1;
		 function = 'p';
	         theta_animate = 1;
		 break;
       case '5': //cone duplo
	     theta_min = 0;
		 alpha_min = -30;
		 theta_max = 2 * 3.14159;
		 alpha_max = 1;
		 function = 'c';
		 theta_animate = 1;
		 break;
         case ' ': 
         if (isAnimate) isAnimate = 0;
		 else isAnimate = 1;
         glutPostRedisplay();
		 break;
      default:
         break;
   }
}
void animate(int value)
{
   if (isAnimate) 
   {
      if ( theta_animate <= theta_div) 
	  {
	  theta_animate += 1;
	  }
   }
   glutTimerFunc(250, animate, 1);
   glutPostRedisplay();
}
/*  Main Loop
 *  Open window with initial window size, title bar, 
 *  RGBA display mode, and handle input events.
 */
int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
    glutCreateWindow(argv[0]);
    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutKeyboardFunc(keyboard);
    gfxinit();
    glutTimerFunc(5, animate, 1);
    glutMainLoop();
    return 0;
}
#else
int main(int argc, char** argv)
{
   
    return 0;
}
#endif



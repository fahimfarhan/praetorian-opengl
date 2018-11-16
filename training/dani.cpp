#include <bits/stdc++.h>

#ifdef _WIN32
#include <windows.h>
#include <glut.h>
#endif

//#ifdef linux
#include "GL/freeglut.h"
#include "GL/gl.h"
//#endif
    using namespace std;
    GLfloat angle = 0.0; //set the angle of rotation
    float Cx=0.0,Cy=0.0,Cz=0.0;
    typedef struct Car
    {
    	float x,y,z;
    	float angle;
    	float zvelocity,xvelocity;
    }Car;
    Car car;
    void keyboard(unsigned char key,int x,int y)
    {
    	if(key =='w')
    		Cz--;
    	if( key == 's')
    		Cz++;
    	if(key =='a')
    		Cx--;
    	if(key == 'd')
    		Cx++;
    	if(key==' ')
    	angle ++;
    }
    void cube (void) {
    	glPushMatrix();
    	glRotatef(angle,0,1,0);
    	glTranslatef(Cx,Cy,Cz); 
    	gluLookAt(0,0,Cz+5,0,0,0, 0.0, 1.0, 0.0);
    	glColor3f(1.0, 0.0, 0.0);
    	glutSolidCube(2);
    	glPopMatrix();
    }
    void display (void) {
    	glClearColor (1.0,1.0,1.0,1.0);
    	glClear (GL_COLOR_BUFFER_BIT);
    	glLoadIdentity();
    	glPushMatrix();
    	glTranslatef(2,0,-15);
    	glColor3f(0,1,0);
    	glutSolidCube(2);
    	glPopMatrix();
    	glPushMatrix();
    	glTranslatef(2,0,5);
    	glColor3f(0,1,0);
    	glutSolidCube(2);
    	glPopMatrix();
    	cube();
    	glutSwapBuffers();
    }
    void reshape (int w, int h) {
    	glViewport (0, 0, (GLsizei)w, (GLsizei)h);
    	glMatrixMode (GL_PROJECTION);
    	glLoadIdentity ();
    	gluPerspective (60, (GLfloat)w / (GLfloat)h, 1.0, 100.0)
    		;
    	glMatrixMode (GL_MODELVIEW);
    }
    int main (int argc, char **argv) {
    	glutInit (&argc, argv);
    	glutInitDisplayMode (GLUT_DOUBLE); //set up the double 
    	glutInitWindowSize (500, 500);
    	glutInitWindowPosition (100, 100);
    	glutCreateWindow ("A basic OpenGL Window");
    	glutDisplayFunc (display);
    	glutIdleFunc (display);
    	glutKeyboardFunc(keyboard);
    	glutReshapeFunc (reshape);
    	glutMainLoop ();
    	return 0;
    }
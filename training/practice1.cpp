#include <bits/stdc++.h>

#ifdef _WIN32
#include <windows.h>
#include <glut.h>
#endif

//#ifdef linux
#include "GL/freeglut.h"
#include "GL/gl.h"
//#endif

#define pi (2*acos(0.0))

using namespace std;

class P{
public:
    double x,y,z;
    P(){ x=0; y=0; }
    P(double x, double y){ this->x = x; this->y = y; }
};

void drawAxes(){
    glColor3f(1,0,0); // r
    glBegin(GL_LINES);
        glVertex3d(100,0,0);
        glVertex3d(-100,0,0);

    glEnd();

    glColor3f(0,1,0); // g
    glBegin(GL_LINES);

        glVertex3d(0,100,0);
        glVertex3d(0,-100,0);
    glEnd();

    glColor3f(0,0,1); // b
    glBegin(GL_LINES);
        
        glVertex3d(0,0,100);
        glVertex3d(0,0,-100);
    glEnd();
}


void drawGrid(){
    for(int i=-100; i<=100; i+=5){
        glBegin(GL_LINES);
        glVertex3d(-100,i,0);
        glVertex3d(100,i,0);
        glEnd();
    }

    for(int i=-100; i<=100; i+=5){
        glBegin(GL_LINES);
        glVertex3d(i,-100,0);
        glVertex3d(i,100,0);
        glEnd();
    }
}


void animate(){
	//angle+=0.05;
	//codes for any changes in Models, Camera
	glutPostRedisplay();
}

void drawCircle(double radius,int segments)
{
    int i;
    P points[100];
    glColor3f(0.7,0.7,0.7);
    //generate points
    for(i=0;i<=segments;i++)
    {
        points[i].x=radius*cos(((double)i/(double)segments)*2*pi);
        points[i].y=radius*sin(((double)i/(double)segments)*2*pi);
    }
    //draw segments using generated points
    for(i=0;i<segments;i++)
    {
        glBegin(GL_LINES);
        {
			glVertex3f(points[i].x,points[i].y,0);
			glVertex3f(points[i+1].x,points[i+1].y,0);
        }
        glEnd();
    }
}

void drawPolygon(double radius, double segment){
    P p[(int)segment];
    glColor3f(.7,.7,.7);
    for(int i=0; i<segment; i++){
        p[i].x = radius*cos( i*2*pi/segment);
        p[i].y = radius*sin( i*2*pi/segment);
    }

    for(int i=0; i<segment-1; i++){
        glBegin(GL_LINES);
        glVertex3d(p[i].x, p[i].y, 05);
        glVertex3d(p[i+1].y, p[i+1].y, 05);
        glEnd();
    }

    glBegin(GL_LINES);
        glVertex3d(p[(int)segment-1].x, p[(int)segment-1].y, 05);
        glVertex3d(p[0].x, p[0].y, 05);
    glEnd();
}


void drawSphere(double radius, double slices, double stacks){
    P p[100][100];
    int i,j;
    double h,r;
    for(i=0; i<=stacks; i++){
        h = radius*sin((double)i/(double)stacks*(pi/2));
        r = radius*cos((double)i/(double)stacks*(pi/2));

        for(j=0; j<=slices; j++){
            p[i][j].x = r*cos( (double)j/(double)slices*2*pi);
                p[i][j].y = r*sin( (double)j/(double)slices*2*pi);
            p[i][j].z = h;
        }
    }

    // draw quads using generated points
    for(i=0; i<stacks; i++){
        glColor3f((double)(i/stacks),(double)(i/stacks),(double)(i/stacks));
        for(j=0; j<slices; j++){
            glBegin(GL_QUADS);
            // upper hemisphere
            glVertex3f(p[i][j].x,p[i][j].y,p[i][j].z );
            glVertex3f(p[i][j+1].x,p[i][j+1].y,p[i][j+1].z );

            glVertex3f(p[i+1][j+1].x,p[i+1][j+1].y,p[i+1][j+1].z );
            glVertex3f(p[i+1][j].x,p[i+1][j].y,p[i+1][j].z );
            // lower hemisphere

            glVertex3f(p[i][j].x,p[i][j].y,-p[i][j].z );
            glVertex3f(p[i][j+1].x,p[i][j+1].y,-p[i][j+1].z );

            glVertex3f(p[i+1][j+1].x,p[i+1][j+1].y,-p[i+1][j+1].z );
            glVertex3f(p[i+1][j].x,p[i+1][j].y,-p[i+1][j].z );
            
            glEnd();
        }
    }

}


void start(){
  
	//clear the display
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0,0,0,0);	//color black
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/********************
	/ set-up camera here
	********************/
gluLookAt(100,200,100,0,0,0,	0,0,1);
	//load the correct matrix -- MODEL-VIEW matrix
	glMatrixMode(GL_MODELVIEW);

	//initialize the matrix
	glLoadIdentity();

	//now give three info
	//1. where is the camera (viewer)?
	//2. where is the camera looking?
	//3. Which direction is the camera's UP direction?

	//gluLookAt(100,100,100,	0,0,0,	0,0,1);
	//gluLookAt(200*cos(cameraAngle), 200*sin(cameraAngle), cameraHeight,		0,0,0,		0,0,1);
 //	gluLookAt(0,0,20,	0,0,0,	0,1,0);

	//again select MODEL-VIEW
	glMatrixMode(GL_MODELVIEW);


	/****************************
	/ Add your objects from here
	****************************/
	//add objects

	drawAxes();
	drawGrid();
    //drawPolygon(7,6);
    //glColor3f(1,0,0);
    //drawSquare(10);

    //drawSS();

    //drawCircle(30,24);

    //drawCone(20,50,24);

	drawSphere(30,24,20);




	//ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
	glutSwapBuffers();

}

void init(){
    //clear the screen
	glClearColor(0,0,0,0);

	/************************
	/ set-up projection here
	************************/
	//load the PROJECTION matrix
	glMatrixMode(GL_PROJECTION);

	//initialize the matrix
	glLoadIdentity();

	//give PERSPECTIVE parameters
	gluPerspective(80,	1,	1,	1000.0);
	//field of view in the Y (vertically)
	//aspect ratio that determines the field of view in the X direction (horizontally)
	//near distance
	//far distance
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE);

    glutInitWindowSize(500,500);
    glutInitWindowPosition(100,100);

    glutCreateWindow("START - 1");
	init();

	glEnable(GL_DEPTH_TEST);	//enable Depth Testing
    glutDisplayFunc(start);
    glutIdleFunc(animate);		//what you want to do in the idle time (when no drawing is occuring)

    glutMainLoop();
    return 0;
}

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
    P(){    x=0; y=0; z=0;  }
    P(double u=0,double v=0, double w=0){
        x = u; y = v; z = w;
    }
};

class V{
public:
    double x,y,z;
    V(){    x=0; y=0; z=0;  }
    V(double u=0,double v=0, double w=0){
        x = u; y = v; z = w;
    }
};

P pos(10,20,10);
V r(1,0,0),l(0,1,0),u(0,0,1);
double angleU, angleL, angleR;

void drawAxes(){
    glColor3f(1,0,0);

    glBegin(GL_LINES);
        // x axis red
        glVertex3f(100,0,0);
        glVertex3f(-100,0,0);
        // y axis green
        glColor3f(0,1,0);
        glVertex3f(0,100,0);
        glVertex3f(0,-100,0);
        // z axis blue
        glColor3f(0,0,1);
        glVertex3f(0,0,100);
        glVertex3f(0,0,-100);
        
    glEnd();
}

void init(){
    glClearColor(0,0,0,0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(80,1,1,1000);

}

void start(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glClearColor(0,0,0,0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(pos.x,pos.y,pos.z,  0,-100,0,  u.x,u.y,u.z);
    glMatrixMode(GL_MODELVIEW);
    
    drawAxes();
}

void animate(){
	//codes for any changes in Models, Camera
	glutPostRedisplay();
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE);

    glutInitWindowSize(500,500);
    glutInitWindowPosition(100,100);

    glutCreateWindow("START - 1");

    glutDisplayFunc(start);
    glutIdleFunc(animate);
    glutMainLoop();
    return 0;
}

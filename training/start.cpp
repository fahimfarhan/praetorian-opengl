#include <bits/stdc++.h>

#ifdef _WIN32
#include <windows.h>
#include <glut.h>
#endif


#ifdef linux
#include "GL/freeglut.h"
#include "GL/gl.h"
#endif

void start(){
    // black back ground start
    glClearColor(0.5,0.5,0.5,0);
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1,1,1);
    glOrtho(-1,1,-1,1,-1,1);

// black back ground end
    
    glBegin(GL_TRIANGLES);
    glColor3f(1,0,0);
    glVertex3f(-.7,.7,0);
    glColor3f(0,1,0);
    glVertex3f(.7,.7,0);
    glColor3f(0,0,1);
    glVertex3f(0,2,0);
    glEnd();
    
/*
    glBegin(GL_LINE);
    glColor3f(1,01,01);
        glVertex3f(-.8,0, 0);
        glVertex3f(.8,-0.8, 0);
    glEnd();
*/
    double x1 = 0; double y1 = 0; double sidelength = 1;
    double halfside = sidelength / 2;

    glColor3d(0,0,0);
    glBegin(GL_POLYGON);

    glVertex2d(x1 + halfside, y1 + halfside);
    glVertex2d(x1 + halfside, y1 - halfside);
    glVertex2d(x1 - halfside, y1 - halfside);
    glVertex2d(x1 - halfside, y1 + halfside);

    glEnd();

    glFlush();
}

void start2(){
    glFlush();
    glClearColor(0,0,0,1);
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1,1,1);
}
/*
class A{
public:
    A(){}
    void start(){
         glClearColor(0.4, 0.4, 0.4, 0.4);
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(1.0, 1.0, 1.0);
    glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);

        glBegin(GL_TRIANGLES);
                glVertex3f(-0.7, 0.7, 0);
                glVertex3f(0.7, 0.7, 0);
                glVertex3f(0, -1, 0);
        glEnd();

    glFlush();
    }
};
*/

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE);

    glutInitWindowSize(500,500);
    glutInitWindowPosition(100,100);

    glutCreateWindow("START - 1");

    glutDisplayFunc(start);
    glutMainLoop();
    return 0;
}

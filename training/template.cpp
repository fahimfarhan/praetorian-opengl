#include <bits/stdc++.h>

#ifdef _WIN32
#include <windows.h>
#include <glut.h>
#endif

#ifdef linux
#include "GL/freeglut.h"
#include "GL/gl.h"
#endif

#define pi (2*acos(0.0))

using namespace std;

void start(){
  
}

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

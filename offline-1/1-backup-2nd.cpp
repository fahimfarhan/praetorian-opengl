#include <bits/stdc++.h>

#ifdef _WIN32
#include <windows.h>
#include <glut.h>
#endif

// #ifdef linux
#include "GL/freeglut.h"
#include "GL/gl.h"
// #endif

#define pi (2*acos(0.0))

using namespace std;

double angle=0;

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

P pos(10,50,10);
V r(1,0,0),l(0,-1,0),u(0,0,1);

void start();
void init();
void animate();
void init();
void drawAxes();
void drawSquare(double a);
void specialKeyListener(int key, int x,int y);
void keyboardListener(unsigned char key, int x,int y);
V cross(V A, V B);
pair<V,V> rotate(V a, V b, V ref);

V cross(const V A,const V B){
    V C(0,0,0);

    C.x = A.y*B.z - A.z*B.y;
    C.y = -A.x*B.z + A.z*B.x;
    C.z = A.x*B.y -A.y*B.x;

    return C;
}

pair<V,V> rotate(V a, V b, V ref){
    V Aperp = cross(a,ref);

    V retA(0,0,0);
    retA.x = a.x*cos(angle) + Aperp.x*sin(angle);
    retA.y = a.y*cos(angle) + Aperp.y*sin(angle);
    retA.z = a.z*cos(angle) + Aperp.z*sin(angle);   

 V Bperp = cross(b,ref);

    V retB(0,0,0);
    retB.x = b.x*cos(angle) + Bperp.x*sin(angle);
    retB.y = b.y*cos(angle) + Bperp.y*sin(angle);
    retB.z = b.z*cos(angle) + Bperp.z*sin(angle);   

    pair<V,V> ret(retA, retB);
    return ret;
}

void drawSquare(double a)
{
    glColor3f(.70,0.70,1.0);
	glBegin(GL_QUADS);{
		glVertex3f( a,-50, a);
		glVertex3f( a,-50,-a);
		glVertex3f(-a,-50,-a);
		glVertex3f(-a,-50, a);
	}glEnd();
}

void keyboardListener(unsigned char key, int x,int y){
	switch(key){

		case '1':
            angle=-3;
        
			break;
        case '2':
            angle= +3;
			break;
        case '3':
            angle= +3;  
			break;
        case '4':
            angle= -3;
			break;
        case '5':
            angle= +3;
			break;
        case '6':
        angle= -3;
			break;
        
		default:
			break;
	}
}

void specialKeyListener(int key, int x,int y){
	switch(key){
		case GLUT_KEY_DOWN:		//down arrow key
			// cameraHeight -= 3.0;
            pos.y+=.5;
            //glTranslatef(pos.x,pos.y,pos.z);
			break;
		case GLUT_KEY_UP:		// up arrow key
			// cameraHeight += 3.0;
            // glTranslatef(pos.x,pos.y,pos.z);
            pos.y-=.5;
			break;

		case GLUT_KEY_RIGHT:
			pos.x -= .5;
			break;
		case GLUT_KEY_LEFT:
			pos.x += .5;
			break;

		case GLUT_KEY_PAGE_UP:
            pos.z+=.5;
			break;
		case GLUT_KEY_PAGE_DOWN:
			pos.z-=.5;
            break;

		case GLUT_KEY_INSERT:
			break;

		case GLUT_KEY_HOME:
			break;
		case GLUT_KEY_END:
			break;

		default:
			break;
	}
}


void start(){
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glClearColor(0,0,0,0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    //gluLookAt(pos.x,pos.y,pos.z,  0,-100,0,  u.x,u.y,u.z);
    gluLookAt(pos.x, pos.y, pos.z,
        pos.x + l.x, pos.y + l.y, pos.z + l.z,
        u.x, u.y, u.z);
    
    glMatrixMode(GL_MODELVIEW);
    
    // glClear(GL_COLOR_BUFFER_BIT);
    drawAxes();
    drawSquare(20);

    glutSwapBuffers();
}

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


void animate(){
    angle += 0.05;
	//codes for any changes in Models, Camera
	glutPostRedisplay();
}

void init(){
    glClearColor(0,0,0,0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(80,1,1,1000);

}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE);

    glutInitWindowSize(500,500);
    glutInitWindowPosition(0,0);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);


    glutCreateWindow("START - 1");

    init();
    glEnable(GL_DEPTH_TEST);
    glutDisplayFunc(start);
    glutIdleFunc(animate);
    
    // glutKeyboardFunc(keyboardListener);
    glutSpecialFunc(specialKeyListener);
    glutMainLoop();
    return 0;
}

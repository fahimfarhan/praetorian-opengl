// os independent headers
#include <bits/stdc++.h>

#ifdef _WIN32
// windows specific headers
#include <windows.h>
#include <glut.h>
#endif

// #ifdef linux
// linux specific headers
#include "GL/freeglut.h"
#include "GL/gl.h"
// #endif

#define pi (2*acos(0.0))

using namespace std;

// prptotype declaration
void start();
void drawAxes();
void init();
void specialKeyListener(int key, int x,int y);
void specialKeyListener(int key, int x,int y);
void drawSquare(double a);
void rotateAlongL();
void rotateAlongR();
void rotateAlongU();

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

void drawSquare(double a)
{
    glColor3f(.80,0.80,1.0);
	glBegin(GL_QUADS);{
		glVertex3f( a,-100,  a);
		glVertex3f( a,-100, -a);
		glVertex3f(-a,-100, -a);
		glVertex3f(-a,-100,  a);
	}glEnd();
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
    

    //glPushMatrix();
    
    rotateAlongL();
    rotateAlongR();
    rotateAlongU();
    glRotatef(angleL, l.x,l.y,l.z);
    glRotatef(angleR, r.x,r.y, r.z);
    glRotatef(angleU, u.x, u.y, u.z);
    
    //glPopMatrix();

    
/*
   glPushMatrix();
    
   glRotatef(angleL, 1,0,0);
   glRotatef(angleR, 0,1,0);
   glRotatef(angleU, 0,0,1);
    glPopMatrix();
*/
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

void keyboardListener(unsigned char key, int x,int y){
	switch(key){

		case '1':
        angleU-=.5;
        
			break;
        case '2':
        angleU+=.5;
			break;
        case '3':
        angleR+=.5;
			break;
        case '4':
        angleR-=.5;
			break;
        case '5':
        angleL+=.5;
			break;
        case '6':
        angleL-=.5;
			break;
        
		default:
			break;
	}
}

void init(){
    glClearColor(0,0,0,0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(80,1,1,1000);

}

void animate(){
	//codes for any changes in Models, Camera
	glutPostRedisplay();
}


void rotateAlongU(){
    double rx,ry, lx,ly;
    rx = r.x*cos(angleU) - r.y*sin(angleU);
    ry = r.y*cos(angleU) + r.x*sin(angleU);

    lx = l.x*cos(angleU) - l.y*sin(angleU);
    ly = l.y*cos(angleU) + l.x*sin(angleU);

    r.x = rx; r.y = ry;
    l.x = lx; l.y = ly;
}

void rotateAlongR(){
    // l-->x, u --> y
    double ux,uy, lx,ly;
    lx = l.x*cos(angleR) - l.y*sin(angleR);
    ly = l.x*sin(angleR) + l.y*cos(angleR);

    ux = u.x*cos(angleR) - u.y*sin(angleR);
    uy = u.x*sin(angleR) + u.y*cos(angleR);

    l.x = lx; l.y = ly;
    u.x = ux; u.y = uy;

}


void rotateAlongL(){
    // u-->x , r --> y  
    double ux,uy, rx,ry;
    ux = u.x*cos(angleL)-u.y*sin(angleL);
    uy = u.x*sin(angleL)+u.y*cos(angleL);

    rx = r.x*cos(angleL) - r.y*sin(angleL);
    ry = r.x*sin(angleL) + r.y*cos(angleL);

    u.x = ux; u.y = uy;
    r.x = rx; r.y = ry;

}


int main(int argc, char **argv)
{

    angleU = 0; 
    angleL = 0;
    angleR = 0;

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
    
    glutKeyboardFunc(keyboardListener);
    glutSpecialFunc(specialKeyListener);
    glutMainLoop();
    return 0;
}

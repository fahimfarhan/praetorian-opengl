#include <bits/stdc++.h>
#include <time.h>
#ifdef _WIN32

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<iostream>

#include <windows.h>
#include <glut.h>
#else
#include "GL/freeglut.h"
#include "GL/gl.h"
#endif

using namespace std;

#define pi (2*acos(0.0))

void initCurve();
void drawHermitCurve();
void pointUpdate();
void drawProjectile();
void drawSquare();

int myState = 0;

struct point2d
{
    double x, y;
};

struct point2d cp[200];
struct point2d dp[200][4][11];

struct point2d pu[20];
struct point2d projectile;
bool showProjectile = false;
double at=0;
int point_update=0;

int cpidx = 0;

void drawProjectile(){
	if(showProjectile){
		int segment = at/10;
		int index = (int)at%10;

		glColor3f(0, 0, 1);
        glPushMatrix();
        {
            glTranslatef(dp[segment][0][index].x, dp[segment][0][index].y, 0);
            drawSquare();
        }
        glPopMatrix();
		//sleep(500);
		int dummy = 0; 
		//for(int i=0; i<INT_MAX; i++){ dummy++;}

	}
}

void drawSquare()
{
    glBegin(GL_QUADS);
    {
        glVertex3d( 3,  3, 0);
        glVertex3d( 3, -3, 0);
        glVertex3d(-3, -3, 0);
        glVertex3d(-3,  3, 0);
    }
    glEnd();
}

void keyboardListener(unsigned char key, int x,int y){
	switch(key){

        case '1':
			break;
		case 'u':
			myState = 2;
			break;
		case 'a':
			showProjectile = !showProjectile;
			break;
 		case 'g':
			break;
		default:
			break;
	}
}

void specialKeyListener(int key, int x,int y){
	switch(key){
		case GLUT_KEY_DOWN:		//down arrow key
			break;
		case GLUT_KEY_UP:		// up arrow key
			break;

		case GLUT_KEY_RIGHT:
			break;
		case GLUT_KEY_LEFT:
			break;

		case GLUT_KEY_PAGE_UP:
			break;
		case GLUT_KEY_PAGE_DOWN:
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


void mouseListener(int button, int state, int x, int y){	//x, y is the x-y of the screen (2D)
	switch(button){
		case GLUT_LEFT_BUTTON:
			if(myState == 0){
					if(state == GLUT_DOWN){		// 2 times?? in ONE click? -- solution is checking DOWN or UP

					std::cout << x << " " << y << std::endl;

					cp[cpidx].x = (double)x;
					cp[cpidx].y = (double)(600 - y);
					cpidx = (cpidx+1)%200;
				}
			}else if(myState == 2){
					if(state == GLUT_DOWN){		// 2 times?? in ONE click? -- solution is checking DOWN or UP
						pu[point_update].x = (double)x;
						pu[point_update].y =  (double)(600 - y);
						point_update++; // = (point_update+1)%2;
						if(point_update == 2){
							point_update = 0;
							myState = 3;
						}
					}
			}
			
			break;

		case GLUT_RIGHT_BUTTON:
			//........
			if(myState == 0){
				if(cpidx == 0 || (cpidx%2 == 1) ){ }
				else{
					myState = 1;
					//initCurve();
					//drawHermitCurve();
				}
			}
			
			break;

		case GLUT_MIDDLE_BUTTON:
			//........
			break;

		default:
			break;
	}
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

void initCurve(){
	int num = cpidx/2;
	for(int k = 0; k<num; k++){
		int x1, y1, dx1,dy1,x4,y4,dx4,dy4;
		x1 = cp[2*k].x;
		y1 = cp[2*k].y;

		dx1 = cp[2*k+1].x - x1;
		dy1 = cp[2*k+1].y - y1; 

		x4 = cp[(2*k+2)%cpidx].x;
		y4 = cp[(2*k+2)%cpidx].y;

		dx4 = cp[(2*k+3)%cpidx].x - x4;
		dy4 = cp[(2*k+3)%cpidx].y - y4;

		// x 
		int ax, bx, cx, dx;
		ax = 2*x1-2*x4+dx1+dx4;
		bx = -3*x1 +3*x4-2*dx1-dx4;
		cx = dx1;
		dx = x1;

		// y 
		int ay,by,cy,dy;
		ay = 2*y1-2*y4+dy1+dy4;
		by = -3*y1 +3*y4-2*dy1-dy4;
		cy = dy1;
		dy = y1;
		double t = 0;

		/*FAILED!!!*/
/*
	   double delta = .1, delta2 = delta*delta, delta3 = delta2*delta;

		double dfx = ax*delta3 + bx*delta3 + cx*delta;
		double dfx2 = 6*ax*delta3 +2*bx*delta2;
		double dfx3 = 6*ax*delta3;

		dp[k][01][0].x = dfx;
		dp[k][02][0].x = dfx2;
		dp[k][03][0].x = dfx3; 

		double dfy = ay*delta3 + by*delta3 + cy*delta;
		double dfy2 = 6*ay*delta3 +2*by*delta2;
		double dfy3 = 6*ay*delta3;

		dp[k][01][0].y = dfy;
		dp[k][02][0].y = dfy2;
		dp[k][03][0].y = dfy3;
		*/

	   // initialize dp
	   for(int i=0; i<4; i++){  // 4 er jaygay 10 hobe
		    dp[k][0][i].x = ax*t*t*t + bx*t*t + cx*t + dx; 
			dp[k][0][i].y = ay*t*t*t + by*t*t + cy*t + dy; 
			t+=.1;
	   } 
		// if fails , comment out below
	   for(int i=1; i<4; i++){
		   for(int j=0; j<4-i; j++){
			dp[k][i][j].x = dp[k][i-1][j+1].x - dp[k][i-1][j].x;
			dp[k][i][j].y = dp[k][i-1][j+1].y - dp[k][i-1][j].y;
		   }
	   }

	   for(int i=1; i<10; i++){	dp[k][3][i] = dp[k][3][i-1]; }
	   for(int i=2; i>=0; i--){
		   for(int j=1; j<10; j++){
			    dp[k][i][j].x = dp[k][i+1][j-1].x + dp[k][i][j-1].x;
				dp[k][i][j].y = dp[k][i+1][j-1].y + dp[k][i][j-1].y;
		   }
	   }
	    

	}


}

void drawHermitCurve(){
	int num = cpidx/2;


	for(int k = 0; k<num; k++){
		glColor3f(1,1,1);
		glBegin(GL_LINES);
			glVertex3f(cp[2*k].x,cp[2*k].y,0);
			glVertex3f(cp[2*k+1].x,cp[2*k+1].y,0);
		
		glEnd();
		glColor3f(1,1,0);
		for(int i=0; i<9; i++){
			glBegin(GL_LINES);
			glVertex3f(dp[k][0][i].x,dp[k][0][i].y,0);
			glVertex3f(dp[k][0][i+1].x,dp[k][0][i+1].y,0);
			glEnd();
		}
		glBegin(GL_LINES);
			glVertex3f(dp[k][0][9].x,dp[k][0][9].y,0);
			glVertex3f(dp[(k+1)%num][0][0].x,dp[(k+1)%num][0][0].y,0);
			glEnd();
	    
	}
}

void pointUpdate(){
	int distance = INT_MAX;
	int index = 0;
	for(int i=0; i<cpidx; i++){
		int temp = abs(cp[i].x - pu[0].x)  + abs(cp[i].y - pu[0].y);
		if(temp<distance){ distance = temp; index = i; }

	}
	cp[index].x = pu[1].x;
	cp[index].y = pu[1].y;
}

void display(){

	//clear the display
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0,0,0,0);	//color black
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/********************
	/ set-up camera here
	********************/
	//load the correct matrix -- MODEL-VIEW matrix
	glMatrixMode(GL_MODELVIEW);

	//initialize the matrix
	glLoadIdentity();

	//now give three info
	//1. where is the camera (viewer)?
	//2. where is the camera looking?
	//3. Which direction is the camera's UP direction?

	//gluLookAt(100,100,100,	0,0,0,	0,0,1);
	//gluLookAt(150*cos(cameraAngle), 150*sin(cameraAngle), cameraHeight,		0,0,0,		0,0,1);
	gluLookAt(0,0,0,	0,0,-1,	0,1,0);


	//again select MODEL-VIEW
	glMatrixMode(GL_MODELVIEW);


	/****************************
	/ Add your objects from here
	****************************/
	//add objects
	if(myState == 1){
		initCurve();
		drawHermitCurve();

	}else if(myState == 3){
		pointUpdate();
		myState = 1;	
	}
	//drawAxes();

	if(showProjectile){
		drawProjectile();
	}

	int i;

    for (i = 0; i < cpidx; i++)
    {
        glColor3f(1, 1, 0);
        glPushMatrix();
        {
            glTranslatef(cp[i].x, cp[i].y, 0);
            drawSquare();
        }
        glPopMatrix();
    }


	//ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
	glutSwapBuffers();
}


void animate(){

	if(showProjectile){ 
		for(int i=0; i<INT_MAX/512; i++){ };
		at = (at + .01); 
		if(at >= (cpidx/2*10) ) at = 0;
	}
	//codes for any changes in Models, Camera
	glutPostRedisplay();
}

void init(){
	//codes for initialization

	cpidx = 0;

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
	gluOrtho2D(0, 800, 0, 600);
	//gluPerspective(80,	1,	1,	1000.0);
	//field of view in the Y (vertically)
	//aspect ratio that determines the field of view in the X direction (horizontally)
	//near distance
	//far distance
}

int main(int argc, char **argv){
	glutInit(&argc,argv);
	glutInitWindowSize(800, 600);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color

	glutCreateWindow("My OpenGL Program");

	init();

	glEnable(GL_DEPTH_TEST);	//enable Depth Testing

	glutDisplayFunc(display);	//display callback function
	glutIdleFunc(animate);		//what you want to do in the idle time (when no drawing is occuring)

	glutKeyboardFunc(keyboardListener);
	glutSpecialFunc(specialKeyListener);
	glutMouseFunc(mouseListener);

	glutMainLoop();		//The main loop of OpenGL

	return 0;
}

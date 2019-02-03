#include <bits/stdc++.h>

#ifdef _WIN32
#include <windows.h>
#include <GL/glut.h>
#include <gl/Gl.h>
#endif

//#ifdef linux
#include "GL/freeglut.h"
#include "GL/gl.h"
 //#endif

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

     V operator=(const V& b) {
         // V v();
         this->x = b.x;
         this->y =  b.y;
         this->z =  b.z;
         return (*this);
      }
};

P pos(10,20,10);
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
void drawGun();
void drawBarrel();
void drawBody();

double bodyX=0, bodyY=-10, bodyZ=0,bodyR = 10,
barrelX=0, barrelY=-5, barrelZ=0,barrelR = 5;
double bodyx0=0,bodyy0=0,bodyz0=0;

double bodyTheta=0.01, bodyPhi=00.01, angleBarrelXY=00.01;
// theta--> xy, phi --> yz
void drawBody(){
    // bodyZ;
    glColor3f(1,01,01);

    //glBegin(GL_LINES);
    
    //glVertex3d(bodyx0,bodyy0,bodyz0);
    //glVertex3d(bodyX,bodyY,bodyZ);
        

    // glEnd();
    //for(int j=0; j<36; j++)
    for(int i=0; i<36; i++){
        glBegin(GL_LINES);
        glVertex3d( (bodyx0 + cos(i*pi/18) ),bodyy0 + sin(i*pi/18),bodyz0  );
        glVertex3d( (bodyX+ cos(i*pi/18) ) ,bodyY+ sin(i*pi/18),bodyZ  );    
        glEnd();    
    }
}

void drawBarrel(){
    glColor3f(1,0,0);

    for(int i=0; i<36; i++){
        glBegin(GL_LINES);

        glVertex3d( (bodyX+ cos(i*pi/18) ) ,bodyY+ sin(i*pi/18),bodyZ  );    
        glVertex3d( (bodyX+barrelX + cos(i*pi/18) ),bodyX+barrelY + sin(i*pi/18),bodyZ+barrelZ  );
        
        glEnd();    
    }
}

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
        { angle=-.01;
            // u
            pair<V,V> p1 = rotate(l,r,u);
            l = p1.first;
            r = p1.second;
			}
            break;
        case '2':
            {angle= +.01;
            pair<V,V> p2 = rotate(l,r,u);
            l = p2.first;
            r = p2.second;
            }break;
        case '3': // r
            {angle= +.01;  
            pair<V,V> p3 = rotate(l,u,r);
            l = p3.first;
            u = p3.second;
            }break;
        case '4':
            {angle= -.01;
            pair<V,V> p4 = rotate(l,u,r);
            l = p4.first;
            u = p4.second;
            }break;
        case '5':  // l
            {angle= -.01;
            pair<V,V> p5 = rotate(r,u,l);
            r = p5.first;
            u = p5.second;
            }break;
        case '6':
        {angle= +.01;
			pair<V,V> p6 = rotate(r,u,l);
            r = p6.first;
            u = p6.second;
        }break;
        case 'q':
        {   bodyTheta += .01;
            bodyY = -bodyR*cos(bodyTheta)*sin(bodyPhi);
            if(bodyY> (-.707*bodyR)){ bodyY =  -(.707*bodyR);  }
            
            bodyX = bodyR*sin(bodyTheta);
            if(bodyX> (.707*bodyR)){ bodyX= (.707*bodyR);  }
            
        }break;  

        case 'w':
        {   bodyTheta -= .01;
            bodyY = -bodyR*cos(bodyTheta);
            if(bodyY> (-.707*bodyR)){ bodyY =  -(.707*bodyR);  }
            
            bodyX = bodyR*sin(bodyTheta);
            if(bodyX< -(.707*bodyR)){ bodyX= -(.707*bodyR);  }
            
        }break;  
        
        case 'e':
        {   bodyPhi += .01;
            
            bodyZ = bodyR*sin(bodyPhi);
            if(bodyZ> (0.707*bodyR)){ bodyZ= (0.707*bodyR);  }
            //bodyZ+=.01;
            //if(bodyZ> (0.707*bodyR)){ bodyZ= (0.707*bodyR);  }
            
        }break;
        case 'r':
        {   bodyPhi -= .01;
            
            bodyZ = bodyR*sin(bodyPhi);
            if(bodyZ< -(0.707*bodyR)){ bodyZ= -(0.707*bodyR);  }
            //bodyZ+=.01;
            //if(bodyZ> (0.707*bodyR)){ bodyZ= (0.707*bodyR);  }
            
        }break;     
		default:
			break;
	}
}

void specialKeyListener(int key, int x,int y){
	switch(key){
		case GLUT_KEY_DOWN:		//down arrow key
			// cameraHeight -= 3.0;
            pos.y += u.y;
            pos.x += u.x;
            pos.z += u.z;
            //glTranslatef(pos.x,pos.y,pos.z);
			break;
		case GLUT_KEY_UP:		// up arrow key
			// cameraHeight += 3.0;
            // glTranslatef(pos.x,pos.y,pos.z);
            //pos.y-=.5;
            pos.y -= u.y;
            pos.x -= u.x;
            pos.z -= u.z;
			break;

		case GLUT_KEY_RIGHT:
			pos.y += r.y;
            pos.x += r.x;
            pos.z += r.z;
			break;
		case GLUT_KEY_LEFT:
			pos.y -= r.y;
            pos.x -= r.x;
            pos.z -= r.z;
			break;

		case GLUT_KEY_PAGE_UP:
            pos.y += l.y;
            pos.x += l.x;
            pos.z += l.z;
			break;
		case GLUT_KEY_PAGE_DOWN:
			pos.y -= l.y;
            pos.x -= l.x;
            pos.z -= l.z;
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

/*

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


*/

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

    drawBody();
     drawBarrel();

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
    
    glutKeyboardFunc(keyboardListener);
    glutSpecialFunc(specialKeyListener);
    glutMainLoop();
    return 0;
}

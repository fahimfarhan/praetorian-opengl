#include <bits/stdc++.h>

#ifdef _WIN32
#include <windows.h>
#include <glut.h>
#else
//#ifdef linux
#include "GL/freeglut.h"
#include "GL/gl.h"
#endif
#include "bitmap_image.hpp"
using namespace std;

#define pi (2*acos(0.0))
#define epsilon (1.0e-6)
#define HP Point
#define MY_INF 10000

double cameraHeight;double cameraAngle;int drawgrid;int drawaxes;double angle;bool myDrawGridFlag;
double disNearPlane, disFarPlane, fovX,fovY, aspectRatio, levelOfRecur, numOfPixel;int widthOfCheckerBoard=50;double ambient, diffuse, reflection;int numOfObj, numOfLight, numOfSpotLight;

void init();void animate();void drawAxes();void display();void keyboardListener(unsigned char key, int x,int y);void specialKeyListener(int key, int x,int y);void mouseListener(int button, int state, int x, int y);
void drawSphere(double radius,int slices,int stacks, double cx, double cy, double cz);
void myDrawSpotlight();void myDrawLight();void myDrawSpheres();void myDrawPyramids();
void doAfterTakingInput();void createDynamicArray();void setPointBuffer(); 
void resetPointColorBuffer(); void imageGenerationCheckerBoard();
void imageGenerationPyramid();

class Vector{
public:
    double x, y, z;
    // constructs a vector with given components
    Vector(double x=0, double y=0, double z=0) {    this->x = x;this->y = y;this->z = z;    }
    // keeps the direction same. recalculates the vector to be unit.
    void normalize(){   double r = sqrt(x*x + y*y + z*z);   x = x / r;  y = y / r;  z = z / r;  }
    double getLength(){ double ret; ret = x*x + y*y + z*z;ret = sqrt(ret);return ret; }    
    // add two vectors
    Vector operator+(const Vector& v){    Vector v1(x+v.x, y+v.y, z+v.z);return v1;   }
    // subtract one vector from another
    Vector operator-(const Vector& v){    Vector v1(x-v.x, y-v.y, z-v.z); return v1;  }
    // scale a vector with a given coefficient
    Vector operator* (double m){    Vector v(x*m, y*m, z*m); return v;  }
    // get the dot product of two vectors
    double dot(Vector b){   return (this->x)*b.x + (this->y)*b.y + (this->z)*b.z;   }
    static double dot(Vector a, Vector b){   return a.x*b.x + a.y*b.y + a.z*b.z; }
    // get the cross product of two vectors
    Vector cross( Vector b){    Vector v( (this->y)*b.z - (this->z)*b.y, b.x*(this->z) - b.z*(this->x), (this->x)*b.y - (this->y)*b.x); return v;   }
    static Vector cross(Vector a, Vector b){   Vector v(a.y*b.z - a.z*b.y, b.x*a.z - b.z*a.x, a.x*b.y - a.y*b.x);return v; }
    Vector operator= (const Vector& point){  this->x = point.x; this->y = point.y; this->z = point.z;  return *this; }
    
    static Vector R(Vector x, Vector a, double angle){
        Vector c,d,e,ret;
        c = x*cos(angle);  d = a*dot(a,x);
        d = d*(1.0-cos(angle)); e = (cross(x,a));
        e = e*sin(angle);
        ret = c + d + e;
        return ret; 
    }

    // print a vector. only for testing purposes.
    void print (){  cout << "Vector" << endl; cout << x << " " << y << " " << z << endl;  }
};

class Point{
public:
    double x, y, z; double zbuffer;
    // constructs a homogeneous point with given coordinates. forces w to be 1.0
    // if w is zero, raises error
    HP(double x=0, double y=0, double z=0){    this->x = x;this->y = y;this->z = z;zbuffer = MY_INF;   }
	void setCoordinate(double x=0, double y=0, double z=0){ this->x = x; this->y=y; this->z=z;   }
    // adds two points. returns a point forcing w to be 1.0
    HP operator+ (const HP& point){ double x = this->x + point.x;double y = this->y + point.y;double z = this->z + point.z;HP p(x, y, z);return p;}
	HP operator+ (const Vector& vec){ double x = this->x + vec.x;double y = this->y + vec.y;    double z = this->z + vec.z;HP p(x, y, z);return p;}
    // subtracts one point from another. returns a point forcing w to be 1.0
    HP operator- (const HP& point){ double x = this->x - point.x; double y = this->y - point.y;    double z = this->z - point.z; HP p(x, y, z);}
	HP operator- (const Vector& vec){ double x = this->x - vec.x;   double y = this->y - vec.y;    double z = this->z - vec.z;HP p(x, y, z); return p;}
    HP operator= (const HP& point){ this->x = point.x;this->y = point.y;    this->z = point.z;this->zbuffer = point.zbuffer;return *this;}
    Vector minus(const HP& point){ Vector ret; ret.x = this->x - point.x;ret.y = this->y - point.y;    ret.x = this->z - point.z;ret.normalize();return ret;}
    // Print the coordinates of a point. exists for testing purpose.
    void print(){   cout<<"(" << x << ", " << y << ", " << z << "), ";    }
};

class Matrix{
public:
    double a1,b1,c1,a2,b2,c2,a3,b3,c3;
    Matrix(){}
    void setter(double x1, double y1, double z1,double x2, double y2, double z2,double x3, double y3, double z3 ){
        a1=x1; b1=y1; c1=z1;
        a2=x2; b2=y2; c2=z2;
        a3=x3; b3=y3; c3=z3;
    }
    double getDeterminant(){    return ((a1*b2*c3+a2*b3*c1+a3*b1*c2) - (a3*b2*c1+a1*b3*c2+a2*b1*c3));   }
};

Point **pointBuffer;
HP cameraPos; Vector l,r,u;
class light{
public:
    double x,y,z,fallOfParameter;
    light(){}
    void printer(){
        cout<<"light\n";
        cout<<x<<" "<<y<<" "<<z<<" "<<fallOfParameter<<"\n";
    }
};

bool isRayInsideSphere(HP p ,Vector v,int spNo); bool isRayInsideObj(HP p , Vector v, int spNo, int pyNo);
bool isRayInsidePyramid(HP p , Vector v,int pyNo);


class spotlight{
public:
    double x,y,z,fallOfParameter, lookx, looky, lookz, angleInDeg;
    spotlight(){}   void printer(){  cout<<"Spotlight\n";   cout<<x<<" "<<y<<" "<<z<<" "<<fallOfParameter<<" "<<"\n";cout<<lookx<<" "<<looky<<" "<<lookz<<" "<<"\n"; cout<<angleInDeg<<"\n";}
};

class MyTriangle{
public:
    HP a,b,c;
    MyTriangle(HP a, HP b, HP c){ this->a = a; this->b=b; this->c=c;    }
};

class mySphere{
public:
    double cx,cy,cz,r, cr,cg,cb, ac, dc, sc, rc, shine;
//    ambient diffuse specular reflection coefficient
    mySphere(){}    void printer(){  cout<<"MySphere\n"; cout<<cx<<" "<<cy<<" "<<cz<<" "<<endl; cout<<r<<" "<<endl;cout<<cr<<" "<<cg<<" "<<cb<<" "<<endl; cout<<ac<<" "<<dc<<" "<<sc<<" "<<rc<<" "<<endl; cout<<shine<<endl;}
};


class myPyramid{
public:
    double px, py, pz, w,h,cr,cg,cb, ac, dc, sc, rc, shine;
    myPyramid(){};  void printer(){cout<<px<<" "<<py<<" "<<pz<<" "<<endl; cout<<w<<" "<<h<<" "<<endl;cout<<cr<<" "<<cg<<" "<<cb<<" "<<endl; cout<<ac<<" "<<dc<<" "<<sc<<" "<<rc<<" "<<endl; cout<<shine<<" "<<endl;}
};

class mySquare{
public:
    double x,y,l;bool toogle;double r,b,g;
    mySquare(double a=0, double b=0, double c=0, bool toogle=false){
        this->x = a;this->y = b;this->l = c;
        this->toogle=toogle;
        if((this->toogle)==false){    this->r=0; this->b=0; this->g=0;}
        else{    r=1; b=1; g=1; }
    }

    void setter(double a=0, double b=0, double c=0, bool toogle=false){
        this->x = a;this->y = b;this->l = c;
        this->toogle=toogle;
        if(!(this->toogle)){    this->r=0; this->b=0; this->g=0;}
        else{    this->r=1; this->b=1; this->g=1;}
    }
};

class Color {
public:
    double r, g, b;
    Color(double r=0, double g=0, double b=0) { this->r = r; this->g = g; this->b = b;    }
    void setColor(double r=0, double g=0, double b=0){  this->r = r;this->g = g;this->b = b;    }    
};

Color **myColorBuffer;

vector<mySphere> ms; vector<myPyramid> mp; vector<light> vl; vector<spotlight> vsl; mySquare **msq;

void myDrawPyramids(){
    for(int i=0; i<mp.size(); i++){
        double w = mp[i].w/2, h=mp[i].h;  HP A,B,C,D,E, F;
        F.setCoordinate(mp[i].px,mp[i].py,mp[i].pz ); E.setCoordinate(mp[i].px,mp[i].py,mp[i].pz+h );
        double r=mp[i].cr, g = mp[i].cg, b= mp[i].cb;
        A.setCoordinate(mp[i].px-w , mp[i].py-w , mp[i].pz); B.setCoordinate(mp[i].px+w , mp[i].py-w , mp[i].pz);
        C.setCoordinate(mp[i].px+w , mp[i].py+w , mp[i].pz); D.setCoordinate(mp[i].px-w , mp[i].py+w , mp[i].pz);
        // set color 
        glColor3f(r,g,b);
        // draw the base square
        glBegin(GL_QUADS);
        glVertex3f(A.x, A.y, A.z);glVertex3f(B.x, B.y, B.z);
        glVertex3f(C.x, C.y, C.z);glVertex3f(D.x, D.y, D.z);
        glEnd();
        // draw 4 triangles
        // ABE 
        glBegin(GL_TRIANGLES);
        glVertex3f(A.x, A.y, A.z);glVertex3f(B.x, B.y, B.z);glVertex3f(E.x, E.y, E.z);
        glEnd();
        // BCE 
        glBegin(GL_TRIANGLES);
        glVertex3f(E.x, E.y, E.z);  glVertex3f(B.x, B.y, B.z);glVertex3f(C.x, C.y, C.z);
        glEnd();
        // CDE 
        glBegin(GL_TRIANGLES);
        glVertex3f(C.x, C.y, C.z);   glVertex3f(D.x, D.y, D.z);glVertex3f(E.x, E.y, E.z);
        glEnd();
        // DAE 
        glBegin(GL_TRIANGLES);
        glVertex3f(D.x, D.y, D.z);glVertex3f(A.x, A.y, A.z);glVertex3f(E.x, E.y, E.z);
        glEnd();
    }
}

void myDrawSpheres(){
    for(int i=0; i<ms.size(); i++){
        // set color!
        glColor3f(ms[i].cr,ms[i].cg, ms[i].cb );
        //draw sphere!
        drawSphere(ms[i].r, 25,25,ms[i].cx, ms[i].cy, ms[i].cz);
    }
}

void myDrawLight(){
    glColor3f(1,1,1);
    for(int i=0; i<vl.size(); i++){drawSphere(10, 25,25,vl[i].x, vl[i].y, vl[i].z);}
}

void myDrawSpotlight(){
        glColor3f(1,1,1);
        for(int i=0; i<vsl.size(); i++){    drawSphere(5, 25,25,vsl[i].x, vsl[i].y, vsl[i].z);  }
}

void drawSphere(double radius=0,int slices=0,int stacks=0, double cx=0, double cy=0, double cz=0){
	HP points[100][100];
	int i,j;    double h,r;
	//generate points
	for(i=0;i<=stacks;i++)
	{
		h=radius*sin(((double)i/(double)stacks)*(pi/2));
		r=radius*cos(((double)i/(double)stacks)*(pi/2));
		for(j=0;j<=slices;j++)
		{
			points[i][j].x=r*cos(((double)j/(double)slices)*2*pi) + cx;
			points[i][j].y=r*sin(((double)j/(double)slices)*2*pi) + cy;
			points[i][j].z=h ;
		}
	}
	//draw quads using generated points
	for(i=0;i<stacks;i++)
	{
        // glColor3f((double)i/(double)stacks,(double)i/(double)stacks,(double)i/(double)stacks);
		for(j=0;j<slices;j++)
		{
			glBegin(GL_QUADS);{
			    //upper hemisphere
				glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z + cz );
				glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z + cz );
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z + cz );
				glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z + cz );
                //lower hemisphere
                glVertex3f(points[i][j].x,points[i][j].y,-points[i][j].z + cz );
				glVertex3f(points[i][j+1].x,points[i][j+1].y,-points[i][j+1].z + cz );
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,-points[i+1][j+1].z + cz );
				glVertex3f(points[i+1][j].x,points[i+1][j].y,-points[i+1][j].z + cz );
			}glEnd();
		}
	}
}

void drawSquare(double a, double b, double c=50, bool toogle=true)
{
    if(toogle) glColor3f(1,1,1);
    else glColor3f(0,0,0);
	glBegin(GL_QUADS);{
		glVertex3f( a, b,0);
		glVertex3f( a+c,b,0);
		glVertex3f(a+c,b+c,0);
		glVertex3f(a, b+c,0);
	}glEnd();
}

void myDrawGrid(){
    bool toogle = true;
    int stop = 2000/widthOfCheckerBoard;
    // stop--;
    int  x = -1000, y = -1000;
    int a=0, b=0;
    for(int i=0; i<stop; i++){
        for(int j=0; j<stop; j++){
            a = x+i*widthOfCheckerBoard;
            b = y+j*widthOfCheckerBoard;
            drawSquare(a,b,widthOfCheckerBoard, toogle);
            msq[i][j].setter(a,b,widthOfCheckerBoard, toogle);
            toogle = !toogle;
        }
        if(stop%2==0){  toogle=!toogle; }
    }
}
void display(){
  	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glClearColor(0,0,0,0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(cameraPos.x, cameraPos.y, cameraPos.z,
        cameraPos.x + l.x, cameraPos.y + l.y, cameraPos.z + l.z,
        u.x, u.y, u.z);
    glMatrixMode(GL_MODELVIEW);
    // glClear(GL_COLOR_BUFFER_BIT);
    drawAxes();
     // uncomment this later!!!
    //drawGrid();
    myDrawGrid(); 
    myDrawSpheres(); myDrawPyramids();
    myDrawLight(); myDrawSpotlight();
   
    glutSwapBuffers();
}

void takeInput(){
    cin>>disNearPlane>>disFarPlane>>fovY>>aspectRatio;
    fovX = fovY*aspectRatio;
    cin>>levelOfRecur;
    cin>>numOfPixel;
    cin>>widthOfCheckerBoard;
    cin>>ambient>>diffuse>>reflection;
    cin>>numOfObj;
    int tempN1234=numOfObj;
    string shapeType;
    while(tempN1234--){
        cin>>shapeType;
        if(shapeType=="sphere"){
            mySphere ob;
            cin>>ob.cx>>ob.cy>>ob.cz;
            cin>>ob.r;
            cin>>ob.cr>>ob.cg>>ob.cb;
            cin>>ob.ac>>ob.dc>>ob.sc>>ob.rc;
            cin>>ob.shine;
            ms.push_back(ob);

        }else if(shapeType=="pyramid"){
            myPyramid ob;
            cin>>ob.px>>ob.py>>ob.pz;
            cin>>ob.w>>ob.h;
            cin>>ob.cr>>ob.cg>>ob.cb;
            cin>>ob.ac>>ob.dc>>ob.sc>>ob.rc;
            cin>>ob.shine;
            mp.push_back(ob);
        }
    }

    cin>>numOfLight;
    int temp123 = numOfLight;
    while(temp123--){
        light ob;
        cin>>ob.x>>ob.y>>ob.z>>ob.fallOfParameter;
        vl.push_back(ob);
    }
    cin>>numOfSpotLight;
    temp123 = numOfSpotLight;

    while(temp123--){
        spotlight ob;
        cin>>ob.x>>ob.y>>ob.z>>ob.fallOfParameter;
        cin>>ob.lookx>>ob.looky>>ob.lookz>>ob.angleInDeg;
        //cin>>ob.angleInDeg;
        vsl.push_back(ob);
    }
}

void doAfterTakingInput(){
    cout<<"do after taking input start\n";cout<<"create array of square objs for checker board\n";
    cout<<"do after taking input end\n";
}

void createDynamicArray(){
     msq = new mySquare*[2001];
    for(int i=0; i<=2000; i++){    msq[i] = new mySquare[2001];}
    pointBuffer = new HP*[(int)numOfPixel+1];  myColorBuffer = new Color*[(int)numOfPixel+1];
    for(int i=0; i<=numOfPixel; i++){
        pointBuffer[i] = new HP[(int)numOfPixel+1]; myColorBuffer[i] = new Color[(int)numOfPixel+1];
    }
}

void setPointBuffer(){
    double screenHeight , screenWidth;
    HP a,b,c,d,e, f;
    // a = screenMidPoint;
    screenHeight = 2*disNearPlane*tan(fovY*pi/360);
    screenWidth  = 2*disNearPlane*tan(fovX*pi/360);
    //cout<<"Height "<<screenHeight<<"\n";
    //cout<<"Width "<<screenWidth<<"\n";

    a = cameraPos + l*disNearPlane;
    b = a + u*(screenHeight/2) + r*(screenWidth/2);
    c = a + u*(screenHeight/2) - r*(screenWidth/2);
    d = a - u*(screenHeight/2) - r*(screenWidth/2);
    e = a - u*(screenHeight/2) + r*(screenWidth/2);

    f = c + (r*0.5)*(screenWidth/numOfPixel) - (u*0.5)*(screenHeight/numOfPixel);
    //cout<<"l, r, u\n";
    //l.print(); r.print(); u.print();
    //cout<<"A,B,C,D,E, F\n";
    //a.print(); b.print(); c.print(); d.print(); e.print(); f.print();
    //cout<<"-------------------\n";
    
    for(int i=0; i<numOfPixel; i++){
        for(int j=0; j<numOfPixel; j++){
            pointBuffer[i][j] = ((f + r*(i*(screenWidth/numOfPixel))) - u*(j*(screenHeight/numOfPixel))); 
        } 
    }
}

void imageGenerationCheckerBoard(){
   double t = 0;
    int count = 0;
    for(int i=0; i<numOfPixel; i++){
        for(int j=0; j<numOfPixel; j++){
            Vector R0(pointBuffer[i][j].x, pointBuffer[i][j].y, pointBuffer[i][j].z);
            Vector Rd(pointBuffer[i][j].x - cameraPos.x,pointBuffer[i][j].y - cameraPos.y,pointBuffer[i][j].z - cameraPos.z );
            Rd.normalize();
            t = -R0.z/Rd.z;
            if( t >= 0 ){
                Vector I = R0 + Rd*t;
                int i1 = ((int)I.x + 1000)/widthOfCheckerBoard;
                int j1 = ((int)I.y + 1000)/widthOfCheckerBoard;
                if(i1>=0 && i1<2000 && j1>=0 && j1<2000){
                    double r123 = msq[i1][j1].r, g123 = msq[i1][j1].g, b123 = msq[i1][j1].b;
                    double disTemp =  t*Rd.getLength();
                    if(disTemp < pointBuffer[i][j].zbuffer ){
                        Color diff,amb;
                        
                        pointBuffer[i][j].zbuffer = disTemp;
                        amb.r = r123*ambient; amb.g = g123*ambient; amb.b = b123*ambient;
                        // ambient done !!! 
                        // phong , lambert to go! 
                        double phong = 0 , lambert = 0;

                        for(int k=0; k<vl.size(); k++){
                            HP P(I.x, I.y, I.z); // s = vl[i];
                            // ps vector 
                            Vector PS(vl[k].x - P.x, vl[k].y - P.y, vl[k].z - P.z); 
                            if(isRayInsideObj(P, PS,-1,-1)){  continue;   }
                            else{
                                Vector toSource = PS;
                                toSource.normalize();
                                Vector N(0,0,1); // Since, Normal at P == Normal on XY plane == z axis
                                N.normalize();
                                double distance1 = PS.getLength();
                                double scaling_factor = distance1*distance1*vl[k].fallOfParameter;
                                scaling_factor = exp(-scaling_factor);

                                lambert+= (toSource.dot(N))*scaling_factor;
                                /*
                                Vector R_; // reflection R = 2 (L.N)N – L 
                                R_ = (N*toSource.dot(N))*2 - toSource;
                                R_.normalize();
                                phong+=pow(R_.dot(toSource), shinniness )*scaling_factor;
                                */
                            }
                        }

                        for(int k=0; k<vsl.size(); k++){
                            HP P(I.x, I.y, I.z); // s = vl[i];
                            // ps vector 
                            Vector PS(vsl[k].x - P.x, vsl[k].y - P.y, vsl[k].z - P.z); 
                            double theta=0;
                            Vector spotLightVec(vsl[k].lookx - vsl[k].x ,vsl[k].looky - vsl[k].y, vsl[k].lookz - vsl[k].z );
                            Vector SP(-PS.x,-PS.y,-PS.z );
                            theta = SP.dot(spotLightVec)/(SP.getLength() * spotLightVec.getLength());
                            theta = acos(theta); // radian 
                            theta = theta*180/pi;
                            if(isRayInsideObj(P, PS,-1,-1) || ( theta > vsl[k].angleInDeg )){  continue;   }
                            else{
                                Vector toSource = PS;
                                toSource.normalize();
                                Vector N(0,0,1); // Since, Normal at P == Normal on XY plane == z axis
                                N.normalize();
                                double distance1 = PS.getLength();
                                double scaling_factor = distance1*distance1*vsl[k].fallOfParameter;
                                scaling_factor = exp(-scaling_factor);
                                lambert+= (toSource.dot(N))*scaling_factor;
                            }
                        }
                        diff.r = diffuse*lambert*r123; diff.g = diffuse*lambert*g123; diff.b = diffuse*lambert*b123;
                        myColorBuffer[i][j].setColor(amb.r+diff.r, amb.g+diff.g, amb.b+diff.b);
                        //myColorBuffer[i][j].setColor(r123*ambient, g123*ambient, b123*ambient);
                    
                    }
                }
            }
        }
    } 
}

void imageGenerationSphere(){
    double t = 0;
    for(int i=0; i<numOfPixel; i++){
        for(int j=0; j<numOfPixel; j++){
            Vector R0(pointBuffer[i][j].x, pointBuffer[i][j].y, pointBuffer[i][j].z);
            Vector Rd(pointBuffer[i][j].x - cameraPos.x,pointBuffer[i][j].y - cameraPos.y,pointBuffer[i][j].z - cameraPos.z );
            Rd.normalize();
             double a1=0,b1=0,c1=0,
                t1=0,t2=0, D=0;
            
            for(int k=0; k<ms.size(); k++){
                HP currCenter(ms[k].cx, ms[k].cy, ms[k].cz);
                R0.x = R0.x - currCenter.x;R0.y = R0.y - currCenter.y; R0.z = R0.z - currCenter.z;
               
                a1 = 1; b1 = Rd.dot(R0);    b1 = b1*2;
                c1 = R0.dot(R0) - ( (ms[k].r)*(ms[k].r));
                D = b1*b1 - 4*c1;
                if(D>=0){
                    D = sqrt(D);
                    t1 = (-b1+D)/(2*a1);    t2 = (-b1-D)/(2*a1);
                
                    double dis1, dis2, dis_minimus, t_minimus;
                    dis1 = Rd.getLength();  dis2 = dis1;
                    dis1 = abs(dis1*t1);    dis2 = abs(dis1*t2);

                    if(dis1 < dis2 ){   dis_minimus = dis1; t_minimus = t1;     }
                    else{   dis_minimus = dis2; t_minimus = t2;     }
                    //now find appropriate i1, j1 like before!
                    //dis_minimus = min(dis1, dis2);
                    if(dis_minimus < pointBuffer[i][j].zbuffer){
                        // R0.x = R0.x + currCenter.x;R0.y = R0.y + currCenter.y; R0.z = R0.z + currCenter.z;
                        Vector I = R0 + Rd*t_minimus;
                        I.x+=currCenter.x; I.y+=currCenter.y; I.z+=currCenter.z; 
                        int i1 = ((int)I.x + 1000)/widthOfCheckerBoard;
                        int j1 = ((int)I.y + 1000)/widthOfCheckerBoard;
                        if(i1>=0 && i1<2000 && j1>=0 && j1<2000){
                            pointBuffer[i][j].zbuffer = dis_minimus;
                            Color amb_sp , diff_sp , spec_sp;
                            amb_sp.setColor(ms[k].cr*ms[k].ac,ms[k].cg*ms[k].ac,ms[k].cb*ms[k].ac);
                            
                            double lambert=0, phong=0;
                            for(int x=0; x<vl.size(); x++){
                                HP P(I.x, I.y, I.z); // s = vl[i];
                                // ps vector 
                                Vector PS(vl[x].x - P.x, vl[x].y - P.y, vl[x].z - P.z); 
                                if( !isRayInsideObj(P,PS, k, -1) ){
                                    Vector toSource = PS;
                                    toSource.normalize();
                                    Vector N(I.x-currCenter.x , I.y-currCenter.y, I.z-currCenter.z); // Since, Normal at P == Normal on XY plane == z axis
                                    N.normalize();
                                    double distance1 = PS.getLength();
                                    double scaling_factor = distance1*distance1*vl[x].fallOfParameter;
                                    scaling_factor = exp(-scaling_factor);

                                    lambert+= (toSource.dot(N))*scaling_factor; 
                                    Vector R_; // reflection R = 2 (L.N)N – L 
                                    R_ = (N*toSource.dot(N))*2 - toSource;
                                    R_.normalize();
                                    phong+=pow(R_.dot(toSource), ms[k].shine )*scaling_factor;    
                                }
                            }

                            for(int x=0; x<vsl.size(); x++){
                                HP P(I.x, I.y, I.z); // s = vl[i];
                                // ps vector 
                                Vector PS(vsl[x].x - P.x, vsl[x].y - P.y, vsl[x].z - P.z); 
                                double theta=0;
                                Vector spotLightVec(vsl[x].lookx - vsl[x].x ,vsl[x].looky - vsl[x].y, vsl[x].lookz - vsl[x].z );
                                Vector SP(-PS.x,-PS.y,-PS.z );
                                theta = SP.dot(spotLightVec)/(SP.getLength() * spotLightVec.getLength());
                                theta = acos(theta); // radian 
                                theta = theta*180/pi;    
                                if( ! (isRayInsideObj(P,PS, k, -1) || ( theta > vsl[x].angleInDeg ) ) ){
                                    Vector toSource = PS;
                                    toSource.normalize();
                                    Vector N(I.x-currCenter.x , I.y-currCenter.y, I.z-currCenter.z); // Since, Normal at P == Normal on XY plane == z axis
                                    N.normalize();
                                    double distance1 = PS.getLength();
                                    double scaling_factor = distance1*distance1*vsl[x].fallOfParameter;
                                    scaling_factor = exp(-scaling_factor);

                                    lambert+= (toSource.dot(N))*scaling_factor; 
                                    Vector R_; // reflection R = 2 (L.N)N – L 
                                    R_ = (N*toSource.dot(N))*2 - toSource;
                                    R_.normalize();
                                    phong+=pow(R_.dot(toSource), ms[k].shine )*scaling_factor;    
                                }
                            }
                            diff_sp.r = ms[k].dc*lambert*ms[k].cr;
                            diff_sp.g = ms[k].dc*lambert*ms[k].cg;
                            diff_sp.b = ms[k].dc*lambert*ms[k].cb;

                            spec_sp.r = ms[k].sc*phong*ms[k].cr;
                            spec_sp.g = ms[k].sc*phong*ms[k].cg;
                            spec_sp.b = ms[k].sc*phong*ms[k].cb;
                            myColorBuffer[i][j].setColor(amb_sp.r+diff_sp.r+spec_sp.r ,
                                                        amb_sp.g+diff_sp.g+spec_sp.g ,
                                                        amb_sp.b+diff_sp.b+spec_sp.b );
                            //myColorBuffer[i][j].setColor(ms[k].cr,ms[k].cg,ms[k].cb );
                        }       
                    }
                }
                R0.x = R0.x + currCenter.x;R0.y = R0.y + currCenter.y; R0.z = R0.z + currCenter.z;
            }
        }
    }
}

void imageGenerationPyramid(){
    double t = 0, alpha=0, beta=0, gamma=0;
    for(int i=0; i<numOfPixel; i++){
        for(int j=0; j<numOfPixel; j++){
            Vector R0(pointBuffer[i][j].x, pointBuffer[i][j].y, pointBuffer[i][j].z);
            Vector Rd(pointBuffer[i][j].x - cameraPos.x,pointBuffer[i][j].y - cameraPos.y,pointBuffer[i][j].z - cameraPos.z );
            Rd.normalize();
            for(int k=0; k< mp.size(); k++){
                HP a,b,c,d,e, g;
                double w = mp[k].w/2, h=mp[k].h;
                g.setCoordinate(mp[k].px,mp[k].py,mp[k].pz+h/2 );
                e.setCoordinate(mp[k].px,mp[k].py,mp[k].pz+h );
                a.setCoordinate(mp[k].px-w , mp[k].py-w , mp[k].pz); b.setCoordinate(mp[k].px+w , mp[k].py-w , mp[k].pz);
                c.setCoordinate(mp[k].px+w , mp[k].py+w , mp[k].pz); d.setCoordinate(mp[k].px-w , mp[k].py+w , mp[k].pz);
                vector<MyTriangle> mt1;
                MyTriangle x1(a,b,d),x2(b,c,d),x3(a,b,e),x4(b,c,e),x5(a,d,e),x6(c,d,e);
                mt1.push_back(x1); mt1.push_back(x2); mt1.push_back(x3);
                mt1.push_back(x4); mt1.push_back(x5); mt1.push_back(x6);
                for(int l=0; l<6; l++){
                    t = 0, alpha=0, beta=0, gamma=0;
                    Matrix A, Ab, Ag,At;
                    // set A 
                    At.setter(mt1[l].a.x - mt1[l].b.x, mt1[l].a.x - mt1[l].c.x, mt1[l].a.x - R0.x,
                             mt1[l].a.y - mt1[l].b.y, mt1[l].a.y - mt1[l].c.y, mt1[l].a.y - R0.y,
                             mt1[l].a.z - mt1[l].b.z, mt1[l].a.z - mt1[l].c.z, mt1[l].a.z - R0.z );

                    Ab.setter(mt1[l].a.x - R0.x, mt1[l].a.x - mt1[l].c.x, Rd.x,
                              mt1[l].a.y - R0.y, mt1[l].a.y - mt1[l].c.y, Rd.y,
                              mt1[l].a.z - R0.z, mt1[l].a.z - mt1[l].c.z, Rd.z );
                    Ag.setter(mt1[l].a.x - mt1[l].b.x , mt1[l].a.x - R0.x, Rd.x, 
                                mt1[l].a.y - mt1[l].b.y , mt1[l].a.y - R0.y, Rd.y, 
                                mt1[l].a.z - mt1[l].b.z , mt1[l].a.z - R0.z, Rd.z );
                    A.setter(mt1[l].a.x - mt1[l].b.x, mt1[l].a.x - mt1[l].c.x, Rd.x,
                             mt1[l].a.y - mt1[l].b.y, mt1[l].a.y - mt1[l].c.y, Rd.y,
                             mt1[l].a.z - mt1[l].b.z, mt1[l].a.z - mt1[l].c.z, Rd.z);
                    
                    try{ beta = Ab.getDeterminant()/A.getDeterminant(); }
                    catch(exception& x){ beta = MY_INF; }
                    try{ gamma = Ag.getDeterminant()/A.getDeterminant(); }
                    catch(exception& x){ gamma = MY_INF; }
                    try{ t = At.getDeterminant()/A.getDeterminant(); }
                    catch(exception& x){ t = MY_INF; }
                    // Intersection if beta + gamma < 1 , beta >0 , gamma > 0
                    if( ( (beta+gamma) < 1) && (beta > 0) && (gamma > 0) ){
                        if( abs(Rd.getLength()*t) < pointBuffer[i][j].zbuffer ){
                            pointBuffer[i][j].zbuffer= abs(Rd.getLength()*t) ;
                            Vector I = R0 + Rd*t;
                            Color amb_sp , diff_sp , spec_sp;
                            amb_sp.setColor(mp[k].cr*mp[k].ac,mp[k].cg*mp[k].ac,mp[k].cb*mp[k].ac);
                            double lambert=0, phong=0;
                            
                            for(int x=0; x<vl.size(); x++){
                                HP P(I.x, I.y, I.z); // s = vl[i];
                                Vector PS(vl[x].x - P.x, vl[x].y - P.y, vl[x].z - P.z); 
                                if( !isRayInsideObj(P,PS, -1, k) ){
                                    Vector toSource = PS;
                                    toSource.normalize();
                                    // (p,b,c) => point normal form = > n = (p-b)x(p-c)
                                    Vector A123(P.x-mt1[l].b.x, P.y-mt1[l].b.y,P.z-mt1[l].b.z);
                                    Vector B123(P.x-mt1[l].c.x, P.y-mt1[l].c.y,P.z-mt1[l].c.z);
                                    Vector N = A123.cross(B123);
                                    N.normalize();
                                    // N or -N verify ?
                                    Vector GP(P.x-g.x, P.y-g.y, P.z-g.z);
                                    if( (GP.dot(N))/(GP.getLength()*N.getLength() )<0  ){
                                        N.x = -N.x; N.y=-N.y; N.z = -N.z; // N = -N;
                                    }
                                    double distance1 = PS.getLength();
                                    double scaling_factor = distance1*distance1*vl[x].fallOfParameter;
                                    scaling_factor = exp(-scaling_factor);

                                    lambert+= (toSource.dot(N))*scaling_factor; 
                                    Vector R_; // reflection R = 2 (L.N)N – L 
                                    R_ = (N*toSource.dot(N))*2 - toSource;
                                    R_.normalize();
                                    phong+=pow(R_.dot(toSource), mp[k].shine )*scaling_factor;

                                }
                            }

                            for(int x=0; x<vsl.size(); x++){
                                HP P(I.x, I.y, I.z); // s = vl[i];
                                Vector PS(vsl[x].x - P.x, vsl[x].y - P.y, vsl[x].z - P.z); 
                                                                double theta=0;
                                Vector spotLightVec(vsl[x].lookx - vsl[x].x ,vsl[x].looky - vsl[x].y, vsl[x].lookz - vsl[x].z );
                                Vector SP(-PS.x,-PS.y,-PS.z );
                                theta = SP.dot(spotLightVec)/(SP.getLength() * spotLightVec.getLength());
                                theta = acos(theta); // radian 
                                theta = theta*180/pi;    

                                if( !( isRayInsideObj(P,PS, -1, k)|| ( theta > vsl[x].angleInDeg ) ) ){
                                    Vector toSource = PS;
                                    toSource.normalize();
                                    // (p,b,c) => point normal form = > n = (p-b)x(p-c)
                                    Vector A123(P.x-mt1[l].b.x, P.y-mt1[l].b.y,P.z-mt1[l].b.z);
                                    Vector B123(P.x-mt1[l].c.x, P.y-mt1[l].c.y,P.z-mt1[l].c.z);
                                    Vector N = A123.cross(B123);
                                    N.normalize();
                                    // N or -N verify ?
                                    Vector GP(P.x-g.x, P.y-g.y, P.z-g.z);
                                    if( (GP.dot(N))/(GP.getLength()*N.getLength() )<0  ){
                                        N.x = -N.x; N.y=-N.y; N.z = -N.z; // N = -N;
                                    }
                                    double distance1 = PS.getLength();
                                    double scaling_factor = distance1*distance1*vsl[x].fallOfParameter;
                                    scaling_factor = exp(-scaling_factor);

                                    lambert+= (toSource.dot(N))*scaling_factor; 
                                    Vector R_; // reflection R = 2 (L.N)N – L 
                                    R_ = (N*toSource.dot(N))*2 - toSource;
                                    R_.normalize();
                                    phong+=pow(R_.dot(toSource), mp[k].shine )*scaling_factor;

                                }
                            }
                            diff_sp.r = mp[k].dc*lambert*mp[k].cr;
                            diff_sp.g = mp[k].dc*lambert*mp[k].cg;
                            diff_sp.b = mp[k].dc*lambert*mp[k].cb;

                            spec_sp.r = mp[k].sc*phong*mp[k].cr;
                            spec_sp.g = mp[k].sc*phong*mp[k].cg;
                            spec_sp.b = mp[k].sc*phong*mp[k].cb;
                            myColorBuffer[i][j].setColor(amb_sp.r+diff_sp.r+spec_sp.r ,
                                                        amb_sp.g+diff_sp.g+spec_sp.g ,
                                                        amb_sp.b+diff_sp.b+spec_sp.b );
                        }
                    } 
                }
            }
        } 
    }
}

bool isRayInsideObj(HP p ,Vector v, int spNo=(-1) , int pyNo=(-1) ){    return ( (isRayInsidePyramid( p ,v, pyNo)) || (isRayInsideSphere( p , v, spNo)) );  }
bool isRayInsidePyramid( HP p ,Vector v, int pyNo=(-1) ){  return false;    }
bool isRayInsideSphere( HP p ,Vector v, int spNo=(-1) ){
    for(int i=0; i<ms.size(); i++){
        if(i != spNo){
            HP currCenter(ms[i].cx,ms[i].cy,ms[i].cz );
            Vector R0;
            R0.x = p.x - currCenter.x;R0.y = p.y - currCenter.y; R0.z = p.z - currCenter.z;
            double a1=0,b1=0,c1=0,
            t1=0,t2=0, D=0;
            a1 = 1;
            Vector Rd = v;
            b1 = Rd.dot(R0);
            b1 = b1*2;
            c1 = R0.dot(R0) - ( (ms[i].r)*(ms[i].r));
            D = b1*b1 - 4*c1;
            if(D<0) {   return false;  }
        }

    }return true;
}

void resetPointColorBuffer(){
    for(int i=0; i<numOfPixel; i++){
        for(int j=0; j<numOfPixel; j++){
            pointBuffer[i][j].zbuffer = MY_INF;
        }
    }
}

void actuallyDrawImage(){
    cout<<"Actual drawing start\n";
    bitmap_image image(numOfPixel, numOfPixel );
    for (int x = 0; x < numOfPixel; x++) {
        for (int y = 0; y < numOfPixel; y++) {
            image.set_pixel(x, y, myColorBuffer[x][y].r*255, myColorBuffer[x][y].g*255, myColorBuffer[x][y].b*255);
            //image.set_pixel(x, y,255,1,1);
        }
    }
    image.save_image("out.bmp");   cout<<"Actual drawing end\n";
}

void drawAxes(){
	glBegin(GL_LINES);
	    // x red 
		glColor3f(1,0,0);glVertex3f(1000,0,0);glVertex3f(0,0,0);
        glColor3f(1,1,1);glVertex3f(0,0,0);glVertex3f(-1000,0,0);
		// y g
		glColor3f(0,1,0);glVertex3f(0,1000,0);glVertex3f(0,0,0);
        glColor3f(1,1,1);glVertex3f(0,0,0);glVertex3f(0,-1000,0);
        // z b
		glColor3f(0,0,1);glVertex3f(0,0,1000);glVertex3f(0,0,0);
        glColor3f(1,1,1);glVertex3f(0,0,0);glVertex3f(0,0,-1000);
	glEnd();
}

void animate(){
	//codes for any changes in Models, Camera
	glutPostRedisplay();
}

void init(){
    glClearColor(0,0,0,0);  glMatrixMode(GL_PROJECTION);    
    glLoadIdentity();   gluPerspective(fovY, aspectRatio, disNearPlane, disFarPlane);
}

void keyboardListener(unsigned char key, int x,int y){
	switch(key){
		case '2':{
            l = l*cos(angle) + r*sin(angle);
            r = r.cross(l,u);
			break;
        }
	    case '1':{ 
			//drawgrid=1-drawgrid;
            l = l*cos(-angle) + r*sin(-angle);
            r = r.cross(l,u);
			break;}
        case '4':{ 
			//drawgrid=1-drawgrid;
            u = u*cos(angle) + l*sin(angle);
            l = l.cross(u,r);
			break;}
    	case '3':{
			//drawgrid=1-drawgrid;
            u = u*cos(-angle) + l*sin(-angle);
            l = l.cross(u,r);
			break;}
        case '5':{
            r = r*cos(angle) + u*sin(angle);
            u = u.cross(r,l);
            break;}
        case '6':{
            r = r*cos(-angle) + u*sin(-angle);
            u = u.cross(r,l);
            break;}
        case 'p':{
                cout<<"Camera position!\n";cameraPos.print();cout<<"\nl,r,u\n";
                l.print(); r.print(); u.print();break;
            }
        case '0':
            {
                resetPointColorBuffer();
                setPointBuffer();
                imageGenerationCheckerBoard();
                imageGenerationSphere();
                imageGenerationPyramid();
                actuallyDrawImage();
                break;
            }
		default:
			break;
	}
}

void specialKeyListener(int key, int x,int y){
	switch(key){
		case GLUT_KEY_DOWN:		//down arrow key
			//cameraHeight -= 3.0;
            cameraPos = cameraPos-l;
			break;
		case GLUT_KEY_UP:		// up arrow key
			//cameraHeight += 3.0;
            cameraPos = cameraPos + l;
			break;
		case GLUT_KEY_RIGHT:
			cameraPos = cameraPos + r;
			break;
		case GLUT_KEY_LEFT:
			cameraPos = cameraPos - r;
			break;
		case GLUT_KEY_PAGE_UP:
            cameraPos = cameraPos + u;
			break;
		case GLUT_KEY_PAGE_DOWN:
			cameraPos = cameraPos - u;
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
			if(state == GLUT_DOWN){		// 2 times?? in ONE click? -- solution is checking DOWN or UP
				drawaxes=1-drawaxes;
			}
			break;
		case GLUT_RIGHT_BUTTON:
			//........
			break;
		case GLUT_MIDDLE_BUTTON:
			//........
			break;
		default:
			break;
	}
}

int main(int argc, char **argv){
	ios_base::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);
    freopen("description.txt","r",stdin);
    cameraPos.setCoordinate(-200,-150,100);
    l.x = 0.7071; l.y = 0.7071; r.y = -l.x;  r.x = l.y;  u = u.cross(r,l);
    angle = 0.5*pi/180; 
    takeInput(); createDynamicArray();
    glutInit(&argc,argv);    
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color
	glutCreateWindow("Offline 4");
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

#include <bits/stdc++.h>

#ifdef _WIN32
#include <windows.h>
#include <glut.h>
#else
#include "GL/freeglut.h"
#include "GL/gl.h"
#endif

#include "bitmap_image.hpp"
using namespace std;

#define pi (2*acos(0.0))
#define epsilon (1.0e-6)
#define HP homogeneous_point

double cameraHeight;
double cameraAngle;
int drawgrid;
int drawaxes;
double angle;
bool myDrawGridFlag;

double disNearPlane, disFarPlane, fovX,fovY, aspectRatio, levelOfRecur, numOfPixel;
int widthOfCheckerBoard=50;
double ambient, diffuse, reflection;
int numOfObj, numOfLight, numOfSpotLight;


void init();
void display();
void animate();
void keyboardListener(unsigned char key, int x,int y);
void specialKeyListener(int key, int x,int y);
void mouseListener(int button, int state, int x, int y);
void drawAxes();
void drawGrid();
void drawSphere(double radius,int slices,int stacks, double cx, double cy, double cz);
void takeInput();
void testDrivenDevelopment();
void myDrawSpheres();
void myDrawPyramids();
void myDrawGrid();
void drawSquare(double a, double b, double c, bool toogle);
void imageGenerationSphere();
void doAfterTakingInput();
void myDrawLight();
void myDrawSpotlight();
void actuallyDrawImage();
void resetPointColorBuffer();
void createDynamicArray();
void myDestructor();
void imageGenerationCheckerBoard();

class Color {
public:
    double r, g, b;
    Color(double r=0, double g=0, double b=0) {
        this->r = r;
        this->g = g;
        this->b = b;
    }

    void setColor(double r=0, double g=0, double b=0){
        this->r = r;
        this->g = g;
        this->b = b;
    }
    
};

Color **myColorBuffer;

class light{
public:
    double x,y,z,fallOfParameter;
    light(){}
    void printer(){
        cout<<"light\n";
        cout<<x<<" "<<y<<" "<<z<<" "<<fallOfParameter<<"\n";
    }
};

class spotlight{
public:
    double x,y,z,fallOfParameter;
    double lookx, looky, lookz;
    double angleInDeg;
    spotlight(){}
    void printer(){
        cout<<"Spotligght\n";
        cout<<x<<" "<<y<<" "<<z<<" "<<fallOfParameter<<" "<<"\n";
        cout<<lookx<<" "<<looky<<" "<<lookz<<" "<<"\n";
        cout<<angleInDeg<<"\n";
    }
};

class mySphere{
public:
    double cx,cy,cz,r, cr,cg,cb,
    ac, dc, sc, rc, shine;
//    ambient diffuse specular reflection coefficient
    mySphere(){}
    void printer(){
        cout<<"MySphere\n";
        cout<<cx<<" "<<cy<<" "<<cz<<" "<<endl;
        cout<<r<<" "<<endl;
        cout<<cr<<" "<<cg<<" "<<cb<<" "<<endl;
        cout<<ac<<" "<<dc<<" "<<sc<<" "<<rc<<" "<<endl;
        cout<<shine<<endl;
    }
};


class myPyramid{
public:
    double px, py, pz, w,h,cr,cg,cb, ac, dc, sc, rc, shine;
    myPyramid(){};
    void printer(){
        cout<<px<<" "<<py<<" "<<pz<<" "<<endl;
        cout<<w<<" "<<h<<" "<<endl;
        cout<<cr<<" "<<cg<<" "<<cb<<" "<<endl;
        cout<<ac<<" "<<dc<<" "<<sc<<" "<<rc<<" "<<endl;
        cout<<shine<<" "<<endl;
    }
};

class mySquare{
public:
    double x,y,l;
    bool toogle;
    double r,b,g;
    mySquare(double a=0, double b=0, double c=0, bool toogle=false){
        this->x = a;
        this->y = b;
        this->l = c;
        this->toogle=toogle;
        if((this->toogle)==false){
            this->r=0; this->b=0; this->g=0;
        }else{
            r=1; b=1; g=1;
        }
    }

    void setter(double a=0, double b=0, double c=0, bool toogle=false){
        this->x = a;
        this->y = b;
        this->l = c;
        this->toogle=toogle;
        if(!(this->toogle)){
            this->r=0; this->b=0; this->g=0;
        }else{
            this->r=1; this->b=1; this->g=1;
        }
    }
};

class myObj{
public:
    string type;
    int i,j,num;
    double xi,yi,zi;
    myObj(){}
};

vector<mySphere> ms;
vector<myPyramid> mp;
mySquare **msq;
myObj **mobj;
vector<light> vl;
vector<spotlight> vsl;




class Vector
{
public:
    double x, y, z;

    // constructs a vector with given components
    Vector(double x=0, double y=0, double z=0) {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    // keeps the direction same. recalculates the vector to be unit.
    void normalize()
    {
        double r = sqrt(x*x + y*y + z*z);
        x = x / r;
        y = y / r;
        z = z / r;
    }

    double getLength(){ 
        double ret;
        ret = x*x + y*y + z*z;
        ret = sqrt(ret);
        return ret;
    }    

    // add two vectors
    Vector operator+(const Vector& v)
    {
        Vector v1(x+v.x, y+v.y, z+v.z);
        return v1;
    }

    // subtract one vector from another
    Vector operator-(const Vector& v)
    {
        Vector v1(x-v.x, y-v.y, z-v.z);
        return v1;
    }

    // scale a vector with a given coefficient
    Vector operator* (double m)
    {
        Vector v(x*m, y*m, z*m);
        return v;
    }

    // scale a vector with a given coefficient
    Vector operator/ (double m)
    {
        Vector v(x/m, y/m, z/m);
        return v;
    }

    // get the dot product of two vectors
    double dot(Vector b){
        return (this->x)*b.x + (this->y)*b.y + (this->z)*b.z;
    }
    static double dot(Vector a, Vector b)
    {
        return a.x*b.x + a.y*b.y + a.z*b.z;
    }

    // get the cross product of two vectors
    Vector cross( Vector b)
    {
        Vector v( (this->y)*b.z - (this->z)*b.y, b.x*(this->z) - b.z*(this->x), (this->x)*b.y - (this->y)*b.x);
        return v;
    }

    static Vector cross(Vector a, Vector b)
    {
        Vector v(a.y*b.z - a.z*b.y, b.x*a.z - b.z*a.x, a.x*b.y - a.y*b.x);
        return v;
    }

    Vector operator= (const Vector& point)
    {
        this->x = point.x;
        this->y = point.y;
        this->z = point.z;
        return *this;
    }

    static Vector R(Vector x, Vector a, double angle){
        Vector c,d,e,ret;
         c = x*cos(angle);
         d = a*dot(a,x);
        d = d*(1.0-cos(angle));
         e = (cross(x,a));
        e = e*sin(angle);

        ret = c + d + e;
        return ret; 
    }

    // print a vector. only for testing purposes.
    void print ()
    {
        cout << "Vector" << endl;
        cout << x << " " << y << " " << z << endl;
    }
};

class homogeneous_point
{
public:
    double x, y, z, w;
    double zbuffer;

    // set the three coordinates, set w to 1
    /*homogeneous_point(double x, double y, double z)
    {
        this->x = x;
        this->y = y;
        this->z = z;
        this->w = 1;
    }*/

    /*
    default constructor. does nothing. allows declarations like below:
        matrix m;
    therefore, usage is dangerous
    */
    homogeneous_point() {
        w=1;
        zbuffer = 5000;
    }

    // constructs a homogeneous point with given coordinates. forces w to be 1.0
    // if w is zero, raises error
    homogeneous_point(double x, double y, double z, double w=1)
    {
        assert (w != 0);
        this->x = x/w;
        this->y = y/w;
        this->z = z/w;
        this->w = 1;
    }
	void setCoordinate(double x=0, double y=0, double z=0, double w=1){
		this->x = x; this->y=y; this->z=z; this->w=1;
	}

    // adds two points. returns a point forcing w to be 1.0
    homogeneous_point operator+ (const homogeneous_point& point)
    {
        double x = this->x + point.x;
        double y = this->y + point.y;
        double z = this->z + point.z;
        double w = this->w + point.w;
        homogeneous_point p(x, y, z, w);
        return p;
    }

	homogeneous_point operator+ (const Vector& vec)
    {
        double x = this->x + vec.x;
        double y = this->y + vec.y;
        double z = this->z + vec.z;
        // double w = this->w + point.w;
        homogeneous_point p(x, y, z, 1);
        return p;
    }

    // subtracts one point from another. returns a point forcing w to be 1.0
    homogeneous_point operator- (const homogeneous_point& point)
    {
        double x = this->x - point.x;
        double y = this->y - point.y;
        double z = this->z - point.z;
        double w = this->w - point.w;
        homogeneous_point p(x, y, z, w);
    }

	homogeneous_point operator- (const Vector& vec)
    {
        double x = this->x - vec.x;
        double y = this->y - vec.y;
        double z = this->z - vec.z;
        // double w = this->w + point.w;
        homogeneous_point p(x, y, z, 1);
        return p;
    }

    homogeneous_point operator= (const homogeneous_point& point)
    {
        this->x = point.x;
    this->y = point.y;
        this->z = point.z;
        this->w = point.w;
        return *this;
    }

    Vector minus(const homogeneous_point& point){
        Vector ret; 
        ret.x = this->x - point.x;
        ret.y = this->y - point.y;
        ret.x = this->z - point.z;
        ret.normalize();
        return ret;
    }

    

    // Print the coordinates of a point. exists for testing purpose.
    void print()
    {
        cout << "Point: " << endl;
        cout << x << " " << y << " " << z << " " << w << endl;
    }

};
homogeneous_point **pointBuffer;

/*
The matrices are forced to be 4x4. This is because in this assignment, we will deal with points in triangles.
Maximum # of points that we will deal with at once is 3. And all the standard matrices are 4x4 (i.e. scale, translation, rotation etc.)
*/


/*
A simple class to hold the color components, r, g, b of a certain shade.
*/
class color {
public:
    double r, g, b;
    color(double r, double g, double b) {
        this->r = r;
        this->g = g;
        this->b = b;
    }
    color() {
    }

    color operator=(const color& v)
    {
        //Vector v1(v.x, v.y, v.z);
        this->r = v.r; this->g = v.g; this->b = v.b;// this->w = w;
        return *this;
    }
};

/* keep all the global variables */
 HP cameraPos;
 Vector l,r,u;

void myDrawPyramids(){
    for(int i=0; i<mp.size(); i++){
        double w = mp[i].w/2, h=mp[i].h;
        HP A,B,C,D,E, F;
        F.setCoordinate(mp[i].px,mp[i].py,mp[i].pz );
        E.setCoordinate(mp[i].px,mp[i].py,mp[i].pz+h );
        
        double r=mp[i].cr, g = mp[i].cg, b= mp[i].cb;
        A.setCoordinate(mp[i].px-w , mp[i].py-w , mp[i].pz);
        B.setCoordinate(mp[i].px+w , mp[i].py-w , mp[i].pz);
        C.setCoordinate(mp[i].px+w , mp[i].py+w , mp[i].pz);
        D.setCoordinate(mp[i].px-w , mp[i].py+w , mp[i].pz);

        // set color 
        glColor3f(r,g,b);
        // draw the base square
        glBegin(GL_QUADS);
        glVertex3f(A.x, A.y, A.z);
        glVertex3f(B.x, B.y, B.z);
        glVertex3f(C.x, C.y, C.z);
        glVertex3f(D.x, D.y, D.z);
        glEnd();
        // draw 4 triangles
        // ABE 
        glBegin(GL_TRIANGLES);
        glVertex3f(A.x, A.y, A.z);
        glVertex3f(B.x, B.y, B.z);
        glVertex3f(E.x, E.y, E.z);
        glEnd();
        // BCE 
        glBegin(GL_TRIANGLES);
        glVertex3f(E.x, E.y, E.z);
        glVertex3f(B.x, B.y, B.z);
        glVertex3f(C.x, C.y, C.z);
        glEnd();
        // CDE 
        glBegin(GL_TRIANGLES);
        glVertex3f(C.x, C.y, C.z);
        glVertex3f(D.x, D.y, D.z);
        glVertex3f(E.x, E.y, E.z);
        // DAE 
        glBegin(GL_TRIANGLES);
        glVertex3f(D.x, D.y, D.z);
        glVertex3f(A.x, A.y, A.z);
        glVertex3f(E.x, E.y, E.z);
        
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
    for(int i=0; i<vl.size(); i++){
        drawSphere(10, 25,25,vl[i].x, vl[i].y, vl[i].z);
    }
}
void myDrawSpotlight(){
        glColor3f(1,1,1);
        for(int i=0; i<vsl.size(); i++){
        drawSphere(5, 25,25,vsl[i].x, vsl[i].y, vsl[i].z);
    }
}

void drawSphere(double radius=0,int slices=0,int stacks=0, double cx=0, double cy=0, double cz=0)
{
	HP points[100][100];
	int i,j;
	double h,r;
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


void drawGrid()
{
	int i;
	//if(drawgrid==1)
	{
		glColor3f(0.6, 0.6, 0.6);	//grey
		glBegin(GL_LINES);{
			for(i=-1000;i<=1000;i+=widthOfCheckerBoard){

				if(i==0)
					continue;	//SKIP the MAIN axes

				//lines parallel to Y-axis
				glVertex3f(i, -1000, 0);
				glVertex3f(i,  1000, 0);

				//lines parallel to X-axis
				glVertex3f(-1000, i, 0);
				glVertex3f( 1000, i, 0);
			}
		}glEnd();
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


void keyboardListener(unsigned char key, int x,int y){
	switch(key){

		case '2':
			//drawgrid=1-drawgrid;
            l = l*cos(angle) + r*sin(angle);
            r = r.cross(l,u);
			break;
	    case '1':
			//drawgrid=1-drawgrid;
            l = l*cos(-angle) + r*sin(-angle);
            r = r.cross(l,u);
			break;
        case '4':
			//drawgrid=1-drawgrid;
            u = u*cos(angle) + l*sin(angle);
            l = l.cross(u,r);
			break;
    	case '3':
			//drawgrid=1-drawgrid;
            u = u*cos(-angle) + l*sin(-angle);
            l = l.cross(u,r);
			break;
        case '5':
            r = r*cos(angle) + u*sin(angle);
            u = u.cross(r,l);
            break;
        case '6':
            r = r*cos(-angle) + u*sin(-angle);
            u = u.cross(r,l);
            break;
        case 'p':{
                cout<<"Camera position!\n";
                cameraPos.print();
                cout<<"\nl,r,u\n";
                l.print(); r.print(); u.print();
                break;
            }
            
        case '0':
            {
                 resetPointColorBuffer();
                //myDestructor();
                //createDynamicArray();
                imageGenerationCheckerBoard();
                imageGenerationSphere();
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
    drawGrid();
    //if(myDrawGridFlag){     myDrawGrid();   }
    myDrawGrid();
    myDrawSpheres();
    myDrawPyramids();
    myDrawLight();
    myDrawSpotlight();
   
    glutSwapBuffers();
}


void drawAxes(){

	glBegin(GL_LINES);
	// x red 
		glColor3f(1,0,0);
		glVertex3f(1000,0,0);
		glVertex3f(-1000,0,0);
		// y g
		glColor3f(0,1,0);
		glVertex3f(0,1000,0);
		glVertex3f(0,-1000,0);
// z b
		glColor3f(0,0,1);
		glVertex3f(0,0,1000);
		glVertex3f(0,0,-1000);
	glEnd();
}


void animate(){
    //angle += 0.05;
	//codes for any changes in Models, Camera
	glutPostRedisplay();
}

void init(){
    glClearColor(0,0,0,0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //gluPerspective(80,1,1,1000);
    gluPerspective(fovY, aspectRatio, disNearPlane, disFarPlane);
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
    cout<<"do after taking input start\n";
    cout<<"create array of square objs for checker board\n";
    msq = new mySquare*[2001];
    mobj = new myObj*[(int)numOfPixel+1];
    for(int i=0; i<=2000; i++){
        msq[i] = new mySquare[2001];
    }
    for(int i=0; i<=numOfPixel; i++){   mobj[i] = new myObj[(int)numOfPixel+1];  }
        cout<<"do after taking input end\n";
}

void createDynamicArray(){
    pointBuffer = new homogeneous_point*[(int)numOfPixel+1];
    myColorBuffer = new Color*[(int)numOfPixel+1];

    for(int i=0; i<=numOfPixel; i++){
        pointBuffer[i] = new homogeneous_point[(int)numOfPixel+1];
        myColorBuffer[i] = new Color[(int)numOfPixel+1];
    }
}
void myDestructor(){
    for(int i=0; i<=numOfPixel; i++){
        delete[] pointBuffer[i];
        delete[] myColorBuffer[i];
    }
    delete[] pointBuffer;
    delete[] myColorBuffer;
    cout<<"Memory freed!\n";
}

void resetPointColorBuffer(){
    for(int i=0; i<numOfPixel; i++){
        for(int j=0; j<numOfPixel; j++){
            pointBuffer[i][j].zbuffer = 4000;
        }
    }
}

void imageGenerationCheckerBoard(){
    cout<<"img generation checker board start!\n";
    double screenHeight , screenWidth;
    HP a,b,c,d,e;
    // a = screenMidPoint;
    screenHeight = 2*disNearPlane*tan(fovY*pi/360);
    screenWidth  = 2*disNearPlane*tan(fovX*pi/360);

    a = cameraPos + l*disNearPlane;
    b = a + u*(screenHeight/2) + r*(screenWidth/2);
    c = a + u*(screenHeight/2) - r*(screenWidth/2);
    d = a - u*(screenHeight/2) - r*(screenWidth/2);
    e = a - u*(screenHeight/2) + r*(screenWidth/2);

    Vector CB(b.x-c.x,b.y-c.y,b.z-c.z ); 
    Vector CD(d.x-c.x,d.y-c.y,d.z-c.z);

    CB=CB/numOfPixel;
    CD=CD/numOfPixel;

    c = c + r*0.5*(screenWidth/numOfPixel) - u*0.5*(screenHeight/numOfPixel);

    for(int i=0; i<numOfPixel; i++){
        for(int j=0; j<numOfPixel; j++){
            // get pointBuff i,j location
            pointBuffer[i][j] = ((c + r*i*(screenWidth/numOfPixel)) - u*j*(screenHeight/numOfPixel)); 
             
        }
    }


    HP O(0,0,0);
    double t = 0;
    int count = 0;
  
    for(int i=0; i<numOfPixel; i++){
        for(int j=0; j<numOfPixel; j++){
            Vector R0(pointBuffer[i][j].x, pointBuffer[i][j].y, pointBuffer[i][j].z);// = pointBuffer[i][j].minus(O);
            Vector Rd(pointBuffer[i][j].x - cameraPos.x,pointBuffer[i][j].y - cameraPos.y,pointBuffer[i][j].z - cameraPos.z );// = pointBuffer[i][j].minus(cameraPos);
            Rd.normalize();
            // P(t) = R0 + tRd 
            // find t, d 
            t = -R0.z/Rd.z;
            Vector Intersection = R0 + Rd*t;
            if( t > 0 && Intersection.z <0.005  && Intersection.z > -.005){
                int i1 = ((int)Intersection.x + 1000)/widthOfCheckerBoard;
                int j1 = ((int)Intersection.y + 1000)/widthOfCheckerBoard;
                if(i1>=0 && i1<2000 && j1>=0 && j1<2000){
                    //count++;
                    //cout<<i1<<" "<<j1<<"\n";
                    double r123 = msq[i1][j1].r, g123 = msq[i1][j1].g, b123 = msq[i1][j1].b; 
                    pointBuffer[i][j].zbuffer = t*Rd.getLength();
                    //cout<<b123<<"\n";
                    myColorBuffer[i][j].setColor(r123*ambient, g123*ambient, b123*ambient);
                    myColorBuffer[i][j].setColor(r123, g123, b123);
                }
            }
            
        }
    }

    //cout<<"total captured squares = "<<count<<"\n";

}


void imageGenerationSphere(){
    cout<<"img generation sphere start!\n";
    double screenHeight , screenWidth;
    HP a,b,c,d,e, f;
    // a = screenMidPoint;
    screenHeight = 2*disNearPlane*tan(fovY*pi/360);
    screenWidth  = 2*disNearPlane*tan(fovX*pi/360);
    cout<<"Height "<<screenHeight<<"\n";
    cout<<"Width "<<screenWidth<<"\n";

    a = cameraPos + l*disNearPlane;
    b = a + u*(screenHeight/2) + r*(screenWidth/2);
    c = a + u*(screenHeight/2) - r*(screenWidth/2);
    d = a - u*(screenHeight/2) - r*(screenWidth/2);
    e = a - u*(screenHeight/2) + r*(screenWidth/2);

    f = c + (r*0.5)*(screenWidth/numOfPixel) - (u*0.5)*(screenHeight/numOfPixel);
    cout<<"l, r, u\n";
    l.print(); r.print(); u.print();
    cout<<"A,B,C,D,E, F\n";
    a.print(); b.print(); c.print(); d.print(); e.print(); f.print();
    cout<<"-------------------\n";
    
    for(int i=0; i<numOfPixel; i++){
        for(int j=0; j<numOfPixel; j++){
            // get pointBuff i,j location
            pointBuffer[i][j] = ((f + r*(i*(screenWidth/numOfPixel))) - u*(j*(screenHeight/numOfPixel))); 
            //pointBuffer[i][j].print();
        } //cout<<"\n";
    }


    HP O(0,0,0);
    // remove it!!!
    
    for(int i=0; i<numOfPixel; i++){
        for(int j=0; j<numOfPixel; j++){
            Vector R0(pointBuffer[i][j].x, pointBuffer[i][j].y, pointBuffer[i][j].z);// = pointBuffer[i][j].minus(O);
            Vector Rd(pointBuffer[i][j].x - cameraPos.x,pointBuffer[i][j].y - cameraPos.y,pointBuffer[i][j].z - cameraPos.z );// = pointBuffer[i][j].minus(cameraPos);
            Rd.normalize();
            // P(t) = R0 + tRd 
            // find t, d 
            for(int k=0; k<ms.size(); k++){
                HP currCenter(ms[k].cx, ms[k].cy, ms[k].cz);

                R0.x = R0.x - currCenter.x;R0.y = R0.y - currCenter.y; R0.z = R0.z - currCenter.z;
                
                double a1=0,b1=0,c1=0,
                t1=0,t2=0, D=0;
                a1 = 1;
                b1 = Rd.dot(R0);
                b1 = b1*2;
                c1 = R0.dot(R0) - ( (ms[k].r)*(ms[k].r));
                
                D = b1*b1 - 4*c1;
                if(D>=0){
                    D = sqrt(D);
                    t1 = (-b1+(D))/(2*a1);
                    t2 = (-b1-(D))/(2*a1);
                
                    double dis1, dis2, dismin;
                    dis1 = Rd.getLength();
                    dis2 = dis1;

                    dis1 = abs(dis1*t1);
                    dis2 = abs(dis1*t2);

                    dismin = min(dis1,dis2);
                    if(dismin<pointBuffer[i][j].zbuffer){
                        pointBuffer[i][j].zbuffer = dismin;
                        //myColorBuffer[i][j].setColor(ms[k].cr,ms[k].cg,ms[k].cb );
                        myColorBuffer[i][j].setColor(ms[k].cr*ms[k].ac,ms[k].cg*ms[k].ac,ms[k].cb*ms[k].ac );
                
                    }
                }

            } 
        }
    }
      // remove it!!!


    cout<<"img generation sphere end!\n";
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
    image.save_image("out.bmp");
    cout<<"Actual drawing end\n";
}

void testDrivenDevelopment(){
/*
double disNearPlane, disFarPlane, fovY, aspectRatio, levelOfRecur, numOfPixel;
int widthOfCheckerBoard=50;
double ambient, diffuse, reflection;
int numOfObj, numOfLight, numOfSpotLight;
*/
    if(disNearPlane == 1){  cout<<"ok\n"; }else{  cout<<"not ok\n"; }
    if(disFarPlane == 1000){  cout<<"ok\n"; }else{  cout<<"not ok\n"; }
    if(fovY == 80){  cout<<"ok\n"; }else{  cout<<"not ok\n"; }
    if(aspectRatio == 1){  cout<<"ok\n"; }else{  cout<<"not ok\n"; }
    if(levelOfRecur == 3){  cout<<"ok\n"; }else{  cout<<"not ok\n"; }
    if(numOfPixel == 500){  cout<<"ok\n"; }else{  cout<<"not ok\n"; }
    if(widthOfCheckerBoard == 50){  cout<<"ok\n"; }else{  cout<<"not ok\n"; }
    if(numOfPixel == 500){  cout<<"ok\n"; }else{  cout<<"not ok\n"; }
    if(ambient == 0.1){  cout<<"ok\n"; }else{  cout<<ambient<<"not ok\n"; }
    if(diffuse == 0.6){  cout<<"ok\n"; }else{  cout<<diffuse<<"not ok\n"; }
    if(reflection == 0.3){  cout<<"ok\n"; }else{  cout<<reflection<<"not ok\n"; }
    cout<<"\n----------\nsphere\n----------\n";
    for(int i=0; i<ms.size(); i++){
        ms[i].printer();
    }
    cout<<"\n----------\npyramid\n----------\n";
    for(int i=0; i<mp.size(); i++){
        mp[i].printer();
    }
        cout<<"\n----------\nlight\n----------\n";
    for(int i=0; i<vl.size(); i++){
        vl[i].printer();
    }
        cout<<"\n----------\nspotlight\n----------\n";
    for(int i=0; i<vsl.size(); i++){
        vsl[i].printer();
    }
}


int main(int argc , char **argv){
    myDrawGridFlag = true;
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);
    freopen("description.txt","r",stdin);
    takeInput();
    doAfterTakingInput();
    createDynamicArray();
    //testDrivenDevelopment();

	//
	cameraPos.setCoordinate(-200,-150,100);
    
	//r ==.x , l==> y , u ==> z
    /*  original formula 
    r.x = 0.7071; r.y = 0.7071; 
	l.x = -r.y; l.y = r.x; 
    */
       l.x = 0.7071; l.y = 0.7071; 
	r.y = -l.x;  r.x = l.y;
   u = u.cross(r,l);
    
    //u = u.cross(r,l);
    //
    angle = 0.5*pi/180; 

    //
    glutInit(&argc, argv);
    glutInitWindowSize(500,500);
    glutInitWindowPosition(0,0);

    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color
	glutCreateWindow("Offline 4");
	init();
    //gluPerspective(1.0*fovY*pi/180, aspectRatio, disNearPlane, disFarPlane);
	glEnable(GL_DEPTH_TEST);	//enable Depth Testing

	glutDisplayFunc(display);	//display callback function
	glutIdleFunc(animate);		//what you want to do in the idle time (when no drawing is occuring)

	glutKeyboardFunc(keyboardListener);
	glutSpecialFunc(specialKeyListener);
	glutMouseFunc(mouseListener);

	glutMainLoop();		//The main loop of OpenGL



    return 0;
}
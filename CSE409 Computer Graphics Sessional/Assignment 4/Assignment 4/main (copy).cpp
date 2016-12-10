using namespace std ;
#include <bits/stdc++.h>
#include <glut.h>
#define pi (2*acos(0.0))
#include "color.h"
#include "points.h"
#include "triangle.h"
#include "square.h"
#include "sphere.h"
#include "pyramid.h"
#include "checker_board.h"
#include "ray_trace.h"

double cameraHeight;
double cameraAngle;
int drawgrid;
int drawaxes;
double angle;
Point pos,u,l,r,normalizedu,normalizedl,normalizedr;


vector<Sphere>spheres ;
vector<Pyramid>pyramids ;
vector<Square>squares ;

void drawAxes(){
	if(drawaxes==1){
		glColor3f(1.0, 1.0, 1.0);
		glBegin(GL_LINES);{
			glVertex3f( 100,0,0);
			glVertex3f(-100,0,0);

			glVertex3f(0,-100,0);
			glVertex3f(0, 100,0);

			glVertex3f(0,0, 100);
			glVertex3f(0,0,-100);
		}glEnd();
	}
}
void drawGrid()
{
	int i;
	if(drawgrid==1)
	{
		glColor3f(0.6, 0.6, 0.6);	//grey
		glBegin(GL_LINES);{
			for(i=-8;i<=8;i++){

				if(i==0)
					continue;	//SKIP the MAIN axes

				//lines parallel to Y-axis
				glVertex3f(i*10, -90, 0);
				glVertex3f(i*10,  90, 0);

				//lines parallel to X-axis
				glVertex3f(-90, i*10, 0);
				glVertex3f( 90, i*10, 0);
			}
		}glEnd();
	}
}
void keyboardListener(unsigned char key, int x,int y){
	switch(key){

		case '1': // look up
            u = Rotation(u,r,(pi/180)*3) ;
            l = Rotation(l,r,(pi/180)*3) ;
			break;
        case '2':
            u = Rotation(u,r,-(pi/180)*3) ;
            l = Rotation(l,r,-(pi/180)*3) ;
            break ;
        case '3':
            l = Rotation(l,u,(pi/180)*3) ;
            r = Rotation(r,u,(pi/180)*3) ;
            break ;
        case '4':
            l = Rotation(l,u,-(pi/180)*3) ;
            r = Rotation(r,u,-(pi/180)*3) ;
            break ;
        case '0':
            /// Ray Tracing is applied
            ray_tracing() ;
		default:
			break;
	}
}



void specialKeyListener(int key, int x,int y){
	switch(key){
		case GLUT_KEY_DOWN:		//down arrow key
			//cameraHeight -= 3.0;
            pos = pos-l*2.00 ;
			break;
		case GLUT_KEY_UP:		// up arrow key
		//	cameraHeight += 3.0;
            pos = pos+l*2.00 ;
			break;

		case GLUT_KEY_RIGHT:
			//cameraAngle += 0.03;
			pos = pos+r*2.00 ;
			break;
		case GLUT_KEY_LEFT:
			pos = pos-r*2.00 ;
			break;
		case GLUT_KEY_PAGE_UP:
			pos = pos+u*2.00 ;
			break;
		case GLUT_KEY_PAGE_DOWN:
			pos = pos-u*2.00 ;
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

	gluLookAt(pos.x,pos.y,pos.z, pos.x+l.x,pos.y+l.y,pos.z+l.z,u.x,u.y,u.z);
	//again select MODEL-VIEW
	glMatrixMode(GL_MODELVIEW);


	/****************************
	/ Add your objects from here
	****************************/
	//add objects

	drawAxes();
	drawGrid();
	//drawSphere(Point(10,10,10),10,100,100) ;
   // drawPyramid(Point(50,50,10),50,15) ;
	//ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)

    ///Draw the checker board
    drawcheckerboard() ;
    ///     Draw the objects
    for(int i=0;i<(int)spheres.size();i++){
        spheres[i].draw() ;
    }
    for(int i=0;i<(int)pyramids.size();i++){
            pyramids[i].draw() ;
    }
    //cout<<"here"<<endl ;
	glutSwapBuffers();
}


void animate(){
	angle+=0.05;
	//codes for any changes in Models, Camera
	glutPostRedisplay();
}

void init(){
	//codes for initialization
	drawgrid=0;
	drawaxes=1;
	cameraHeight=150.0;
	cameraAngle=1.0;
	angle=0;
	pos.x=100 ;
	pos.y=100;
	pos.z=50 ;
    //////////////////////////// up vector ////////////////////
    u.x=u.y=0 ;
    u.z=1 ;
    /////////////////////////// right vector ////////////////////
    r.x=-1.00/sqrt(2) ;
    r.y=1.00/sqrt(2) ;
    r.z =0 ;
    ///////////////////////////// left vector////////////////////
    l.x=r.x ;
    l.y=r.x ;
    l.z=0 ;
	//clear the screen
	l = normalize(l) ;
	u = normalize(u) ;
	r = normalize(r) ;
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

	/// read from file and draw the objects
    FILE *fp ;
    fp = fopen("description.txt","r") ;
    int N ,screen,recursionLevel;
    Sphere sp ;
    Pyramid py ;
    char name[55] ;
    double radius,ambient,diffuse,specular,reflection,shineness,x,y,z,r,g,b,height,width ;
    fscanf(fp,"%d %d %d",&N,&screen,&recursionLevel) ;
    while(N--){
        fscanf(fp,"%s",name) ;
        if(strcmp(name,"sphere")==0){
                fscanf(fp,"%lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf",&x,&y,&z,&radius,&r,&g,&b,&ambient,&diffuse,&specular,&reflection,&shineness) ;
                sp = Sphere(Point(x,y,z),radius,Color(r,g,b),ambient,diffuse,specular,reflection,shineness) ;
                spheres.push_back(sp) ;
        }
        else{ /// this is a pyramid
            fscanf(fp,"%lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf",&x,&y,&z,&width,&height,&r,&g,&b,&ambient,&diffuse,&specular,&reflection,&shineness) ;
            py = Pyramid(Point(x,y,z),width,height,Color(r,g,b),ambient,diffuse,specular,reflection,shineness) ;
            pyramids.push_back(py) ;
        }
    }
    ///
    fclose(fp) ;
}

int main(int argc, char **argv){
	glutInit(&argc,argv);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color
	glutCreateWindow("Ray Casting");
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

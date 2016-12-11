/* ===============================
Group Project - 3d Maze

Justin, Chau, Shana, Jeremy

Creates a 3d Maze through which 
a user can move

=============================== */

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <math.h>
#include <ctime>
#include <iostream>

#define PI 3.14159265

//toggles for debugging
const bool gridDebug = false;
const bool keyDebug = true;

//set where to look
static float eyeX = 1.5;
static float eyeY = 2.0;
static float eyeZ = 1.5;
static float focusX = 2.0;
static float focusY = 0.5;
static float focusZ = 1.5;
static float rotation = 0;
static float rotationAmt = PI / 12;
static int playerX = eyeX;
static int playerZ = eyeZ;

//create the full grid
const int gridX = 25;
const int gridY = 25;
int maze[gridX][gridY];

// ---------- wall ---------------

void wall(int width)
{
	glPushMatrix();
	glTranslated(0.5, 0.5 * width, 0.5);
	glScaled(1.0, width, 1.0);
	glutSolidCube(1.0);
	glPopMatrix();
}

// ---------- createCubes --------

void createCubes(void)
{
	//for each spot in the maze
	for(int spotX = 0; spotX < gridX; spotX++)
	{
		for(int spotY = 0; spotY < gridY; spotY++)
		{
			//if there should be a cube there (1)
			if(maze[spotX][spotY] == 1){
				//create a cube there
				glPushMatrix();
				glTranslated(spotX + 0.5, 0.5, spotY + 0.5);
				glutSolidCube(1.0);
				glPopMatrix();
			}
		}
	}
}

// ---------- createSphere -------

void createSphere(void)
{
	int randX = rand() % 26;
	int randY = rand() % 26;
	int iterations = 0;

	while(maze[randX][randY]==1){
		randX = randX + 1 ;
		iterations = iterations + 1;
		if(iterations % 25==0){
			randY = randY + 1;
			}
		}
		
	glPushMatrix();
	glTranslated((randX + 0.5), 0.5, (randY + 0.5));
	glScaled(0.4, 0.4, 0.4);
	glutSolidSphere(0.2, 15, 15);
	glPopMatrix();
	
}

// ---------- grid ---------------

void grid(void)
{	
	//length and width of maze
	int spotX;
	int spotY;
	
	//for each row and column of maze
	if(maze[0][0] != 1)
	{
		for(spotX = 0; spotX < gridX; spotX++)
		{
			for(spotY = 0; spotY < gridY; spotY++)
			{
				if((spotX == 0)||(spotY == 0)||(spotX == gridX - 1)||(spotY == gridY - 1))
				{
					maze[spotX][spotY] = 1;
				}
				else 
				{
					if(rand() % 3 == 1)
					{
						maze[spotX][spotY] = 1;
					}
				}
			}
		}
	}
	
	std::cout << "\n";
	
	//if debugging, print the numbers stored in the maze	
	if(gridDebug){
		for(int intX = 0; intX < gridX; intX++)
		{
			for(int intY = 0; intY < gridY; intY++)
			{
				std::cout << maze[intX][intY] << " ";
			}
			
			std::cout << "\n";
		}
		
		std::cout << "\n";
	}
	
	//call to the creation of maze
	createCubes();
	
	createSphere();
	
}

// ---------- floor --------------

void floor(int gridX, int gridY)
{
	//scale the floor to appropriate size
	glPushMatrix();
	glScaled(gridX, gridY, 1.0);
	wall(0.02);
	glPopMatrix();	
}

// ---------- display ------------

void display(void)
{
	// set the initial object colors: white
	GLfloat matAmbient[] = {1.0f, 1.0f, 1.0f, 1.0f};
	GLfloat matDiffuse[] = {0.6f, 0.6f, 0.6f, 1.0f};
	GLfloat matSpecular[] = {1.0f, 1.0f, 1.0f, 1.0f};
	GLfloat matShininess[] = {1.0f};
	glMaterialfv(GL_FRONT,GL_AMBIENT,matAmbient);
	glMaterialfv(GL_FRONT,GL_DIFFUSE,matDiffuse);
	glMaterialfv(GL_FRONT,GL_SPECULAR,matSpecular);
	glMaterialfv(GL_FRONT,GL_SHININESS,matShininess);
	
	// set the light source
	GLfloat lightPosition[] = {2.0f, 6.0f, 3.0f, 0.0f};
	GLfloat lightIntensity[] = {0.7f, 0.7f, 0.7f, 1.0f};
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightIntensity);
	
	// set the initial camera
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	double winHt = 1.0; // half-height of the window
	glFrustum(-winHt*64/48.0, winHt*64/48.0, -winHt, winHt, 0.1, 100.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	gluLookAt(eyeX, eyeY, eyeZ, focusX, focusY, focusZ, 0.0, 1.0, 0.0);
	
	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	
	grid();
	
	glFlush();
}

// ---------- keys ---------------

void keys(unsigned char key, int x, int y)
{
	double distance = 0.25;
	switch(key){
		case 'w':
			eyeX += distance * cos(rotation);
			eyeZ += distance * sin(rotation);
			focusX += distance * cos(rotation);
			focusZ += distance * sin(rotation);
			if(keyDebug)
			{
				std::cout << rotation << " " << eyeX << "," << eyeZ;
				std::cout << "\n " << focusX << "," << focusZ << "\n";
				std::cout << playerX << " " << playerZ;
			}
			glutPostRedisplay();
			break;
		case 'a':
			rotation -= rotationAmt;
			focusX = eyeX + cos(rotation);
			focusZ = eyeZ + sin(rotation);
			if(keyDebug)
			{
				std::cout << rotation << " " << eyeX << "," << eyeZ;
				std::cout << "\n " << rotation / PI * 180 << "," << rotation  / PI * 180 ;
			}
			glutPostRedisplay();
			break;
		case 's':
			eyeX -= distance * cos(rotation);
			eyeZ -= distance * sin(rotation);
			focusX -= distance * cos(rotation);
			focusZ -= distance * sin(rotation);
			if(keyDebug)
			{
				std::cout << rotation << " " << eyeX << "," << eyeZ; 
				std::cout << " " << focusX << "," << focusZ;
			}
			glutPostRedisplay();
			break;
		case 'd':
			rotation += rotationAmt;
			focusX = eyeX + cos(rotation);
			focusZ = eyeZ + sin(rotation);
			if(keyDebug)
			{
				std::cout << rotation << " " << eyeX << "," << eyeZ;
				std::cout << "\n " << rotation / PI * 180 << "," << rotation  / PI * 180 ;
			}
			glutPostRedisplay();
			break;
		case 27:
			exit(0);
	}	
	
	
}

// ---------- main ---------------

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE| GLUT_RGB| GLUT_DEPTH);
	glutInitWindowSize(640,480);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Our A'maze'ing Project 2");
	//glutIdleFunc(update);
	glutDisplayFunc(display);
	glutKeyboardFunc(keys);
	glEnable(GL_LIGHTING); // enable the light source
	glEnable(GL_LIGHT0);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST); // for hidden surface removal
	glEnable(GL_NORMALIZE); // normalize vectors for proper shading
	glClearColor(0.6f,0.6f,0.6f,0.0f); // background is light gray
	glViewport(0, 0, 640, 480);
	glutMainLoop();
	
	
	
	return(0);
}
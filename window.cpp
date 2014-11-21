#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <numeric>
#include <vector>
#include "GLee.h"
#include <GL/glut.h>
#include <time.h>
#include "Window.h"
#include "Cube.h"
#include "Matrix4.h"
#include "Vector3.h"
#include "main.h"
#include "Camera.h"
#include "Robot.h"
#include "MatrixTransform.h"
#include "Cube4.h"
#include "shader.h"

#define PI 3.14159265

using namespace std;

int Window::width  = 512;   // set window width in pixels here
int Window::height = 512;   // set window height in pixels here

int toggle = 0;
int mode = 1;

//Trackball
static bool left_clicked = false;
static bool right_clicked = false;
static int x_mouse;
static int y_mouse;
//End of trackball

//----------------------------------------------------------------------------
// Callback method called when system is idle.
void Window::idleCallback()
{
	//if (toggle == 1)
		//Globals::cube.spin(-0.5);   // rotate cube; if it spins too fast try smaller values and vice versa
	//else
		//Globals::cube.spin(0.5);
  displayCallback();         // call display routine to show the cube
}

//----------------------------------------------------------------------------
// Callback method called by GLUT when graphics window is resized by the user
void Window::reshapeCallback(int w, int h)
{
  cerr << "Window::reshapeCallback called" << endl;
  width = w;
  height = h;
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glViewport(0, 0, w, h);  // set new viewport size
  gluPerspective(30.0, double(width)/(double)height, 1.0, 1000.0); // set perspective projection viewing frustum
  glTranslatef(0, 0, -20);    // move camera back 20 units so that it looks at the origin (or else it's in the origin)
  glMatrixMode(GL_MODELVIEW);

  Globals::camera.setFrustum(30.0, (double)(width) / (double)height, 1.0, 1000.0);
}

int bunnyNumFaces = 69666;
int bunnyNumVerts = 34835;

std::vector<Vector3> bunnyVert;
std::vector<Vector3> bunnyRgb;
std::vector<Vector3> bunnyNorm;
std::vector<Vector3> bunnyFaceVert;
std::vector<Vector3> bunnyFaceNorm;

float bunnyScale = 0;
float bunnyMidX = 0;
float bunnyMidY = 0;
float bunnyMidZ = 0;

void Window::loadBunny2()
{
	FILE* fp;
	float x, y, z;
	float r, g, b;
	float nx, ny, nz;
	float fvx, fvy, fvz;
	float fnx, fny, fnz;
	int c1, c2;

	float xMax = 0;
	float xMin = 0;

	float yMax = 0;
	float yMin = 0;

	float zMax = 0;
	float zMin = 0;

	fp = fopen("bunny.obj", "rb");

	do{
		c1 = fgetc(fp);
		c2 = fgetc(fp);
		if ((c1 == 'v') && (c2 == ' '))
		{
			fscanf(fp, "%f %f %f %f %f %f", &x, &y, &z, &r, &g, &b);
			Vector3 xyz = Vector3(x, y, z);
			Vector3 colors = Vector3(r, g, b);
			bunnyVert.push_back(xyz);
			bunnyRgb.push_back(colors);

			xMax = fmaxf(xMax, x);
			xMin = fminf(xMin, x);

			yMax = fmaxf(yMax, y);
			yMin = fminf(yMin, y);

			zMax = fmaxf(zMax, z);
			zMin = fminf(zMin, z);
		}
		else if ((c1 == 'v') && (c2 == 'n'))
		{
			fscanf(fp, "%f %f %f", &nx, &ny, &nz);
			Vector3 m = Vector3(nx, ny, nz);
			m.normalize();
			bunnyNorm.push_back(m);
		}
		else if( (c1 == 'f') && (c2 == ' '))
		{
			fscanf(fp, "%f//%f %f//%f %f//%f", &fvx, &fnx, &fvy, &fny, &fvz, &fnz);
			Vector3 fv = Vector3(fvx, fvy, fvz);
			Vector3 fn = Vector3(fnx, fny, fnz);
			bunnyFaceVert.push_back(fv);
			bunnyFaceNorm.push_back(fn);
		}
		else
		{
			fscanf(fp, "");
		}
	} while (c1 != EOF);

	float len = 10 * tan(30 * PI / 180);

	bunnyScale = (len / (xMax - xMin));

	bunnyMidX = xMin + (xMax - xMin) / 2;
	bunnyMidY = yMin + (yMax - yMin) / 2;
	bunnyMidZ = zMin + (zMax - zMin) / 2;
}

void drawBunny()
{
	Matrix4 bunnyTranslate;
	bunnyTranslate.makeTranslate(-bunnyMidX, -bunnyMidY, -bunnyMidZ);

	Matrix4 bunnyScaling;
	bunnyScaling.makeScale(bunnyScale, bunnyScale, bunnyScale);

	for (int i = 0; i < bunnyNumFaces - 1; i++)
	{
		//First Vertex
		int first = bunnyFaceVert[i].x;
		Vector4 bunny = Vector4(bunnyVert[first - 1].x, bunnyVert[first - 1].y, bunnyVert[first - 1].z, 1);

		bunny = bunnyTranslate*bunny;
		bunny = bunnyScaling*bunny;

		Vector3 bunnyColor = bunnyRgb[first - 1];

		int fn = bunnyFaceNorm[i].x;
		Vector3 normBunny = bunnyNorm[fn - 1];

		glColor3d(bunnyColor.x, bunnyColor.y, bunnyColor.z);
		glNormal3d(normBunny.x, normBunny.y, normBunny.z);
		glVertex3d(bunny.x, bunny.y, bunny.z);

		//Second Vertex
		int second = bunnyFaceVert[i].y;
		Vector4 bunny2 = Vector4(bunnyVert[second - 1].x, bunnyVert[second - 1].y, bunnyVert[second - 1].z, 1);

		Vector3 bunnyColor2 = bunnyRgb[second - 1];

		int sn = bunnyFaceNorm[i].y;
		Vector3 normBunny2 = bunnyNorm[sn - 1];

		bunny2 = bunnyTranslate*bunny2;
		bunny2 = bunnyScaling*bunny2;

		glColor3d(bunnyColor2.x, bunnyColor2.y, bunnyColor2.z);
		glNormal3d(normBunny2.x, normBunny2.y, normBunny2.z);
		glVertex3d(bunny2.x, bunny2.y, bunny2.z);

		//Third Vertex
		int third = bunnyFaceVert[i].z;
		Vector4 bunny3 = Vector4(bunnyVert[third - 1].x, bunnyVert[third - 1].y, bunnyVert[third - 1].z, 1);

		Vector3 bunnyColor3 = bunnyRgb[third - 1];

		int tn = bunnyFaceNorm[i].z;
		Vector3 normBunny3 = bunnyNorm[tn - 1];

		bunny3 = bunnyTranslate*bunny3;
		bunny3 = bunnyScaling*bunny3;

		glColor3d(bunnyColor3.x, bunnyColor3.y, bunnyColor3.z);
		glNormal3d(normBunny3.x, normBunny3.y, normBunny3.z);
		glVertex3d(bunny3.x, bunny3.y, bunny3.z);
	}
	glEnd();
}

int dragNumFaces = 871168;
int dragNumVerts = 435476;

std::vector<Vector3> dragVert;
std::vector<Vector3> dragNorm;
std::vector<Vector3> dragFaceVert;
std::vector<Vector3> dragFaceNorm;

float dragScale = 0;
float dragMidX = 0;
float dragMidY = 0;
float dragMidZ = 0;

void Window::loadDragon2()
{
	FILE* fp;
	float x, y, z;
	float nx, ny, nz;
	float fvx, fvy, fvz;
	float fnx, fny, fnz;
	int c1, c2;

	float xMax = 0;
	float xMin = 0;

	float yMax = 0;
	float yMin = 0;

	float zMax = 0;
	float zMin = 0;

	fp = fopen("dragon.obj", "rb");

	do{
		c1 = fgetc(fp);
		c2 = fgetc(fp);
		if ((c1 == 'v') && (c2 == ' '))
		{
			fscanf(fp, "%f %f %f", &x, &y, &z);
			Vector3 xyz = Vector3(x, y, z);
			dragVert.push_back(xyz);

			xMax = fmaxf(xMax, x);
			xMin = fminf(xMin, x);

			yMax = fmaxf(yMax, y);
			yMin = fminf(yMin, y);

			zMax = fmaxf(zMax, z);
			zMin = fminf(zMin, z);
		}
		else if ((c1 == 'v') && (c2 == 'n'))
		{
			fscanf(fp, "%f %f %f", &nx, &ny, &nz);
			Vector3 m = Vector3(nx, ny, nz);
			m.normalize();
			dragNorm.push_back(m);
		}
		else if ((c1 == 'f') && (c2 == ' '))
		{
			fscanf(fp, "%f//%f %f//%f %f//%f", &fvx, &fnx, &fvy, &fny, &fvz, &fnz);
			Vector3 fv = Vector3(fvx, fvy, fvz);
			Vector3 fn = Vector3(fnx, fny, fnz);
			dragFaceVert.push_back(fv);
			dragFaceNorm.push_back(fn);
		}
		else
		{
			fscanf(fp, "");
		}
	} while (c1 != EOF);

	float len = 10 * tan(30 * PI / 180);

	dragScale = (len / (xMax - xMin));

	dragMidX = xMin + (xMax - xMin) / 2;
	dragMidY = yMin + (yMax - yMin) / 2;
	dragMidZ = zMin + (zMax - zMin) / 2;
}

void drawDragon()
{
	Matrix4 dragTranslate;
	dragTranslate.makeTranslate(-dragMidX, -dragMidY, -dragMidZ);

	Matrix4 dragScaling;
	dragScaling.makeScale(dragScale, dragScale, dragScale);

	for (int i = 0; i < dragNumFaces - 1; i++)
	{
		//First Vertex
		int first = dragFaceVert[i].x;
		Vector4 dragon = Vector4(dragVert[first - 1].x, dragVert[first - 1].y, dragVert[first - 1].z, 1);

		dragon = dragTranslate*dragon;
		dragon = dragScaling*dragon;

		int fn = dragFaceNorm[i].x;
		Vector3 normDrag = dragNorm[fn - 1];

		//myShader.bind();

		//glColor3d(dragColor.x, dragColor.y, dragColor.z);
		glNormal3d(normDrag.x, normDrag.y, normDrag.z);
		glVertex3d(dragon.x, dragon.y, dragon.z);

		//Second Vertex
		int second = dragFaceVert[i].y;
		Vector4 dragon2 = Vector4(dragVert[second - 1].x, dragVert[second - 1].y, dragVert[second - 1].z, 1);

		int sn = dragFaceNorm[i].y;
		Vector3 normDrag2 = dragNorm[sn - 1];

		dragon2 = dragTranslate*dragon2;
		dragon2 = dragScaling*dragon2;

		//glColor3d(dragColor2.x, dragColor2.y, dragColor2.z);
		glNormal3d(normDrag2.x, normDrag2.y, normDrag2.z);
		glVertex3d(dragon2.x, dragon2.y, dragon2.z);

		//Third Vertex
		int third = dragFaceVert[i].z;
		Vector4 dragon3 = Vector4(dragVert[third - 1].x, dragVert[third - 1].y, dragVert[third - 1].z, 1);

		int tn = dragFaceNorm[i].z;
		Vector3 normDrag3 = dragNorm[tn - 1];

		dragon3 = dragTranslate*dragon3;
		dragon3 = dragScaling*dragon3;

		//glColor3d(dragColor3.x, dragColor3.y, dragColor3.z);
		glNormal3d(normDrag3.x, normDrag3.y, normDrag3.z);
		glVertex3d(dragon3.x, dragon3.y, dragon3.z);
	}
	glEnd();
}

//----------------------------------------------------------------------------
// Callback method called by GLUT when window readraw is necessary or when glutPostRedisplay() was called.

void Window::displayCallback()
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // clear color and depth buffer

	//Shader
	Shader myShader = Shader("diffuse_shading.vert","diffuse_shading.frag", true);
	myShader.bind();
	myShader.printLog();
	//End of shader

	//Light
	float specular[] = { 1.0, 1.0, 1.0, 1.0 };
	float shininess[] = { 100.0 };
	float position[] = { 0.0, 100.0, 1.0, 0.0 };
	float diffuse[] = { 0.0, 0.0, 1.0, 1.0 };
	float ambient[] = { 1.0, 1.0, 1.0, 1.0 };
	float bunnyDiff[] = { 1.0, 1.0, 1.0, 1.0 };

	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_SMOOTH);

	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, bunnyDiff);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);

	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);

	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, position);

	//Spotlight
	//glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 1.0);
	//glPushMatrix();
	//float spot_direction[] = { 0.0, 0.0, -20.0, 0.0 };
	//glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spot_direction);
	//glEnable(GL_LIGHT0);
	//glPopMatrix();
	glEnable(GL_LIGHTING);
	//glEnable(GL_LIGHT0);
	//glEnable(GL_LIGHT0);

	Matrix4 i;
	i.identity();
	glLoadMatrixd(i.getPointer());
	GLfloat spot_diffuse[] = { 1.0, 0.0, 0.0, 1.0 };
	glLightfv(GL_LIGHT1, GL_DIFFUSE, spot_diffuse);
	//glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 1);
	//glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 2.0);
	//glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 2.0);
	GLfloat spot_direction[] = { 0.0, 0.0, -1.0};
	GLfloat mypos[] = { 0, -1, 20, 1 };
	glLightfv(GL_LIGHT1, GL_POSITION, mypos);
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spot_direction);
	Matrix4 a;
	a.identity();
	glLoadMatrixd(a.getPointer());
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 4);
	glEnable(GL_LIGHT1);
	
  //End of Light

  glMatrixMode(GL_MODELVIEW);  // make sure we're in Modelview mode

  Matrix4 glmatrix;
  glmatrix = Globals::cube.getMatrix();
  glmatrix.transpose();
  glLoadMatrixd(glmatrix.getPointer());

  glBegin(GL_TRIANGLES);

  drawBunny();
  //drawDragon();
  glEnd();
  
  glFlush();  
  glutSwapBuffers();
}

void Window::keyboardCallback(unsigned char key, int x, int y)
{
	Matrix4 scaleOut;
	scaleOut.makeScale(.9, .9, .9);

	Matrix4 scaleIn;
	scaleIn.makeScale(1.1, 1.1, 1.1);

	Matrix4 rotateClock;
	rotateClock.makeRotateY(10);

	Matrix4 rotateCClock;
	rotateCClock.makeRotateY(-10);

	switch (key)
	{
		case 66:
			//box = !box;
			break;
		case 98:
			//bound = !bound;
			break;
		case 99:
			//frustum = !frustum;
			break;
		case 120:
			Globals::cube.moveLeftRight(-2);
			break;
		case 88:
			Globals::cube.moveLeftRight(2);
			break;
		case 121:
			Globals::cube.moveUpDown(-2);
			break;
		case 89:
			Globals::cube.moveUpDown(2);
			break;
		case 122:
			Globals::camera.c = Globals::camera.c * scaleOut;
			//Globals::cube.moveOutIn(2);
			break;
		case 90:
			Globals::camera.c = Globals::camera.c * scaleIn;
			//Globals::cube.moveOutIn(-2);
			break;
		case 114:
			//Globals::camera.c = Globals::camera.c * rotateClock;
			Globals::cube.rotateY(5);
			break;
		case 82:
			//Globals::camera.c = Globals::camera.c * rotateCClock;
			Globals::cube.rotateY(-5);
			break;
		case 111:
			Globals::cube.orbit(10);
			break;
		case 79:
			Globals::cube.orbit(-10);
			break;
		case 115:
			Globals::cube.scale(0.9, 0.9, 0.9);
			break;
		case 83:
			Globals::cube.scale(1.1, 1.1, 1.1);
			break;
		case 116:
			if (toggle == 0)
				toggle = 1;
			else
				toggle = 0;
			break;
		default:
			break;
	}
}

void Window::specialFuncCallback(int key, int x, int y)
{
	switch (key)
	{
		case GLUT_KEY_F1:
			mode = 1;
			break;
		case GLUT_KEY_F2:
			mode = 2;
			break;
		case GLUT_KEY_F3:
			mode = 3;
			break;
		case GLUT_KEY_F4:
			mode = 4;
			break;
		case GLUT_KEY_F5:
			mode = 5;
			break;
		default:
			break;
	}
}



void Window::processMouseClick(int button, int state, int x, int y) {
	switch (button) {
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_DOWN) {
			left_clicked = true;
			x_mouse = x;
			y_mouse = y;
		}
		else
			left_clicked = false;
		break;

	case GLUT_RIGHT_BUTTON:
		if (state == GLUT_DOWN) {
			right_clicked = true;
			x_mouse = x;
			y_mouse = y;
		}
		else
			right_clicked = false;
		break;
	}
}

void Window::processMouseMove(int x, int y) {
	//if (!freeze) {
		if (left_clicked) { // rotate modelview
			if (x != x_mouse || y != y_mouse) {
				Globals::cube.getMatrix().ballRotation(Window::width, Window::height, x_mouse, y_mouse, x, y);
				x_mouse = x;
				y_mouse = y;
			}
		}
		else if (right_clicked) { // zoom modelview
			if (y < y_mouse) {
				Matrix4 scaler;
				scaler.makeScale(1.05, 1.05, 1.05);
				Globals::cube.getMatrix()*scaler;
				y_mouse = y;
			}
			else if (y > y_mouse) {
				Matrix4 cutter;
				cutter.makeScale(.95, .95, .95);
				Globals::cube.getMatrix()*cutter;
				y_mouse = y;
			}
		}
	//}
		/*
	else {
		if (left_clicked) { // rotate light
			if (x != x_mouse || y != y_mouse) {
				if (y < y_mouse)
					theta -= 100 * acos((x*x_mouse + y*y_mouse) / (sqrtf(x*x + y*y) * sqrtf(x_mouse*x_mouse + y_mouse*y_mouse)));
				else if (y > y_mouse)
					theta += 100 * acos((x*x_mouse + y*y_mouse) / (sqrtf(x*x + y*y) * sqrtf(x_mouse*x_mouse + y_mouse*y_mouse)));

				if (theta > 360) theta = 0;
				if (theta < 0) theta = 360;

				x_mouse = x;
				y_mouse = y;
			}
		}
		else if (right_clicked) { // zoom light
			float y_diff = ((float)(y - y_mouse)) / height;

			float y_scale = 1 + y_diff;
			cout << y_scale << "\n";

			d_position[1] *= y_scale;
			d_position[2] *= y_scale;
			shape.directional.setPosition(d_position);

			p_position[0] *= y_scale;
			p_position[1] *= y_scale;
			shape.point.setPosition(p_position);

			s_position[0] *= y_scale;
			s_position[2] *= y_scale;
			shape.spot.setPosition(s_position);

			y_mouse = y;
		}
	}
	*/
}
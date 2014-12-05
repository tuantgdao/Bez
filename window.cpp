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
#include "Light.h"
#include "Material.h"
#include "Bezier.h"

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
static bool freeze = false;
static float theta = 0;
GLfloat d_position[] = { 0, 1, 0, 0 };
GLfloat p_position[] = { 0, -5, 0, 1 };
GLfloat s_position[] = { -10, 0, 0, 1 };
GLfloat s_direction[] = { 1, 0, 0 };
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
	/* Testing their reshape
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
  */

	/* tell OpenGL we want to display in a recangle that is the
	same size as the window
	*/
	glViewport(0, 0, width, height);

	/* switch to the projection matrix */
	glMatrixMode(GL_PROJECTION);

	/* clear the projection matrix */
	glLoadIdentity();

	/* set the camera view, orthographic projection with 4x4 unit square canvas*/
	glOrtho(-2, 2, -2, 2, 2, -2);

	/* switch back to the model view matrix */
	glMatrixMode(GL_MODELVIEW);
}

//----------------------------------------------------------------------------
// Callback method called by GLUT when window readraw is necessary or when glutPostRedisplay() was called.

int pixel = 0;
int light = 0;
int col = 0;
int spot = 0;

//Texture.cpp provided

unsigned char* loadPPM(const char* filename, int& width, int& height)
{
	const int BUFSIZE = 128;
	FILE* fp;
	unsigned int read;
	unsigned char* rawData;
	char buf[3][BUFSIZE];
	char* retval_fgets;
	size_t retval_sscanf;

	if ((fp = fopen(filename, "rb")) == NULL)
	{
		std::cerr << "error reading ppm file, could not locate " << filename << std::endl;
		width = 0;
		height = 0;
		return NULL;
	}

	// Read magic number:
	retval_fgets = fgets(buf[0], BUFSIZE, fp);

	// Read width and height:
	do
	{
		retval_fgets = fgets(buf[0], BUFSIZE, fp);
	} while (buf[0][0] == '#');
	retval_sscanf = sscanf(buf[0], "%s %s", buf[1], buf[2]);
	width = atoi(buf[1]);
	height = atoi(buf[2]);

	// Read maxval:
	do
	{
		retval_fgets = fgets(buf[0], BUFSIZE, fp);
	} while (buf[0][0] == '#');

	// Read image data:
	rawData = new unsigned char[width * height * 3];
	read = fread(rawData, width * height * 3, 1, fp);
	fclose(fp);
	if (read != 1)
	{
		std::cerr << "error parsing ppm file, incomplete data" << std::endl;
		delete[] rawData;
		width = 0;
		height = 0;
		return NULL;
	}

	return rawData;
}

int stop = 0;
GLuint texture[6];
int twidth, theight;
unsigned char* tdata0;
unsigned char* tdata1;
unsigned char* tdata2;
unsigned char* tdata3;
unsigned char* tdata4;
unsigned char* tdata5;

void genTextures()
{
	glGenTextures(1, &texture[0]);
	//glGenTextures(6, &texture[0]);

	tdata0 = loadPPM("PalldioPalace_extern_front.ppm", twidth, theight);
	if (tdata0 == NULL) return;

	//tdata1 = loadPPM("PalldioPalace_extern_right.ppm", twidth, theight);
	if (tdata1 == NULL) return;

	//tdata2 = loadPPM("PalldioPalace_extern_left.ppm", twidth, theight);
	if (tdata2 == NULL) return;

	//tdata3 = loadPPM("PalldioPalace_extern_back.ppm", twidth, theight);
	if (tdata3 == NULL) return;

	//tdata4 = loadPPM("PalldioPalace_extern_top.ppm", twidth, theight);
	if (tdata4 == NULL) return;

	//tdata5 = loadPPM("PalldioPalace_extern_top.ppm", twidth, theight);
	if (tdata5 == NULL) return;
}

int fuckyou = 0;

// load image file into texture object
void loadTextureFront()
{

	// Set this texture to be the one we are working with
	glBindTexture(GL_TEXTURE_2D, texture[0]);

	// Make sure no bytes are padded:
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	// Select GL_MODULATE to mix texture with polygon color for shading:
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	// Use bilinear interpolation:
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Generate the texture
	glTexImage2D(GL_TEXTURE_2D, 0, 3, twidth, theight, 0, GL_RGB, GL_UNSIGNED_BYTE, tdata0);

	// Set bi-linear filtering for both minification and magnification
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void loadTextureRight()
{

	// Set this texture to be the one we are working with
	glBindTexture(GL_TEXTURE_2D, texture[1]);

	// Generate the texture
	glTexImage2D(GL_TEXTURE_2D, 0, 3, twidth, theight, 0, GL_RGB, GL_UNSIGNED_BYTE, tdata1);

	// Set bi-linear filtering for both minification and magnification
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void loadTextureLeft()
{

	// Set this texture to be the one we are working with
	glBindTexture(GL_TEXTURE_2D, texture[2]);

	// Generate the texture
	glTexImage2D(GL_TEXTURE_2D, 0, 3, twidth, theight, 0, GL_RGB, GL_UNSIGNED_BYTE, tdata2);

	// Set bi-linear filtering for both minification and magnification
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void loadTextureBack()
{

	// Set this texture to be the one we are working with
	glBindTexture(GL_TEXTURE_2D, texture[3]);

	// Generate the texture
	glTexImage2D(GL_TEXTURE_2D, 0, 3, twidth, theight, 0, GL_RGB, GL_UNSIGNED_BYTE, tdata3);

	// Set bi-linear filtering for both minification and magnification
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void loadTextureTop()
{

	// Set this texture to be the one we are working with
	glBindTexture(GL_TEXTURE_2D, texture[4]);

	// Generate the texture
	glTexImage2D(GL_TEXTURE_2D, 0, 3, twidth, theight, 0, GL_RGB, GL_UNSIGNED_BYTE, tdata4);

	// Set bi-linear filtering for both minification and magnification
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void loadTextureBottom()
{

	// Set this texture to be the one we are working with
	glBindTexture(GL_TEXTURE_2D, texture[5]);

	// Generate the texture
	glTexImage2D(GL_TEXTURE_2D, 0, 3, twidth, theight, 0, GL_RGB, GL_UNSIGNED_BYTE, tdata5);

	// Set bi-linear filtering for both minification and magnification
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

//End of texture.cpp

void Window::displayCallback()
{
	if (fuckyou == 0)
		genTextures();
	fuckyou = 1;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // clear color and depth buffer

	glMatrixMode(GL_MODELVIEW);  // make sure we're in Modelview mode

	Matrix4 glmatrix;
	glmatrix = Globals::camera.c;
	glmatrix.transpose();
	glLoadMatrixd(glmatrix.getPointer());

	//glBegin(GL_QUADS);

	Bezier my = Bezier(1, 1);

	for (float t1 = -0.5; t1 < 0.5; t1 += 0.01)
	{
		for (float t2 = -0.5; t2 < 0.5; t2 += 0.01)
		{
			//my.tessellate(t1, t2, 0.01);
		}
	}

	//glEnd();

	// set the current drawing color to white to use original texture colors
	glColor3f(1, 1, 1);

	// specify texture coordinates for each vertex
	// note that textures are stored "upside down"

	loadTextureFront();
	glBegin(GL_QUADS);
	//front
	glTexCoord2f(0, 1); glVertex3f(-2, -1, 0);
	glTexCoord2f(1, 1); glVertex3f(2, -1, 0);
	glTexCoord2f(1, 0); glVertex3f(2, 1, 0);
	glTexCoord2f(0, 0); glVertex3f(-2, 1, 0);

	glEnd();

	/*
	//loadTextureRight();
	//glBegin(GL_QUADS);
	//right
	glTexCoord2f(0, 1); glVertex3f(2, -2, -2);
	glTexCoord2f(1, 1); glVertex3f(2, -2, 2);
	glTexCoord2f(1, 0); glVertex3f(2, 2, 2);
	glTexCoord2f(0, 0); glVertex3f(2, 2, -2);

	//glEnd();

	//loadTextureLeft();
	//glBegin(GL_QUADS);
	//left
	glTexCoord2f(0, 1); glVertex3f(-2, -2, 2);
	glTexCoord2f(1, 1); glVertex3f(-2, -2, -2);
	glTexCoord2f(1, 0); glVertex3f(-2, 2, -2);
	glTexCoord2f(0, 0); glVertex3f(-2, 2, 2);

	//glEnd();

	//loadTextureBack();
	//glBegin(GL_QUADS);
	//back
	glTexCoord2f(0, 1); glVertex3f(-2, -2, 2);
	glTexCoord2f(1, 1); glVertex3f(2, -2, 2);
	glTexCoord2f(1, 0); glVertex3f(2, 2, 2);
	glTexCoord2f(0, 0); glVertex3f(-2, 2, 2);

	//glEnd();

	//loadTextureTop();
	//glBegin(GL_QUADS);
	//top
	glTexCoord2f(0, 1); glVertex3f(-2, 2, 2);
	glTexCoord2f(1, 1); glVertex3f(2, 2, 2);
	glTexCoord2f(1, 0); glVertex3f(2, 2, -2);
	glTexCoord2f(0, 0); glVertex3f(-2, 2, -2);

	//glEnd();

	//loadTextureBottom();
	//glBegin(GL_QUADS);
	//bottom
	glTexCoord2f(0, 1); glVertex3f(-2, -2, 2);
	glTexCoord2f(1, 1); glVertex3f(2, -2, 2);
	glTexCoord2f(1, 0); glVertex3f(2, -2, -2);
	glTexCoord2f(0, 0); glVertex3f(-2, -2, -2);

	//glEnd();
	*/

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
	case 49:
		col = 0;
		break;
	case 50:
		col = 1;
		break;
	case 51:
		col = 2;
		break;
	case 113:
		light = 0;
		break;
	case 119:
		light = 1;
		break;
	case 101:
		light = 2;
		break;

		case 66:
			//box = !box;
			break;
		case 98:
			//bound = !bound;
			break;
		case 99:
			//frustum = !frustum;
			break;
		case 112:
			pixel = !pixel;
			break;
		case 120:
			Globals::cube.moveLeftRight(-2);
			break;
		case 88:
			Globals::cube.moveLeftRight(2);
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
			Globals::camera.d.x += .5;
			Globals::camera.c = Globals::camera.getInverseCamera();
			//Globals::cube.rotateY(5);
			break;
		case 82:
			Globals::camera.c = Globals::camera.c * rotateCClock;
			//Globals::cube.rotateY(-5);
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
	if (!freeze) {
		if (left_clicked) { // rotate modelview
			if (x != x_mouse || y != y_mouse) {
				//Globals::cube.getMatrix().ballRotation(Window::width, Window::height, x_mouse, y_mouse, x, y);
				Globals::camera.c.ballRotation(Window::width, Window::height, x_mouse, y_mouse, x, y);
				x_mouse = x;
				y_mouse = y;
			}
		}
		else if (right_clicked) { // zoom modelview
			if (y < y_mouse) {
				Matrix4 scaler;
				scaler.makeScale(1.05, 1.05, 1.05);
				//Globals::cube.getMatrix()*scaler;
				Globals::camera.c*scaler;
				y_mouse = y;
			}
			else if (y > y_mouse) {
				Matrix4 cutter;
				cutter.makeScale(.95, .95, .95);
				//Globals::cube.getMatrix()*cutter;
				Globals::camera.c*cutter;
				y_mouse = y;
			}
		}
	}
		
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

			p_position[0] *= y_scale;
			p_position[1] *= y_scale;

			s_position[0] *= y_scale;
			s_position[2] *= y_scale;

			y_mouse = y;
		}
	}
}
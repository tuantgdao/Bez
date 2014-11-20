#ifdef _WIN32
#include <windows.h>
#endif

#include <iostream>
#include <math.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include "Matrix4.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Camera.h"
#include "cube.h"
#include "Light.h"

#define PI 3.14159265

static int window_width = 512, window_height = 512;
static float* pixels = new float[window_width * window_height * 3];

static float* buffer = new float[window_width * window_height * 3];


using namespace std;

namespace Globals
{
	Cube cube;
	Camera camera;
}

struct Color    // generic color class
{
	float r, g, b;  // red, green, blue
};

float bunnyCoor[36000][6];
float dragonCoor[110000][6];

float bunnyScale = 0;
float bunnyMidX = 0;
float bunnyMidY = 0;
float bunnyMidZ = 0;

float dragonScale = 0;
float dragonMidX = 0;
float dragonMidY = 0;
float dragonMidZ = 0;

int spin = 0;
int scale = 0;
int camColor = 0;
int buffOn = 0;

void loadBunny()
{
	float vert1, vert2, vert3, norm1, norm2, norm3;
	FILE* bunny;
	bunny = fopen("bunny.xyz", "r");

	float maxX = 0;
	float maxY = -1;
	float maxZ = 0;

	float minX = 0;
	float minY = 1;
	float minZ = 0;

		for (int i = 0; i < 36000; i++)
		{
			if (fscanf(bunny, "%f %f %f %f %f %f", &vert1, &vert2, &vert3, &norm1, &norm2, &norm3) == 6)
			{
				bunnyCoor[i][0] = vert1;

				maxX = fmaxf(maxX, bunnyCoor[i][0]);
				minX = fminf(minX, bunnyCoor[i][0]);

				bunnyCoor[i][1] = vert2;
				maxY = fmaxf(maxY, bunnyCoor[i][1]);
				minY = fminf(minY, bunnyCoor[i][1]);

				bunnyCoor[i][2] = vert3;
				maxZ = fmaxf(maxZ, bunnyCoor[i][2]);
				minZ = fminf(minZ, bunnyCoor[i][2]);

				bunnyCoor[i][3] = norm1;
				bunnyCoor[i][4] = norm2;
				bunnyCoor[i][5] = norm3;
			}
		}

	float len = 40 * tan(30 * PI / 180);
	bunnyScale = (len / (maxY - minY));

	bunnyMidX = minX + (maxX - minX) / 2;
	bunnyMidY = minY + (maxY - minY) / 2;
	bunnyMidZ = minZ + (maxZ - minZ) / 2;

	fclose(bunny);
}

void loadDragon()
{
	float vert1, vert2, vert3, norm1, norm2, norm3;
	FILE* dragon;
	dragon = fopen("dragon.xyz", "r");

	float maxX = 0;
	float maxY = -1;
	float maxZ = 0;

	float minX = 0;
	float minY = 1;
	float minZ = 0;

	for (int i = 0; i < 110000; i++)
	{
		if (fscanf(dragon, "%f %f %f %f %f %f", &vert1, &vert2, &vert3, &norm1, &norm2, &norm3) == 6)
		{
			dragonCoor[i][0] = vert1;

			maxX = fmaxf(maxX, dragonCoor[i][0]);
			minX = fminf(minX, dragonCoor[i][0]);

			dragonCoor[i][1] = vert2;
			maxY = fmaxf(maxY, dragonCoor[i][1]);
			minY = fminf(minY, dragonCoor[i][1]);

			dragonCoor[i][2] = vert3;
			maxZ = fmaxf(maxZ, dragonCoor[i][2]);
			minZ = fminf(minZ, dragonCoor[i][2]);

			dragonCoor[i][3] = norm1;
			dragonCoor[i][4] = norm2;
			dragonCoor[i][5] = norm3;
		}
	}

	float len = 40 * tan(30 * PI / 180);
	dragonScale = (len / (maxY - minY));

	dragonMidX = minX + (maxX - minX) / 2;
	dragonMidY = minY + (maxY - minY) / 2;
	dragonMidZ = minZ + (maxZ - minZ) / 2;

	fclose(dragon);
}

	// This data structure defines a simple house

	int nVerts = 42;    // your vertex array needs to have this many entries

	// These are the x,y,z coordinates of the vertices of the triangles
	float vertices[] = {
		-4, -4, 4, 4, -4, 4, 4, 4, 4, -4, 4, 4,     // front face
		-4, -4, -4, -4, -4, 4, -4, 4, 4, -4, 4, -4, // left face
		4, -4, -4, -4, -4, -4, -4, 4, -4, 4, 4, -4,  // back face
		4, -4, 4, 4, -4, -4, 4, 4, -4, 4, 4, 4,     // right face
		4, 4, 4, 4, 4, -4, -4, 4, -4, -4, 4, 4,     // top face
		-4, -4, 4, -4, -4, -4, 4, -4, -4, 4, -4, 4, // bottom face

		-20, -4, 20, 20, -4, 20, 20, -4, -20, -20, -4, -20, // grass
		-4, 4, 4, 4, 4, 4, 0, 8, 4,                       // front attic wall
		4, 4, 4, 4, 4, -4, 0, 8, -4, 0, 8, 4,               // left slope
		-4, 4, 4, 0, 8, 4, 0, 8, -4, -4, 4, -4,             // right slope
		4, 4, -4, -4, 4, -4, 0, 8, -4 };                   // rear attic wall

	// These are the RGB colors corresponding to the vertices, in the same order
	float colors[] = {
		1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0,  // front is red
		0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0,  // left is green
		1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0,  // back is red
		0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0,  // right is green
		0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1,  // top is blue
		0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1,  // bottom is blue

		0, 0.5, 0, 0, 0.5, 0, 0, 0.5, 0, 0, 0.5, 0, // grass is dark green
		0, 0, 1, 0, 0, 1, 0, 0, 1,                // front attic wall is blue
		1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0,         // left slope is green
		0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0,         // right slope is red
		0, 0, 1, 0, 0, 1, 0, 0, 1, };              // rear attic wall is red

	// The index data stores the connectivity of the triangles; 
	// index 0 refers to the first triangle defined above
	int indices[] = {
		0, 2, 3, 0, 1, 2,      // front face
		4, 6, 7, 4, 5, 6,      // left face
		8, 10, 11, 8, 9, 10,     // back face
		12, 14, 15, 12, 13, 14,   // right face
		16, 18, 19, 16, 17, 18,   // top face
		20, 22, 23, 20, 21, 22,   // bottom face

		24, 26, 27, 24, 25, 26,   // grass
		28, 29, 30,             // front attic wall
		31, 33, 34, 31, 32, 33,   // left slope
		35, 37, 38, 35, 36, 37,   // right slope
		39, 40, 41 };            // rear attic wall


void loadData()
{
	// point cloud parser goes here
	loadBunny();
	loadDragon();
}

// Clear frame buffer
void clearBuffer()
{
	Color clearColor = { 0.0, 0.0, 0.0 };   // clear color: black
	for (int i = 0; i<window_width*window_height; ++i)
	{
		pixels[i * 3] = clearColor.r;
		pixels[i * 3 + 1] = clearColor.g;
		pixels[i * 3 + 2] = clearColor.b;
	}
	Vector3 maxBuffer = Vector3(1.0, 1.0, 1.0);
	for (int i = 0; i<window_width*window_height; ++i)
	{
		buffer[i * 3] = maxBuffer.x;
		buffer[i * 3 + 1] = maxBuffer.y;
		buffer[i * 3 + 2] = maxBuffer.z;
	}
}

int vary = 0;

// Draw a point into the frame buffer
void drawPoint(int x, int y, float r, float g, float b, float size)
{
	if (0 <= x && x < window_width)
	{
		if (0 <= y  && y < window_height)
		{
			int offset = y*window_width * 3 + x * 3;

			if (vary == 0)
			{
				//cout << "vary = 0" << endl;
				pixels[offset] = r;
				pixels[offset + 1] = g;
				pixels[offset + 2] = b;
			}
			else
			{
				if (size < 1)
				{
					pixels[offset] = r;
					pixels[offset + 1] = g;
					pixels[offset + 2] = b;
				}
				//cout << "is this printing?" << endl;
				else
				{
					pixels[offset - 3 + window_width * 3] = r;
					pixels[offset - 2 + window_width * 3] = g;
					pixels[offset - 1 + window_width * 3] = b;

					pixels[offset - 3 - window_width * 3] = r;
					pixels[offset - 2 - window_width * 3] = g;
					pixels[offset - 1 - window_width * 3] = b;

					pixels[offset - 3] = r;
					pixels[offset - 2] = g;
					pixels[offset - 1] = b;

					pixels[offset] = r;
					pixels[offset + 1] = g;
					pixels[offset + 2] = b;

					pixels[offset + 3] = r;
					pixels[offset + 4] = g;
					pixels[offset + 5] = b;

					pixels[offset + 3 + window_width * 3] = r;
					pixels[offset + 4 + window_width * 3] = g;
					pixels[offset + 5 + window_width * 3] = b;

					pixels[offset + 3 - window_width * 3] = r;
					pixels[offset + 4 - window_width * 3] = g;
					pixels[offset + 5 - window_width * 3] = b;

					pixels[offset - window_width * 3] = r;
					pixels[offset + 1 - window_width * 3] = g;
					pixels[offset + 2 - window_width * 3] = b;

					pixels[offset + window_width * 3] = r;
					pixels[offset + 1 + window_width * 3] = g;
					pixels[offset + 2 + window_width * 3] = b;
				}
			}
		}
	}
}

Matrix4 cameraMatrix;

float angle = 0;
int stop = 0;

void spinAngle(float degree)
{
	if (stop == 0)
		angle = angle + degree;
	else
		angle = angle;
	if (angle > 360.0 || angle < -360.0) angle = 0.0;
}

float scaled = 1;

void scaledDegree(float degree)
{
	scaled = scaled*degree;
}

int scaling = 0;

int choice = 1;

float size( double distance)
{
	float idk = distance / .9;
	return idk;
}

void rasterize()
{
	// Camera Matrix
	cameraMatrix = Globals::camera.getGLMatrix1();

	// Projection Matrix
	float aspect = window_width / window_height;
	float fov = 0.57735026919;
	float near1 = 1.0;
	float far1 = 1000.0;

	Matrix4 proMatrix;
	proMatrix.m[0][0] = 1 / (aspect*fov);
	proMatrix.m[0][1] = 0;
	proMatrix.m[0][2] = 0;
	proMatrix.m[0][3] = 0;

	proMatrix.m[1][0] = 0;
	proMatrix.m[1][1] = 1 / (fov);
	proMatrix.m[1][2] = 0;
	proMatrix.m[1][3] = 0;

	proMatrix.m[2][0] = 0;
	proMatrix.m[2][1] = 0;
	proMatrix.m[2][2] = (near1 + far1) / (near1 - far1);
	proMatrix.m[2][3] = (2 * near1*far1) / (near1 - far1);

	proMatrix.m[3][0] = 0;
	proMatrix.m[3][1] = 0;
	proMatrix.m[3][2] = -1;
	proMatrix.m[3][3] = 0;

	// Viewport Matrix
	float x0 = 0;
	float x1 = window_width;
	float y0 = 0;
	float y1 = window_height;

	Matrix4 viewPort;
	viewPort.m[0][0] = (x1 - x0) / 2;
	viewPort.m[0][1] = 0;
	viewPort.m[0][2] = 0;
	viewPort.m[0][3] = (x1 + x0) / 2;

	viewPort.m[1][0] = 0;
	viewPort.m[1][1] = (y1 - y0) / 2;
	viewPort.m[1][2] = 0;
	viewPort.m[1][3] = (y1 + y0) / 2;

	viewPort.m[2][0] = 0;
	viewPort.m[2][1] = 0;
	viewPort.m[2][2] = 0.5;
	viewPort.m[2][3] = 0.5;

	viewPort.m[3][0] = 0;
	viewPort.m[3][1] = 0;
	viewPort.m[3][2] = 0;
	viewPort.m[3][3] = 1;

	Matrix4 dragTranslate;
	dragTranslate.makeTranslate(-dragonMidX, -dragonMidY, -dragonMidZ);

	Matrix4 dragScale;
	dragScale.makeScale(dragonScale, dragonScale, dragonScale);

	Matrix4 bunnyTranslate;
	bunnyTranslate.makeTranslate(-bunnyMidX, -bunnyMidY, -bunnyMidZ);

	Matrix4 bunnyScaling;
	bunnyScaling.makeScale(bunnyScale, bunnyScale, bunnyScale);

	// Start of Lighting
	Light myLight = Light();
	myLight.position.z = 0;
	myLight.position.y = 100;
	myLight.position.x = 0;
	myLight.color.x = 0;
	myLight.color.z = 1.0;
	myLight.color.y = 1.0;
	// End of Lighting

	float myRed = 1.0;
	float myGreen = 1.0;
	float myBlue = 1.0;

	// Put your main rasterization loop here
	// It should go over the point model and call drawPoint for every point in it

	/*
	for (int i = 0; i <= 59; i++)
	{
		int index = indices[i];
		float color = colors[index * 3];
		float color2 = colors[(index * 3) + 1 ];
		float color3 = colors[(index * 3) + 2 ];

		float vertex = vertices[index * 3];
		float vertex2 = vertices[(index * 3) + 1];
		float vertex3 = vertices[(index * 3) + 2];

		Vector4 my = Vector4(vertex, vertex2, vertex3, 1);

	*/

	if (choice == 1)
	{
		for (int i = 0; i < 36000; i++)
		{
			Vector4 bunny = Vector4(bunnyCoor[i][0], bunnyCoor[i][1], bunnyCoor[i][2], 1);
			Vector3 normBunny = Vector3(bunnyCoor[i][3], bunnyCoor[i][4], bunnyCoor[i][5]);

			Vector3 vertBunny = Vector3(bunny.x, bunny.y, bunny.z);
			Vector3 lightDistance = myLight.position - vertBunny;
			lightDistance.normalize();
			normBunny.normalize();

			double dot = vertBunny.dot(lightDistance, normBunny);
			double distance = lightDistance.length();

			double brightness = dot * myLight.brightness;
			double denom = distance*distance*PI;

			double cF = brightness / denom;

			if (camColor == 1)
			{
				myLight.color.x = 1.0;
				myLight.color.y = 1.0;
				myLight.color.z = 1.0;
			}
			if (camColor == 2)
			{
				myLight.color.x = 0.0;
				myLight.color.y = 1.0;
				myLight.color.z = 1.0;
			}

			double red = myLight.color.x;
			double green = myLight.color.y;
			double blue = myLight.color.z;

			double newRed = cF*red;
			double newGreen = cF*green;
			double newBlue = cF*blue;

			Vector3 colorFactor = Vector3(newRed,newGreen,newBlue);

			bunny = bunnyTranslate*bunny;
			bunny = bunnyScaling*bunny;
			if (spin == 1)
			{
				spinAngle(-.0001);
				Matrix4 spinMatrix;
				spinMatrix.makeRotateY(angle);
				bunny = spinMatrix*bunny;
			}
			else
			{
				spinAngle(.0001);
				Matrix4 spinMatrix;
				spinMatrix.makeRotateY(angle);
				bunny = spinMatrix*bunny;
			}
			Matrix4 scaleMatrix;
			scaleMatrix.makeScale(scaled, scaled, scaled);
			if (scaling == 1)
			{
				scaledDegree(1.1);
				scaling = 0;
			}
			else if (scaling == 2)
			{
				scaledDegree(0.9);
				scaling = 0;
			}

			bunny = scaleMatrix* bunny;
			bunny = cameraMatrix*bunny;
			bunny = proMatrix*bunny;
			bunny.dehomogenize();
			bunny = viewPort*bunny;


			int bufferZ = (int)(bunny.y) * 3 * window_width + (int)(bunny).x*3;

			if (buffOn == 0)
			{
				drawPoint(bunny.x, bunny.y, newRed, newGreen, newBlue, size(bunny.z));
			}
			else if (buffOn == 1)
			{
				if (bunny.z < buffer[bufferZ])
				{
					buffer[bufferZ] = bunny.z;
					drawPoint(bunny.x, bunny.y, newRed, newGreen, newBlue, size(bunny.z));
				}
			}
		}
	}

	if (choice == 2)
	{
		for (int i = 0; i < 100500; i++)
		{
			Vector4 dragon = Vector4(dragonCoor[i][0], dragonCoor[i][1], dragonCoor[i][2], 1);

			Vector3 normDragon = Vector3(dragonCoor[i][3], dragonCoor[i][4], dragonCoor[i][5]);

			Vector3 vertDragon = Vector3(dragon.x, dragon.y, dragon.z);
			Vector3 lightDistance = myLight.position - vertDragon;
			lightDistance.normalize();
			normDragon.normalize();

			double dot = vertDragon.dot(lightDistance, normDragon);
			double distance = lightDistance.length();

			double brightness = dot * myLight.brightness;
			double denom = distance*distance*PI;

			double cF = brightness / denom;

			if (camColor == 1)
			{
				myLight.color.x = 1.0;
				myLight.color.y = 1.0;
				myLight.color.z = 1.0;
			}
			if (camColor == 2)
			{
				myLight.color.x = 0.0;
				myLight.color.y = 1.0;
				myLight.color.z = 1.0;
			}

			double red = myLight.color.x;
			double green = myLight.color.y;
			double blue = myLight.color.z;

			double newRed = cF*red;
			double newGreen = cF*green;
			double newBlue = cF*blue;

			Vector3 colorFactor = Vector3(newRed, newGreen, newBlue);

			dragon = dragTranslate*dragon;
			dragon = dragScale*dragon;
			if (spin == 1)
			{
				spinAngle(-.0001);
				Matrix4 spinMatrix;
				spinMatrix.makeRotateY(angle);
				dragon = spinMatrix* dragon;
			}
			else
			{
				spinAngle(.0001);
				Matrix4 spinMatrix;
				spinMatrix.makeRotateY(angle);
				dragon = spinMatrix* dragon;
			}
			Matrix4 scaleMatrix;
			scaleMatrix.makeScale(scaled, scaled, scaled);
			if (scaling == 1)
			{
				scaledDegree(1.1);
				scaling = 0;
			}
			if (scaling == 2)
			{
				scaledDegree(0.9);
				scaling = 0;
			}
			dragon = scaleMatrix* dragon;
			dragon = cameraMatrix*dragon;
			dragon = proMatrix*dragon;
			dragon.dehomogenize();
			dragon = viewPort*dragon;

			//cout << "CAm color in Rasterizer: " << camColor << endl;

			int bufferZ = (int)(dragon.y) * 3 * window_width + (int)(dragon).x * 3;

			if (buffOn == 0)
			{
				drawPoint(dragon.x, dragon.y, newRed, newGreen, newBlue, size(dragon.z));
			}
			else if (buffOn == 1)
			{
				if (dragon.z < buffer[bufferZ])
				{
					buffer[bufferZ] = dragon.z;
					buffer[bufferZ + 1] = dragon.z;
					buffer[bufferZ + 2] = dragon.z;
					drawPoint(dragon.x, dragon.y, newRed, newGreen, newBlue, size(dragon.z));
				}
			}
		}
	}
}

void displayCallback()
{
	clearBuffer();
	rasterize();

	// glDrawPixels writes a block of pixels to the framebuffer
	glDrawPixels(window_width, window_height, GL_RGB, GL_FLOAT, pixels);

	glutSwapBuffers();
}

// Called whenever the window size changes
void reshapeCallback(int new_width, int new_height)
{
	window_width = new_width;
	window_height = new_height;

	delete[] pixels;
	pixels = new float[window_width * window_height * 3];
	displayCallback();
}

void keyboardCallback(unsigned char key, int, int)
{
	cerr << "Key pressed: " << key << endl;
	switch (key)
	{
		case 'r':
			if (spin == 0)
				spin = 1;
			else if (spin == 1)
				spin = 0;
			break;
		case 'R':
			if (stop == 0)
			{
				stop = 1;
				break;
			}
			else if (stop == 1)
			{
				stop = 0;
				break;
			}
			break;
		case 's':
			scaling = 1;
			break;
		case 'S':
			scaling = 2;
			break;
		case '1':
			camColor = 1;
			//cout << "camColor: " << camColor << endl;
			break;
		case '2':
			camColor = 2;
			//cout << "camColor: " << camColor << endl;
			break;
		case '3':
			if (buffOn == 0)
			{
				buffOn = 1;
				break;
			}
			else
			{
				buffOn = 0;
				break;
			}
		case '4':
			if (vary == 0)
			{
				vary = 1;
				break;
			}
			else
			{
				vary = 0;
				break;
			}
		default:
			break;
	}
}

void specialFuncCallback(int key, int, int)
{
	cerr << "Key pressed: " << key << endl;
	switch (key)
	{
	case GLUT_KEY_F1:
		choice = 1;
		break;
	case GLUT_KEY_F2:
		choice = 2;
		break;
	default:
		break;
	}
}

void idleCallback()
{
	displayCallback();
}

int main(int argc, char** argv) {

	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(window_width, window_height);
	glutCreateWindow("Rasterizer");

	//Enable Lighting
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	GLfloat lightpos[] = { .5, 1, 1, 0 };
	glLightfv(GL_LIGHT0, GL_POSITION, lightpos);
	GLfloat cyan[] = { 0.f, .8f, .8f, 1.f };
	glMaterialfv(GL_FRONT, GL_DIFFUSE, cyan);
	gluLookAt(0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	//End of lighting

	loadData();

	glutReshapeFunc(reshapeCallback);
	glutDisplayFunc(displayCallback);
	glutKeyboardFunc(keyboardCallback);
	glutSpecialFunc(specialFuncCallback);
	glutIdleFunc(idleCallback);
	glutMainLoop();
}
#ifndef _WINDOW_H_
#define _WINDOW_H_

#include "Matrix4.h"

class Window	  // OpenGL output window related routines
{
  public:
    static int width, height; 	            // window size

    static void idleCallback(void);
    static void reshapeCallback(int, int);
    static void displayCallback(void);

	//What i'm adding
	Matrix4 modelView;
	Matrix4 camera;
	Matrix4 model;

	Matrix4& getCameraMatrix();
	Matrix4& getModelMatrix();
	Matrix4& getModelViewMatrix();
	
	Matrix4 setScaleMatrix(float factor);

	void updateModelViewMatrix();

	Window::Window();

	static void keyboardCallback(unsigned char key, int x, int y);
	static void specialFuncCallback(int key, int x, int y);
	static void loadBunny();
	static void loadBunny2();
	static void loadDragon();
	static void loadDragon2();
	static void loadBear();
	//
	void processMouseClick(int, int, int, int);
	void processMouseMove(int, int);
	//End of what i'm adding
};

#endif


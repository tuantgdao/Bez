#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "GLee.h"
#include <GL/glut.h>

#include "Window.h"
#include "Cube.h"
#include "Matrix4.h"
#include "main.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Camera.h"
#include "shader.h"
#include "Bezier.h"

using namespace std;

namespace Globals
{
  Cube cube;
  Camera camera(Vector3(-20,20,-10), Vector3(0,0,0), Vector3(0,1,0) );
  int renderInt;
  int frustum;
  int bound;
  int box;
};

int main(int argc, char *argv[])
{ 
  glutInit(&argc, argv);      	      	      // initialize GLUT
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);   // open an OpenGL context with double buffering, RGB colors, and depth buffering
  glutInitWindowSize(Window::width, Window::height);      // set initial window size
  glutCreateWindow("OpenGL Cube");    	      // open window and set window title

  glEnable(GL_DEPTH_TEST);            	      // enable depth buffering
  glClear(GL_DEPTH_BUFFER_BIT);       	      // clear depth buffer
  //glClearColor(0.0, 0.0, 0.0, 0.0);   	      // set clear color to black
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);  // set polygon drawing mode to fill front and back of each polygon
  glDisable(GL_CULL_FACE);     // disable backface culling to render both sides of polygons
  //glShadeModel(GL_SMOOTH);             	      // set shading to smooth
  glMatrixMode(GL_PROJECTION);
  glEnable(GL_NORMALIZE);
  glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
  
  //Window::loadBunny2();
  //Window::loadDragon2();
  //Window::loadBear();
  
  // Install callback functions:
  glutDisplayFunc(Window::displayCallback);
  glutReshapeFunc(Window::reshapeCallback);
  glutIdleFunc(Window::idleCallback);

  //What i'm adding
  glutKeyboardFunc(Window::keyboardCallback);
  glutSpecialFunc(Window::specialFuncCallback);
  //
  glutMouseFunc(Window::processMouseClick);
  glutMotionFunc(Window::processMouseMove);
  //End of what i'm adding
    
  // Initialize cube matrix:
  Globals::cube.getMatrix().identity();
    
  glutMainLoop();
  return 0;
}


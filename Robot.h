#ifndef _ROBOT_H_
#define _ROBOT_H_

#include "Group.h"
#include "Cube4.h"
#include "Cube5.h"
#include "Sphere.h"
#include "wireSphere.h"
#include "MatrixTransform.h"
#include "GL\glut.h"

class Robot
{
	public:
		float rotateAngle = 0;
		float translateX = 0;
		float translateZ = 0;
		int boundingSphere = 0;
		int frustum = 0;

		MatrixTransform root;
		void draw(Matrix4 C);
		Robot::Robot(float, float,float, int, int);
};

#endif _ROBOT_H_
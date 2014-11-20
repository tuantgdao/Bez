#ifndef _PLANE_H_
#define _PLANE_H_

#include "Vector3.h"

class Plane
{
	public:
		Vector3 normal;
		Vector3 point;
		float d;

		void setPoints(Vector3, Vector3, Vector3);
		float distance(Vector3);
};

#endif
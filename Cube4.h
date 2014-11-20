#ifndef _CUBE4_H_
#define _CUBE4_H_

#include "Geode.h"
#include <math.h>

class Cube4 : public Geode
{
	public:
		Vector4 center = Vector4(0,0,0,1);
		float distance = sqrt(3);
		Vector4 d = Vector4(distance, distance, distance, 1);
		Vector4 sample = center + d;
		void render( Matrix4);
};

#endif
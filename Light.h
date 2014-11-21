#ifndef _LIGHT_H_
#define _LIGHT_H_

#include "Vector3.h"
#include "main.h"
#include "window.h"

class Light
{
	public:
		float diffuse[4];
		float position[4];
		float spot_direction[3];
		Light(int, Vector4, Vector4);
		Light(int, Vector4, Vector4, Vector3);
};

#endif
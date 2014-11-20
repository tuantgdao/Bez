#ifndef _LIGHT_H_
#define _LIGHT_H_

#include "Vector3.h"

class Light
{
	public:
		Vector3 position;
		Vector3 color;
		float brightness;

		Light();
};

#endif
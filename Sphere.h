#ifndef _SPHERE_H_
#define _SPHERE_H_

#include "Geode.h"

class Sphere : public Geode
{
	public:
		void render(Matrix4);
};
#endif
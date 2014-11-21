#ifndef _MATERIAL_H_
#define _MATERIAL_H_

#include "main.h"
#include "window.h"

class Material
{
	public:
		float specular[4];
		float ambient[4];
		float diffuse[4];
		float shininess[1];

		Material(Vector4, Vector4, Vector4, float);
};
#endif
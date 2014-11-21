#include "Material.h"

Material::Material(Vector4 spec, Vector4 amb, Vector4 diff, float shine)
{

	specular[0] = spec.x;
	specular[1] = spec.y;
	specular[2] = spec.z;
	specular[3] = spec.w;

	ambient[0] = amb.x;
	ambient[1] = amb.y;
	ambient[2] = amb.z;
	ambient[3] = amb.w;

	diffuse[0] = diff.x;
	diffuse[1] = diff.y;
	diffuse[2] = diff.z;
	diffuse[3] = diff.w;

	shininess[0] = shine;

	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_SMOOTH);

	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);

	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);
}
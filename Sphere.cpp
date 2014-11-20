#include "Sphere.h"

void Sphere::render(Matrix4 m)
{
	extern int box;

	glColor3f(1, 0, 0);
	m.transpose();
	glLoadMatrixd(m.getPointer());
	glutSolidSphere(1,20,20);

	if (box == 1)
	{
		glColor3f(1, 1, 1);
		glutWireSphere(1, 20, 20);
	}

}
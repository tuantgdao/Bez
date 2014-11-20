#include "Cube4.h"

void Cube4::render(Matrix4 m)
{
	extern int box;

	glColor3f(0, 0, 1);
	m.transpose();
	glLoadMatrixd(m.getPointer());
	glutSolidCube(1);

	if (box == 1)
	{
		glColor3f(1, 1, 1);
		glutWireSphere(1, 20, 20);
	}	
}
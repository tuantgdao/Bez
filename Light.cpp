#include "Light.h"

Light::Light(int lightNum, Vector4 diff, Vector4 pos)
{
	diffuse[0] = diff.x;
	diffuse[1] = diff.y;
	diffuse[2] = diff.z;
	diffuse[3] = diff.w;

	position[0] = pos.x;
	position[1] = pos.y;
	position[2] = pos.z;
	position[3] = pos.w;

	Matrix4 i;
	i.identity();

	Matrix4 t;
	t.makeTranslate(position[0], position[1], position[2]);
	i = i*t;
	i.transpose();
	glLoadMatrixd(i.getPointer());

	glColor3f(1, 1, 1);
	glutSolidSphere(.3, 20, 20);

	switch (lightNum)
	{
		case 0:
		{
			glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
			glLightfv(GL_LIGHT0, GL_POSITION, position);
			glEnable(GL_LIGHT0);
			break;
		}
		default:
			break;
	}
}

Light::Light(int lightNum, Vector4 diff, Vector4 pos, Vector3 spot_dir)
{

	diffuse[0] = diff.x;
	diffuse[1] = diff.y;
	diffuse[2] = diff.z;
	diffuse[3] = diff.w;

	position[0] = pos.x;
	position[1] = pos.y;
	position[2] = pos.z;
	position[3] = pos.w;

	spot_direction[0] = spot_dir.x;
	spot_direction[1] = spot_dir.y;
	spot_direction[1] = spot_dir.z;

	Matrix4 a;
	a.identity();
	glLoadMatrixd(a.getPointer());

	switch (lightNum)
	{
		case 0:
			break;
		case 1:
			glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse);
			glLightfv(GL_LIGHT1, GL_POSITION, position);
			glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spot_direction);
			glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 10);
			//glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 2.0);
			glEnable(GL_LIGHT1);
		default:
			break;
	}

	
	Matrix4 t2;
	t2.makeTranslate(position[0], position[1], position[2] + 1);
	a = a*t2;
	a.transpose();
	glLoadMatrixd(a.getPointer());

	glColor3f(1, 1, 1);
	glutSolidCone(.1, 0.1, 20, 20);
	
}
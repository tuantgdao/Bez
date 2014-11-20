#include "wireSphere.h"

void wireSphere::render(Matrix4 m)
{
	glColor3f(1, 0, 1);
	Vector3 center = Vector3(m.m[0][3], m.m[1][3], m.m[2][3]);
	//m.print("");
	//m.printInfo();
	//cout << "Center.x: " << center.x << " Center.y: " << center.y << " Center.z " << center.z << endl;
	m.transpose();
	//Vector3 center = Vector3(m.m[0][3], m.m[1][3], m.m[2][3]);
	extern int renderInt;
	extern int frustum;
	extern int bound;
	//m.print("");
	//glLoadMatrixd(m.getPointer());

	//Vector3 center = Vector3(m.m[3][0], m.m[3][1], m.m[3][2]);
	//cout << "Center.x: " << center.x << " Center.y: " << center.y << " Center.z " << center.z << endl;

	//Frustrum
	if (frustum == 1)
	{
		glLoadMatrixd(m.getPointer());
		if (Globals::camera.sphereInFrustum(center, 200) != Globals::camera.OUTSIDE)
		{
			renderInt = 1;
			if (bound == 1)
			{
				glutWireSphere(10, 20, 20);
			}
		}

		glBegin(GL_QUADS);
		glColor3f(1, 1, 1);
		glNormal3f(0.0, -1.0, 0.0);
		glVertex3f(-10.0, -10.0, -10.0);
		glVertex3f(10.0, -10.0, -10.0);
		glVertex3f(10.0, -10.0, 10.0);
		glVertex3f(-10.0, -10.0, 10.0);
		glEnd();
	}

	//No Frustum
	else
	{
		glLoadMatrixd(m.getPointer());
		if (bound == 1)
		{
			glutWireSphere(10, 20, 20);
			//glutSolidCube(10);
		}

		glBegin(GL_QUADS);
		glColor3f(1, 1, 1);
		glNormal3f(0.0, -1.0, 0.0);
		glVertex3f(-10.0, -10.0, -10.0);
		glVertex3f(10.0, -10.0, -10.0);
		glVertex3f(10.0, -10.0, 10.0);
		glVertex3f(-10.0, -10.0, 10.0);
		glEnd();
	}
}
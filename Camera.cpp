#include "Camera.h"
#include <math.h>
#include <stdio.h>

Camera::Camera(Vector3 ie, Vector3 id, Vector3 iup)
{
	e = ie;
	d = id;
	up = iup;
	c = getInverseCamera();
}

Camera::Camera()
{
	e = Vector3(0, 0, 0);
	d = Vector3(0, 0, 0);
	up = Vector3(0, 0, 0);
	c = getInverseCamera();
}

Matrix4& Camera::getInverseCamera()
{
	Vector3 z = (e - d);
	z.normalize();
	Vector3 x = up.cross(z);
	x.normalize();
	Vector3 y = z.cross(x);

	c.m[0][0] = x.x;
	c.m[0][1] = y.x;
	c.m[0][2] = z.x;
	c.m[0][3] = 0;

	c.m[1][0] = x.y;
	c.m[1][1] = y.y;
	c.m[1][2] = z.y;
	c.m[1][3] = 0;

	c.m[2][0] = x.z;
	c.m[2][1] = y.z;
	c.m[2][2] = z.z;
	c.m[2][3] = 0;

	c.m[3][0] = 0;
	c.m[3][1] = 0;
	c.m[3][2] = 0;
	c.m[3][3] = 1;

	c.transpose();

	Matrix4 inverse;

	inverse.m[0][0] = 1;
	inverse.m[0][1] = 0;
	inverse.m[0][2] = 0;
	inverse.m[0][3] = -e.x;

	inverse.m[1][0] = 0;
	inverse.m[1][1] = 1;
	inverse.m[1][2] = 0;
	inverse.m[1][3] = -e.y;

	inverse.m[2][0] = 0;
	inverse.m[2][1] = 0;
	inverse.m[2][2] = 1;
	inverse.m[2][3] = -e.z;

	inverse.m[3][0] = 0;
	inverse.m[3][1] = 0;
	inverse.m[3][2] = 0;
	inverse.m[3][3] = 1;

	Matrix4 translate;
	translate.makeTranslate(0,0, -20);

	c = c*inverse;

	return c;
}

void Camera::setPlane()
{
	Vector3 direction, nearCenter, farCenter;

	Vector3 z = (e - d);
	z.normalize();
	Vector3 x = up.cross(z);
	x.normalize();
	Vector3 y = z.cross(x);

	//cout << "z.x: " << z.x << " z.y " << z.y << " z.z" << z.z << endl;

	Vector3 znD = z*nearDistance;
	Vector3 zfD = z*farDistance;

	nearCenter = e - znD;
	farCenter = e - zfD;

	//cout << "fc.x: " << farCenter.x << " fc.y " << farCenter.y << " fc.z" << farCenter.z << endl;

	Vector3 ynH = y*nearHeight;
	Vector3 yfH = y*farHeight;
	Vector3 xnW = x*nearWidth;
	Vector3 xfW = x*farWidth;

	//cout << "yfH.x: " << yfH.x << " yfH.y " << yfH.y << " yfH.z" << yfH.z << endl;
	//cout << "xfW.x: " << xfW.x << " xfW.y " << xfW.y << " xfW.z" << xfW.z << endl;

	Vector3 nhws = ynH - xnW;
	Vector3 nhwa = ynH + xnW;

	ntl = nearCenter + nhws;
	ntr = nearCenter + nhwa;
	nbl = nearCenter - nhwa;
	nbr = nearCenter - nhws;

	//cout << "fbr.x: " << fbr.x << " fbr.y " << fbr.y << " nfbr.z" << fbr.z << endl;

	Vector3 fhws = yfH - xfW;
	Vector3 fhwa = yfH + xfW;

	//cout << "\nfhwa.x: " << fhwa.x << " \nfhwa.y " << fhwa.y << " \nfhwa.z " << fhwa.z << endl;

	ftl = farCenter + fhws;
	ftr = farCenter + fhwa;
	fbl = farCenter - fhwa;
	fbr = farCenter - fhws;

	//cout << "\nfbl.x: " << fbl.x << " \nfbl.y " << fbl.y << " \nnfbl.z" << fbl.z << endl;

	p1[TOP].setPoints(ntr, ntl, ftl);
	p1[BOTTOM].setPoints(nbl, nbr, fbr);
	p1[LEFT].setPoints(ntl, nbl, fbl);
	p1[RIGHT].setPoints(nbr, ntr, fbr);
	p1[NEARP].setPoints(ntl, ntr, nbr);
	p1[FARP].setPoints(ftr, ftl, fbl);
}

void Camera::setFrustum(float a, float r, float nD, float fD)
{
	angle = a;
	ratio = r;
	nearDistance = nD;
	farDistance = fD;

	//cout << "angle: " << angle << endl;
	//cout << "ratio: " << ratio << endl;
	//cout << "nearDistance: " << nearDistance << endl;
	//cout << "farDistance: " << farDistance << endl;

	tang = (float)(tan(radians*angle*0.5));
	nearHeight = nearDistance * tang;
	nearWidth = nearHeight * ratio;
	farHeight = farDistance * tang;
	farWidth = farHeight * ratio;

	//cout << "Tang: " << tang << endl;
	//cout << "nh: " << nearHeight << endl;
	//cout << "nw: " << nearWidth << endl;
	//cout << "fh: " << farHeight << endl;
	//cout << "fw: " << farWidth << endl;
}

void Camera::setCamera(Vector3 ie, Vector3 id, Vector3 iup)
{
	e = ie;
	d = id;
	up = iup;
	c = getInverseCamera();
}

int Camera::sphereInFrustum(Vector3 center, float radius)
{
	float distance;
	int result = INSIDE;

	//cout << "Center.x: " << center.x << " Center.y: " << center.y << " Center.z: " << center.z << endl;

	for (int i = 0; i < 6; i++)
	{
		distance = p1[i].distance(center);
		//cout << "Distance: " << distance << endl;
		if (distance < -radius)
			return OUTSIDE;
		else if (distance < radius)
			result = INTERSECT;
	}
	return(result);
}
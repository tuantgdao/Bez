#include "Plane.h"

void Plane::setPoints(Vector3 v1, Vector3 v2, Vector3 v3)
{
	Vector3 vector1 = v1 - v2;
	Vector3 vector2 = v3 - v2;

	normal = vector2.cross(vector1);
	normal.normalize();

	point = v2;

	double x = normal.dot(normal, point);
	//cout << "Point.x: " << point.x << " Point.y: " << point.y << " Point.z " << point.z << endl;;
	//cout << "X: " << x << endl;
	d = -(normal.dot(normal,point));

	//double x = normal.dot(normal, point);
	//cout << "D: " << d << endl;
}

float Plane::distance(Vector3 point)
{
	//cout << "D: " << d << endl;
	double x = normal.dot(normal, point);
	//cout << "Point.x: " << point.x << " Point.y: " << point.y << " Point.z " << point.z << endl;;
	//cout << "normal dot point: " << x << endl;
	return (d + x);
}
#include <math.h>
#include "Matrix4.h"

Matrix4::Matrix4()
{
  for (int i=0; i<4; ++i)
  {
    for (int j=0; j<4; ++j)
    {
      m[i][j] = 0;
    }
  }
}

//Method im adding to test other methods
void Matrix4::changeValue(int i, int j, double value)
{
	m[i][j] = value;
}

void Matrix4::allChange(int a)
{
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			m[i][j] = a;
}
//End of my method

Matrix4& Matrix4::operator=(const Matrix4& m2)
{
  if (this != &m2)
  {
    for (int i=0; i<4; ++i)
    {
      for (int j=0; j<4; ++j)
      {
        m[i][j] = m2.m[i][j];
      }
    }
  }
  return *this;
}

Matrix4& Matrix4::operator*(const Matrix4& m2)
{
	double x = 0;
	double fuckyou[4][4];
	for (int k = 0; k < 4; k++)
	{
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				x = x + (m[k][j])*(m2.m[j][i]);
			}
			fuckyou[k][i] = x;
			x = 0;
		}
	}
	Matrix4 yoFace;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			m[j][i] = fuckyou[j][i];
		}
	}

	return *this;
}

Vector4& Matrix4::operator*(const Vector4& v)
{
	double x = v.x*m[0][0] + v.y*m[0][1] + v.z*m[0][2] + v.w*m[0][3];
	double y = v.x*m[1][0] + v.y*m[1][1] + v.z*m[1][2] + v.w*m[1][3];
	double z = v.x*m[2][0] + v.y*m[2][1] + v.z*m[2][2] + v.w*m[2][3];
	double w = v.x*m[3][0] + v.y*m[3][1] + v.z*m[3][2] + v.w*m[3][3];

	Vector4 newVector(x, y, z, w);
	return newVector;
}

// return pointer to matrix elements
double* Matrix4::getPointer()
{
  return &m[0][0];
}

// set matrix to identity matrix
void Matrix4::identity()
{
  for (int i=0; i<4; ++i)
  {
    for (int j=0; j<4; ++j)
    {
      if (i==j) 
        m[i][j] = 1.0; 
      else 
        m[i][j] = 0.0;
    }
  }
}

// transpose the matrix (mirror at diagonal)
void Matrix4::transpose()
{
  Matrix4 temp;
  for (int i=0; i<4; ++i)
  {
    for (int j=0; j<4; ++j)
    {
      temp.m[j][i] = m[i][j];
    }
  }
  *this = temp;  // copy temporary values to this matrix
}

void Matrix4::makeRotateX(double angle)
{
	angle = angle / 180.0 * M_PI;  // convert from degrees to radians
	identity();
	m[1][1] = cos(angle);
	m[2][1] = sin(angle);
	m[1][2] = -sin(angle);
	m[2][2] = cos(angle);
}

// Creates a rotation matrix which rotates about the y axis.
// angle is expected in degrees
void Matrix4::makeRotateY(double angle)
{
  angle = angle / 180.0 * M_PI;  // convert from degrees to radians
  identity();
	m[0][0] = cos(angle);
	m[0][2] = sin(angle);
	m[2][0] = -sin(angle);
	m[2][2] = cos(angle);
}

void Matrix4::makeRotateZ(double angle)
{
	angle = angle / 180.0 * M_PI;  // convert from degrees to radians
	identity();
	m[0][0] = cos(angle);
	m[1][0] = sin(angle);
	m[0][1] = -sin(angle);
	m[1][1] = cos(angle);
}

void Matrix4::makeRotate(double angle, const Vector3& axis)
{
	angle = angle / 180.0 * M_PI;  // convert from degrees to radians
	identity();
	m[0][0] = 1+(1-cos(angle))*((axis.x*axis.x)-1);
	m[1][0] = axis.z*sin(angle) + (1 - cos(angle))*axis.y*axis.x;
	m[2][0] = -axis.y*sin(angle) + (1 - cos(angle))*axis.z*axis.x;
	m[0][1] = -axis.z*sin(angle) + (1 - cos(angle))*axis.x*axis.y;
	m[1][1] = 1 + (1 - cos(angle))*((axis.y*axis.y) - 1);
	m[2][1] = axis.x*sin(angle) + (1 - cos(angle))*axis.z*axis.y;
	m[0][2] = axis.y*sin(angle) + (1 - cos(angle))*axis.x*axis.z;
	m[1][2] = -axis.x*sin(angle) + (1 - cos(angle))*axis.y*axis.z;
	m[2][2] = 1 + (1 - cos(angle))*((axis.z*axis.z) - 1);
}

void Matrix4::makeScale(double sx, double sy, double sz)
{
	identity();
	m[0][0] = sx;
	m[1][1] = sy;
	m[2][2] = sz;
}

void Matrix4::makeTranslate(double tx, double ty, double tz)
{
	identity();
	m[0][3] = tx;
	m[1][3] = ty;
	m[2][3] = tz;
}

void Matrix4::print(string comment)
{
	cout << comment << endl;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			cout << m[i][j] << " ";
		}
		cout << endl;
	}
}

void Matrix4::printInfo()
{
	double normX = m[0][3];
	double normY = m[1][3];
	double normZ = m[2][3];
	Vector3 norm(normX, normY, normZ);
	norm.print("");
}
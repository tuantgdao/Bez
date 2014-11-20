#ifndef _MATRIX4_H_
#define _MATRIX4_H_

#include <string>
#include "Vector3.h"
#include "Vector4.h"
#include "GL/glut.h"

#define M_PI 3.14159265358979323846

using namespace std;

class Matrix4
{
  protected:
    //double m[4][4];   // matrix elements; first index is for rows, second for columns (row-major)
    
  public:
	  double m[4][4];
    Matrix4();     
    Matrix4& operator=(const Matrix4&);
	Matrix4& operator*(const Matrix4&);
	Vector4& operator*(const Vector4&);
    double* getPointer(); 
    void identity(); 
    void transpose();
	void makeRotateX(double);
    void makeRotateY(double);
	void makeRotateZ(double);
	void makeRotate(double angle, const Vector3& axis);
	void makeScale(double sx, double sy, double sz);
	void makeTranslate(double tx, double ty, double tz);
	void print(string comment);

	//what im adding
	void changeValue(int i, int j, double value);
	void allChange(int);
	void printInfo();
	//end of what I added
};

#endif
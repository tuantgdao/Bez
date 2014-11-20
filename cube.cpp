#include "Cube.h"
#include "Matrix4.h"

//What I'm Adding
#include "window.h"
//End

using namespace std;

Cube::Cube()
{
  angle = 0.0;
}

Matrix4& Cube::getMatrix()
{
  return model2world;
}

void Cube::spin(double deg)   // deg is in degrees
{
  angle = deg;
  if (angle > 360.0 || angle < -360.0) angle = 0.0;
  
  //model2world.makeRotateY(angle);   // This creates the matrix to rotate the cube

  //My own spin method
  Matrix4 spinMatrix;
  spinMatrix.makeRotateY(angle);
  model2world = model2world*spinMatrix;
  //End of my method
}

//What I'm Adding

void Cube::moveLeftRight(double x)
{
	Matrix4 moveMatrix = getMatrix();
	moveMatrix.makeTranslate(x, 0, 0);
	model2world = moveMatrix*model2world;
}

void Cube::moveUpDown(double x)
{
	Matrix4 moveMatrix = getMatrix();
	moveMatrix.makeTranslate(0, x, 0);
	model2world = moveMatrix*model2world;
}

void Cube::moveOutIn(double x)
{
	Matrix4 moveMatrix = getMatrix();
	moveMatrix.makeTranslate(0, 0, x);
	model2world = moveMatrix*model2world;
}

void Cube::orbit(double angle)
{
	Matrix4 orbitMatrix = getMatrix();
	orbitMatrix.makeRotateZ(angle);
	model2world = orbitMatrix*model2world;
}

void Cube::scale(double x, double y, double z)
{
	Matrix4 scaleMatrix = getMatrix();
	scaleMatrix.makeScale(x, y, z);
	model2world = model2world*scaleMatrix;
}

void Cube::reset()
{
	model2world.makeRotateX(0);
}

#ifndef _CUBE_H_
#define _CUBE_H_

#include "Matrix4.h"

class Cube
{
  protected:
    Matrix4 model2world;            // model matrix (transforms model coordinates to world coordinates)
    double angle;                   // rotation angle [degrees]

  public:
    Cube();   // Constructor
    Matrix4& getMatrix();
    void spin(double);      // spin cube [degrees]

	//What I'm Adding
	void moveLeftRight(double);
	void moveUpDown(double);
	void moveOutIn(double);
	void orbit(double);
	void scale(double,double,double);
	void reset();
};

#endif


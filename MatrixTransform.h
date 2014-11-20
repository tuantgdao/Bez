#ifndef _MATRIXTRANSFORM_H_
#define _MATRIXTRANSFORM_H_

#include "Matrix4.h"
#include "Group.h"

class MatrixTransform : public Group
{
	public:
		Matrix4 M;
		MatrixTransform::MatrixTransform();
		MatrixTransform::MatrixTransform(Matrix4);
		void setTransformationMatrix(Matrix4);
		void draw(Matrix4 C);
		Matrix4& getM();
};

#endif
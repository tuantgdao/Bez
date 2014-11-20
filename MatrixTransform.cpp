#include "MatrixTransform.h"

MatrixTransform::MatrixTransform()
{
	M.identity();
}

MatrixTransform::MatrixTransform(Matrix4 C)
{
	M = C;
}

void MatrixTransform::setTransformationMatrix(Matrix4 C)
{
	M = C;
}

Matrix4& MatrixTransform::getM()
{
	return M;
}

void MatrixTransform::draw(Matrix4 C)
{
	M = C*M;
	for (int i =0; i < numChild; i++)
	{
		myNodes[i]->draw(M);
	}
}

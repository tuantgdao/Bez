#ifndef _NODE_H_
#define _NODE_H_

#include "Matrix4.h"
#include "Vector4.h"
#include <GL/glut.h>

class Node
{
	public:
		Node* parent;
		virtual void draw(Matrix4 C) = 0;
		void setParent(Node*);
		//virtual void update();
};

#endif _NODE_H_
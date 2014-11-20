#ifndef _GROUP_H_
#define _GROUP_H_

#include "Node.h"
//#include "MatrixTransform.h"
//#include "Cube4.h"
#include <vector>

class Group : public Node
{
	public:
		vector<Node*> myNodes;
		int numChild;

		Group::Group();
		void addChild(Node*);
		void removeChild();
		int getNumChild();
		virtual void draw(Matrix4 C) = 0;
};
#endif
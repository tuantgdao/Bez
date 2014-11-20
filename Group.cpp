#include "Group.h"

Group::Group()
{
	numChild = 0;
}

void Group::addChild(Node* child)
{
	myNodes.push_back(child);
	child->setParent(this);
	numChild++;
}

void Group::removeChild()
{
	
}

int Group::getNumChild()
{
	return numChild;
}

/*
void Group::draw(Matrix4 C)
{
	for (std::list<Node*>::iterator i = myNodes.begin(); i != myNodes.end(); ++i)
	{
		(*(*i)).draw(C);
	}
}
*/
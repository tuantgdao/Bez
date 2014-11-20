#ifndef _GEODE_H_
#define _GEODE_H_

#include "Node.h"

class Geode : public Node
{
	public:
		virtual void render(Matrix4) = 0;
		void draw(Matrix4);
};

#endif
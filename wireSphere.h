#ifndef _WIRESPHERE_H_
#define _WIRESPHERE_H_

#include "Geode.h"
#include "main.h"

class wireSphere : public Geode
{
public:
	void render(Matrix4);
};
#endif
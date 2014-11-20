#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "Vector3.h"
#include "Matrix4.h"
#include "Plane.h"

#define radians 3.1415926/180.0

class Camera
{
	private:
		enum {
			TOP = 0, BOTTOM, LEFT,
			RIGHT, NEARP, FARP
		};

	public:
		//cam variables
		Vector3 e;
		Vector3 d;
		Vector3 up;
		Matrix4 c;

		//frustum variables
		static enum{OUTSIDE, INTERSECT, INSIDE};
		Plane p1[6];
		Vector3 ntl, ntr, nbl, nbr, ftl, ftr, fbl, fbr;

		float nearDistance;
		float farDistance;
		float ratio;
		float angle;
		float tang;
		float nearWidth;
		float nearHeight;
		float farWidth;
		float farHeight;

		//Cam Functions
		Camera(Vector3, Vector3, Vector3);
		Camera();
		Matrix4& getInverseCamera();

		//Frustum Functions
		void setFrustum(float, float, float, float);
		void setPlane();
		void setCamera(Vector3, Vector3, Vector3);
		int sphereInFrustum(Vector3, float);
};

#endif _CAMERA_H_
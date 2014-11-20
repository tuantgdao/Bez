#include "Robot.h"

Robot::Robot( float angle, float x, float z, int bound, int f)
{
	rotateAngle = angle;
	translateX = x;
	translateZ = z;
	boundingSphere = bound;
	frustum = f;
}

void Robot::draw(Matrix4 C)
{
	//Head
	Matrix4 s;
	s.makeScale(2, 2, 2);
	MatrixTransform head(s);
	s.makeTranslate(0, 3, 0);
	MatrixTransform headup(s);
	head.addChild(&headup);
	headup.addChild(&Sphere());

	//Body
	Matrix4 x;
	x.makeScale(5, 8, 3);
	MatrixTransform body(x);
	body.addChild(&Cube4());

	//Right Shoulder
	Matrix4 y;
	y.makeTranslate(3.5, 3, 0);
	MatrixTransform rightShoulder(y);
	y.makeScale(1, 1, 1);
	MatrixTransform rightShoulderScale(y);
	rightShoulder.addChild(&rightShoulderScale);
	rightShoulderScale.addChild(&Sphere());

	//Left Shoulder
	Matrix4 z;
	z.makeTranslate(-3.5, 3, 0);
	MatrixTransform leftShoulder(z);
	z.makeScale(1, 1, 1);
	MatrixTransform leftShoulderScale(z);
	leftShoulder.addChild(&leftShoulderScale);
	leftShoulderScale.addChild(&Sphere());

	//Right Arm
	Matrix4 v;
	v.makeScale(1, 5, 1);
	MatrixTransform rightArmScale(v);
	v.makeTranslate(0,-2.5, 0);
	MatrixTransform rightArmTranslate(v);
	v.makeRotateX(rotateAngle);
	MatrixTransform rightArmRotate(v);
	v.makeTranslate(-3.5,2.5, 0);
	MatrixTransform rightArm(v);

	rightArm.addChild(&rightArmRotate);
	rightArmRotate.addChild(&rightArmTranslate);
	rightArmTranslate.addChild(&rightArmScale);
	rightArmScale.addChild(&Cube5());

	//Left Arm
	
	Matrix4 q;
	q.makeTranslate(0, -2.5, 0);
	MatrixTransform leftArmTranslate(q);
	q.makeScale(1,5,1);
	MatrixTransform leftArmScale(q);
	q.makeRotateX(115);
	MatrixTransform leftArmRotate(q);
	q.makeTranslate(3.5, 2.5, 0);
	MatrixTransform leftArm(q);

	leftArm.addChild(&leftArmRotate);
	leftArmRotate.addChild(&leftArmTranslate);
	leftArmTranslate.addChild(&leftArmScale);
	leftArmScale.addChild(&Cube5());
	
	//Right Leg
	Matrix4 b;
	b.makeTranslate(0, -3, 0);
	MatrixTransform rightLeg(b);
	b.makeScale(1, 6, 1);
	MatrixTransform rightLegScale(b);
	b.makeRotateX(rotateAngle);
	MatrixTransform rightLegRotate(b);
	b.makeTranslate(-1, -3, 0);
	MatrixTransform rightLegTranslate(b);

	rightLeg.addChild(&rightLegRotate);
	rightLegRotate.addChild(&rightLegTranslate);
	rightLegTranslate.addChild(&rightLegScale);
	rightLegScale.addChild(&Cube5());

	//Left Leg
	Matrix4 a;
	a.makeTranslate(0, -3, 0);
	MatrixTransform leftLeg(a);
	a.makeScale(1, 6, 1);
	MatrixTransform leftLegScale(a);
	a.makeRotateX(-rotateAngle);
	MatrixTransform leftLegRotate(a);
	a.makeTranslate(1, -3, 0);
	MatrixTransform leftLegTranslate(a);

	leftLeg.addChild(&leftLegRotate);
	leftLegRotate.addChild(&leftLegTranslate);
	leftLegTranslate.addChild(&leftLegScale);
	leftLegScale.addChild(&Cube5());

	//Draw the robot

	Matrix4 t;
	t.makeTranslate(translateX, 0, translateZ);
	MatrixTransform transMatrix(t);

	transMatrix.addChild(&head);
	transMatrix.addChild(&body);
	transMatrix.addChild(&rightShoulder);
	transMatrix.addChild(&leftShoulder);
	transMatrix.addChild(&rightArm);
	transMatrix.addChild(&leftArm);
	transMatrix.addChild(&rightLeg);
	transMatrix.addChild(&leftLeg);

	root.addChild(&transMatrix);

	root.draw(C);
}
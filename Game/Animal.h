#pragma once
#include "Object.h"
class CAnimal :
	public Object
{
public:
	int center_x, center_y,state;
	int now_x, now_y, dis_x, dis_y,speed;
public:
	CAnimal();
	CAnimal(int x, int y, int speed);
	void move();
	~CAnimal();
};


#pragma once
#include "Object.h"
class CHuman :
	public Object
{
public:
	int now_x, now_y, dis_x, dis_y,speed,state;
public:
	CHuman();
	CHuman(int x, int y,int speed);
	void Move();
	~CHuman();
};


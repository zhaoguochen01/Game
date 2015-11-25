#pragma once
#include "Object.h"

#define SOUTH 0
#define SOUTH_WEST 1
#define SOUTH_EAST 7
#define NORTH_WEST 3
#define NORTH 4
#define NORTH_EAST 5
#define EAST 6
#define WEST 2
#define STAND 8

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


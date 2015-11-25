#include "stdafx.h"
#include "Human.h"


CHuman::CHuman()
{
}


CHuman::~CHuman()
{
}

CHuman::CHuman(int x, int y,int speed)
{
	this->now_x = x;
	this->now_y = y;
	this->speed = speed;
}

void CHuman::Move()
{
	bool north = false, south = false, east = false , west = false;
	if (now_x == dis_x && now_y == dis_y)
	{
		state = STAND;
	}
	if (now_x < dis_x)
	{
		east = true;
		if (abs(now_x - dis_x) <= speed)
			now_x = dis_x;
		else 
			now_x += speed;
	}
	if (now_y < dis_y)
	{
		south = true;
		if (abs(now_y - dis_y) <= speed)
			now_y = dis_y;
		else 
			now_y += speed;
	}
	if (now_x > dis_x)
	{
		west = true;
		if (abs(now_x - dis_x) <= speed)
			now_x = dis_x;
		else
			now_x -= speed;
	}
	if (now_y > dis_y)
	{
		north = true;
		if (abs(now_y - dis_y) <= speed)
			now_y = dis_y;
		else 
			now_y -= speed;
	}
	if (east^west^north^south==0)
	{
		if (east&&north)
		{
			state = NORTH_EAST;
		}
		if (east&&south)
		{
			state = SOUTH_EAST;
		}
		if (west&&north)
		{
			state = NORTH_WEST;
		}
		if (west&&south)
		{
			state = SOUTH_WEST;
		}
	}
	else
	{
		if (east)
		{
			state = EAST;
		}
		if (west)
		{
			state = WEST;
		}
		if (north)
		{
			state = NORTH;
		}
		if (SOUTH)
		{
			state = SOUTH;
		}
	}
}

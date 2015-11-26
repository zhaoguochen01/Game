#pragma once
#define SOUTH 0
#define SOUTH_WEST 1
#define SOUTH_EAST 7
#define NORTH_WEST 3
#define NORTH 4
#define NORTH_EAST 5
#define EAST 6
#define WEST 2
#define STAND 8
class Object
{
public:
	int x, y;
public:
	Object();
	Object(int x, int y);
	~Object();
};


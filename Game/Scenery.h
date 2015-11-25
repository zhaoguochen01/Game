#pragma once
#include "Object.h"
class CScenery :
	public Object
{
public:
	int x, y;
	HBITMAP pic;
public:
	CScenery();
	CScenery(int x, int y,CString path);
	~CScenery();
};


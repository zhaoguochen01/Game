#include "stdafx.h"
#include "Scenery.h"
#include "Object.h"

CScenery::CScenery()
{
}


CScenery::~CScenery()
{
}

CScenery::CScenery(int x, int y, CString path):Object(x,y)
{
	this->x = x;
	this->y = y;
	this->pic = (HBITMAP)LoadImage(AfxGetInstanceHandle(), path, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
}
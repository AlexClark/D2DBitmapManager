#pragma once


//#include "PCH.h"

struct BmpAnimate
{
	ID2D1Bitmap* bmp;
	FLOAT dt, framerate;
	D2D_POINT_2F offset, dimensions, padding;
	DWORD framecount, currframe;
	bool vertical, stopAnim;

	BmpAnimate(void);

	void Update(FLOAT _dt);

	D2D_RECT_F GenerateFrame();
};


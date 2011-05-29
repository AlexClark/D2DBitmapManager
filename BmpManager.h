#pragma once

#include "PCH.h"

struct BmpObj
{
	ID2D1Bitmap* bmp;
	wstring key;

	BmpObj() {bmp = nullptr;}
	bool operator==(const BmpObj& other) const { if(key == other.key) return true; else return false;}
	~BmpObj() {}
};

class BmpManager
{
	HTable<BmpObj> table;
	GraphicsInterface& gi;

public:
	BmpManager(GraphicsInterface& refgi);
	~BmpManager(void);

	ID2D1Bitmap* LoadImage(PCWSTR uri, ID2D1BitmapBrush** ppBrush = nullptr);

};


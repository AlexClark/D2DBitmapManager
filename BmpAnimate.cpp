struct BmpAnimate;


#include "PCH.h"


BmpAnimate::BmpAnimate()
{
	bmp = nullptr;
	dt = framerate = 0.0f;
	framecount = currframe = 0;
	vertical = stopAnim = false;
	offset = dimensions = padding = D2D1::Point2F();
}

void BmpAnimate::Update(FLOAT _dt)
{
	if(!stopAnim)
	{
		dt += _dt * 1000;
		if(framecount != 0)
		{
			if( dt >= (1000.0f / framerate) )
			{
				dt -= (1000.0f / framerate);
				currframe++;
				if(currframe >= framecount)
					currframe = 0;
			}
		}
	}
}

D2D_RECT_F BmpAnimate::GenerateFrame()
{
	D2D_RECT_F tmp;

	if(framecount == 0)
	{
		tmp.top = 0.0f;
		tmp.left = 0.0f;

		if(bmp)
		{
			tmp.bottom = bmp->GetSize().height;
			tmp.right = bmp->GetSize().width;
		}
		else
		{
			tmp.bottom = 0.0f;
			tmp.right = 0.0f;
		}
		return tmp;
	}


	tmp.top = (FLOAT)offset.y + (currframe * (padding.y));
	tmp.left = (FLOAT)offset.x + (currframe * (padding.x));

	if(!vertical)
		tmp.left += currframe * dimensions.x;
	else
		tmp.top += currframe * dimensions.y;

	while(tmp.left >= bmp->GetSize().width && !vertical)
	{
		tmp.left -= (bmp->GetSize().width - offset.x);
		tmp.top += dimensions.y + padding.y;
	}


	tmp.right = tmp.left + dimensions.x;
	tmp.bottom = tmp.top + dimensions.y;

	return tmp;
}

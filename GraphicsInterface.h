#pragma once

#include "PCH.h"

class GraphicsInterface
{
	ID2D1Factory* pD2DFactory;
	ID2D1HwndRenderTarget* pRT;
	ID2D1SolidColorBrush* pBrush;
	IWICImagingFactory* pIWICFactory;
	ID2D1BitmapBrush* pBmpBrush;

	D2D1::ColorF background; // The clear color.

	// The DirectWrite interfaces:
	IDWriteFactory* pDWFactory;
	IDWriteTextFormat* pFont;


public:
	GraphicsInterface(void);
	~GraphicsInterface(void);

	HRESULT Initialize(HWND hWnd);

	void Release();

	IWICImagingFactory* GetWIC() {return pIWICFactory;}
	ID2D1Factory* GetFactory() {return pD2DFactory;}
	ID2D1HwndRenderTarget* GetRenderTarget() {return pRT;}
	ID2D1SolidColorBrush*& GetBrush() {return pBrush;}
	D2D1::ColorF GetBG() {return background;}
	void SetBG(D2D1::ColorF bg) {background = bg;}
	IDWriteTextFormat* GetTextFormat() {return pFont;}
	HRESULT SetTextFormat(LOGFONT& lf);
	ID2D1BitmapBrush*& GetBmpBrush() {return pBmpBrush;}
	void SetBmpBrush(ID2D1Bitmap* bmp);
};


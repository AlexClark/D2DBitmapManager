#include "PCH.h"
#include "GraphicsInterface.h"


GraphicsInterface::GraphicsInterface(void) : background(D2D1::ColorF::WhiteSmoke)
{
	pD2DFactory = nullptr;
	pRT = nullptr;
	pBrush = nullptr;

	memset(szBuffer, 0, sizeof(szBuffer[0]) * 512);

	// The DirectWrite interfaces:
	pDWFactory = nullptr;
	pFont = nullptr;
}


GraphicsInterface::~GraphicsInterface(void)
{
	Release();
}

HRESULT GraphicsInterface::Initialize(HWND hWnd)
{
	// Initialize the Direct2D Factory.
	HRESULT hr;
	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &pD2DFactory);
	if (FAILED(hr))
	{
		MessageBox(HWND_DESKTOP, _T("ERROR: Failed to Create the Direct2D Factory."),
			_T("Direct2D Error"), MB_OK | MB_ICONERROR);
		return hr;
	}

	// Get the dimensions of the client.
	RECT rClient;
	GetClientRect(hWnd, &rClient);

	// Initialize a Direct2D Size Structure.
	D2D1_SIZE_U size = D2D1::SizeU(rClient.right, rClient.bottom);

	// Create the Direct2D Render Target.
	hr = pD2DFactory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties(hWnd, size), &pRT);
	if (FAILED(hr))
	{
		MessageBox(HWND_DESKTOP, _T("ERROR: Failed to Create the Direct2D Render Target."),
			_T("Direct2D Error"), MB_OK | MB_ICONERROR);
		return hr;
	}

	// Create the Direct2D Solid Color Brush.
	hr = pRT->CreateSolidColorBrush(D2D1::ColorF(0x0), &pBrush);
	if (FAILED(hr))
	{
		MessageBox(HWND_DESKTOP, _T("ERROR: Failed to Create the Direct2D Solid Color Brush."),
			_T("Direct2D Error"), MB_OK | MB_ICONERROR);
		return hr;
	}

	// Initialize the DirectWrite Factory.
	hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory),
			(IUnknown**)&pDWFactory);
	if (FAILED(hr))
	{
		MessageBox(HWND_DESKTOP, _T("ERROR: Failed to Create the DirectWrite Factory."),
			_T("DirectWrite Error"), MB_OK | MB_ICONERROR);
		return hr;
	}

	// Create the DirectWrite Text Format.
	hr = pDWFactory->CreateTextFormat(_T("Veranda"), NULL,
		DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL,
		20, _T("en-US"), &pFont);
	if (FAILED(hr))
	{
		MessageBox(HWND_DESKTOP, _T("ERROR: Failed to Create the DirectWrite Text Format."),
			_T("DirectWrite Error"), MB_OK | MB_ICONERROR);
		return hr;
	}

	 hr = CoCreateInstance(
        CLSID_WICImagingFactory,
        NULL,
        CLSCTX_INPROC_SERVER,
        IID_PPV_ARGS(&pIWICFactory)
        );
	if (FAILED(hr))
	{
		MessageBox(HWND_DESKTOP, _T("ERROR: Failed to Create the WIC Factory."),
			_T("WIC Error"), MB_OK | MB_ICONERROR);
		return hr;
	}


	return S_OK;
}

void GraphicsInterface::SetBmpBrush(ID2D1Bitmap* bmp)
{
	HRESULT hr;
	hr = pRT->CreateBitmapBrush(bmp, &pBmpBrush);
	pBmpBrush->SetExtendModeX(D2D1_EXTEND_MODE_WRAP);
	pBmpBrush->SetExtendModeY(D2D1_EXTEND_MODE_WRAP);
}

HRESULT GraphicsInterface::SetTextFormat(LOGFONT& lf)
{
	HRESULT hr;
	hr = pDWFactory->CreateTextFormat(lf.lfFaceName, NULL,
		(DWRITE_FONT_WEIGHT)lf.lfWeight, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL,
		(FLOAT)abs(lf.lfHeight), _T("en-US"), &pFont);
	if (FAILED(hr))
	{
		MessageBox(HWND_DESKTOP, _T("ERROR: Failed to Create the DirectWrite Text Format."),
			_T("DirectWrite Error"), MB_OK | MB_ICONERROR);
		return hr;
	}

	return S_OK;
}

void GraphicsInterface::Release()
{
	//Release Bmp Brush
	if(pBmpBrush)
	{
		pBmpBrush->Release();
		pBmpBrush = nullptr;
	}

	// Release the WIC Factory.
	if (pIWICFactory)
	{
		pIWICFactory->Release();
		pIWICFactory = NULL;
	}

	// Release the DirectWrite Text Format.
	if (pFont)
	{
		pFont->Release();
		pFont = NULL;
	}
	// Release the DirectWrite Factory.
	if (pDWFactory)
	{
		pDWFactory->Release();
		pDWFactory = NULL;
	}
	// Release the Direct2D Solid Color Brush.
	if (pBrush)
	{
		pBrush->Release();
		pBrush = NULL;
	}
	// Release the Direct2D Render Target.
	if (pRT)
	{
		pRT->Release();
		pRT = NULL;
	}
	// Release the Direct2D Factory.
	if (pD2DFactory)
	{
		pD2DFactory->Release();
		pD2DFactory = NULL;
	}
}


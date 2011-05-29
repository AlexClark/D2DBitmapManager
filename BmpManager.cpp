#include "PCH.h"
#include "BmpManager.h"

#define BMPHASHBUCKETS 1009

unsigned int HashBmp(const BmpObj& v)
{
	const wchar_t* cstr = v.key.c_str();
	unsigned int offset = 1;
	unsigned long long rem = 0, hash = 0, sumHash = 3, tmp = 0;
	int dir = 1;

	for(unsigned int i = 1; i < v.key.length() - 1; sumHash *= cstr[i], i++);
	sumHash %= 8091;

	for(unsigned int i = 1; i < v.key.length() - 2; i++)
	{
		rem ^= (unsigned int)(cstr[i - 1] & 3);
		tmp = (unsigned int)(cstr[i]) & ((rem << 6) | 63);
		hash ^= (unsigned int)(tmp << offset);
		offset += dir;

		if(offset >= 30 || offset == 0)
			dir = -dir;
	}

	hash ^= (unsigned int)(cstr[v.key.length() - 1]);
	hash ^= rem;
	hash ^= sumHash;
	hash ^= 25;
	hash ^= 37;
	hash ^= 122;
	hash %= BMPHASHBUCKETS;
	
	return (unsigned int)hash;
}


template<class Interface>
inline void
SafeRelease(
    Interface **ppInterfaceToRelease
    )
{
    if (*ppInterfaceToRelease != NULL)
    {
        (*ppInterfaceToRelease)->Release();

        (*ppInterfaceToRelease) = NULL;
    }
}


BmpManager::BmpManager(GraphicsInterface& refgi) 
	: table(BMPHASHBUCKETS, HashBmp), gi(refgi)
{

}


BmpManager::~BmpManager(void)
{
	for(unsigned int i = 0; i < BMPHASHBUCKETS; i++)
	{
		SLList<BmpObj>& bmpList = table.getList(i);
		SLLIter<BmpObj> bmpIter(bmpList);
		bmpIter.begin();
		while(!bmpIter.end())
		{
			SafeRelease(&(bmpIter.current().bmp));
			++bmpIter;
		}
	}

}

ID2D1Bitmap* BmpManager::LoadImage(PCWSTR uri, ID2D1BitmapBrush** ppBrush)
{
	BmpObj tmp;
	tmp.key = uri;
	BmpObj* pBmp = table.findAndGet(tmp);

	if(pBmp)
		return pBmp->bmp;

	IWICBitmapDecoder *pDecoder = NULL;
    IWICBitmapFrameDecode *pSource = NULL;
    IWICFormatConverter *pConverter = NULL;
    IWICBitmapScaler *pScaler = NULL;

    HRESULT hr = gi.GetWIC()->CreateDecoderFromFilename(
        uri,
        NULL,
        GENERIC_READ,
        WICDecodeMetadataCacheOnLoad,
        &pDecoder
        );
        
    if (SUCCEEDED(hr))
    {
        // Create the initial frame.
        hr = pDecoder->GetFrame(0, &pSource);
    }
    if (SUCCEEDED(hr))
    {

        // Convert the image format to 32bppPBGRA
        // (DXGI_FORMAT_B8G8R8A8_UNORM + D2D1_ALPHA_MODE_PREMULTIPLIED).
        hr = gi.GetWIC()->CreateFormatConverter(&pConverter);

    }
 
 
    if (SUCCEEDED(hr))
    {
        
		hr = pConverter->Initialize(
			pSource,
			GUID_WICPixelFormat32bppPBGRA,
			WICBitmapDitherTypeNone,
			NULL,
			0.f,
			WICBitmapPaletteTypeMedianCut
			);
    }
    if (SUCCEEDED(hr))
    {
    
        // Create a Direct2D bitmap from the WIC bitmap.
        hr = gi.GetRenderTarget()->CreateBitmapFromWicBitmap(
            pConverter,
            NULL,
            &(tmp.bmp)
            );
    }

	if(ppBrush != nullptr)
		hr = gi.GetRenderTarget()->CreateBitmapBrush(tmp.bmp, ppBrush);

    SafeRelease(&pDecoder);
    SafeRelease(&pSource);
    SafeRelease(&pConverter);
    SafeRelease(&pScaler);

	table.insert(tmp);

	if(tmp.bmp)
		return tmp.bmp;
	else
		return nullptr;
}

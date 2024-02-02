#pragma once

#include <windows.h>
#include <thumbcache.h>

class ImageThumbnailProvider :
	public IInitializeWithStream,
	public IThumbnailProvider {
public:
	// IUnknown
	IFACEMETHODIMP QueryInterface(REFIID riid, void** ppv);
	IFACEMETHODIMP_(ULONG) AddRef();
	IFACEMETHODIMP_(ULONG) Release();

	// IInitializeWithStream
	IFACEMETHODIMP Initialize(IStream* pStream, DWORD grfMode);

	// IThumbnailProvider
	IFACEMETHODIMP GetThumbnail(UINT cx, HBITMAP* phbmp, WTS_ALPHATYPE* pdwAlpha);

	ImageThumbnailProvider();

protected:
	~ImageThumbnailProvider();

private:
	long m_cRef;
	IStream* m_pStream;
};
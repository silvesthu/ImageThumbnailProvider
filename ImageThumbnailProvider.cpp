#include "pch.h"
#include "ImageThumbnailProvider.h"
#include "GetThumbnail.h"

extern HINSTANCE g_hInst;
extern long g_cDllRef;

ImageThumbnailProvider::ImageThumbnailProvider() : m_cRef(1), m_pStream(NULL) {
	InterlockedIncrement(&g_cDllRef);
}

ImageThumbnailProvider::~ImageThumbnailProvider() {
	InterlockedDecrement(&g_cDllRef);
}

// IUnknown

IFACEMETHODIMP ImageThumbnailProvider::QueryInterface(REFIID riid, void** ppv) {
	static const QITAB qit[] =
	{
		QITABENT(ImageThumbnailProvider, IThumbnailProvider),
		QITABENT(ImageThumbnailProvider, IInitializeWithStream),
		{ 0 },
	};
	return QISearch(this, qit, riid, ppv);
}

IFACEMETHODIMP_(ULONG) ImageThumbnailProvider::AddRef() {
	return InterlockedIncrement(&m_cRef);
}

IFACEMETHODIMP_(ULONG) ImageThumbnailProvider::Release() {
	ULONG cRef = InterlockedDecrement(&m_cRef);

	if (0 == cRef)
		delete this;

	return cRef;
}

// IInitializeWithStream

IFACEMETHODIMP ImageThumbnailProvider::Initialize(IStream* pStream, DWORD grfMode) {

	OutputDebugStringA("[ImageThumbnailProvider] ImageThumbnailProvider::Initialize\n");

	HRESULT hr = HRESULT_FROM_WIN32(ERROR_ALREADY_INITIALIZED);

	if (m_pStream == NULL)
		hr = pStream->QueryInterface(&m_pStream);

	return hr;
}

// IThumbnailProvider

IFACEMETHODIMP ImageThumbnailProvider::GetThumbnail(UINT cx, HBITMAP* phbmp, WTS_ALPHATYPE* pdwAlpha) {

	OutputDebugStringA("[ImageThumbnailProvider] ImageThumbnailProvider::GetThumbnail\n");

	*pdwAlpha = WTSAT_ARGB;
	*phbmp = GetEXRThumbnail(m_pStream);

	// m_pStream->Release();

	return *phbmp != NULL ? NOERROR : E_NOTIMPL;
}

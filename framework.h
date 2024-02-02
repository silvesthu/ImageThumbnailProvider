#pragma once

#define WIN32_LEAN_AND_MEAN						// Exclude rarely-used stuff from Windows headers
// Windows Header Files
#include <windows.h>

#undef min
#undef max

#include <unknwn.h>								// For IClassFactory
#include <Shlwapi.h>							// For QITAB
#include <new>									// For std::nothrow

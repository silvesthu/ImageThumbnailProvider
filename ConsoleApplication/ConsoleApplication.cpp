// ConsoleApplication.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

#define WIN32_LEAN_AND_MEAN						// Exclude rarely-used stuff from Windows headers
#include <windows.h>
#include <Shlwapi.h>
#include <assert.h>

#undef min
#undef max

#define GET_EXR_THUMBNAIL_DUMP_BITMAP_PATH "1.bmp"
#include "../GetThumbnail.h"

#pragma comment(lib, "Shlwapi.lib")

int main()
{
    IStream* stream;
    SHCreateStreamOnFileEx(L"1.exr", STGM_READ, FILE_ATTRIBUTE_NORMAL, false, NULL, &stream);
    GetEXRThumbnail(stream);
    return 0;
}
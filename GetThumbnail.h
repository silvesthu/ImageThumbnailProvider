#pragma once

#define TINYEXR_USE_MINIZ 0
#define TINYEXR_USE_STB_ZLIB 1
#define TINYEXR_IMPLEMENTATION
#include "tinyexr/tinyexr.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#pragma warning(disable : 4996)
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"
#pragma warning(default : 4996)
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "stb/stb_image_resize2.h"

inline HBITMAP GetEXRThumbnail(IStream* stream)
{
    OutputDebugStringA("[ImageThumbnailProvider] GetEXRThumbnail\n");

    HBITMAP bitmap = NULL;

    if (stream == NULL)
        return bitmap;

    LARGE_INTEGER move;
    move.QuadPart = 0;
    ULARGE_INTEGER large_size;
    stream->Seek(move, SEEK_END, &large_size);
    ULARGE_INTEGER position;
    stream->Seek(move, SEEK_SET, &position);

    size_t size = (size_t)large_size.QuadPart;
    uint8_t* memory = new uint8_t[size];
    ULONG read = 0;
    stream->Read(memory, (ULONG)size, &read);
    assert(size == read);

    float* rgba = nullptr;
    int width = 0;
    int height = 0;
    const char* err = nullptr;
    if (LoadEXRFromMemory(&rgba, &width, &height, memory, size, &err) == TINYEXR_SUCCESS)
    {
        int resized_width = 0;
        int resized_height = 0;

        if (width > height) {
            resized_width = 256;
            resized_height = height * resized_width / width;
        }
        else {
            resized_height = 256;
            resized_width = width * resized_height / height;
        }

        float* resized_rgba = new float[resized_width * resized_height * 4];
        COLORREF* thumbnail_rgba = new COLORREF[resized_width * resized_height];

        stbir_resize_float_linear(
            rgba, width, height, 0,
            resized_rgba, resized_width, resized_height, 0, STBIR_4CHANNEL);

        for (int h = 0; h < resized_height; h++)
            for (int w = 0; w < resized_width; w++)
            {
                static auto to_u8 = [](float value)
                {
                    return std::max(std::min((uint32_t)(value * 255.0f), 255u), 0u);
                };

                int index = h * resized_width * 4 + w * 4;
                uint8_t r = to_u8(resized_rgba[index + 0]);
                uint8_t g = to_u8(resized_rgba[index + 1]);
                uint8_t b = to_u8(resized_rgba[index + 2]);
#ifdef GET_EXR_THUMBNAIL_DUMP_BITMAP_PATH
                thumbnail_rgba[h * resized_width + w] = RGB(r, g, b);
#else
                thumbnail_rgba[h * resized_width + w] = RGB(b, g, r);
#endif // GET_EXR_THUMBNAIL_DUMP_BITMAP_PATH
            }

        bitmap = CreateBitmap(resized_width, resized_height, 1, 32, thumbnail_rgba);

#ifdef GET_EXR_THUMBNAIL_DUMP_BITMAP_PATH
        stbi_write_bmp(GET_EXR_THUMBNAIL_DUMP_BITMAP_PATH, resized_width, resized_height, 4, thumbnail_rgba);
#endif // GET_EXR_THUMBNAIL_DUMP_BITMAP_PATH

        delete[] thumbnail_rgba;
        delete[] resized_rgba;
    }
    else
    {
        if (err != nullptr)
        {
            std::stringstream ss;
            ss << err << "\n";
            OutputDebugStringA(ss.str().c_str());
        }
    }
    FreeEXRErrorMessage(err);
    if (rgba != nullptr)
        free(rgba); // no free API somehow...
    delete[] memory;

    return bitmap;
}

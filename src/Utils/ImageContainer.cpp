#include "ImageContainer.h"

#include <fstream>
#include <stdlib.h>
#include "spng.h"


ImageRGBA::ImageRGBA(const std::string& filename)
{
    // Load a png image with the spng library

    FILE* png = fopen(filename.c_str(), "rb");
    spng_ctx* ctx = spng_ctx_new(0);

    spng_set_png_file(ctx, png);
    struct spng_ihdr ihdr;
    auto ret = spng_get_ihdr(ctx, &ihdr);
    if (ret)
    {
        printf("spng_get_ihdr() error: %s\n", spng_strerror(ret));
    }
    spng_decoded_image_size(ctx, SPNG_FMT_RGBA8, &m_size);
    
    m_data = (char*) malloc(m_size);
    if (auto decode_ret = spng_decode_image(ctx, m_data, m_size, SPNG_FMT_RGBA8, 0))
    {
        printf("spng_decode_image() error: %s\n", spng_strerror(decode_ret));
        throw;
    }

    width = ihdr.width;
    height = ihdr.height;

    spng_ctx_free(ctx);
}
#include "../inc/rastrum_image.hpp"
#include <stdexcept>

using namespace rastrum_image;

RastrumImage::RastrumImage(std::string const& fname,
                           uint32_t width, uint32_t height, rgba_t bgclr)
    : fname_(fname), width_(width), height_(height), bgclr_(bgclr) {}

std::pair<uint32_t, uint32_t> RastrumImage::getSize() const noexcept
{   return std::make_pair(width_, height_);  }

std::string const& RastrumImage::getName() const noexcept
{   return fname_;  }

void RastrumImage::setPixelSet(std::unordered_set<pu32_t> const& pset, rgba_t clr)
{   for (const auto [x,y] : pset) setPixel(x,y,clr);    }

void RastrumImage::setPixelSet(std::vector<bool> const& pset, rgba_t clr)
{
    const size_t maxi = std::min(pset.size(), static_cast<size_t>(width_*height_));
    for (size_t i = 0; i < maxi; i++)
    if (pset[i])
    {
        const uint32_t px_x = i % width_;
        const uint32_t px_y = i / width_;
        setPixel(px_x, px_y, clr);
    }
}

PngImage::PngImage(std::string const& fname, uint32_t width, uint32_t height, rgba_t bgclr)
    : RastrumImage(fname, width, height, bgclr)
{
    fp_ = fopen(fname_.c_str(), "wb");
    if(!fp_)
        throw std::runtime_error("open file");

    png_ = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_)
        throw std::runtime_error("png_create_write_struct");

    info_ = png_create_info_struct(png_);
    if (!info_)
        throw std::runtime_error("png_create_info_struct");

    if (setjmp(png_jmpbuf(png_)))
        throw std::runtime_error("setjmp");

    png_init_io(png_, fp_);

    // create IHDR chunk
    constexpr int BIT_DEPTH = 8;
    png_set_IHDR(
        png_,
        info_,
        width, height,
        BIT_DEPTH,
        PNG_COLOR_TYPE_RGBA,
        PNG_INTERLACE_NONE,
        PNG_COMPRESSION_TYPE_DEFAULT,
        PNG_FILTER_TYPE_DEFAULT
    );
    png_write_info(png_, info_);

    // allocate memory for IDAT chunk
    row_pointers_ = static_cast<png_byte **>(malloc(sizeof(png_byte *) * height));
    if (!row_pointers_) std::bad_alloc();

    for(uint32_t y = 0; y < height; y++)
    {
        row_pointers_[y] = static_cast<png_byte *>(malloc(png_get_rowbytes(png_, info_)));
        if (!row_pointers_[y]) std::bad_alloc();
    }

    for (uint32_t y = 0; y < height; y++)
    for (uint32_t x = 0; x < width; x++)
        setPixel(x, y, bgclr);
}

void PngImage::load()
{   throw std::runtime_error("TODO: void load() override"); }

void PngImage::save()
{
    png_write_image(png_, row_pointers_);
    png_write_end(png_, NULL);
    saved_ = true;
}

void PngImage::visit(Visitor const& visitor)
{   visitor.visit(*this);   }

void PngImage::setPixel(uint32_t x, uint32_t y, rgba_t clr)
{
    if (x >= width_ || y >= height_) return;
    const auto [r,g,b,a] = clr;
    png_byte * px = &row_pointers_[y][x * 4];
    px[0] = r;  px[1] = g;
    px[2] = b;  px[3] = a;
}

rgba_t PngImage::getPixel(uint32_t x, uint32_t y) const noexcept
{
    if (x >= width_ || y >= height_) return {0,0,0,0};
    const png_byte * px = &row_pointers_[y][x * 4];
    return {px[0], px[1], px[2], px[3]};
}

PngImage::~PngImage() noexcept
{
    if (!saved_) save();

    for(uint32_t y = 0; y < height_; y++)
        free(row_pointers_[y]);
    free(row_pointers_);

    fclose(fp_);
    png_destroy_write_struct(&png_, &info_);
}

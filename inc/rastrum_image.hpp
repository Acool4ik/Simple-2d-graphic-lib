#pragma once
#include <string>
#include <unordered_set>
#include <functional>
#include <vector>
#include <cmath>
#include <png.h>

namespace rastrum_image {

using pu32_t = std::pair<uint32_t, uint32_t>;
struct rgb_t { uint8_t r,g,b; };
struct rgba_t { uint8_t r,g,b,a=255; };

struct Visitor;

struct RastrumImage
{
    virtual void setPixel(uint32_t x, uint32_t y, rgba_t clr) = 0;
    virtual rgba_t getPixel(uint32_t x, uint32_t y) const noexcept = 0;

    virtual void load() = 0;
    virtual void save() = 0;
    virtual void visit(Visitor const& vis) = 0;
    virtual ~RastrumImage() noexcept {}

    std::pair<uint32_t, uint32_t> getSize() const noexcept;
    std::string const& getName() const noexcept;
    void setPixelSet(std::unordered_set<pu32_t> const& pset, rgba_t clr);
    void setPixelSet(std::vector<bool> const& pset, rgba_t clr);
protected:
    RastrumImage(std::string const& fname,
                 uint32_t width, uint32_t height, rgba_t bgclr);
    std::string fname_;
    uint32_t width_;
    uint32_t height_;
    rgba_t bgclr_;
    bool saved_ = false;
};

struct PngImage final : RastrumImage
{
    PngImage(std::string const& fname, uint32_t width, uint32_t height, rgba_t bgclr);

    void setPixel(uint32_t x, uint32_t y, rgba_t color) override;
    rgba_t getPixel(uint32_t x, uint32_t y) const noexcept override;

    void load() override;
    void save() override;
    void visit(Visitor const& vis) override;

    ~PngImage() noexcept override;
private:
    FILE * fp_;
    png_struct * png_;
    png_info * info_;
    png_byte ** row_pointers_;
};

struct Visitor
{
    virtual void visit(PngImage& image) const = 0;
};

namespace clrs {

#define INIT_CLR_FUNCTION(name,r,g,b)  \
    inline rgba_t name (uint8_t alpha = 255)   { return {(r),(g),(b),alpha};     }

INIT_CLR_FUNCTION(red,      255,0,0)
INIT_CLR_FUNCTION(green,    0,255,0)
INIT_CLR_FUNCTION(blue,     0,0,255)
INIT_CLR_FUNCTION(yellow,   255,255,0)
INIT_CLR_FUNCTION(magenta,  255,0,255)
INIT_CLR_FUNCTION(cyan,     0,255,255)
INIT_CLR_FUNCTION(white,    255,255,255)
INIT_CLR_FUNCTION(gray,     189,195,199)
INIT_CLR_FUNCTION(black,    0,0,0)

#undef INIT_CLR_FUNCTION

inline rgba_t gammaCorr(rgba_t clr) noexcept
{
    auto [r,g,b,a] = clr;
    r = static_cast<uint8_t>(std::pow(r / 255.f, 1/2.2f) * 255);
    g = static_cast<uint8_t>(std::pow(g / 255.f, 1/2.2f) * 255);
    b = static_cast<uint8_t>(std::pow(b / 255.f, 1/2.2f) * 255);
    return {r,g,b,a};
}

inline rgba_t mixClrs(rgba_t clr1, rgba_t clr2) noexcept
{
    using cfloat = const float;
    const auto [r1,g1,b1,a1] = clr1;
    const auto [r2,g2,b2,a2] = clr2;

    if (std::abs(a1 + a2) == 0) return {0,0,0,0};

    cfloat alpha1 = a1 / (float)(a1 + a2);
    cfloat alpha2 = a2 / (float)(a1 + a2);

    auto mix = [alpha1,alpha2](uint8_t c1, uint8_t c2)
    {   return static_cast<uint8_t>(c1*alpha1 + c2*alpha2); };
    return {mix(r1,r2),mix(g1,g2),mix(b1,b2),mix(a1,a2)};
}

} // namespace clrs

} // namespace rastrum_image


// impl hash for "setPixelSet" method
namespace std {

template<>
struct hash<std::pair<uint32_t,uint32_t>>
{
    size_t operator()(std::pair<uint32_t,uint32_t> const& pu32) const noexcept
    {
        const auto [x, y] = pu32;
        const size_t h1 = std::hash<std::uint32_t>{}(x);
        const size_t h2 = std::hash<std::uint32_t>{}(y);
        return h1 ^ (h2 << 1);
    }
};

} // namespace std

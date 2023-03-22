#include "../inc/shapes.hpp"
#include <iostream> // for debug

using namespace shapes;
using cfloat    = const float;
using cint32_t  = const int32_t;
using cuint32_t = const uint32_t;

template<typename T>
inline auto pow2(T x) { return std::pow(x, 2); }

Shape::Shape(std::shared_ptr<RastrumImage> rimage, rgba_t clr)
    : rimage_(rimage), clr_(clr) {}

const std::shared_ptr<RastrumImage> Shape::getRastrumImage() const noexcept
{   return rimage_; }

rgba_t Shape::getClr() const noexcept
{   return clr_; }

Circle::Circle(std::shared_ptr<RastrumImage> rimage,
    float xc, float yc, float r, rgba_t clr)
    : Shape(rimage, clr), xc_(xc), yc_(yc), r_(std::abs(r)) {}

void Circle::draw()
{
    const auto [width, height] = rimage_->getSize();
    const auto square = std::min(width, height);

    cint32_t px_xc = (width + square * xc_) / 2.0f;
    cint32_t px_yc = (height - square * yc_) / 2.0f;
    cint32_t px_r = square * r_ / 2.0f;
    drawed_ = true;

    for (int32_t x = -px_r; x <= px_r; x++)
    for (int32_t y = -px_r; y <= px_r; y++)
        if (pow2(x) + pow2(y) <= pow2(px_r))
            rimage_->setPixel(x + px_xc, y + px_yc, clr_);
}

Circle::~Circle() noexcept
{   if (!drawed_) draw();   }

bool Circle::isIntersect(Circle const& cir) const noexcept
{   return is_intersect(*this, cir); }
bool Circle::isIntersect(Rectangle const& rec) const noexcept
{   return is_intersect(*this, rec);  }
bool Circle::isIntersect(Shape const& sh) const noexcept
{   return sh.isIntersect(*this);    }

bool Circle::isBelongPoint(float x, float y) const noexcept
{   return pow2(x - xc_) + pow2(y - yc_) <= pow2(r_);   }


Rectangle::Rectangle(std::shared_ptr<RastrumImage> rimage,
    float x, float y, float w, float h, rgba_t clr)
    : Shape(rimage, clr), w_(std::abs(w)), h_(std::abs(h))
{
    x_ = (w >= 0) ? x : (x + w);
    y_ = (h >= 0) ? y : (y + h);
}

void Rectangle::draw()
{
    const auto [width, height] = rimage_->getSize();
    const auto square = std::min(width, height);

    cint32_t px_x = (width + square * x_) / 2.0f;
    cint32_t px_y = (height - square * y_) / 2.0f;
    cint32_t px_w = w_ * square / 2.0f;
    cint32_t px_h = h_ * square / 2.0f;
    drawed_ = true;

    for (int32_t x = px_x; x < px_x + px_w; x++)
    for (int32_t y = px_y - px_h; y < px_y; y++)
        rimage_->setPixel(x, y, clr_);
}

Rectangle::~Rectangle() noexcept
{   if (!drawed_) draw();   }

bool Rectangle::isIntersect(Circle const& cir) const noexcept
{   return is_intersect(cir, *this); }
bool Rectangle::isIntersect(Rectangle const& rec) const noexcept
{   return is_intersect(rec, *this);  }
bool Rectangle::isIntersect(Shape const& sh) const noexcept
{   return sh.isIntersect(*this);    }

bool Rectangle::isBelongPoint(float x, float y) const noexcept
{   return (x_ <= x && x <= x_ + w_) && (y_ <= y && y <= y_ + h_); }

bool shapes::is_intersect(Circle const& cir1, Circle const& cir2)
{
    std::cout << "(Circle x Circle)\n";
    cfloat dxc  = cir1.xc_ - cir2.xc_;
    cfloat dyc  = cir1.yc_ - cir2.yc_;
    cfloat sumr = cir1.r_ + cir2.r_;
    return pow2(dxc) + pow2(dyc) <= pow2(sumr);
}
bool shapes::is_intersect(Circle const& cir, Rectangle const& rec)
{
    std::cout << "(Circle x Rectangle)\n";
    cfloat dxc  = std::abs(cir.xc_ - (rec.x_ + rec.w_ / 2.0f));
    cfloat dyc  = std::abs(cir.yc_ - (rec.y_ + rec.h_ / 2.0f));
    cfloat drig = dxc - rec.w_ / 2.0f;
    cfloat dtop = dyc - rec.h_ / 2.0f;

    if (drig > 0 && dtop > 0)
        return pow2(drig) + pow2(dtop) <= pow2(cir.r_);
    else if (drig > 0 && dtop <= 0)
        return drig <= cir.r_;
    else if (drig <= 0 && dtop > 0)
        return dtop <= cir.r_;
    else
        return drig <= 0 && dtop <= 0;
}
bool shapes::is_intersect(Rectangle const& rec1, Rectangle const& rec2)
{
    std::cout << "(Rectangle x Rectangle)\n";
    cfloat dxc = std::abs((rec1.x_ + rec1.w_ / 2.0f) - (rec2.x_ + rec2.w_ / 2.0f));
    cfloat dyc = std::abs((rec1.y_ + rec1.h_ / 2.0f) - (rec2.y_ + rec2.h_ / 2.0f));
    cfloat dw = (rec1.w_ / 2.0f) - (dxc - rec2.w_ / 2.0f);
    cfloat dh = (rec1.h_ / 2.0f) - (dyc - rec2.h_ / 2.0f);
    return dw >= 0 && dh >= 0;
}

std::pair<float,float> shapes::pixelToFloat(std::pair<uint32_t,uint32_t> size,
    uint32_t px_x, uint32_t px_y) noexcept
{
    const auto [width, height] = size;
    const auto square = std::min(width, height);
    cfloat x = (px_x - width / 2.0f) / (square / 2.0f);
    cfloat y = (height / 2.0f - px_y) / (square / 2.0f);
    return std::make_pair(x, y);
}

bool shapes::unionPixelSet(std::vector<bool>& pset, Shape const& sh)
{
    const auto size = sh.getRastrumImage()->getSize();
    bool isInter = false;

    for (size_t i = 0; i < pset.size(); i++)
    if (pset[i])
    {
        cuint32_t px_x = i % size.first;
        cuint32_t px_y = i / size.first;
        const auto [x,y] = pixelToFloat(size, px_x, px_y);
        sh.isBelongPoint(x,y) ? (isInter = true) : (pset[i] = false);
    }

    return isInter;
}

bool shapes::unionPixelSet(std::vector<bool>& pset,
                           std::vector<std::shared_ptr<Shape>> const& vsh,
                           std::vector<bool> const& bmap)
{
    if (vsh.size() == 0) return false;
    const auto size = vsh[0]->getRastrumImage()->getSize();
    bool isInter = false;
    bool isBitmap = bmap.size() == vsh.size();

    for (size_t i = 0; i < pset.size(); i++)
    if (pset[i] )
    {
        bool isBelong = true;
        cuint32_t px_x = i % size.first;
        cuint32_t px_y = i / size.first;
        const auto [x,y] = pixelToFloat(size, px_x, px_y);

        for (size_t j = 0; j < vsh.size() && isBelong; j++)
            if ((isBitmap && bmap[j]) || !isBitmap)
                isBelong = vsh[j]->isBelongPoint(x,y);
        isBelong ? (isInter = true) : (pset[i] = false);
    }

    return isInter;
}

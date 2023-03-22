#pragma once
#include <memory>
#include <cmath>
#include "rastrum_image.hpp"

namespace shapes {

namespace {

using RastrumImage  = ::rastrum_image::RastrumImage;
using rgba_t        = ::rastrum_image::rgba_t;

} // namespace

struct Circle;
struct Rectangle;

struct Shape
{
    virtual void draw() = 0;
    virtual bool isIntersect(Shape const& sh) const noexcept = 0;
    virtual bool isIntersect(Circle const& cir) const noexcept = 0;
    virtual bool isIntersect(Rectangle const& rec) const noexcept = 0;
    virtual bool isBelongPoint(float x, float y) const noexcept = 0;
    virtual ~Shape() noexcept = default;

    const std::shared_ptr<RastrumImage> getRastrumImage() const noexcept;
    rgba_t getClr() const noexcept;
protected:
    Shape(std::shared_ptr<RastrumImage> rimage, rgba_t clr);
    std::shared_ptr<RastrumImage> rimage_;
    rgba_t clr_;
    bool drawed_ = false;
};

struct Circle final : Shape
{
    Circle(std::shared_ptr<RastrumImage> rimage,
           float xc, float yc, float r, rgba_t clr);
    ~Circle() noexcept override;

    void draw() override;
    bool isIntersect(Shape const& sh) const noexcept override;
    bool isIntersect(Circle const& cir) const noexcept override;
    bool isIntersect(Rectangle const& rec) const noexcept override;
    bool isBelongPoint(float x, float y) const noexcept override;

    friend bool is_intersect(Circle const& cir1, Circle const& cir2);
    friend bool is_intersect(Circle const& cir, Rectangle const& rec);
    friend bool is_intersect(Rectangle const& rec1, Rectangle const& rec2);
private:
    float xc_, yc_, r_;
};

struct Rectangle final : Shape
{
    Rectangle(std::shared_ptr<RastrumImage> rimage,
              float x, float y, float w, float h, rgba_t clr);
    ~Rectangle() noexcept override;

    void draw() override;
    bool isIntersect(Shape const& sh) const noexcept override;
    bool isIntersect(Circle const& cir) const noexcept override;
    bool isIntersect(Rectangle const& rec) const noexcept override;
    bool isBelongPoint(float x, float y) const noexcept override;

    friend bool is_intersect(Circle const& cir1, Circle const& cir2);
    friend bool is_intersect(Circle const& cir, Rectangle const& rec);
    friend bool is_intersect(Rectangle const& rec1, Rectangle const& rec2);
private:
    float x_, y_, w_, h_;
};

bool is_intersect(Circle const& cir1, Circle const& cir2);
bool is_intersect(Circle const& cir, Rectangle const& rec);
bool is_intersect(Rectangle const& rec1, Rectangle const& rec2);

std::pair<float,float> pixelToFloat(std::pair<uint32_t,uint32_t> size,
                                    uint32_t px_x, uint32_t px_y) noexcept;

bool unionPixelSet(std::vector<bool>& pset, Shape const& sh);
bool unionPixelSet(std::vector<bool>& pset, std::vector<std::shared_ptr<Shape>> const& vsh,
                   std::vector<bool> const& bmap = {});

} // namespace shapes

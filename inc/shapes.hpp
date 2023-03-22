#pragma once
#include <memory>
#include "rastrum_image.hpp"

namespace shapes {

namespace {

using RastrumImage  = ::rastrum_image::RastrumImage;
using rgba_t        = ::rastrum_image::rgba_t;
using pf32_t        = ::rastrum_image::pf32_t;

} // namespace

struct Circle;
struct Rectangle;

class Shape
{
public:
    virtual void draw() = 0;
    virtual bool isIntersect(Shape const& sh) const noexcept = 0;
    virtual bool isIntersect(Circle const& cir) const noexcept = 0;
    virtual bool isIntersect(Rectangle const& rec) const noexcept = 0;
    virtual bool isBelongPoint(float x, float y) const noexcept = 0;
    virtual ~Shape() = default;

    Shape(Shape const& sh) = delete;
    Shape(Shape && sh) = delete;
    Shape& operator=(Shape const& sh) = delete;
    Shape& operator=(Shape && sh) = delete;

    std::shared_ptr<const RastrumImage> getRastrumImage() const noexcept;
    rgba_t getClr() const noexcept;
protected:
    Shape(std::shared_ptr<RastrumImage> rimage, rgba_t clr);
    std::shared_ptr<RastrumImage> rimage_;
    rgba_t clr_;
    bool drawed_ = false;
};

class Circle final : public Shape
{
public:
    Circle(std::shared_ptr<RastrumImage> rimage,
           float xc, float yc, float r, rgba_t clr);
    ~Circle() override;

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

class Rectangle final : public Shape
{
public:
    Rectangle(std::shared_ptr<RastrumImage> rimage,
              float x, float y, float w, float h, rgba_t clr);
    ~Rectangle() override;

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

pf32_t pixelToFloat(pf32_t size, uint32_t px_x, uint32_t px_y) noexcept;

bool unionPixelSet(std::vector<bool>& pset, Shape const& sh);
bool unionPixelSet(std::vector<bool>& pset,
                   std::vector<std::shared_ptr<Shape>> const& vsh,
                   std::vector<bool> const& bmap = {});

} // namespace shapes

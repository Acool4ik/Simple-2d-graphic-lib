#include "../inc/shapes.hpp"
#include "../inc/backtrack.hpp"

using namespace rastrum_image;
using namespace shapes;

// Example 1: how would need implement visitor pattern
struct VisitorImpl final : Visitor
{
    void visit(PngImage& image) const override
    {
        // it draws cells
        const auto [w, h] = image.getSize();
        for (uint32_t x = 0; x < w; x++)
        for (uint32_t y = 0; y < h; y++)
            if (x % 64U == 0 || y % 64U == 0)
                image.setPixel(x, y, clrs::black());
    }
};

// Example 2: how would need generate pixel set for cansav
void generatePixelSet(std::unordered_set<pu32_t>& pset, uint32_t w, uint32_t h)
{
    // it draws inclined lines
    for (uint32_t x = 0; x < w; x++)
    for (uint32_t y = 0; y < h; y++)
        if ((x + y) % 64U == 0) pset.insert({x,y});
}

int main()
{
    const char * fname = "result.png";
    const uint32_t w = 1920U;
    const uint32_t h = 1080U;

    // STEP1: create canvas
    auto canvas = std::make_shared<PngImage>(fname, w, h, clrs::gray());


    // STEP2: create shapes for canvas
    std::vector<std::shared_ptr<shapes::Shape>> shapes
    {
        // props:                         xc,   yc,   r,    color
        std::make_shared<Circle>(canvas, +0.4, +0.4, +0.5,  clrs::red()),
        std::make_shared<Circle>(canvas, -0.4, +0.4, +0.5,  clrs::green()),
        std::make_shared<Circle>(canvas, -0.4, -0.4, -0.5,  clrs::blue()),
        std::make_shared<Circle>(canvas, +0.4, -0.4, +0.5,  clrs::yellow()),
        std::make_shared<Circle>(canvas, +0.9, +0.0, +0.5,  clrs::white()),
        std::make_shared<Circle>(canvas, -0.9, +0.0, +0.5,  clrs::black()),
        // props:                            xc,   yc,   w,    h,   color
        std::make_shared<Rectangle>(canvas, +0.2, +0.2, +0.4, +0.4, clrs::yellow()),
        std::make_shared<Rectangle>(canvas, -0.2, +0.2, -0.4, +0.4, clrs::blue()),
        std::make_shared<Rectangle>(canvas, -0.2, -0.2, -0.4, -0.4, clrs::green()),
        std::make_shared<Rectangle>(canvas, +0.2, -0.2, +0.4, -0.4, clrs::red())
    };


    // STEP3: apply all shapes
    for (size_t i = 0; i < shapes.size(); i++)
        shapes[i]->draw();


    // STEP4* (advansed): visualisation of inclusion–exclusion principle
    std::vector<bool> used(shapes.size(), false);
    std::vector<bool> pixelSet1(w * h, true);
    std::function<void()> intersect = [&canvas,&shapes,&used,&pixelSet1]()
    {
        pixelSet1.assign(pixelSet1.size(), true);
        if (unionPixelSet(pixelSet1, shapes, used))
        {
            rgba_t mixClr = {0,0,0,0};
            for (size_t i = 0; i < used.size(); i++)
                if (used[i] == true)
                    mixClr = clrs::mixClrs(mixClr, shapes[i]->getClr());
            canvas->setPixelSet(pixelSet1, clrs::gammaCorr(mixClr));
        }
    };

    for (size_t n = 2; n <= 3; n++)
        bt::reqBacktrack(0, n, 0, used, intersect);


    // STEP5: apply filter, complexity shape, procedure textures, etc...
    std::unordered_set<pu32_t> pixelSet2;
    generatePixelSet(pixelSet2, w, h);

    canvas->setPixelSet(pixelSet2, clrs::black());  // method 1
    canvas->visit(VisitorImpl());                   // method 2
    return 0;
}

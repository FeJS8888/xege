#include "feege.h"
#include "ege_head.h"
#include "ege_common.h"

// Helper defined in egegapi.cpp to construct rounded-rectangle GraphicsPath objects.
Gdiplus::GraphicsPath* createRoundRectPath(float x, float y, float w, float h,
    float radius1, float radius2, float radius3, float radius4);

namespace ege
{

void EGEAPI ege_setclippath(const ege_path* path, PIMAGE pimg)
{
    PIMAGE img = CONVERT_IMAGE(pimg);
    if (img && path) {
        const Gdiplus::GraphicsPath* graphicsPath = (const Gdiplus::GraphicsPath*)path->data();
        if (graphicsPath) {
            Gdiplus::Graphics* graphics = img->getGraphics();
            if (graphics) {
                graphics->SetClip(graphicsPath, Gdiplus::CombineModeReplace);
            }
        }
    }
    CONVERT_IMAGE_END;
}

void EGEAPI ege_resetclippath(PIMAGE pimg)
{
    PIMAGE img = CONVERT_IMAGE(pimg);
    if (img) {
        Gdiplus::Graphics* graphics = img->getGraphics();
        if (graphics) {
            graphics->ResetClip();
        }
    }
    CONVERT_IMAGE_END;
}

void EGEAPI ege_path_addroundrect(ege_path* path, float x, float y, float width, float height, float radius)
{
    if (path != NULL) {
        ege_path_addroundrect(path, x, y, width, height, radius, radius, radius, radius);
    }
}

void EGEAPI ege_path_addroundrect(ege_path* path, float x, float y, float width, float height,
    float radius1, float radius2, float radius3, float radius4)
{
    if (path != NULL) {
        Gdiplus::GraphicsPath* graphicsPath = (Gdiplus::GraphicsPath*)path->data();
        if (graphicsPath != NULL) {
            Gdiplus::GraphicsPath* roundRect = createRoundRectPath(x, y, width, height, radius1, radius2, radius3, radius4);
            if (roundRect != NULL) {
                graphicsPath->AddPath(roundRect, false);
                delete roundRect;
            }
        }
    }
}

} // namespace ege

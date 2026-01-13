#include "feege.h"
#include "ege_head.h"
#include "ege_common.h"

static Gdiplus::GraphicsPath* createRoundRectPath(float x, float y, float w, float h,
    float radius1, float radius2, float radius3, float radius4)
{
    if ((w <= 0.0f) || (h <= 0.0f))
        return NULL;

    radius1 = clamp(radius1, 0.0f, MIN(w, h));
    radius2 = clamp(radius2, 0.0f, MIN(w - radius1, h));
    radius3 = clamp(radius3, 0.0f, MIN(h - radius2, w));
    radius4 = clamp(radius4, 0.0f, MIN(h - radius1, w - radius3));

    Gdiplus::GraphicsPath* path = new Gdiplus::GraphicsPath;

    if (radius2 < w - radius1)
        path->AddLine(x + radius1,  y,  x + w - radius2,  y);

    if (radius2 > 0.0f)
        path->AddArc (x + w - (radius2 * 2),  y,  radius2 * 2,  radius2 * 2,  270,  90);

    if (radius3 < h - radius2)
        path->AddLine(x + w,  y + radius2,  x + w,  y + h - radius3);

    if (radius3 > 0.0f)
        path->AddArc (x + w - (radius3 * 2),  y + h - (radius3 * 2),  radius3 * 2,  radius3 * 2,  0,  90);

    if (radius4 < w - radius3)
        path->AddLine(x + w - radius3,  y + h,  x + radius4,  y + h);

    if (radius4 > 0.0f)
        path->AddArc (x,  y + h - (radius4 * 2),  radius4 * 2,  radius4 * 2,  90,  90);

    if (radius4 < w - radius1)
        path->AddLine(x,  y + h - radius4 ,  x,  y + radius1);

    if (radius1 > 0.0f)
        path->AddArc (x,  y, radius1 * 2,  radius1 * 2,  180,  90);

    path->CloseFigure();

    return path;
}

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

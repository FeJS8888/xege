#include "feege.h"
#include "ege_head.h"

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

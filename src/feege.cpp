#include "feege.h"
#include "ege_head.h"
#include "ege_common.h"
#include <memory>
#include <unordered_map>

namespace ege
{

namespace
{

struct AaClipState
{
    std::unique_ptr<IMAGE>                 backup;
    std::unique_ptr<Gdiplus::GraphicsPath> path;
};

std::unordered_map<IMAGE*, AaClipState> g_aaclip_states;

inline BYTE blend_component(BYTE fg, BYTE bg, BYTE alpha)
{
    return static_cast<BYTE>((fg * alpha + bg * (255 - alpha) + 128) >> 8);
}

void apply_aaclip_mask(IMAGE* img, const IMAGE* backup, const Gdiplus::GraphicsPath* path)
{
    if ((img == NULL) || (backup == NULL) || (path == NULL)) {
        return;
    }

    const int width  = img->getwidth();
    const int height = img->getheight();

    if ((width <= 0) || (height <= 0) || (backup->getwidth() != width) || (backup->getheight() != height)) {
        return;
    }

    gdiplusinit();

    Gdiplus::Bitmap mask(width, height, PixelFormat32bppARGB);
    Gdiplus::Rect   maskRect(0, 0, width, height);
    {
        Gdiplus::Graphics g(&mask);
        g.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
        g.Clear(Gdiplus::Color(0, 0, 0, 0));
        Gdiplus::SolidBrush brush(Gdiplus::Color(255, 255, 255, 255));
        g.FillPath(&brush, path);
    }

    Gdiplus::BitmapData maskData;
    if (mask.LockBits(&maskRect,
            Gdiplus::ImageLockModeRead,
            PixelFormat32bppARGB,
            &maskData) != Gdiplus::Ok) {
        return;
    }

    BYTE*        maskRow = static_cast<BYTE*>(maskData.Scan0);
    const LONG   stride  = maskData.Stride;
    color_t*     dst     = reinterpret_cast<color_t*>(img->m_pBuffer);
    const color_t* src   = reinterpret_cast<const color_t*>(backup->m_pBuffer);

    for (int y = 0; y < height; ++y) {
        BYTE* row = maskRow + y * stride;
        for (int x = 0; x < width; ++x) {
            const BYTE alpha = row[x * 4 + 3];
            color_t&    dstPixel = dst[y * width + x];

            if (alpha == 255) {
                continue;
            }

            if (alpha == 0) {
                dstPixel = src[y * width + x];
                continue;
            }

            const color_t srcPixel = src[y * width + x];

            BYTE fgA = static_cast<BYTE>((dstPixel >> 24) & 0xFF);
            BYTE fgR = static_cast<BYTE>((dstPixel >> 16) & 0xFF);
            BYTE fgG = static_cast<BYTE>((dstPixel >> 8) & 0xFF);
            BYTE fgB = static_cast<BYTE>(dstPixel & 0xFF);

            BYTE bgA = static_cast<BYTE>((srcPixel >> 24) & 0xFF);
            BYTE bgR = static_cast<BYTE>((srcPixel >> 16) & 0xFF);
            BYTE bgG = static_cast<BYTE>((srcPixel >> 8) & 0xFF);
            BYTE bgB = static_cast<BYTE>(srcPixel & 0xFF);

            BYTE outA = blend_component(fgA, bgA, alpha);
            BYTE outR = blend_component(fgR, bgR, alpha);
            BYTE outG = blend_component(fgG, bgG, alpha);
            BYTE outB = blend_component(fgB, bgB, alpha);

            dstPixel = (static_cast<color_t>(outA) << 24) |
                (static_cast<color_t>(outR) << 16) |
                (static_cast<color_t>(outG) << 8) |
                static_cast<color_t>(outB);
        }
    }

    mask.UnlockBits(&maskData);
}

} // namespace

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
        g_aaclip_states.erase(img);
    }
    CONVERT_IMAGE_END;
}

void EGEAPI ege_beginaaclip(const ege_path* path, PIMAGE pimg)
{
    PIMAGE img = CONVERT_IMAGE(pimg);
    if (img && path) {
        const Gdiplus::GraphicsPath* graphicsPath = (const Gdiplus::GraphicsPath*)path->data();
        if (graphicsPath) {
            AaClipState state;
            state.backup = std::make_unique<IMAGE>(img->getwidth(), img->getheight());
            state.backup->copyimage(img);
            state.path.reset(graphicsPath->Clone());
            g_aaclip_states[img] = std::move(state);
        }
    }
    CONVERT_IMAGE_END;
}

void EGEAPI ege_endaaclip(PIMAGE pimg)
{
    PIMAGE img = CONVERT_IMAGE(pimg);
    if (img) {
        auto it = g_aaclip_states.find(img);
        if (it != g_aaclip_states.end()) {
            if (it->second.path && it->second.backup) {
                apply_aaclip_mask(img, it->second.backup.get(), it->second.path.get());
            }
            g_aaclip_states.erase(it);
        }
    }
    CONVERT_IMAGE_END;
}

} // namespace ege

#pragma once

#include "ege.h"

namespace ege
{

/// @brief Set the GDI+ clipping region of the target image to the specified path.
/// @param path Path describing the clipping region
/// @param pimg Target image, NULL for current working image
void EGEAPI ege_setclippath(const ege_path* path, PIMAGE pimg = NULL);

/// @brief Reset the GDI+ clipping region of the target image.
/// @param pimg Target image, NULL for current working image
void EGEAPI ege_resetclippath(PIMAGE pimg = NULL);

/// @brief Begin an anti-aliased clipping session using the specified path.
///        Subsequent drawing will be clipped, and edges will be smoothed when ended.
/// @param path Path describing the clipping region
/// @param pimg Target image, NULL for current working image
void EGEAPI ege_beginaaclip(const ege_path* path, PIMAGE pimg = NULL);

/// @brief End the anti-aliased clipping session started by ege_beginaaclip.
/// @param pimg Target image, NULL for current working image
void EGEAPI ege_endaaclip(PIMAGE pimg = NULL);
} // namespace ege

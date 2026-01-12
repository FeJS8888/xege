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

/// @brief Add rounded rectangle to path with uniform corner radius.
/// @param path Path object pointer
/// @param x Rectangle top-left corner x coordinate
/// @param y Rectangle top-left corner y coordinate
/// @param width Rectangle width
/// @param height Rectangle height
/// @param radius Corner radius (applied to all corners)
void EGEAPI ege_path_addroundrect(ege_path* path, float x, float y, float width, float height, float radius);

/// @brief Add rounded rectangle to path with independent corner radii.
/// @param path Path object pointer
/// @param x Rectangle top-left corner x coordinate
/// @param y Rectangle top-left corner y coordinate
/// @param width Rectangle width
/// @param height Rectangle height
/// @param radius1 Top-left corner radius
/// @param radius2 Top-right corner radius
/// @param radius3 Bottom-right corner radius
/// @param radius4 Bottom-left corner radius
void EGEAPI ege_path_addroundrect(ege_path* path, float x, float y, float width, float height,
    float radius1, float radius2, float radius3, float radius4);
} // namespace ege

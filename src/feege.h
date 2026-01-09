#pragma once

#include "ege.h"

/// @brief Set the GDI+ clipping region of the target image to the specified path.
/// @param path Path describing the clipping region
/// @param pimg Target image, NULL for current working image
void EGEAPI ege_setclippath(const ege_path* path, PIMAGE pimg = NULL);

/// @brief Reset the GDI+ clipping region of the target image.
/// @param pimg Target image, NULL for current working image
void EGEAPI ege_resetclippath(PIMAGE pimg = NULL);

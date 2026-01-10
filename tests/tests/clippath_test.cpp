/*
 * ege_setclippath / ege_resetclippath functional test
 */

#define SHOW_CONSOLE 1
#include "ege.h"
#include "../test_framework.h"
#include "../performance_timer.h"
#include "../../src/feege.h"

using namespace ege;

int main()
{
    TestFramework framework;

    if (!framework.initialize(200, 200)) {
        return 1;
    }
    framework.hideWindow();

    framework.addTestCase("clippath_basic", "verify clipping and reset behavior", []() -> bool {
        const int width = 20;
        const int height = 20;

        PIMAGE img = newimage(width, height);
        TEST_ASSERT(img != nullptr, "failed to create image");

        settarget(img);
        setbkcolor(WHITE);
        cleardevice();

        ege_path path;
        ege_path_addrect(&path, 0.0f, 0.0f, 10.0f, static_cast<float>(height));

        ege_setclippath(&path, img);
        setfillcolor(GREEN);
        bar(0, 0, width - 1, height - 1);

        color_t inside = getpixel(5, 5, img);
        color_t outside = getpixel(15, 5, img);

        TEST_ASSERT(inside == GREEN, "clipped area not filled");
        TEST_ASSERT(outside == WHITE, "outside area should remain background");

        ege_resetclippath(img);
        setfillcolor(BLUE);
        bar(0, 0, width - 1, height - 1);

        color_t resetInside = getpixel(5, 5, img);
        color_t resetOutside = getpixel(15, 5, img);

        TEST_ASSERT(resetInside == BLUE, "reset clip failed (inside)");
        TEST_ASSERT(resetOutside == BLUE, "reset clip failed (outside)");

        settarget(nullptr);
        delimage(img);
        return true;
    });

    bool ok = framework.runAllTests();
    framework.cleanup();
    return ok ? 0 : 1;
}

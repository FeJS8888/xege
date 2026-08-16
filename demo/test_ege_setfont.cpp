/*
 * Test demo for ege_setfont function
 * This demo demonstrates the use of floating-point font sizes with GDI+
 * Font sizes are in pixels to match the existing setfont() behavior
 */

#include <graphics.h>
#include <stdio.h>

int main()
{
#ifdef EGE_GDIPLUS
    // Initialize graphics window
    initgraph(800, 600);
    setbkcolor(WHITE);
    ege_enable_aa(true); // Enable anti-aliasing for smoother text
    cleardevice();

    // Test 1: Basic ege_setfont with floating-point size
    settextcolor(BLACK);
    float sz = 10;
    while(is_run()){
        ege_setfont(sz,"Arial"); // Set font to Arial with size 24.5 pixels
        sz += 0.05;
        ege_outtextxy(100, 100, "Hello, EGE!"); // Draw text at (100, 100)
        delay_ms(10);
        cleardevice();
    }
    
    closegraph();
#endif
    return 0;
}

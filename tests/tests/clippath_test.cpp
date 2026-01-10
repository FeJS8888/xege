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
    SetProcessDPIAware();
    initgraph(800,600,INIT_RENDERMANUAL);
    ege_enable_aa(true);
    ege_path p;
    ege_path_addcircle(&p,200,200,100);
    ege_setclippath(&p);
    setcolor(GREEN);
    setfillcolor(GREEN);
    ege_fillrect(0,0,800,600);
    delay_ms(10000000);
    return 0;
}

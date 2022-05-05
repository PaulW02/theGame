#include "timers.h"

#define PUBLIC /* empty */
#define PRIVATE static


PUBLIC void timerUpdate(Soldier s){
    //Frame timer
    setSoldierFrameTimer(s, getSoldierFrameTimer(s) + 1);
}

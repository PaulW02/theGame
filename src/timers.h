#ifndef timers_h
#define timers_h

#include "application.h"
#include "player/soldier.h"
#include "collision/powers.h"
#include "handlers/playerhandler.h"

void timerUpdate(Soldier s, PowerUps p);
int countDown();

#endif
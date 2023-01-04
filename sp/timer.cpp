/**
 *  Author: Amélie Heinrich
 *  Company: Amélie Games
 *  License: MIT
 *  Create Time: 04/01/2023 23:34
 */

#include "timer.h"

#include <time.h>

void TimerInit(timer *Timer)
{   
    Timer->start = clock();
}

float TimerGetElapsed(timer *Timer)
{
    return clock() - Timer->start;
}

void TimerRestart(timer *Timer)
{
    Timer->start = clock();
}

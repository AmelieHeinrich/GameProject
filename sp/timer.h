/**
 *  Author: Amélie Heinrich
 *  Company: Amélie Games
 *  License: MIT
 *  Create Time: 04/01/2023 23:32
 */

#pragma once

struct timer
{
    float start;
};

void TimerInit(timer *Timer);
float TimerGetElapsed(timer *Timer);
void TimerRestart(timer *Timer);

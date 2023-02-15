/**
 *  Author: Amélie Heinrich
 *  Company: Amélie Games
 *  License: MIT
 *  Create Time: 15/02/2023 21:17
 */

#include "rng_system.hpp"

#include <cmath>

struct rng_state
{
    int FirstValue;
    int SecondValue;
    int ThirdValue;
};

rng_state RngState;

void RngInit(float Value)
{
    RngState.FirstValue = 100 * Value;
    RngState.SecondValue = round(Value / 100);
    RngState.ThirdValue = RngState.SecondValue * 69;
}

double RngGenerate()
{
    RngState.FirstValue =  (171 * RngState.FirstValue) % 30269;
    RngState.SecondValue = (172 * RngState.SecondValue) % 30307;
    RngState.ThirdValue =  (170 * RngState.ThirdValue) % 30323;

    return fmod(RngState.FirstValue / 30269.0 + RngState.SecondValue / 30307.0 + RngState.ThirdValue / 30323, 1.0);
}
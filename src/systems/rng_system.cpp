/**
 *  Author: Amélie Heinrich
 *  Company: Amélie Games
 *  License: MIT
 *  Create Time: 15/02/2023 21:17
 */

#include "rng_system.hpp"

#include <cmath>

double RngGenerate()
{
    static int FirstValue = 100;
    static int SecondValue = 100;
    static int ThirdValue = 100;

    FirstValue = (171 * FirstValue) % 30269;
    SecondValue = (172 * SecondValue) % 30307;
    ThirdValue = (170 * ThirdValue) % 30323;

    return fmod(FirstValue / 30269.0 + SecondValue / 30307.0 + ThirdValue / 30323, 1.0);
}
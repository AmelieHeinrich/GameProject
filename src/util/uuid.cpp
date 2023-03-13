/**
 *  Author: Amélie Heinrich
 *  Company: Amélie Games
 *  License: MIT
 *  Create Time: 13/03/2023 23:59
 */

#include "uuid.hpp"

#include "systems/rng_system.hpp"

uuid NewUUID()
{
    uuid ID;
    ID.ID = RngGenerate() * 1000000000;
    return ID;
}

/**
 *  Author: Amélie Heinrich
 *  Company: Amélie Games
 *  License: MIT
 *  Create Time: 13/03/2023 23:57
 */

#pragma once

#include <cstdint>

struct uuid
{
    uint64_t ID;

    operator uint64_t() const { return ID; }

    void operator=(const uuid& Other)
    {
        ID = Other.ID;
    }
};

uuid NewUUID();

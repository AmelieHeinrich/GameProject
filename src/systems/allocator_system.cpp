/**
 *  Author: Amélie Heinrich
 *  Company: Amélie Games
 *  License: MIT
 *  Create Time: 16/02/2023 12:55
 */

#include "allocator_system.hpp"

#include "log_system.hpp"

void LinearAllocatorInit(linear_allocator *Allocator, uint64_t Size)
{
    Allocator->Size = Size;
    Allocator->Start = 0;
    Allocator->Current = Allocator->Start;
    Allocator->End = Size;
}

void LinearAllocatorFree(linear_allocator *Allocator)
{
    Allocator->Size = 0;
}

uint64_t LinearAllocatorAlloc(linear_allocator *Allocator, uint64_t Size)
{
    int Pointer = Allocator->Current + Size;
    if (Pointer >= Allocator->End)
    {
        LogError("Linear Allocator: OUT OF MEMORY!");
        return 0;
    }
    Allocator->Current += Size;
    return Pointer;
}

void LinearAllocatorFree(linear_allocator *Allocator, uint64_t Offset)
{
    (void)Allocator;
    (void)Offset;
}

void LinearAllocatorReset(linear_allocator *Allocator)
{
    Allocator->Current = Allocator->Start;
}

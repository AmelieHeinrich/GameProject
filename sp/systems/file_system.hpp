/**
 *  Author: Amélie Heinrich
 *  Company: Amélie Games
 *  License: MIT
 *  Create Time: 10/01/2023 09:42
 */

#pragma once

#include <cstdint>
#include <string>

struct file_buffer
{
    void *Data;
    uint64_t Size;
};

bool FileBufferExists(const std::string& Path);
uint64_t FileBufferGetSize(const std::string& Path);
void FileBufferRead(const std::string& Path, file_buffer *Buffer);
void FileBufferFree(file_buffer *Buffer);

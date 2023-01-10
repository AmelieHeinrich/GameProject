/**
 *  Author: Amélie Heinrich
 *  Company: Amélie Games
 *  License: MIT
 *  Create Time: 10/01/2023 09:44
 */

#include "file_system.hpp"

#include <Windows.h>
#include <sys/stat.h>
#include <stdio.h>

bool FileBufferExists(const std::string& Path)
{
    struct stat Info;
    if (stat(Path.c_str(), &Info) != 0)
        return false;
    return true;
}

uint64_t FileBufferGetSize(const std::string& Path)
{
    if (!FileBufferExists(Path))
        return 0;
    struct stat Info;
    stat(Path.c_str(), &Info);
    return Info.st_size;
}

void FileBufferRead(const std::string& Path, file_buffer *Buffer)
{
    if (!FileBufferExists(Path))
        return;
    Buffer->Size = FileBufferGetSize(Path);
    Buffer->Data = malloc(Buffer->Size);
    FILE* File = fopen(Path.c_str(), "r");
    fread(Buffer->Data, Buffer->Size, 1, File);
    fclose(File);
}

void FileBufferFree(file_buffer *Buffer)
{
    free(Buffer->Data);
}

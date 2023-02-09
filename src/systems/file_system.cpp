/**
 *  Author: Amélie Heinrich
 *  Company: Amélie Games
 *  License: MIT
 *  Create Time: 10/01/2023 09:44
 */

#include "file_system.hpp"

#include <Windows.h>
#include <sstream>
#include <fstream>

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
    std::ifstream FileStream(Path);
    auto Size = FileBufferGetSize(Path);
    FileStream.read(Buffer->Data.data(), Size);
    FileStream.close();
}

std::string FileRead(const std::string& Path)
{
    if (!FileBufferExists(Path))
        return "";
    std::ifstream Stream(Path);
    std::stringstream StringStream;
    StringStream << Stream.rdbuf();
    Stream.close();
    return StringStream.str();
}

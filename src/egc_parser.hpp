/**
 *  Author: Amélie Heinrich
 *  Company: Amélie Games
 *  License: MIT
 *  Create Time: 08/01/2023 17:00
 */

#pragma once

#include <map>
#include <vector>
#include <string>

enum class egc_variable_type
{
    String,
    Int,
    Boolean,
    Float
};

struct egc_variable
{
    egc_variable_type Type;
    std::string str;
    int i32;
    bool b32;
    float f32;
};

struct egc_file
{
    std::string Header;
    std::map<std::string, egc_variable> Variables;
};

#define EgcI32(File, Name) reinterpret_cast<egc_file*>(&File)->Variables[Name].i32
#define EgcStr(File, Name) reinterpret_cast<egc_file*>(&File)->Variables[Name].str
#define EgcF32(File, Name) reinterpret_cast<egc_file*>(&File)->Variables[Name].f32
#define EgcB32(File, Name) reinterpret_cast<egc_file*>(&File)->Variables[Name].b32

void EgcParseFile(const std::string& Path, egc_file *File);
void EgcWriteFile(const std::string& Path, egc_file *File);

/**
 *  Author: Amélie Heinrich
 *  Company: Amélie Games
 *  License: MIT
 *  Create Time: 08/01/2023 17:02
 */

#include "egc_parser.hpp"

#include "systems/log_system.hpp"

#include <fstream>
#include <sstream>

std::string GetSubstringBefore(const std::string& String, char Character)
{
    std::string::size_type Position = String.find(Character);
    if (Position != std::string::npos)
        return String.substr(0, Position);
    else
        return String;
}

std::string GetSubstringAfter(const std::string& String, char Character)
{
    std::string::size_type Position = String.find(Character);
    if (Position != std::string::npos)
        return String.substr(Position + 1, String.length());
    else
        return String;
}

egc_variable_type GetTypeFromString(const std::string& String)
{
    if (String == "i32")
        return egc_variable_type::Int;
    if (String == "str")
        return egc_variable_type::String;
    if (String == "b32")
        return egc_variable_type::Boolean;
    if (String == "f32")
        return egc_variable_type::Float;
}

std::string GetStringFromType(egc_variable_type Type)
{
    switch (Type)
    {
        case egc_variable_type::Boolean:
            return "(b32)";
        case egc_variable_type::Float:
            return "(f32)";
        case egc_variable_type::String:
            return "(str)";
        case egc_variable_type::Int:
            return "(i32)";
    }
}

std::string ConvertVariableStr(egc_variable Variable)
{
    switch (Variable.Type)
    {
        case egc_variable_type::Boolean:
        {
            return Variable.b32 ? "true" : "false";
        }
        case egc_variable_type::Float:
        {
            char Buffer[512];
            sprintf(Buffer, "%.1f", Variable.f32);
            return std::string(Buffer);
        }
        case egc_variable_type::Int:
        {
            char Buffer[512];
            sprintf(Buffer, "%d", Variable.i32);
            return std::string(Buffer);
        }
        case egc_variable_type::String:
        {
            return Variable.str;
        }
    }
}

void EgcParseFile(const std::string& Path, egc_file *File)
{
    std::ifstream FileStream(Path);
    if (!FileStream.is_open())
    {
        LogError("Failed to open .egc file: %s", Path.c_str());
        return;
    }

    std::string Line;
    while (std::getline(FileStream, Line))
    {
        if (Line[0] == '[')
        {
            File->Header = Line;
            continue;
        }
        std::string VariableName = GetSubstringBefore(Line, '(');
        std::string VariableType = GetSubstringBefore(GetSubstringAfter(Line, '('), ')');
        std::string VariableValue = GetSubstringAfter(Line, '=');
        
        egc_variable_type Type = GetTypeFromString(VariableType);
        switch (Type)
        {
            case egc_variable_type::Boolean:
            {
                File->Variables[VariableName].Type = egc_variable_type::Boolean;
                File->Variables[VariableName].b32 = VariableValue == "false" ? false : true;
            } break;
            case egc_variable_type::String:
            {
                File->Variables[VariableName].Type = egc_variable_type::String;
                File->Variables[VariableName].str = VariableValue;
            } break;
            case egc_variable_type::Float:
            {
                File->Variables[VariableName].Type = egc_variable_type::Float;
                File->Variables[VariableName].f32 = std::atof(VariableValue.c_str());
            } break;
            case egc_variable_type::Int:
            {
                File->Variables[VariableName].Type = egc_variable_type::Int;
                File->Variables[VariableName].i32 = std::atoi(VariableValue.c_str());
            } break;
        }
    }
    
    FileStream.close();

    LogInfo("Parsed EGC file (%s)", Path.c_str());
}

void EgcWriteFile(const std::string& Path, egc_file *File)
{
    std::ofstream FileStream(Path, std::ios_base::trunc);
    FileStream << File->Header << std::endl;

    for (std::pair<std::string, egc_variable> Variable : File->Variables)
    {
        // NOTE(amelie.h): variable_name(type)=value
        FileStream << Variable.first << GetStringFromType(Variable.second.Type) << "=" << ConvertVariableStr(Variable.second) << std::endl;
    }

    FileStream.close();

    LogInfo("Wrote EGC file (%s)", Path.c_str());
}

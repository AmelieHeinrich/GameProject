/**
 *  Author: Amélie Heinrich
 *  Company: Amélie Games
 *  License: MIT
 *  Create Time: 04/01/2023 17:26
 */

#include "game_config.hpp"

#include <iostream>
#include <fstream>
#include <vector>

void GameConfigLoad(const std::string& Path, game_config_file *File)
{
    File->Path = Path;

    std::ifstream FileStream(Path);
    if (!FileStream.is_open()) 
    {
        // TODO(amelie.h): Error handling
    }

    std::vector<std::string> Lines;
    std::string Line;
    while (std::getline(FileStream, Line)) 
        Lines.push_back(Line);

    File->Width = std::atoi(Lines[0].c_str());
    File->Height = std::atoi(Lines[1].c_str());
    File->AssetPath = Lines[2];
}

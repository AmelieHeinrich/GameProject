/**
 *  Author: Amélie Heinrich
 *  Company: Amélie Games
 *  License: MIT
 *  Create Time: 04/01/2023 17:25
 */

#pragma once

#include <string>

struct game_config_file
{
    std::string Path;
    int Width;
    int Height;
    std::string AssetPath;
};

void GameConfigLoad(const std::string& Path, game_config_file *File);

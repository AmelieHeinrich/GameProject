/**
 *  Author: Amélie Heinrich
 *  Company: Amélie Games
 *  License: MIT
 *  Create Time: 22/02/2023 23:44
 */

#include "argument_validator.hpp"

#include <iostream>
#include <fstream>

bool ArgumentValidator::ValidateArguments(int ArgumentCount, char **ppArgumentValues)
{
    if (ArgumentCount != 4 && ArgumentCount != 2) {
        std::cout << "Invalid argument count! ./egs_compiler -h for help" << std::endl;
        return false;
    }
    if (ArgumentCount == 2) {
        if (ValidateHelper(ppArgumentValues[1]))
            return false;
        std::cout << "Invalid flag!" << std::endl;
        return false;
    }
    if (!ValidatePath(ppArgumentValues[1])) {
        std::cout << "Invalid path! " << ppArgumentValues[1] << std::endl;
        return false;
    }
    if (!ValidateMode(ppArgumentValues[3])) {
        std::cout << "Invalid mode! ./egs_compiler -h for help" << std::endl;
        return false;
    }
    return true;
}

bool ArgumentValidator::ValidateHelper(char *pString)
{
    if (strcmp(pString, "-h") == 0) {
        std::cout << "USAGE" << std::endl;
        std::cout << "\t./egs_compiler path output [-v -p -c]" << std::endl;
        std::cout << "DESCRIPTION" << std::endl;
        std::cout << "\tpath The path of the shader to compile." << std::endl;
        std::cout << "\toutput The output file." << std::endl;
        std::cout << "FLAGS" << std::endl;
        std::cout << "\t-v Compile a vertex shader." << std::endl;
        std::cout << "\t-p Compile a pixel shader." << std::endl;
        std::cout << "\t-c Compile a compute shader." << std::endl;
        return true;
    }
    return false;
}

bool ArgumentValidator::ValidateMode(char *pString)
{
    if (strcmp(pString, "-v") == 0)
        return true;
    if (strcmp(pString, "-p") == 0)
        return true;
    if (strcmp(pString, "-c") == 0)
        return true;
    return false;
}

bool ArgumentValidator::ValidatePath(char *pString)
{
    std::fstream Stream(pString);
    if (!Stream.is_open())
        return false;
    Stream.close();
    return true;
}

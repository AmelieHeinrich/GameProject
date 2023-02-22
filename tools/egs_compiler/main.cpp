/**
 *  Author: Amélie Heinrich
 *  Company: Amélie Games
 *  License: MIT
 *  Create Time: 22/02/2023 23:43
 */

#include "argument_validator.hpp"

int main(int argc, char **argv)
{
    if (!ArgumentValidator::ValidateArguments(argc, argv))
        return -1;
}

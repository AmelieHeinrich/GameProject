/**
 *  Author: Amélie Heinrich
 *  Company: Amélie Games
 *  License: MIT
 *  Create Time: 22/02/2023 23:44
 */

#pragma once

class ArgumentValidator
{
public:
    static bool ValidateArguments(int ArgumentCount, char **ppArgumentValues);

private:
    static bool ValidateHelper(char *pString);
    static bool ValidateMode(char *pString);
    static bool ValidatePath(char *pString);
};

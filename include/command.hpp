#pragma once
#include <string>
#include <vector>
#include <array>

#include <variables.hpp>
#include <extras.hpp>
#include <io.hpp>
#include <ctype.h>

enum RETTYPE {
    SUCCESS,
    ARMQUIT,
};

int FindCommand(std::string command);
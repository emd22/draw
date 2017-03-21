#pragma once
#include <fstream>
#include <string>
#include <vector>
#include <iostream>

#include <variables.hpp>
#include <extras.hpp>

class IO {
public:
    static void Save(std::string filename);
    static void Load(std::string filename);
    static void ParseLoaded(std::string line);
    static void LoadPresets();
    static void Export(std::string filename);
    static void LoadExp(std::string filename);
    static void SaveToExp();
    static void ResetExp();
};
#pragma once
#include "romfs.hpp"
#include <map>
struct Config {
public:
    map<string, string> config;
    Config(ROMfs *rom, string fname);
    ~Config();
};

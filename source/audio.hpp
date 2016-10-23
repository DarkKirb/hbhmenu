#pragma once
#include <3ds.h>
#include "romfs.hpp"
#include <string>
using namespace std;
class Audio {
private:
    bool playing;
    ROMfs &rom;
public:
    uint8_t* stream;
    size_t size;
    Audio(ROMfs &rom);
    ~Audio();
    auto play(string file) -> void;
    auto stop() -> void;
};

#pragma once
#include <3ds.h>
#include <vector>
#include <string>
using namespace std;
class ROMfs {
private:
    vector<FILE*> openFiles;
public:
    ROMfs();
    ~ROMfs();
    auto open(string fname) -> uint32_t;
    auto close(uint32_t handle) -> bool;
    auto read(uint32_t handle, size_t bytes) -> uint8_t *;
    auto readTo(uint32_t handle, size_t bytes, uint8_t *buf) -> void;
    auto size(uint32_t handle) -> size_t;
};

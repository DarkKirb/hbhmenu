#include "romfs.hpp"
#include <3ds/romfs.h>
#include <iostream>
#include <cstdio>
#include <3ds/sdmc.h>
ROMfs::ROMfs() {
    Result rc = romfsInit();
    if(rc) {
        //cout << "Romfs init: " << rc << endl;
        throw nullptr;
    }
    rc = sdmcInit();
    if(rc) {
        //cout << "sdmc init: " << rc << " (Not critical)" << endl;
    }
}
ROMfs::~ROMfs() {
    for(uint32_t i=0;i<openFiles.size();i++) {
        if(openFiles[i])
            fclose(openFiles[i]);
    }
    romfsExit();
}
auto ROMfs::open(string fn) -> uint32_t {
    string fname=string("sdmc:/hbhmenu/")+fn;
    FILE* f=fopen(fname.c_str(),"r");
    if(!f) { //Fall back to the romfs file.
        fname=string("romfs:/")+fn;
        f=fopen(fname.c_str(),"r");
    }
    if(!f)
        throw f;
    for(uint32_t i=0;i<openFiles.size();i++) {
        if(!openFiles[i]) {
            openFiles[i]=f;
            return i;
        }
    }
    openFiles.push_back(f);
    return openFiles.size()-1;
}
auto ROMfs::close(uint32_t handle) -> bool {
    fclose(openFiles[handle]);
    openFiles[handle]=0;
    return true;
}
auto ROMfs::read(uint32_t handle, size_t size) -> uint8_t *{
    uint8_t *buf=new uint8_t[size+1];
    fread(buf, 1, size, openFiles[handle]);
    buf[size]=0;
    return buf;
}
void ROMfs::readTo(uint32_t handle, size_t bytes, uint8_t *buf) {
    fread(buf, 1, bytes, openFiles[handle]);
}
auto ROMfs::size(uint32_t handle) -> size_t {
    size_t orig=ftell(openFiles[handle]);
    fseek(openFiles[handle],0, SEEK_END);
    size_t size=ftell(openFiles[handle]);
    fseek(openFiles[handle],orig, SEEK_SET);
    return size;
}

#include "config.hpp"
#include "static.hpp"
#include <iostream>
using namespace std;
Config::Config(ROMfs *rom, string fname) {
    uint32_t fd=rom->open(fname);
    size_t size=rom->size(fd);
    char* x=(char*)rom->read(fd,size);
    string str=x;
    delete[] x;
    rom->close(fd);
    vector<string> lines=split(str, "\n");
    for(uint32_t i=0;i<lines.size();i++) {
        if(lines[i][0]=='#')
            continue;
        auto pos = lines[i].find("=");
        if(pos==std::string::npos)
            continue;
        string a = lines[i].substr(0, pos);
        lines[i].erase(0,pos+1);
        config[a]=lines[i];
    }
}
Config::~Config() {

}

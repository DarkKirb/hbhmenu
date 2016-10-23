#include "audio.hpp"
#include <iostream>
#include <cstring>
Audio::Audio(ROMfs &rom): rom(rom) {
    srvInit();
    aptInit();
    hidInit();
    csndInit();
}
Audio::~Audio() {
    if(playing)
        stop();
    if(stream)
        delete[] stream;
    csndExit();
    hidExit();
    aptExit();
    srvExit();
}
auto Audio::play(string file) -> void {
    if(playing)
        stop();
    uint32_t fd=rom.open(file);
    stream=(uint8_t*)linearAlloc(5292000);
    size=rom.size(fd)<5292000?rom.size(fd):5292000; //Maximum size: 2 minutes of 22kHz PCM (1 Channel)
    rom.readTo(fd, size,stream);
    rom.close(fd);
    playing=true;
    csndPlaySound(8, SOUND_FORMAT_16BIT | SOUND_REPEAT, 22050, 1, 0, stream, stream, size);
}
auto Audio::stop() -> void {
    if(!playing)
        return;
    csndExecCmds(true);
    CSND_SetPlayState(0x8, 0);
    memset(stream, 0, size);
    GSPGPU_FlushDataCache(stream, size);
    linearFree(stream);
    stream=nullptr;
    playing=false;
}

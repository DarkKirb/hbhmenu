#include "audio.hpp"
#include <iostream>
#include <cstring>
bool threadFinished=false;
auto audioThread(void * stream) -> void {
    cout << "Entering thread" << endl;
    Audio *aud=(Audio*)stream;
    csndPlaySound(8, SOUND_FORMAT_16BIT | SOUND_REPEAT, 22050, 1, 0, aud->stream, aud->stream, aud->size);
    cout << "Exiting thread" << endl;
    threadFinished=true;
    //threadExit(0);
}
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
    threadFinished=false;
    csndPlaySound(8, SOUND_FORMAT_16BIT | SOUND_REPEAT, 22050, 1, 0, stream, stream, size);
    //audioThread((void*)this);
    //threadCreate(::audioThread, (void*)this, 0x10000, 0x28, 0, true);
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
    /*for(int i=0;i<65536;i++) {
        if(threadFinished) {
            cout << "Thread ended..." << endl;
            break;
        }
    }*/
}

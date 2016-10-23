#include "romfs.hpp"
#include "config.hpp"
#include "compresslib/compress.hpp"
#include <iostream>
#include <cstdio>
#include <cstring>
#include <cstdlib>
ROMfs rom;
void printfile(string path)
{
    uint32_t fd=rom.open(path);
    size_t size=rom.size(fd);
    char * data=(char *)rom.read(fd, size);
    cout << data << endl;
    delete[] data;
    rom.close(fd);
}
bool csnd_inited=false;
int main(int argc, char **argv) {
    try {
        gfxInit(GSP_RGBA8_OES, GSP_RGBA8_OES, false);
        consoleInit(GFX_BOTTOM, NULL);
        u8* fbTop=gfxGetFramebuffer(GFX_TOP, GFX_LEFT, NULL, NULL);
        cout << "Initializing Homebrew hmenu" << endl;
        Config cfg(&rom, "config.cfg");
        uint32_t fd=rom.open(cfg.config["topScreenFlash"]);
        rom.readTo(fd,rom.size(fd),fbTop);
        rom.close(fd);
        printfile(cfg.config["msgFileLoc"]);
        gfxSwapBuffers();
        uint8_t *audioBuffer=nullptr;
        size_t audioSize=0;
        if(cfg.config["playBGAudio"]=="true") {
            srvInit();
            aptInit();
            cout << "Loading audio track" << endl;
            csndInit();
            csnd_inited=true;
            fd=rom.open(cfg.config["BGAudioPath"]);
            audioSize=rom.size(fd);
            audioBuffer=(uint8_t*)linearAlloc(audioSize);
            rom.readTo(fd, audioSize,audioBuffer);
            rom.close(fd);
            /*if(cfg.config["BGAudioFormat"]=="DSPADPCM") 
                csndPlaySound(8, SOUND_FORMAT_ADPCM | SOUND_REPEAT, 48000, 1, 0, audioBuffer, audioBuffer, audioSize);
            else*/
                csndPlaySound(8, SOUND_FORMAT_16BIT | SOUND_REPEAT, 22050, 1, 0, audioBuffer, audioBuffer, audioSize);
            
        }
        while(aptMainLoop()) {
            hidScanInput();
            if(hidKeysDown() & KEY_START) {
                break;
            }
    
            gfxFlushBuffers();
            gspWaitForVBlank();
        }
        gfxExit();
        if(csnd_inited) {
            csndExecCmds(true);
            CSND_SetPlayState(0x8, 0);
            GSPGPU_FlushDataCache(audioBuffer, audioSize);
            csndExit();
            linearFree(audioBuffer);
            aptExit();
            srvExit();
        }
        if(audioBuffer)
            delete[] audioBuffer;
        return 0;
    }catch(...) {
        puts("En error occurred...\n");
    }
}

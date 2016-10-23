#include "romfs.hpp"
#include "config.hpp"
#include "compresslib/compress.hpp"
#include <iostream>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include "audio.hpp"
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
        Audio audio(rom);
        uint32_t fd=rom.open(cfg.config["topScreenFlash"]);
        rom.readTo(fd,rom.size(fd),fbTop);
        rom.close(fd);
        printfile(cfg.config["msgFileLoc"]);
        gfxSwapBuffers();
        uint8_t *audioBuffer=nullptr;
        size_t audioSize=0;
        if(cfg.config["playBGAudio"]=="true") {
            audio.play(cfg.config["BGAudioPath"]);            
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
        return 0;
    }catch(...) {
        puts("En error occurred...\n");
    }
}

// Put at end of file
#include <zlib.h>
#include <fstream>
#include <string>
#include <sstream>
#include <cstdio>
#include <cassert>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include "compress.hpp"
using namespace std;
#define CHUNK 65536
auto gunzip(const char *fn) -> uint8_t* {
    uint8_t *decompressed=nullptr;
    cout << "Decompressing "<<fn<<endl;
    string fname=string("sdmc:/hbhmenu/")+fn;
    FILE* source=fopen(fname.c_str(),"r");
    if(!source) {
        cout << "Not found on SD." << endl;
        fname=string("romfs:/")+fn;
        source=fopen(fname.c_str(),"r");
        if(!source) {
            cout << "Not found on ROMFS." << endl;
            throw nullptr;
        }
    }
    int ret, flush;
    size_t last=0;
    decompressed=nullptr;
    unsigned have;
    z_stream strm;
    unsigned char in[CHUNK];
    unsigned char out[CHUNK];
    /* allocate inflate state */
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    strm.avail_in = 0;
    strm.next_in = Z_NULL;
    ret = inflateInit(&strm);
    if (ret != Z_OK)
        throw ret;
    /* decompress until deflate stream ends or end of file */
    do {
        cout << "Reading..." << endl;
        strm.avail_in = fread(in, 1, CHUNK, source);
        if (ferror(source)) {
            cout << "Error reading source" << endl;
            (void)inflateEnd(&strm);
            throw Z_ERRNO;
        }
        if (strm.avail_in == 0)
            break;
        strm.next_in = in;
        /* run inflate() on input until output buffer not full */
        do {
            strm.avail_out = CHUNK;
            strm.next_out = out;
            ret = inflate(&strm, Z_NO_FLUSH);
            if(ret==Z_STREAM_ERROR)
                cout << "Stream error" << endl;
            assert(ret != Z_STREAM_ERROR);  /* state not clobbered */
            switch (ret) {
            case Z_NEED_DICT:
                cout << "I need a dictionary" << endl;
                ret = Z_DATA_ERROR;     /* and fall through */
            case Z_DATA_ERROR:
                cout << "Data error" << endl;
            case Z_MEM_ERROR:
                cout << "Memory error" << endl;
                (void)inflateEnd(&strm);
                throw ret;
            }
            have = CHUNK - strm.avail_out;
            if(!decompressed) {
                cout << "Allocating " << have << " bytes of memory" << endl;
                decompressed=(uint8_t*)malloc(last+have+1);
            } else {
                cout << "Allocating " << have << " bytes of memory" << endl;
                uint8_t * tmp=(uint8_t*)realloc((void*)decompressed, last+have+1);
                if(tmp) {
                    cout << "Location of buffer changed!" << endl;
                    decompressed=tmp;
                }
            }
            memcpy(decompressed+last, out, have);
        } while (strm.avail_out == 0);
        /* done when inflate() says it's done */
    } while (ret != Z_STREAM_END);
    /* clean up and return */
    (void)inflateEnd(&strm);
    fclose(source);
}


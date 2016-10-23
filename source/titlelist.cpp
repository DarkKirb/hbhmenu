// Parts of the code are based off of https://github.com/Steveice10/FBI/blob/71e670edde3d513eb739dbd30d5df4df8e68a201/source/ui/section/task/listtitles.c
#include "titlelist.hpp"
#include <cstdlib>
#include <algorithm>
#include <iostream>
#include <cstring>
void Titles::insertTWL(FS_MediaType mediaType, u64 titleId) {
    u64 realTitleId=0;
    u16 version=0;
    u64 installedSize=0;
    char productCode[12] = {'\0'};
    u8 header[0x3B4] = {0};
    Result res = FSUSER_GetLegacyRomHeader(mediaType, titleId, header);
    AM_TitleEntry entry;
    if(R_SUCCEEDED(res = AM_GetTitleInfo(mediaType, 1, &titleId, &entry))) {
        realTitleId = titleId;
        AM_GetTitleProductCode(mediaType, titleId, productCode);
        version = entry.version;
        installedSize = entry.size;
    } else {
        memcpy(&realTitleId, &header[0x230], sizeof(realTitleId));
        memcpy(productCode, header, sizeof(productCode));
        version = header[0x01E];
        u32 size = 0;
        if((header[0x012] & 0x2) != 0) {
            memcpy(&size, &header[0x210], sizeof(size));
        } else {
            memcpy(&size, &header[0x080], sizeof(size));
        }
        installedSize=size;
    }
    cout << "Found TWL title "<<productCode << " with TID "<<realTitleId << "v"<<version<<" (Size "<<installedSize<<")"<<endl;
    if(R_SUCCEEDED(res)) {
        Title title;
        title.tid=realTitleId;
        title.twl=true;
        BNR *bnr=new BNR;
        if(bnr) {
            if(R_SUCCEEDED(FSUSER_GetLegacyBannerData(mediaType, titleId, (u8*) bnr))) {
                u8 systemLanguage = CFG_LANGUAGE_EN;
                CFGU_GetSystemLanguage(&systemLanguage);
                char titl[0x80*6] = {'\0'}; //0x80 characters up to 6 bytes each
                utf16_to_utf8((uint8_t*) titl, bnr->titles[systemLanguage], sizeof(titl) - 1);
                title.title=titl;
                //Continue later.
            }
        }
    }
}
void Titles::populate(FS_MediaType mediaType, bool useDSiWare) {
    bool inserted;
    FS_CardType type;
    if(mediaType==MEDIATYPE_GAME_CARD) {
        if(R_FAILED(FSUSER_CardSlotIsInserted(&inserted))) {
            cout << "No card inserted" << endl;
            return;
        }
        if(!inserted || R_FAILED(FSUSER_GetCardType(&type))) {
            cout << "No card inserted" << endl;
            return;
        }
    }
    Result res=0;
    if(mediaType != MEDIATYPE_GAME_CARD || type == CARD_CTR) {
        u32 titleCount = 0;
        if(R_SUCCEEDED(res=AM_GetTitleCount(mediaType, &titleCount))) {
            u64* titleIds = (u64*) calloc(titleCount, sizeof(u64));
            if(titleIds != NULL) {
                if(R_SUCCEEDED(res=AM_GetTitleList(&titleCount, mediaType, titleCount, (u64*)titleIds))) {
                    vector<u64> idvec(titleIds, titleIds+titleCount);
                    sort(idvec.begin(), idvec.end());
                    for(u32 i=0;i<titleCount && R_SUCCEEDED(res); i++) {
                        bool dsiWare=((titleIds[i] >> 32) & 0x8000) != 0;
                        if(dsiWare != useDSiWare)
                            continue;
                        if(dsiWare)
                            insertTWL(mediaType, titleIds[i]);
                        else {}
                            //insertCTR(mediaType, titleIds[i]);
                    }
                }
                free(titleIds);
            }
        }
    } else {
        insertTWL(mediaType, 0);
    }
}
Titles::Titles(uint8_t *fb): fb(fb) {
    
}

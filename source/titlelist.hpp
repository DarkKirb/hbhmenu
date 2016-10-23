#include <3ds.h>
#include <vector>
#include <string>
using namespace std;
struct Title {
    u64 tid;
    string title;
    string desc;
    string author;
    bool twl;
};
struct SMDHTitle {
    uint16_t shortDescription[0x40];
    uint16_t longDescription[0x80];
    uint16_t publisher[0x40];
};
struct SMDH {
    char magic[0x04];
    uint16_t version;
    uint16_t reserved1;
    SMDHTitle titles[0x10];
    uint8_t ratings[0x10];
    uint32_t region;
    uint32_t matchMakerId;
    uint64_t matchMakerBitId;
    uint32_t flags;
    uint16_t EULAVersion;
    uint16_t reserved2;
    uint32_t optimalBannerFrame;
    uint32_t streetpassId;
    uint64_t reserved3;
    uint8_t smallIcon[0x480];
    uint8_t largeIcon[0x1200];
};
struct BNR {
    uint8_t version;
    bool animated;
    uint16_t crc16[4];
    uint8_t reserved[0x16];
    uint8_t mainIconBitmap[0x200];
    uint16_t mainIconPalette[0x10];
    uint16_t titles[16][0x80];
    uint8_t animatedFrameBitmaps[8][0x200];
    uint16_t animatedFramePalettes[8][0x10];
    uint16_t animationSequence[0x40];
};
struct Titles {
private:
    void insertTWL(FS_MediaType mediaType, u64 tid);
    void populate(FS_MediaType mediaType, bool useDSiWare);
public:
    uint8_t* fb;
    vector<Title> list;
    Titles(uint8_t *fb);
    ~Titles();
    auto launch(int id) -> void;
};

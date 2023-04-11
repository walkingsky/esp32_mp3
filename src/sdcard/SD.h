#ifndef _SD_H_
#define _SD_H_

#include "FS.h"
#include "SPI.h"
#include "sd_defines.h"
#include "gpios.h"

#define MOSO_PIN CF_DATA0
#define MOSI_PIN CF_CMD
#define SCLK_PIN CF_CLK
#define CS_PIN CF_CS

namespace fs
{

    class SDFS : public FS
    {
    protected:
        uint8_t _pdrv;

    public:
        SDFS(FSImplPtr impl);
        bool begin(uint8_t ssPin = SS, SPIClass &spi = SPI, uint32_t frequency = 4000000, const char *mountpoint = "/sd", uint8_t max_files = 5, bool format_if_empty = false);
        void end();
        sdcard_type_t cardType();
        uint64_t cardSize();
        size_t numSectors();
        size_t sectorSize();
        uint64_t totalBytes();
        uint64_t usedBytes();
        bool readRAW(uint8_t *buffer, uint32_t sector);
        bool writeRAW(uint8_t *buffer, uint32_t sector);
    };

}

extern fs::SDFS SD;

using namespace fs;
typedef fs::File SDFile;
typedef fs::SDFS SDFileSystemClass;
#define SDFileSystem SD

#endif /* _SD_H_ */

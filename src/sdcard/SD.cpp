
#include "vfs_api.h"
#include "sd_diskio.h"
#include "ff.h"
#include "FS.h"
#include "SD.h"

using namespace fs;

SDFS::SDFS(FSImplPtr impl) : FS(impl), _pdrv(0xFF) {}

bool SDFS::begin(uint8_t ssPin, SPIClass &spi, uint32_t frequency, const char *mountpoint, uint8_t max_files, bool format_if_empty)
{
    log_e("ssPin:%d,frequency:%ld,mountpoint:%s,max_files:%d,format_if_empty:%s", ssPin, frequency, mountpoint, max_files, format_if_empty ? "true" : "false");
    if (_pdrv != 0xFF)
    {
        return true;
    }
    log_e("_pdrv:0x%x", _pdrv);
    spi.begin(SCLK_PIN, MOSO_PIN, MOSI_PIN, CS_PIN);

    _pdrv = sdcard_init(ssPin, &spi, frequency);
    log_e("_pdrv:0x%x", _pdrv);
    if (_pdrv == 0xFF)
    {
        log_e("_pdrv:0x%x", _pdrv);
        return false;
    }

    if (!sdcard_mount(_pdrv, mountpoint, max_files, format_if_empty))
    {
        sdcard_unmount(_pdrv);
        sdcard_uninit(_pdrv);
        _pdrv = 0xFF;
        return false;
    }

    _impl->mountpoint(mountpoint);
    return true;
}

void SDFS::end()
{
    if (_pdrv != 0xFF)
    {
        _impl->mountpoint(NULL);
        sdcard_unmount(_pdrv);

        sdcard_uninit(_pdrv);
        _pdrv = 0xFF;
    }
}

sdcard_type_t SDFS::cardType()
{
    if (_pdrv == 0xFF)
    {
        return CARD_NONE;
    }
    return sdcard_type(_pdrv);
}

uint64_t SDFS::cardSize()
{
    if (_pdrv == 0xFF)
    {
        return 0;
    }
    size_t sectors = sdcard_num_sectors(_pdrv);
    size_t sectorSize = sdcard_sector_size(_pdrv);
    return (uint64_t)sectors * sectorSize;
}

size_t SDFS::numSectors()
{
    if (_pdrv == 0xFF)
    {
        return 0;
    }
    return sdcard_num_sectors(_pdrv);
}

size_t SDFS::sectorSize()
{
    if (_pdrv == 0xFF)
    {
        return 0;
    }
    return sdcard_sector_size(_pdrv);
}

uint64_t SDFS::totalBytes()
{
    FATFS *fsinfo;
    DWORD fre_clust;
    if (f_getfree("0:", &fre_clust, &fsinfo) != 0)
        return 0;
    uint64_t size = ((uint64_t)(fsinfo->csize)) * (fsinfo->n_fatent - 2)
#if _MAX_SS != 512
                    * (fsinfo->ssize);
#else
                    * 512;
#endif
    return size;
}

uint64_t SDFS::usedBytes()
{
    FATFS *fsinfo;
    DWORD fre_clust;
    if (f_getfree("0:", &fre_clust, &fsinfo) != 0)
        return 0;
    uint64_t size = ((uint64_t)(fsinfo->csize)) * ((fsinfo->n_fatent - 2) - (fsinfo->free_clst))
#if _MAX_SS != 512
                    * (fsinfo->ssize);
#else
                    * 512;
#endif
    return size;
}

bool SDFS::readRAW(uint8_t *buffer, uint32_t sector)
{
    return sd_read_raw(_pdrv, buffer, sector);
}

bool SDFS::writeRAW(uint8_t *buffer, uint32_t sector)
{
    return sd_write_raw(_pdrv, buffer, sector);
}

SDFS SD = SDFS(FSImplPtr(new VFSImpl()));

#include "wm8978.h"

void wm8978Init()
{
    /* Setup wm8978 I2C interface */
    if (!dac.begin(I2C_SDA, I2C_SCL))
    {
        log_e("Error setting up dac. System halted");
        while (1)
            delay(100);
    }
    dac.setSPKvol(40); /* max 63 */
    dac.setHPvol(32, 32);

    /* set i2s pins */
    audio.setPinout(I2S_BCK, I2S_WS, I2S_DOUT, I2S_DIN);

    log_i("Connected. Starting MP3...");
    audio.connecttohost("http://m10.music.126.net/20230326120311/973a01bed64e33e402312238b661c44c/ymusic/530e/0f0b/065c/dd42da1d8a8a643a9fe516f9a7e75719.mp3");
}

#include "wm8978.h"

Audio audio;
WM8978 dac;

void wm8978Init()
{
    /* Setup wm8978 I2C interface */
    if (!dac.begin(I2C_SDA, I2C_SCL))
    {
        log_e("Error setting up dac. System halted");
        while (0)
            delay(100);
    }
    dac.setSPKvol(50); /* max 63 */
    dac.setHPvol(32, 32);
    /* set i2s pins */
    audio.i2s_mclk_pin_select(I2S_MCLK);
    // audio.setPinout(I2S_BCK, I2S_WS, I2S_DOUT, I2S_DIN);
    audio.setPinout(I2S_BCK, I2S_WS, I2S_DOUT);
    log_e("Connected. Starting MP3...");
    bool host = audio.connecttohost("http://192.168.1.7/2603174988.mp3");
    // bool host = audio.connecttohost("http://icecast.omroep.nl/3fm-bb-mp3");
    audio.setVolume(20);
    int i = audio.getCodec();
    log_e("host:%s code:%d", host ? "true" : "false", i);
}

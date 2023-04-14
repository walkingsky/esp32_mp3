#include "wm8978card.h"

Audio audio(false, I2S_DAC_CHANNEL_DISABLE, I2S_NUM_0);
WM8978 dac;

void wm8978Init()
{
    /* Setup wm8978 I2C interface */
    if (!dac.begin(I2C_SDA, I2C_SCL, 400000))
    // if (!dac.begin(4, 21))
    {
        log_e("Error setting up dac. System halted");
        while (0)
            delay(100);
    }
    dac.setSPKvol(63); /* max 63 */
    dac.setHPvol(63, 63);
    /* set i2s pins */
    i2s_set_dac_mode(I2S_DAC_CHANNEL_DISABLE);
    pinMode(I2S_BCK, OUTPUT);
    pinMode(I2S_WS, OUTPUT);
    pinMode(I2S_DOUT, OUTPUT);
    pinMode(I2S_DIN, ANALOG);
    // audio.i2s_mclk_pin_select(3);
    //  audio.setPinout(I2S_BCK, I2S_WS, I2S_DOUT, I2S_DIN);
    audio.setPinout(I2S_BCK, I2S_WS, I2S_DOUT);

    log_e("Connected. Starting MP3...");
    bool host = audio.connecttohost("http://192.168.1.7/2603174988.mp3");
    // bool host = audio.connecttohost("http://icecast.omroep.nl/3fm-bb-mp3");
    audio.setVolume(10);
    int i = audio.getCodec();
    log_e("host:%s code:%d", host ? "true" : "false", i);
}

bool wm8978_sdcard()
{
    bool result = true;
    /*
    pinMode(CF_CS, OUTPUT);
    digitalWrite(CF_CS, HIGH);
    if (!SD.begin(CF_CS))
    {
        Serial.print("sdcard init failed! \r\n");
        return;
    }
    */

    result = audio.setPinout(I2S_BCK, I2S_WS, I2S_DOUT);
    audio.setVolume(12); // 0...21

    // audio.connecttoFS(SD, "test.mp3");
    audio.connecttoFS(SD, "test_8bit_stereo.wav");
    return result;
}

// optional
void audio_info(const char *info)
{
    Serial.print("info        ");
    Serial.println(info);
}
void audio_id3data(const char *info)
{ // id3 metadata
    Serial.print("id3data     ");
    Serial.println(info);
}
void audio_eof_mp3(const char *info)
{ // end of file
    Serial.print("eof_mp3     ");
    Serial.println(info);
}

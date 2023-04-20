#include "wm8978card.h"

#ifdef _COMPONENT_WM8978_AUDIO
Audio audio(false, I2S_DAC_CHANNEL_DISABLE, I2S_NUM_0);
#endif
#ifdef _COMPONENT_WM8978
WM8978 dac;
#endif

#ifdef _COMPONENT_WM8978
void wm8978Init()
{
    /* Setup wm8978 I2C interface */
    pinMode(I2S_MCLK, OPEN_DRAIN); // v1.0 板子上把io14 设为开路，MCLK 跳线到RXD0（IO3）
    if (!dac.begin(I2C_SDA, I2C_SCL))
    {
        log_e("Error setting up dac. System halted");
        while (0)
            delay(100);
    }
    dac.setSPKvol(45); /* max 63 */
    dac.setHPvol(40, 40);
    /* set i2s pins */
    i2s_set_dac_mode(I2S_DAC_CHANNEL_DISABLE);
}
#endif
#ifdef _COMPONENT_WM8978_AUDIO
void wm8978_i2s_init()
{
    audio.i2s_mclk_pin_select(3);
    // pinMode(I2S_DIN, INPUT);
    // audio.setPinout(I2S_BCK, I2S_WS, I2S_DOUT);
    audio.setPinout(I2S_BCK, I2S_WS, I2S_DOUT, I2S_DIN);
}

void wm8978_record(char *path)
{
    if (audio.isRunning()) // 暂停播放
        audio.stopSong();

    // dac.Read_Reg(2, 0x1BF);
    // dac.cfgI2S(2, 0);
    dac.cfgADDA(1, 0);
    dac.cfgInput(0, 1, 0);
    dac.setMICgain(0);
    dac.setAUXgain(0);
    dac.setLINEINgain(6);
    dac.cfgOutput(0, 1);
    dac.cfgI2S(2, 0);
    audio.RecordToSD(SD, path);
}

void wm8978_stop_record()
{
    dac.cfgADDA(0, 1);
    dac.cfgInput(0, 0, 0);
    dac.setMICgain(0);
    dac.setAUXgain(0);
    dac.setLINEINgain(0);
    dac.cfgOutput(1, 0);
    audio.StopRecord();
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

    log_e("Connected. Starting MP3...");
    bool host = audio.connecttohost("http://192.168.1.7/2603174988.mp3");
    // bool host = audio.connecttohost("http://icecast.omroep.nl/3fm-bb-mp3");
    audio.setVolume(10);
    int i = audio.getCodec();
    log_e("host:%s code:%d", host ? "true" : "false", i);

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
#endif
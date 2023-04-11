#include "a2dp_audio.h"
#include <Arduino.h>

BluetoothA2DPSource a2dp_source;

// The supported audio codec in ESP32 A2DP is SBC. SBC audio stream is encoded
// from PCM data normally formatted as 44.1kHz sampling rate, two-channel 16-bit sample data
int32_t get_data_channels(Frame *frame, int32_t channel_len)
{
    static double m_time = 0.0;
    double m_amplitude = 10000.0; // -32,768 to 32,767
    double m_deltaTime = 1.0 / 44100.0;
    double m_phase = 0.0;
    double double_Pi = PI * 2.0;
    // fill the channel data
    for (int sample = 0; sample < channel_len; ++sample)
    {
        double angle = double_Pi * c3_frequency * m_time + m_phase;
        frame[sample].channel1 = m_amplitude * sin(angle);
        frame[sample].channel2 = frame[sample].channel1;
        m_time += m_deltaTime;
    }

    return channel_len;
}

// Return true to connect, false will continue scanning
bool isValid(const char *ssid, esp_bd_addr_t address, int rssi)
{
    Serial.print("available SSID: ");
    Serial.println(ssid);
    return false;
}

void a2dp_setup()
{
    // a2dp_source.set_auto_reconnect(false);
    a2dp_source.set_ssid_callback(isValid);
    a2dp_source.set_auto_reconnect(false);
    // a2dp_source.start("HE05X", get_data_channels);
    a2dp_source.start(get_data_channels);
    a2dp_source.set_volume(30);
}
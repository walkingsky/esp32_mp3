#ifndef __GPIOS_H__
#define __GPIOS_H__

#define VER 2.0

/*
I2S			wm8978		IO					WM8987_IO 		接口板H_IO				开发板IO
I2S_WS		LRC			SAII_FSA	5		FSA				H1_1	FSA			D16
I2S_BCK		BCLK		SAII_SCKA	6		SCK				H1_2	SCK			D21
I2SIN      ADCDAT		SAII_SDB	4		SDB				H2_12	SDB			D22
I2SOUT		DACDAT		SAII_SDA	7		SDA				H2_13	SDA			D17
I2S_MCLK	MCLK		SAII_MCLKA	3		MCLK			H2_11	MCLK		rx0 D3

            SCLK		IIC_SCL		9		IIC_SCL			H1_7	IIC_SDL		D5
            SDIN		IIC_SDA		10		IIC_SDA			H1_6	IIC_SDA		D18


*/

/*  WM8978 I2C pins */
#define I2C_SDA GPIO_NUM_18
#define I2C_SCL GPIO_NUM_5

/* WM8978 I2S pins */
#define I2S_BCK GPIO_NUM_21
#define I2S_WS GPIO_NUM_16
#define I2S_DOUT GPIO_NUM_17
#define I2S_DIN GPIO_NUM_22
#define I2S_MCLK GPIO_NUM_3

/*LED*/
#define IO2_LED GPIO_NUM_2

/* DHT11 */
#define DHTPIN GPIO_NUM_35

/* 按键key */
#define ADC_KEY GPIO_NUM_34
// 板子有错误（IO23不是adc口），跳线到D4

/*sdcard*/
#define CF_DATA0 GPIO_NUM_2
#define CF_CS GPIO_NUM_4
#define CF_CMD GPIO_NUM_15
#define CF_CLK GPIO_NUM_14

/*oled屏幕 ssd1306*/
#define LED_SDA GPIO_NUM_27
#define LED_SCL GPIO_NUM_13

#endif
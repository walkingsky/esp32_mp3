## ESP32-wroom-32 接口转接板

 

 

![obednK55kfaPAliGs18i76L3zvKPe81rLfOL9tvy.png](https:////image.lceda.cn/pullimage/obednK55kfaPAliGs18i76L3zvKPe81rLfOL9tvy.png)

 

![a5cmZheIWYyWWhVetuJq13s0DrljHaMVP2NepnGg.png](https:////image.lceda.cn/pullimage/a5cmZheIWYyWWhVetuJq13s0DrljHaMVP2NepnGg.png)

 

## 硬件接口功能

- 板载一个*MicroSD*（tf）卡卡槽，SPI方式接线
- 板载5颗贴片按键开关，ADC方式接线，占用一个IO口
- 和ESP32-wroom-32 核心板排针对接的排母
- 和普中WM8978 MP3模块对接的排母
- 连接DHT11温湿度传感器的3针排针
- IIC 或者 SPI 外接OLED显示屏接口排针
- 其余未使用的IO使用排针方式引出，供后期功能扩展使用

## 其他配套硬件板卡

- ESP32-wroom-32U（或者32D）核心开发板
- 普中WM8978 MP3 模块
- DHT11温湿度传感器模块
- 0.96寸OLED液晶显示屏（IIC接口），128*64分辨率
- tf卡一个

 

## 软件功能

- smartconfig WIFI连接配置功能，方便配置WIFI连接设置
- 主页面显示时间、日期，实时温湿度![N3rysxgFjTM3qUuNmV4VB6RA8oY2YTDiiiHgyIG9.png](https:////image.lceda.cn/pullimage/N3rysxgFjTM3qUuNmV4VB6RA8oY2YTDiiiHgyIG9.png)
- TF卡音频文件播放![mqbaRRw9y0wyVkQxsedOcHOBKGLSwsxhphewOSxD.png](https:////image.lceda.cn/pullimage/mqbaRRw9y0wyVkQxsedOcHOBKGLSwsxhphewOSxD.png)![kKSOptw9I8XrI4SXbkZMO9ERj3LLLvDRgc9DF3m4.png](https:////image.lceda.cn/pullimage/kKSOptw9I8XrI4SXbkZMO9ERj3LLLvDRgc9DF3m4.png)
- 录音功能：分手动录音和自动录音，自动录音实时监听，超过程序默认的分贝值后自动开启录音，15秒无声音后自动停止录音。录音文件自动保存到TF卡record目录，使用wav格式保存![z8N0DqJHVuviU0ilv1ReXyaQrnY21o67PiNgp3cW.png](https:////image.lceda.cn/pullimage/z8N0DqJHVuviU0ilv1ReXyaQrnY21o67PiNgp3cW.png)![Co33QM5L5YzFV9AqnDlNBu6nJJgveECL5brXg94g.png](https:////image.lceda.cn/pullimage/Co33QM5L5YzFV9AqnDlNBu6nJJgveECL5brXg94g.png)
- 网络收音机功能，读取tf中的m3u目录中的fm_m3u.txt文件中的电台列表，可按键切换![lai5Jf0WjlWxqQzIcqhi8OGrWgcrUgZ5iywYWbmq.png](https:////image.lceda.cn/pullimage/lai5Jf0WjlWxqQzIcqhi8OGrWgcrUgZ5iywYWbmq.png)![5Zr9DL2hKZQvnFfv3yHHJAMniBCHSO4J6woWCUy2.png](https:////image.lceda.cn/pullimage/5Zr9DL2hKZQvnFfv3yHHJAMniBCHSO4J6woWCUy2.png)

 

 

## 转接板硬件开源地址

https://oshwhub.com/marco_daydreaming/esp32-zhuan-jie-ban
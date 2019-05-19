# 脉冲除尘设计

因为毕业设计的单片机控制要求，我设计了旋风除尘装置的三个控制要求，并且进行了设计。
**项目使用正点原子例程修改而来**
芯片：stm32f103c8t6
![IMG_20190519_105731.jpg](https://upload-images.jianshu.io/upload_images/15198536-fc134eee36eea1fe.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

![IMG_20190519_105723.jpg](https://upload-images.jianshu.io/upload_images/15198536-76aadb2ec82ace23.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

## 显示和交互部分

显示用了0.96英寸oled显示屏（SPI通讯），按键普通微动开关。4个分别接到引脚上，另一侧接地，引脚一侧并连到与门并接入32的外部中断。

## 这个项目包含三个功能模块

### 直流电机PWM调速部分

用了L298N模块，里面有2组三极管搭建的H型电桥，可以驱动2路直流电机调速正反转。

PWM发生用了TIM的pwm模块

### 继电器控制1ms间隔开断

这个好简单，我就不说了

用了TIM的定时器中断，定时时间太长，好像只能用中断。

### AD质量采集模块

HX711，淘宝卖家已经提供了例程51、32通用，基本上没咋移植，就是我到现在还没调准，这玩意回不了零。



## 电路连接图

这部分等会我再过来填坑。

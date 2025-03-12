# AutoClickII
1. 定时自动向目标窗体发送按键，
* 单按键
* 定时任务输入时可输入Space，Enter,Tab
* 按Ctrl+alt+a添加当前激活窗口的程序ID

2. 支持在界面上按键也会发送

3. 程序多开；自动打开exe,根据文本文件账户,密码自动登录
账户1,密码1
账户2,密码2




#mqtt版本

1. 需要用到编译静态包
paho.mqtt.c
paho.mqtt.cpp

2. 接收的topic为：ow/key_sync

输入

vs导入连接器：
paho-mqttpp3-static.lib
paho-mqtt3a-static.lib
paho-mqtt3c-static.lib
ws2_32.lib

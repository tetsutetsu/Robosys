# myled.c

RaspBerry Pi 3 Model B のLEDを点灯させるデバイスドライバ

# 各bashファイルの説明

### flas.bash
`bash flash.bash n m`

n：点滅回数

m：点滅の秒間隔

###flash_loop.bash
`bash flash_loop.bash m`

m：点滅の秒間隔

点滅回数制限なし

###flash_loop_random.bash
`bash flash_loop_random.bash`

ランダム(5秒以内)の間隔で点滅

点滅回数制限なし

# 配線
RaspBerry Pi 3 Model B のGPIO25にLEDを接続する


# 使用例
    ~$ make
    ~$ sudo insmod myled.ko
    ~$ sudo chmod 666 /dev/myled0
    ~$ bash flash.bash 5 1 
    ~$ bash flash_loop.bash 1
    ~$ bash flash_loop_random.bash
    ~$ sudo rmmod myled

# 動画URL(YouTube)
<https://youtu.be/8c4Sxfx2lgU>

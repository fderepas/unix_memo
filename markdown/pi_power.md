This page details how to use the [Raspberry Pi](https://www.raspberrypi.com/) 4 or 5 to control electrical devices using relays.

![Pi Relay Board](../img/pi_relay.png)

# Hardware used

This uses a Raspberry Pi 4, [USB breakount cable](https://www.adafruit.com/product/4448) and a [Raspberry Pi 8-ch Relay Expansion Board](https://www.waveshare.com/rpi-relay-board-b.htm).

# Powering the Pi

The Red (+5V) and Black (Ground) cable from the USB breakount cable are plugged in corresponding location in the Relay Expansion Board:

![Pi Relay Board USB power](../img/pi_power_usb.jpg)

# Code

## Packages to install

Here is what needs to be installed on the Rasperry Pi:
```
get http://www.airspayce.com/mikem/bcm2835/bcm2835-1.71.tar.gz
tar zxvf bcm2835-1.71.tar.gz
cd bcm2835-1.71/
sudo ./configure && sudo make && sudo make check && sudo make install
cd
git clone https://github.com/WiringPi/WiringPi.git
cd WiringPi
./build
```

# Turning on

Here is the C code to activate the relays:
```
include <bcm2835.h>
#include <stdio.h>

int main(int argc, char **argv) {
    uint8_t Relay[] = {5, 6, 13, 16, 19, 20, 21, 26};
    uint8_t i;

    if(!bcm2835_init()) { //use BCM2835 Pin number table
        printf("bcm2835 init failed !\r\n");
        return 1;
    }
    
    //GPIO config
    for(i=0; i<8; i++) {
        bcm2835_gpio_fsel(Relay[i], BCM2835_GPIO_FSEL_OUTP);
        bcm2835_gpio_write(Relay[i], LOW);
    }

    return 0;
}
```
# Turning off

Here is the C code to activate the relays:
```c
include <bcm2835.h>
#include <stdio.h>

int main(int argc, char **argv) {
    uint8_t Relay[] = {5, 6, 13, 16, 19, 20, 21, 26};
    uint8_t i;

    if(!bcm2835_init()) { //use BCM2835 Pin number table
        printf("bcm2835 init failed !\r\n");
        return 1;
    }
    
    //GPIO config
    for(i=0; i<8; i++) {
        bcm2835_gpio_fsel(Relay[i], BCM2835_GPIO_FSEL_OUTP);
        bcm2835_gpio_write(Relay[i], HIGH);
    }

    return 0;
}
```
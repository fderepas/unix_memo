This page details how to use the [Raspberry Pi Debug Probe](https://www.raspberrypi.com/products/debug-probe/).


# USB Connection

The debug probe is connected to the computer using a usb cable.

Under Linux the probe should appear under ```/dev/ttyACM0```.

In case ```/dev/ttyACM0``` does not show up when plugging the probe, make sure the CDC ADM driver is present. The line ```CONFIG_USB_ACM=m``` should be present in ```.config``` when compiling the kernel.


# On a Raspberry Pi Model 3 or 4

We use the UART port on the probe that we connect to the folling GPIO pins:

![Connection the probe on UART GPIO pins](https://github.com/fderepas/unix_memo/blob/main/img/rpi_debug_probe.jpg)

On the Raspberry Pi UART is not enabled by default. Make sure that in the file ```/boot/firmware/config.txt``` there is:
```
[all]
enable_uart=1
```
Otherwise add these two lines. Then reboot the Raspberry Pi.

On the computer connected by USB type:
```
sudo apt install screen
sudo screen /dev/ttyACM0 11520
```

It's then possible to connect from the computer, using login and password of a user on the Raspberry Pi.






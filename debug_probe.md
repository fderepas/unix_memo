Here is how to the Raspberry Pi Debug Probe

# On a Raspberry Pi Model 3 or 4

In this case we connect the debug probe to the computer using the usb.

We use the UART port on the probe that we connect to the folling GPIO pins:

![Connection the probe on UART GPIO pins](https://github.com/fderepas/unix_memo/blob/main/img/rpi_debug_probe.jpg)

On the Raspberry Pi UART is not enabled by default. Make sure that in the file ```/boot/firmware/config.txt``` there is:
```
[all]
enable_uart=1
```
Otherwise add these two lines.

Then on the computer connected by USB typing:
```
sudo apt install screen
sudo screen /dev/ttyACM0 11520
```
In case you are on linux and ```/dev/ttyACM0``` does not show up when plugging the probe, make sure the CDC ADM driver is present. The line ```CONFIG_USB_ACM=m``` should be present in ```.config``` when compiling the kernel.







This page details how to use the [Raspberry Pi](https://www.raspberrypi.com/) 4 or 5 as a USB device.

# Powering the Raspberry Pi

The USB connection might not be enough to power the Raspberry Pi when plugged to a standard USB port. So we need to directly power the Pi. This can be done by plugging pins 4 to 5V and pin 6 to ground.

![Powering the Pi](../img/rpi_gpio.png)

This powering can be done using a USB breakount cable. Here is an example of direct powering of Rasberry pi:
![Powering the Pi](../img/pi_power.jpg)
In [these kinds](https://www.adafruit.com/product/4448) of breakout cables:
- Red wire connects to 5V power (from USB host)
- Black wire connects to common data/power ground

Powering the Pi via the GPIO header, you are connecting your power supply directly to the main 5V rail. This means you bypass:
- The [Polyfuse](https://elinux.org/Polyfuses_explained): A resettable fuse on the USB-C input that protects against over-current events.
- Over-voltage Protection: Circuitry that protects the Pi from voltage spikes.
- Reverse-Polarity Protection: A diode that prevents damage if you were to accidentally connect the power backward.
  
# USB Connection

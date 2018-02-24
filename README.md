# Introduction
This application counts number of detected blinks and calculates the number
of blinks per minute. It logs this information every second to the UART.
It uses a Wemos D1 mini equipped with a Light distance detector from
Flying Fish.

# Build
Install PlatformIO Core like described here:
https://platformio.org/get-started/cli

Or just run:

```
pip install platformio
```

When PlatformIO is installed, build with:

```
platformio run
```


# Load the app to target

1. Connect the thing to a USB port on your computer.
2. Remove the light distance detector from the Wemos D1 Mini.
3. Run ``` platformio run --target upload ```.
4. Attach the light distance detector again.

# Use it
When the thing is connected with USB to your computer it should identify
itself as a serial port. Use your favourite tool to open and read from this
serial port. The settings should be: 115200 bps, no parity, 8 data bits and 1
start bit.

# Random notes
The application tries to minimize the "contact bounce" effect which happens
at a specific distance. It would be better to solve this with proper hardware
instead.

# INA2xx

NOTE: This library has not been updated to work with SensESP version 2, and it will not be updated, because you don't
need it with version 2. Instead, you'll use the new `RepeatSensor` class to read the INA2xx sensor using whatever library
for that sensor that you choose. See the [RepeatSensor Tutorial Part 1](https://signalk.org/SensESP/pages/tutorials/bmp280/)
and [Part 2](https://signalk.org/SensESP/pages/tutorials/bmp280_part_2/).

If you want to use this library with SensESP version 1, you can - see Step 5 below.

*********************************************************

Library for reading one or more INAxxx and INAxxxx High Side / Low Side Current and Power Sensors in SignalK/SensESP.

This library allows you to use one or more - of the same or different variants - of the INAxxx and INAxxxx in the same
SensESP Project (`main.cpp`). It recognizes the 3-channel INA3221 as three different sensors. Here's how to use it.

1. Follow the instructions in the SensESP [README](https://github.com/SignalK/SensESP/tree/v1#sensesp) to install Signal K
and SensESP.

2. Don't try to use this library until after you have successfully built and uploaded one of the SensESP examples
and seen the desired output in the Signal K Server.

3. Start a new Project in PlatformIO.

4. Follow the instructions in the SensESP README (and the Wiki if you need [detailed instructions](https://github.com/SignalK/SensESP/wiki/SensESP-Overview-and-Programming-Details-(for-SensESP-version-1)#getting-a-good-platformioini-file)) for getting a good `platformio.ini` file for
your Project.

5. Make one modification to your shiny new `platformio.ini` - add this library to the `lib_deps` section, like this:
```
lib_deps =
   SignalK/SensESP @ 1.0.8
   https://github.com/ba58smith/INA2xx.git
```

6. Replace your Project's `main.cpp` file with the [example file](https://github.com/ba58smith/INA2xx/blob/main/examples/main.cpp) that's included with this library.

7. Modify your `main.cpp` to reflect the sensor or sensors that you're actually testing with. Be sure
to read the comments in the example that explain how to determine the order of multiple sensors, how to use the 3-channel
INA3221, etc. Also, it's CRITICAL that you set the correct values for `max_amps` and `shunt_micro_ohms`. See [this document](https://github.com/ba58smith/INA2xx/blob/main/Setting%20of%20Max%20Expected%20Amps%20in%20INA2xx.pdf) for details.

8. Build, upload, and test your Project.

Once you have the example working properly, you can include this library in any SensESP Project just by following Step 5 above.



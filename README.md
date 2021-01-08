# INA2xx
Library for reading one or more INAxxx and INAxxxx High Side / Low Side Current and Power Sensors in SignalK/SensESP.

This library allows you to use one or more - of the same or different variants - of the INAxxx and INAxxxx in the same
SensESP Project (`main.cpp`). It recognizes the 3-channel INA3221 as three different sensors. Here's how to use it.

1. Follow the instructions in the SensESP [README](https://github.com/SignalK/SensESP#sensesp) to install Signal K
and SensESP.

2. Don't try to use this library until after you have successfully built and uploaded one of the SensESP examples
and seen the desired output in the Signal K Server.

3. Start a new Project in PlatformIO.

4. Follow the instructions in the SensESP README (and the Wiki if you need [detailed instructions](https://github.com/SignalK/SensESP/wiki/SensESP-Overview-and-Programming-Details#getting-a-good-platformioini-file)) for getting a good `platformio.ini` file for
your Project.

5. Make one modification to your shiny new `platformio.ini` - add this library to the `lib_deps` section, like this:
```
lib_deps =
   SignalK/SensESP
   https://github.com/SensESP/INA2xx.git
```

6. Replace the contents of your Project's `main.cpp` file with the contents of the [example file](https://github.com/SensESP/INA2xx/blob/main/examples/ina2xx_example.cpp) that's included with this library.

7. Modify your `main.cpp` to reflect the sensor or sensors that you're actually testing with. Be sure
to read the comments in the example that explain how to determine the order of multiple sensors, how to use the 3-channel
INA3221, etc.

8. Build, upload, and test your Project.

Once you have it working properly, you can include this library in any SensESP Project just by following Step 5 above.



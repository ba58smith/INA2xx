// INA2xx_example.cpp

#include <Arduino.h>

#define USE_LIB_WEBSOCKET true

#include "sensesp_app.h"
#include "sensors/ina2xx.h"
#include "signalk/signalk_output.h"

ReactESP app([]() {
#ifndef SERIAL_DEBUG_DISABLED
  SetupSerialDebug(115200);
#endif

  /**
   * This example illustrates how you can use multiple different INA2xx 
   * High-Side/Low-Side Bi-Directional I2C Current and Power sensors in the same
   * project. It uses an INA219 to monitor the 12V house bank and an INA226 for the
   * 24V bow thruster battery bank.
   * 
   * This works even if one of your INA sensors is the three-channel INA3221. See below.
   * 
   * For illustrative purposes, it reads two of the available values for the house
   * bank, and the other two values for the bow thruster bank. You can read any or
   * all of the four available values for any INA sensor you use in your project.
  **/ 

  sensesp_app = new SensESPApp();

  /** Create an INA2xx object, which represents ALL of the physical sensors. It looks
   * at every address from 0x40 through 0x4F for an INA sensor, in address order, and
   * assigns each of the found sensors a deviceNumber, starting with 0. It then calls
   * INA::begin(), which 
   * initializes ALL of the sensors with max_amps and shunt_micro_ohms. (Which you may
   * need to change later for some sensor(s)- keep reading.)
   * 
   * If any of your INA sensors is an INA3221, its three channels will be represented
   * with three consecutive deviceNumbers. If you have only an INA3221 in your project,
   * the three channels will be 0, 1, and 2, even if you don't have all of them physically
   * connected to anything. If you have, for example, an INA219 at address 0x40 and an
   * INA3221 at 0x41, the INA219 will be deviceNumber 0, and the three channels of the
   * INA3221 will be 1, 2, and 3.
   * 
   * Due to the way that I2C can be declared with ESP LoRa 32/8266, and that
   * different pins from the standard SDA and SCL can be used, the INA library does
   * not issue a "Wire.begin()" for them. Rather, it is expected that the developer
   * issues this call prior to creating the INA2xx in main.cpp. You would do that here:
   * 
     Wire.begin( <<whatever parameters you need here>> );

   */

  uint16_t max_amps = 1;  // The maximum amperage you expect to read on deviceNumber 0.
  uint32_t shunt_micro_ohms = 100000; // 0.1 ohm - the resistor value on deviceNumber 0.

  auto* ina2xx = new INA2xx(max_amps, shunt_micro_ohms);

  /** Give a name to the deviceNum of each of the two physical INA's that are connected.
   * Device numbers are automatically assigned in the ascending order of the I2C
   * address of each device, starting with 0. In this example, the address of the
   * INA219 is 0x40, and the INA226 is 0x42.
  **/
  const uint8_t house_bank = 0;
  const uint8_t bow_thruster = 1;

  /**
   * In this example, the INA226 has a different value shunt resistor than the INA219,
   * so we need to set both the max_amps and shunt_micro_ohms to be different than those
   * values defined above. The example values may not be realistic, but in your project,
   * it's critical that you get them right. See https://github.com/Zanduino/INA/wiki/begin()
   * for details on determining the value of these parameters for your project. This is just
   * to show you how to do it.
   **/
  uint16_t bow_thruster_max_amps = 200;  // The maximum amperage you expect to read on the INA226.
  uint32_t bow_thruster_shunt_micro_ohms = 250; // 0.00025 ohm - the resistor value on the INA226.

  ina2xx->ina_->begin(bow_thruster_max_amps, bow_thruster_shunt_micro_ohms, bow_thruster);

     /** The INA library allows you to set other values besides the two parameters
   * above. SensESP uses the library's default values for all of these, EXCEPT
   * setAveraging() - SensESP defaults to 60 for that. If you want to change any
   * or all of them, do so like this here, referring to the library
   * for valid values. If you don't provide a deviceNumber, the change will be made
   * for all devices.

     ina2xx->ina_->setBusConversion(some_valid_value, optionalDeviceNumber);
     ina2xx->ina_->setShuntConversion(some_valid_value, optionalDeviceNumber);
     ina2xx->ina_->setAveraging();  // SensESP defaults to 60
     ina2xx->ina_->setMode(some_valid_value, optionalDeviceNumber);
     ina2xx->ina_->setI2CSpeed(some_valid_value, optionalDeviceNumber);

   **/

  /* Define the read_delay you're going to use, if other than the default of 500.
   * For this example, there is no config_path to configure read_delay, just to
   * keep it simple. */
  const uint read_delay = 1000;  // once per second

  /** 
   * Create an INA2xxValue, which is used to read a specific value from a
   * specific sensor, and send its output to Signal K as a number (float).
   * To find valid Signal K Paths that fits your need you look at this link:
   * https://signalk.org/specification/1.4.0/doc/vesselsBranch.html
   *  
   * This one is for the bus voltage of the house bank.
   * 
   * Because the path for the output is already
   * in the Signal K Specification, we don't need to send any metadata. If it were
   * a path that's not in the spec, you would need to send at least the "units".
   * See the analog_input example for how to do that.
   * 
   * house_bank_bus_voltage is connected to an SKOutputNumber to send it to Signal K. Note
   * that there are other versions of SKOutputNumber (which sends a float):
   * SKOutputInt, SKOutputBook, and SKOutputString.
   */ 
  auto* house_bank_bus_voltage = new INA2xxValue(ina2xx, house_bank, INA2xxValue::bus_volts, read_delay);

  house_bank_bus_voltage->connect_to(new SKOutputNumber("electrical.batteries.house.voltage"));

  /* Repeat the above pattern to read the shunt_volts of the house bank. */
  auto* house_bank_shunt_voltage = new INA2xxValue(ina2xx, house_bank, INA2xxValue::shunt_volts, read_delay);

  house_bank_shunt_voltage->connect_to(new SKOutputNumber("electrical.batteries.house.shunt_voltage"));  

  /* Repeat the above pattern for the watts and amps of the bow_thruster bank. */ 
  auto* bow_thruster_watts = new INA2xxValue(ina2xx, bow_thruster, INA2xxValue::watts, read_delay);

  bow_thruster_watts->connect_to(new SKOutputNumber("electrical.bowThruster.watts"));

  auto* bow_thruster_amps = new INA2xxValue(ina2xx, bow_thruster, INA2xxValue::amps, read_delay);

  bow_thruster_amps->connect_to(new SKOutputNumber("electrical.bowThruster.amps"));


  /* Start SensESP */
  sensesp_app->enable();
});

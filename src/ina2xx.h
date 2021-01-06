#ifndef _ina2xx_H_
#define _ina2xx_H_

#include <INA.h>
#include <Wire.h>

#include "sensor.h"


  /**
   * @brief INA2xx represents a Texas Instruments (or compatible) INA2xx
   * High-Side/Low-Side Bi-Directional I2C Current and Power sensor. It works
   * with several different INA chips, but only one at a time.
   * 
   * The INA3221 has three channels, and although the Zanduino library is capable
   * of accessing all three of them in the same program, this implementation
   * handles only one of them - channel_0.
   *
   * For a list of all the sensors it works with, see
   * https://github.com/Zanduino/INA/wiki
   *
   * The constructor creates a pointer to a sensor, and starts up the
   * sensor. The pointer is passed to INA2xxValue, which retrieves the specified
   * value.
   *
   * Unlike many I2C libaries, this one does not require nor provide a way
   * for you to set the hardware address of the sensor - it figures it out
   * automatically.
   *
   * @param max_amps The maximum amperage that you expect to read with the
   * sensor.
   *
   * @param shunt_micro_ohms Value of the shunt resistor on the sensor in
   * micro-ohms, e.g. 100000 is 0.1 0hm.
   *
   * See https://github.com/Zanduino/INA/wiki/begin() for details on determining
   * the value of these parameters.
   *
   * The INA library allows you to set other values besides the two parameters
   * above. SensESP uses the library's default values for all of these, EXCEPT
   * setAveraging() - SensESP defaults to 60 for that. If you want to change any
   * or all of them, do so like this in your main.cpp, referring to the library
   * for valid values:
   
     auto* ina226 = new ina2xx(max_amps, shunt_micro_ohms);
     ina226->ina_->setBusConversion(some_valid_value);
     ina226->ina_->setShuntConversion(some_valid_value);
     ina226->ina_->setAveraging();  // SensESP defaults to 60
     ina226->ina_->setMode(some_valid_value);
     ina226->ina_->setI2CSpeed(some_valid_value);

   * 
   * Due to the way that I2C can be declared with ESP LoRa 32/8266, and that
   * different pins from the standard SDA and SCL can be used, the INA library does
   * not issue a "Wire.begin()" for them. Rather, it is expected that the developer
   * issues this call prior to creating the INA2xx in main.cpp.
   *
   */
  class INA2xx {
   public:
    INA2xx(uint16_t max_amps, uint32_t shunt_micro_ohms, INA_Class* ina_class = NULL);
    INA_Class* ina_;
    bool sensor_detected_ = true;
};


/**
 * @brief INA2xxValue reads and outputs the specified value of an INA2xx sensor.
 * 
 * @param ina2xx A pointer to an instance of an INA2xx.
 * 
 * @param deviceNum_ Identifies the physical INAxxx that is found on the I2C bus.
 * 0 is the first device, 1 is the second, etc. Devices are in ascending order of
 * the I2C address. 
 * 
 * @param val_type The type of value you're reading:
 *      bus_volts, shunt_volts, watts, or amps.
 * 
 * @param read_delay How often to read the sensor, in ms. The default is 500,
 * which works with the combination of values set by setBusConversion(8500), 
 * setShuntConversion(8500), and setAveraging(60). (These are defaults used
 * in this Class.) If you make read_delay any faster, you'll simply get the
 * same value twice in a row, unless you change the "conversion" times or 
 * the number of samples to use in the "averaging". See the INA library for
 * details.
 * 
 * @param config_path Path in the Config UI to configure read_delay
 */
class INA2xxValue : public NumericSensor {
 public:
  
  enum INA2xxValType { bus_volts, shunt_volts, watts, amps };
  INA2xxValue(INA2xx* ina2xx, uint8_t deviceNum, INA2xxValType val_type,
              uint read_delay = 500, String config_path = "");
  void enable() override final;
  INA2xx* ina2xx_;

 private:
  uint8_t deviceNum_;
  INA2xxValType val_type_;
  uint read_delay_;
  virtual void get_configuration(JsonObject& doc) override;
  virtual bool set_configuration(const JsonObject& config) override;
  virtual String get_config_schema() override;
};

#endif

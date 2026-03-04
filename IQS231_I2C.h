/**
 * IQS231_I2C
 * Arduino library for the Azoteq IQS231 series capacitative proximity sensors.
 * by Ben Wheeler <ben@uniqcode.com>
 * NB: The IQS231 needs to have I2C enabled in its One Time Programming configuration.
 * The variant 00004000 has it enabled (on address 0x44) from the factory.
 * Otherwise, or to use one of the alternative addresses, you will need to use Azoteq's 
 * USBProg2 software and one of their programming devices to set the appropriate bits.
 * 
 * MIT license applies.
 */

#ifndef IQS231_I2C_H
#define IQS231_I2C_H

#include <Arduino.h>
#include <Wire.h>

// The default I2C address is 0x44. 
#define IQS231_DEFAULT_ADDRESS 0x44

enum MainEventBits {
  IQS231_MAINEVENTBIT_PROXIMITY,
  IQS231_MAINEVENTBIT_TOUCH,
  IQS231_MAINEVENTBIT_RELEASE,
  IQS231_MAINEVENTBIT_COLDBOOT,
  IQS231_MAINEVENTBIT_WARMBOOT,
  IQS231_MAINEVENTBIT_SENSINGDISABLED,
  IQS231_MAINEVENTBIT_DEBUG,
};

enum DebugEventBits {
  IQS231_DEBUGEVENTBIT_MOVEMENT,
  IQS231_DEBUGEVENTBIT_ENTERMOVDETECT,
  IQS231_DEBUGEVENTBIT_EXITMOVDETECT,
  IQS231_DEBUGEVENTBIT_QUICKRELEASE,
  IQS231_DEBUGEVENTBIT_RESERVED,
  IQS231_DEBUGEVENTBIT_CH0ATI,
  IQS231_DEBUGEVENTBIT_ATIERROR,
  IQS231_DEBUGEVENTBIT_RESERVED2,
};

class IQS231_I2C {
public:
  IQS231_I2C();
  ~IQS231_I2C();

  bool begin(uint8_t i2cAddr = IQS231_DEFAULT_ADDRESS, TwoWire *wire = &Wire);
  bool isPresent();
  bool readRegister(uint8_t regAddr, uint8_t *result, uint8_t *events = nullptr);
  bool readRegisters(uint8_t startAddr, uint8_t *result, size_t numBytes, uint8_t *events = nullptr);
  bool writeRegister(uint8_t regAddr, uint8_t value);

protected:

private:
  TwoWire *_wire;
  uint8_t _i2cAddr;
  size_t dbg_print(const char *s);
  size_t dbg_println(const char *s = "");
  template <typename... Types> size_t dbg_printf(Types&&... args);
};

#endif

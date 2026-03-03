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

#include "Arduino.h"
#include "IQS231_I2C.h"

IQS231_I2C::IQS231_I2C(void) {}

IQS231_I2C::~IQS231_I2C(void) {}

size_t IQS231_I2C::dbg_print(const char *s)
{
  Serial.print("IQS231_I2C: ");
  return Serial.print(s);
}

size_t IQS231_I2C::dbg_println(const char *s)
{
  Serial.print("IQS231_I2C: ");
  return Serial.println(s);
}

template <typename... Types>
size_t IQS231_I2C::dbg_printf(Types&&... args)
{
  Serial.print("IQS231_I2C: ");
  return Serial.printf(std::forward<Types>(args)...);
}

bool IQS231_I2C::begin(uint8_t i2cAddr, TwoWire *wire)
{
  _wire = wire;
  _i2cAddr = i2cAddr;
  if (!isPresent())
  {
    return false;
  }
  return true;
}

// Check that the expected device is answering on the given address
bool IQS231_I2C::isPresent()
{
  // First check that something is answering on that address
  _wire->beginTransmission(_i2cAddr);
  if (_wire->endTransmission() != 0)
  {
    dbg_printf("No response from address %u\n", _i2cAddr);
    return false;
  }
  // Now check it's the expected thing
  // Register 0x00 should return 0x40
  // Register 0x01 should return 0x06 (IQS231A) or 0x07 (IQS231B)
  constexpr uint8_t numBytes = 2;
  _wire->beginTransmission(_i2cAddr);
  _wire->write(0x00);
  if (_wire->endTransmission() != 0)
  {
    dbg_print("No ACK\n");
    return false;
  }

  // Remember that all register reads first return the MAIN_EVENTS byte, so we must request an extra byte
  _wire->requestFrom(_i2cAddr, numBytes + 1U);
  if (!_wire->available()) {
    dbg_print("No data came back");
    return false;
  }

  uint8_t events = _wire->read();
  uint8_t buf[numBytes] = {0x00, 0x00};
  for (uint8_t i = 0; i < numBytes; i++)
  {
    buf[i] = _wire->read();
  }
  if (buf[0] == 0x40)
  {
    if (buf[1] == 0x06)
    {
      dbg_println("Detected IQS231A");
      return true;
    }
    if (buf[1] == 0x07)
    {
      dbg_println("Detected IQS231B");
      return true;
    }
    dbg_printf("Product number correct but unknown software version - expected 0x06 or 0x07, got 0x%02x\n", buf[1]);
    return false;
  } 
  dbg_printf("Product number incorrect - expected 0x40, got 0x%02x\n", buf[0]);
  return false;
}

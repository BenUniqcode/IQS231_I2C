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
  return Serial.print(s);
}

size_t IQS231_I2C::dbg_printf(const char *fmt, ...)
{
    char buf[MAXLEN_LOGMSG + 1];
    va_list ap;
    va_start(ap, fmt);
    vsnprintf_P(buf, MAXLEN_LOGMSG, fmt, ap);
    va_end(ap);
    return dbg_print(buf);
}

bool IQS231_I2C::begin(uint8_t i2cAddr, TwoWire *wire)
{
  _wire = wire;
  _i2cAddr = i2cAddr;
  if (!isPresent())
  {
    return false;
  }
}


// Check that the expected device is answering on the given address
bool IQS231_I2C::isPresent()
{
  // First check that something is answering on that address
  _wire->beginTransmission(_i2cAddr);
  if (_wire->endTransmission() != 0)
  {
    dprintf("IQS231: No response from address %u\n", _i2cAddr);
    return false;
  }
  // Now check it's the expected thing
  // Register 0 should return 0x40
  // Register 1 should return 0x06 (IQS231A) or 0x07 (IQS231B)
  // Remember that all register reads first return the MAIN_EVENTS byte
  constexpr int numBytes = 2;
  uint8_t buf[numBytes] = {0x00, 0x00};
  uint8_t events;
  readRegisters(0x00, numBytes, &events, buf);
  _wire->beginTransmission(_i2cAddr);
  _wire->write(0x00);
  if (!_wire->endTransmission())
  {
#ifdef IQS231_I2C_DEBUG
    Serial.println("IQS231_I2C: ");
#endif

  _wire->requestFrom(_i2cAddr, numBytes);
  if (_wire->available()) {
    events = _wire->read();
    for (uint8_t i = 0; i < numBytes; i++)
    {
      buf[i] = _wire->read();
    }
  }
  else
  {

  if (buf[0] == 0x40)
  {
    if (buf[1] == 0x06)
    {
#ifdef IQS231_I2C_DEBUG
      Serial.println("IQS231A detected");
#endif
      return true;
    }
    if (buf[1] == 0x07)
    {
#ifdef IQS231_I2C_DEBUG
      Serial.println("IQS231B detected");
#endif
      return true;
    }
#ifdef IQS231_I2C_DEBUG
    Serial.printf("Product number correct but unknown software version - expected 0x06 or 0x07, got 0x%02x\n", buf[1]);
#endif
    return false;
  } 
#ifdef IQS231_I2C_DEBUG
  Serial.printf("Product number incorrect - expected 0x40, got 0x%02x", buf[0]);
#endif
  return false;
}

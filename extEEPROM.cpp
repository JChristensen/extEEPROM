/*----------------------------------------------------------------------*
 * extEEPROM.h - Arduino library to support external I2C EEPROMs.       *
 * Devices supported include:                                           *
 * - 24XX256 series 256kb/32kB                                          *
 * - M24M02 2Mb/512kB                                                   *
 *                                                                      *
 * The extEEPROM library instantiates an object named EEEP (External EE *
 * Prom) with which the library's methods are accessed.                 *
 *                                                                      *
 * Note the following limitations:                                      *
 * - The Arduino Wire library has a buffer size of 32 bytes, this       *
 *   limits the size of I/Os that can be done to the EEPROMs. For       *
 *   writes, two bytes are used for address, so data is therefore       *
 *   limited to 30 bytes.                                               *
 * - Multiple EEPROM devices can be addressed on the same bus; the      *
 *   user must connect the EEPROM address pins appropriately.           *
 * - The user must ensure that I/Os do not cross page or device         *
 *   boundaries.                                                        *
 * - To use the extEEPROM library, the Arduino Wire library must be     *
 *   included.                                                          *
 *                                                                      *
 * Jack Christensen 23Mar2013                                           *
 *                                                                      *
 * This work is licensed under the Creative Commons Attribution-        *
 * ShareAlike 3.0 Unported License. To view a copy of this license,     *
 * visit http://creativecommons.org/licenses/by-sa/3.0/ or send a       *
 * letter to Creative Commons, 171 Second Street, Suite 300,            *
 * San Francisco, California, 94105, USA.                               *
 *----------------------------------------------------------------------*/

#include <extEEPROM.h>
#include <Wire.h>

/*----------------------------------------------------------------------*
 * Constructor.                                                         *
 * deviceCapacity is the capacity of a single EEPROM device in          *
 * kilobytes (kB) and should be either 32 or 512. Other values will     *
 * result in undefined behavior!                                        *
 *----------------------------------------------------------------------*/
extEEPROM::extEEPROM(unsigned int deviceCapacity)
{
    _devSize = deviceCapacity;
    if (_devSize == 512)
        _addrShift = 16;
    else
        _addrShift = 15;
    Wire.begin();
}

/*----------------------------------------------------------------------*
 * Write multiple bytes to external EEPROM.                             *
 * Number of bytes (nBytes) must be between 1 and 30 (Wire library      *
 * limitation).                                                         *
 *----------------------------------------------------------------------*/
byte extEEPROM::write(unsigned long addr, byte *values, byte nBytes)
{
    byte deviceAddr = EEPROM_ADDR | (byte) (addr >> _addrShift);
    byte txStatus;
    
    Wire.beginTransmission(deviceAddr);
    i2cWrite( (byte) (addr >> 8) );            //address high byte
    i2cWrite( (byte) addr );                   //address low byte
    i2cWrite(values, nBytes);
    txStatus = Wire.endTransmission();
    if (txStatus != 0) return txStatus;
    
    //wait up to 50ms for the write to complete
    for (uint8_t i=100; i; --i) {
        delayMicroseconds(500);                //no point in waiting too fast
        Wire.beginTransmission(deviceAddr);
        i2cWrite(0); i2cWrite(0);              //high and low address bytes
        txStatus = Wire.endTransmission();
        if (txStatus == 0) break;
    }
    return txStatus;
}

/*----------------------------------------------------------------------*
 * Write a single byte to external EEPROM.                              *
 *----------------------------------------------------------------------*/
byte extEEPROM::write(unsigned long addr, byte value)
{
    return write(addr, &value, 1);
}

/*----------------------------------------------------------------------*
 * Read multiple bytes from external EEPROM.                            *
 * Number of bytes (nBytes) must be between 1 and 32 (Wire library      *
 * limitation).                                                         *
 *----------------------------------------------------------------------*/
byte extEEPROM::read(unsigned long addr, byte *values, byte nBytes)
{
    byte deviceAddr = EEPROM_ADDR | (byte) (addr >> _addrShift);
    byte rxStatus;

    Wire.beginTransmission(deviceAddr);
    i2cWrite( (byte) (addr >> 8) );            //address high byte
    i2cWrite( (byte) addr );                   //address low byte
    rxStatus = Wire.endTransmission();
    if (rxStatus != 0) return rxStatus;        //read error
    
    Wire.requestFrom(deviceAddr, nBytes );
    for (byte i=0; i<nBytes; i++) values[i] = i2cRead();
    return 0;
}

/*----------------------------------------------------------------------*
 * Read a single byte from external EEPROM.                             *
 *----------------------------------------------------------------------*/
byte extEEPROM::read(unsigned long addr)
{
    byte value;

    read(addr, &value, 1);
    return value;
}

extEEPROM EEEP = extEEPROM();    //instantiate an External EEProm object

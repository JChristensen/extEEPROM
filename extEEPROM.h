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

#ifndef extEEPROM_h
#define extEEPROM_h

#if defined(ARDUINO) && ARDUINO >= 100
#include <Arduino.h> 
#else
#include <WProgram.h> 
#endif

//define release-independent I2C functions
#if ARDUINO >= 100
#define i2cRead Wire.read
#define i2cWrite Wire.write
#else
#define i2cRead Wire.receive
#define i2cWrite Wire.send
#endif

//24XX256 I2C Address
#define EEPROM_ADDR 0x50

class extEEPROM
{
    public:
        extEEPROM(unsigned int deviceCapacity);
        byte write(unsigned long addr, byte *values, byte nBytes);
        byte write(unsigned long addr, byte value);
        byte read(unsigned long addr, byte *values, byte nBytes);
        byte read(unsigned long addr);
        
    private:
        unsigned int _dvcSize;      //capacity of one EEPROM device, in kilobytes (kB)
        byte _addrShift;
};

extern extEEPROM EEEP;

#endif

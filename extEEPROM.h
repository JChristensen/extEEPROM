/*----------------------------------------------------------------------*
 * extEEPROM.h - Arduino library to support external I2C EEPROMs.       *
 * Devices supported include:                                           *
 * - 24XX256 series 256k bit/32k Byte                                   *
 * - M24M02 2M bit/256k Byte                                            *
 *                                                                      *
 * To use the extEEPROM library, the Arduino Wire library must be       *
 * included.                                                            *
 *                                                                      *
 * Jack Christensen 23Mar2013 v1                                        *
 * 29Mar2013 v2 - updated to span page boundaries (and therefore also   *
 * device boundaries, assuming an integral number of pages per device)  *
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
        extEEPROM(unsigned int deviceCapacity, byte nDevice, unsigned int pageSize);
        byte write(unsigned long addr, byte *values, byte nBytes);
        byte read(unsigned long addr, byte *values, byte nBytes);
        
    private:
        uint16_t _dvcCapacity;          //capacity of one EEPROM device, in kilobytes (kB)
        uint8_t _nDevice;               //number of devices on the bus
        uint16_t _pageSize;
        uint8_t _addrShift;
        unsigned long _totalCapacity;   //capacity of all EEPROM devices on the bus, in bytes
};

#endif

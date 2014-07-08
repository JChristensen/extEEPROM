/*-----------------------------------------------------------------------------*
 * extEEPROM.cpp - Arduino library to support external I2C EEPROMs.            *
 *                                                                             *
 * This library will work with most I2C serial EEPROM chips between 2k bits    *
 * and 2048k bits (2M bits) in size. Multiple EEPROMs on the bus are supported *
 * as a single address space. Certain assumptions are made regarding the       *
 * EEPROM device addressing. These assumptions should be true for most EEPROMs *
 * but there are exceptions, so read the datasheet and know your hardware.     *
 *                                                                             *
 * The library should also work for EEPROMs smaller than 2k bits, assuming     *
 * that there is only one on the bus and also that the user is careful not     *
 * to exceed the maximum address for the EEPROM.                               *
 *                                                                             *
 * Device addressing assumptions:                                              *
 * 1. The I2C address sequence consists of a control byte followed by one      *
 *    address byte (for EEPROMs <= 16k bits) or two address bytes (for         *
 *    EEPROMs > 16k bits).                                                     *
 * 2. The three least-significant bits in the control byte (excluding the R/W  *
 *    bit) comprise the three most-significant bits for the entire address     *
 *    space, i.e. all chips on the bus. As such, these may be chip-select      *
 *    bits or block-select bits (for individual chips that have an internal    *
 *    block organization), or a combination of both (in which case the         *
 *    block-select bits must be of lesser significance than the chip-select    *
 *    bits).                                                                   *
 * 3. Regardless of the number of bits needed to address the entire address    *
 *    space, the three most-significant bits always go in the control byte.    *
 *    Depending on EEPROM device size, this may result in one or more of the   *
 *    most significant bits in the I2C address bytes being unused (or "don't   *
 *    care").                                                                  *
 *                                                                             *
 * To use the extEEPROM library, the Arduino Wire library must also            *
 * be included.                                                                *
 *                                                                             *
 * Jack Christensen 23Mar2013 v1                                               *
 * 29Mar2013 v2 - Updated to span page boundaries (and therefore also          *
 * device boundaries, assuming an integral number of pages per device)         *
 * 08Jul2014 v3 - Generalized for 2kb - 2Mb EEPROMs.                           *
 *                                                                             *
 * External EEPROM Library by Jack Christensen is licensed under CC BY-SA 4.0, *
 * http://creativecommons.org/licenses/by-sa/4.0/                              *
 *-----------------------------------------------------------------------------*/

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

//EEPROM size in kilobits. EEPROM part numbers are usually designated in k-bits.
enum eeprom_size_t {
    kbits_2 = 2,
    kbits_4 = 4,
    kbits_8 = 8,
    kbits_16 = 16,
    kbits_32 = 32,
    kbits_64 = 64,
    kbits_128 = 128,
    kbits_256 = 256,
    kbits_512 = 512,
    kbits_1024 = 1024,
    kbits_2048 = 2048
};

//EEPROM addressing error, returned by write() or read() if upper address bound is exceeded
const uint8_t EEPROM_ADDR_ERR = 9;

class extEEPROM
{
    public:
        extEEPROM(eeprom_size_t deviceCapacity, byte nDevice, unsigned int pageSize);
        byte begin(byte eepromAddr = 0x50);
        byte write(unsigned long addr, byte *values, byte nBytes);
        byte read(unsigned long addr, byte *values, byte nBytes);

    private:
        uint8_t _eepromAddr;            //eeprom i2c address
        uint16_t _dvcCapacity;          //capacity of one EEPROM device, in kbits
        uint8_t _nDevice;               //number of devices on the bus
        uint16_t _pageSize;             //page size in bytes
        uint8_t _csShift;               //number of bits to shift address for chip select bits in control byte
        uint16_t _nAddrBytes;           //number of address bytes (1 or 2)
        unsigned long _totalCapacity;   //capacity of all EEPROM devices on the bus, in bytes
};

#endif

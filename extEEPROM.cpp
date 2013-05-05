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

#include <extEEPROM.h>
#include <Wire.h>

/*----------------------------------------------------------------------*
 * Constructor.                                                         *
 * deviceCapacity is the capacity of a single EEPROM device in          *
 * kilobytes (kB) and should be either 32 or 256. Other values will     *
 * result in undefined behavior!                                        *
 * nDevice is the number of EEPROM devices on the I2C bus.              *
 * pageSize is in bytes.                                                *
 *----------------------------------------------------------------------*/
extEEPROM::extEEPROM(unsigned int deviceCapacity, byte nDevice, unsigned int pageSize)
{
    _dvcCapacity = deviceCapacity;
    _nDevice = nDevice;
    _pageSize = pageSize;
    _totalCapacity = _dvcCapacity * _nDevice * 1024UL;
    if (_dvcCapacity == 256)
        _addrShift = 16;
    else
        _addrShift = 15;
    Wire.begin();
}

/*----------------------------------------------------------------------*
 * Write bytes to external EEPROM.                                      *
 * If the I/O would extend past the top of the EEPROM address space,    *
 * a status of EEPROM_ADDR_ERR is returned. For I2C errors, the status  *
 * from the Arduino Wire library is passed back through to the caller.  *
 *----------------------------------------------------------------------*/
byte extEEPROM::write(unsigned long addr, byte *values, byte nBytes)
{
    uint8_t deviceAddr;     //I2C device address
    uint8_t txStatus;       //transmit status
    uint8_t nWrite;         //number of bytes to write
    uint16_t nPage;         //number of bytes remaining on current page, starting at addr
    
    if (addr + nBytes > _totalCapacity) {   //will this write go past the top of the EEPROM?
        return EEPROM_ADDR_ERR;             //yes, tell the caller
    }
    
    while (nBytes > 0) {
        nPage = _pageSize - ( addr & (_pageSize - 1) );
        //find min(nBytes, nPage, BUFFER_LENGTH) -- BUFFER_LENGTH is defined in the Wire library.
        nWrite = nBytes < nPage ? nBytes : nPage;
        nWrite = BUFFER_LENGTH - 2 < nWrite ? BUFFER_LENGTH - 2 : nWrite;   //allow 2 bytes for address
        deviceAddr = EEPROM_ADDR | (byte) (addr >> _addrShift);
        Wire.beginTransmission(deviceAddr);
        i2cWrite( (byte) (addr >> 8) );            //address high byte
        i2cWrite( (byte) addr );                   //address low byte
        i2cWrite(values, nWrite);
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
        if (txStatus != 0) return txStatus;
        
        addr += nWrite;         //increment the EEPROM address
        values += nWrite;       //increment the input data pointer
        nBytes -= nWrite;       //decrement the number of bytes left to write
    }
    return txStatus;
}

/*----------------------------------------------------------------------*
 * Read bytes from external EEPROM.                                     *
 * If the I/O would extend past the top of the EEPROM address space,    *
 * a status of EEPROM_ADDR_ERR is returned. For I2C errors, the status  *
 * from the Arduino Wire library is passed back through to the caller.  *
 *----------------------------------------------------------------------*/
byte extEEPROM::read(unsigned long addr, byte *values, byte nBytes)
{
    byte deviceAddr;
    byte rxStatus;
    uint8_t nRead;              //number of bytes to read
    uint16_t nPage;             //number of bytes remaining on current page, starting at addr

    if (addr + nBytes > _totalCapacity) {   //will this read take us past the top of the EEPROM?
        return EEPROM_ADDR_ERR;             //yes, tell the caller
    }

    while (nBytes > 0) {
        nPage = _pageSize - ( addr & (_pageSize - 1) );
        nRead = nBytes < nPage ? nBytes : nPage;
        nRead = BUFFER_LENGTH < nRead ? BUFFER_LENGTH : nRead;
        deviceAddr = EEPROM_ADDR | (byte) (addr >> _addrShift);
        Wire.beginTransmission(deviceAddr);
        i2cWrite( (byte) (addr >> 8) );            //address high byte
        i2cWrite( (byte) addr );                   //address low byte
        rxStatus = Wire.endTransmission();
        if (rxStatus != 0) return rxStatus;        //read error
    
        Wire.requestFrom(deviceAddr, nRead);
        for (byte i=0; i<nRead; i++) values[i] = i2cRead();

        addr += nRead;          //increment the EEPROM address
        values += nRead;        //increment the input data pointer
        nBytes -= nRead;        //decrement the number of bytes left to write
    }
    return 0;
}

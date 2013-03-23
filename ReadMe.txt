ReadMe file for Arduino extEEPROM Library Alpha Version*
*Use at your own risk. Not all functionality has been tested.
http://github.com/JChristensen/extEEPROM
Jack Christensen Mar 2013

This work is licensed under the Creative Commons Attribution-ShareAlike 3.0
Unported License. To view a copy of this license, visit
http://creativecommons.org/licenses/by-sa/3.0/ or send a letter to Creative
Commons, 171 Second Street, Suite 300, San Francisco, California, 94105, USA.

================================================================================
Arduino library to support external I2C EEPROMs, including:
 - 24XX256 series (256k bit, 32k Byte)
 - M24M02 (2M bit, 256k Byte)

Tested with 24LC256, 24FC256, M24M02. Should also work with 24AA256.
--------------------------------------------------------------------------------
To use the library:
(1) Go to http://github.com/JChristensen/extEEPROM and click the ZIP button to
    download the repository as a ZIP file to a convenient location on your PC.
(2) Uncompress the downloaded file. This will result in a folder containing all
    the files for the library, that has a name that includes the branch name,
    for example "extEEPROM-master".
(3) Rename the folder to just "extEEPROM".
(4) Copy the renamed folder to the Arduino sketchbook\libraries folder.

--------------------------------------------------------------------------------
The following example sketches are included with the library:

(No examples yet.)

--------------------------------------------------------------------------------
Note the following limitations:
- The Arduino Wire library has a buffer size of 32 bytes, this
  limits the size of I/Os that can be done to the EEPROMs. For
  writes, two bytes are used for address, so data is therefore
  limited to 30 bytes.
- Multiple EEPROM devices can be addressed on the same bus; the
  user must connect the EEPROM address pins appropriately.
- The user must ensure that I/Os do not cross page or device
  boundaries.
- To use the extEEPROM library, the Arduino Wire library must be
  included.

For brevity, these includes are not repeated in the examples below:
#include <extEEPROM.h>  //http://github.com/JChristensen/extEEPROM
#include <Wire.h>       //http://arduino.cc/en/Reference/Wire
                        //(Wire.h is included with the Arduino IDE)

================================================================================
More to come...

# usbtan-cli
Request chipTAN from USB-cardreaders via libchipcard.

**Note:** This does not actually work for me. Published for investigation only.

## Usage

Parameters:

    usbtan-cli STARTCODE [IBAN] [AMOUNT]
    
Must use the last 10 digits of the IBAN only.  
Amount must use comma for decimal point.

Example:

    usbtan-cli 42424242 1234567890 3,50

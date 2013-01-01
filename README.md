ServoLight
==========

Lightweight servo library for Energia (MSP430 Launchpad).
- Removed all floating point (map() function)
- Pin selection at object instantiation
- Only 1 function: write()
- write value is 8-bit integer (0..255) corresponding to 0 to 180 degrees.
The sweep example compiles in 905 bytes of FLASH (in energia-0101E0009).
This allows you to use small MSP430 chips with only 1024bytes of FLASH for small projects.

TODO: Only works at 16MHZ clock at the moment. Stil have to make tick calculation depenent on the selected 
clock frequency at compile time.

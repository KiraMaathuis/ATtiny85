# ATtiny85
ATtiny85 experiments to learn how to properly code embedded chips

## Wiring of Raspberry Pi to ATtiny85

*insert picture of wiring*

## Projects:

### blinky

Blinky was the first project, meant to learn how to program an ATtiny from my Raspberry Pi

### ATtiny SPI

Trying to bitbang SPI communication protocol between the Raspberry Pi and the ATtiny.
#### Goals:
* keep 2 pins open on ATtTiny to use
* be able to use chip select
* use 3 wires for communication (MOSI and MISO combined)

&#9745; read 8bit SPI message from RPi by ATtiny \
&#9745; return 8bit SPI message to RPi from ATtiny \
&#9745; turn on/off LED on ATtiny on command from RPi \
&#9744; read sensor with ATtiny and send information to RPi \
&#9744; be able to use chip select \
&#9744; combine MISO and MOSI

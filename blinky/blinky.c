#define F_CPU 1000000L
#include <avr/io.h>
#include <util/delay.h>

typedef struct{
  unsigned int bit0:1;
  unsigned int bit1:1;
  unsigned int bit2:1;
  unsigned int bit3:1;
  unsigned int bit4:1;
  unsigned int bit5:1;
  unsigned int bit6:1;
  unsigned int bit7:1;
  } _io_reg;

typedef union{
  _io_reg bits;
  unsigned char pinout;
  }pinbank;

pinbank pins;

int init(){

  DDRB = 0xFF; // -> 00010010 (PB7 ... PB0)
  PORTB = 0x00; // Make pins low to start (PORTB is output)
  pins.pinout = 0; // make outputs 0 ( in memory) 

  return 0;
}

int main(void){

  init();

  for (;;) {
    pins.bits.bit4 ^= 1; // set PB4 to high in memory
    PORTB = pins.pinout; // make current pinout the output
    _delay_ms(1000); // wait some time
  }
  return 0;
}



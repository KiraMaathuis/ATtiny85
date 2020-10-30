#define F_CPU 1000000L 
#include <avr/io.h>
#include <util/delay.h>

#define MISO    0x02 //Same as MOSI!
#define MOSI    0x01
#define SCLK    0x04
#define SS      0x02 
#define LEDPIN  0x10 // we want two ADC pins free, and so we cheat.
#define TEMPPIN 0x08 // If you want you can just change the SS pin.

#define CMD_NOTHING  0b00000001
#define CMD_ECHO     0b00000010
#define CMD_TEMP     0b00000011
#define CMD_LEDON    0b00000100
#define CMD_LEDOFF   0b00000101

#define RESP_ERROR   0b00000000
#define CLK_ERROR    0b11111111

//TIMEOUT_S is the amount of seconds to wait for a clock
// right now the code assumes one clock cycle per timer count
// but it is much longer, so these won't be real seconds
#define CLK_TIMEOUT_S    0.001  

unsigned char read_buffer;
unsigned char write_buffer;

int init_SPI();                                          // done
void SPI_write(unsigned char SPI_resp);                  // done
unsigned char SPI_read();                                // done
int SPI_CLK(int polarity);                               // done
unsigned char SPI_SS_check();                            // temp
void command_handler(unsigned char SPI_command);         // done
void set_LED(int state);                                 // done
unsigned char temp_read();                               // temp

int init_SPI(){
  
  DDRB = 0x12; // -> 00010010 (PB7 ... PB0)
  PORTB = 0x00; // Make pins low to start (PORTB is output)

  return 0;
}

int main(void){

  init_SPI();

  while(1){
    //if (SPI_SS_check()) {
      command_handler(SPI_read());
    //}
  }
  PORTB = PORTB|LEDPIN;
  return 0;
}

unsigned char SPI_SS_check(){
/* returns the value of the SS line, sets MISO line to write
   inputs  = none
   outputs = value of SS line */

  DDRB = DDRB&~SS; // bitwise operation to set SS bit low
  
  unsigned char iobank = PINB;
  iobank = iobank&SS;

  DDRB = DDRB|SS; /* bitwise operation to set SS bit high 
                  /* (activate MISO)*/
  return 1;//iobank;
}

int SPI_CLK(int polarity){
/* waits untill clock turns high or low depending on polarity.
   it waits for turning high for receiving and low for sending.
   inputs  = polarity 0 for descending flank, 1 for ascending
   outputs = none*/
  unsigned char last_state;
  unsigned char current_state=PINB&SCLK;
  int timeout = F_CPU*CLK_TIMEOUT_S; // counter of clock cycles
                                     // to count to timeout
  while(timeout){                    // not real seconds! while
    last_state=current_state;        // loop might take longer
    current_state=PINB&SCLK;
    timeout--;
    
    if(last_state == 0 && current_state > 0 && polarity>0){
      
      return 0;
    }
    if(last_state > 0 && current_state == 0 && polarity==0){
      
      return 0;
    }
  } 
  return 1;
}

unsigned char SPI_read(){
/* reads the byte message from master using SPI_CLK() and calls
   the command handler to handle the message
   inputs  = none
   outputs = unsigned char with the byte of from master */
  unsigned char SPI_message=0; // might be filled with garbage
  int i;                       // so we instantiate it as 0

  DDRB = DDRB&~MOSI; // set mosi pin to read mode
  
  for (i=0;i<8;i++){
    if(SPI_CLK(1)) return CLK_ERROR; // 1 to select ascending flank
    SPI_message >>= 1; // first bit is LSB (least sign. bit)
    if(PINB&MOSI){
      
      SPI_message |= 0b10000000;
      //PORTB = PORTB|LEDPIN; // to check if we get something
    }
    //else{PORTB = PORTB&~LEDPIN;} // also for checking 
  }
  return SPI_message;
}

void SPI_write(unsigned char SPI_resp){
/* writes a byte to master
   inputs  = response byte
   outputs = none */
   
   DDRB = DDRB|MISO; // to set MISO pin to output mode

   int i;            
   for (i=0;i<8;i++){
     SPI_CLK(0); // 0 to select descending flank
     if (SPI_resp&1){PORTB = PORTB|MISO;}
     else {PORTB = PORTB&~MISO;}
     SPI_resp >>= 1;
   }
}

void command_handler(unsigned char SPI_command){
/* gets SPI byte from master and decodes the command
   then it calls the corresponding functions to execute
   inputs  = SPI command byte
   outputs = none */
  
  unsigned char temp_response;
  
  switch(SPI_command){
    case 1:
      // do nothing
      break;
    case 2:
      // echo command
      SPI_write(SPI_command);
      break;
    case 3:
      // read temperature and relay to master
      temp_response = temp_read();
      SPI_write(temp_response);
      break;
    case 4:
      // turn on LED
      set_LED(1);
      break;
    case 5:
      // turn off LED
      set_LED(0);
      break;
    case CLK_ERROR:
      //no one cares about this shiznip
      break;
    default:
      // respond with command unknown message
      // which we chose to be 0b00000000
      SPI_write(RESP_ERROR); 
      break; 
   
  }
  //set_LED(1);
}

void set_LED(int state){
/* sets the LED to be either on or off.
   inputs  = LED state, 0 is off 1 is on
   outputs = none  */
  
  if (state){
    PORTB = PORTB|LEDPIN;
  }
  
  
  if (state==0) {
    PORTB = PORTB&~LEDPIN;
  }

}

unsigned char temp_read(){
  return 0b00101010; // 42! temporary
}

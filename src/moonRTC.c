#define F_CPU 1000000UL      // 1MHz internal clock

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdbool.h>
#include <util/delay.h>
#include <string.h>
#include <time.h>

#include "lcd_lib/lcd.h"
#include "rtc_ds1307_lib/rtc.h"
#include "i2c_lib/i2cmaster.h"

void init()
{
  int8_t i;
  DDRD = 0b11110000;
  lcd_init(LCD_DISP_ON);
  lcd_clrscr();
  lcd_gotoxy(0,0);
  lcd_puts_P("Real-time clock");
  _delay_ms(1000);

  i2c_init();
  DS1307read();
  
  // check if the highest bit of the Seconds-register is a 1
  // if so, the clock is not running and needs to be initialized
  if (DS1307_regs[dsSEC] & 0b10000000)
  {
    for (i=0; i<DSREGS; i++)
    {
      DS1307_regs[i] = DS1307_const[i];
    }
    DS1307write();
  }
}
uint32_t time_per_step = 1;
uint32_t last_step_time;
uint32_t time_per_cycle;
uint8_t steps [8] =
    {
      0b00010000 ,
      0b00110000 ,
      0b00100000 ,
      0b01100000 ,
      0b01000000 ,
      0b11000000 ,
      0b10000000 ,
      0b10010000
    };
uint8_t fazel=0;
int main(void)
{
  uint8_t year, month, day, dow, hour, min, sec;
  //uint32_t totalSecondsPassed = 0;
  char buffer[20];
  init();

  while (1) 
  {
    lcd_clrscr();
    DS1307read();

    sec  = fromBCD(DS1307_regs[dsSEC]);
    min  = fromBCD(DS1307_regs[dsMIN]);
    hour = fromBCD(DS1307_regs[dsHOUR]);
    dow  = DS1307_regs[dsDOW];
    day  = fromBCD(DS1307_regs[dsDAY]);
    month= fromBCD(DS1307_regs[dsMONTH]);
    year = fromBCD(DS1307_regs[dsYEAR]);

    sprintf(buffer, "%02d:%02d:%02d",hour,min,sec);
    lcd_gotoxy(0,0);
    lcd_puts(buffer);


    uint32_t totalSecondsPassed = dateTimeToSeconds(year,month, day, hour, min, sec);
    uint32_t last_step_time = readDataEEPROM((uint8_t*)46);
    lcd_gotoxy(0,1);
    if((totalSecondsPassed-last_step_time)>time_per_step)
        {
          int steps_to_do = (totalSecondsPassed - last_step_time)/time_per_step;
          for (int j = 0; j <= steps_to_do; j++)
          {
            PORTD = steps [fazel];
            fazel++;
            fazel%=8;
            _delay_ms(100);
            PORTD = steps [fazel];
            fazel++;
            fazel%=8;
          }
          writeDataEEPROM((uint8_t*)46, totalSecondsPassed); 
        }
        
        
    }
    
    //sprintf(buffer, "%02d/%02d/%04d", day, month, year+2000);
    //uint32_t test = writeDataEEPROM(50, totalSecondsPassed);

    //sprintf(buffer, "%lu %d", test, eeprombool);
    //sprintf(buffer, "%lu, %d", totalSecondsPassed, eeprombool);


    
    sprintf(buffer, "%lu", last_step_time);

    lcd_puts(buffer);
    
    //writeDataEEPROM((uint8_t*)46, totalSecondsPassed);
   
  
  
  return 0;
}

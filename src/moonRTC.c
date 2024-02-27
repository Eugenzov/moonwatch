#define F_CPU 1000000UL      // 1MHz internal clock

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdbool.h>
#include <util/delay.h>
#include <string.h>

#include "lcd_lib/lcd.h"
#include "rtc_ds1307_lib/rtc.h"
#include "i2c_lib/i2cmaster.h"

/* Unsigned 8-bit BCD operations.
  in BCD each half of an 8-bit unsigned integer represents one digit from 0-9
  similar to hexadecimal coding, but the higher bit combinations 1010 to 1111 
  are not used 
*/
#define fromBCD(x) (((x) >> 4) * 10 + ((x) & 0xF))
#define toBCD(x)   ((((x) / 10) << 4) | ((x) % 10))

/* DS1307 I2C address */
#define DS1307_I2C_ADDR			0xD0 /* 11010000 */

// definition of the DS1307 hardware registers - eight 8-bit registers
#define DSREGS 8
#define dsSEC 0
#define dsMIN 1
#define dsHOUR 2
#define dsDOW 3
#define dsDAY 4
#define dsMONTH 5
#define dsYEAR 6
#define dsSTATUS 7

// predefined values to set the real-time clock: seconds minutes hours day date
// month year control
// 14:05:30 on Wednesday (3) 24 November 2021
static const uint8_t DS1307_const[DSREGS] = {0x30, 0x05, 0x14, 4, 0x24, 0x11, 0x21, 0b00000000};

// memory copy of the DS1307 registers
uint8_t DS1307_regs [DSREGS];


void init(void) {
  PORTD = 0x4;                // pull-up resistor active PD2 (S1)

  lcd_init(LCD_DISP_ON);
  lcd_clrscr();
  lcd_puts("Hello World!");
}

int main(void) {

  init();
//  lcd_gotoxy(0, 0);


  return 0;
}
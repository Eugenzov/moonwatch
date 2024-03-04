#include <avr/io.h>
#include <stdio.h>
#include <string.h> 	/* memset() */
#include <stdint.h>
#include <util/delay.h>

#include <rtc_ds1307_lib/rtc.h>

// read the eight registers from the DS1307 into the memory
void DS1307read(void){ 
  uint8_t i;
  ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
  {
    i2c_start_wait(DS1307_ADDR+I2C_WRITE);
    i2c_write(0x00);
    i2c_stop();
    /* Commence the read. */
    _delay_us(20);
    i2c_start_wait(DS1307_ADDR+I2C_READ);
    for (i=0; i<(DSREGS-1); i++)
    {
      _delay_us(10);
      DS1307_regs[i] = i2c_read(1);
    }
    _delay_us(10);
    DS1307_regs[DSREGS-1] = i2c_read(0);
    i2c_stop();
  }
}

// write the contents of the memory into the eight hardware registers of the DS1307
void DS1307write(void) {
  uint8_t i;
  ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
  {
    i2c_start_wait(DS1307_ADDR+I2C_WRITE);
    i2c_write(0x00);  //start address
    for (i=0; i<DSREGS; i++)
    {
      i2c_write(DS1307_regs[i]);
    }
    i2c_stop();
  }
}

unsigned long int dateTimeToSeconds(int year, int month, int day,
                                    int hour, int min, int seconds) {
  const int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
  unsigned long int totalSeconds = 0;
  int fullYear = year + 2000;

  for (int yy = 1970; yy < fullYear; yy++) {
    totalSeconds += 365UL * 24UL * 3600UL;
  }
  for (int mm = 1; mm < month; mm++) {
    totalSeconds += daysInMonth[mm-1] * 24UL * 3600UL;
  }
  totalSeconds += (day-1) * 24UL * 3600UL;
  totalSeconds += (hour*3600UL) + (min*60UL) + seconds;
  return totalSeconds; 
}





/*
void getLocalTime(void) {
  time_t currentTime = time(NULL);

  // Create DS1307 array variable
    uint8_t DS1307_Current_time[DSREGS] = {
        tm->tm_sec,         // seconds
        tm->tm_min,         // minutes
        tm->tm_hour,        // hours
        tm->tm_wday,        // day of the week (0-6, Sunday = 0)
        tm->tm_mday,        // day of the month
        tm->tm_mon + 1,     // month (0-11, January = 0)
        tm->tm_year - 100,  // year (subtract 100 to get last two digits)
        0b00000000         // control byte
    };
}
  
*/

/**
 * BcdToBinary()
 * Brief	: Converts the passed Binary-Coded Decimal (BCD) value to binary
 * Param	: @bcd - binary-coded decimal value to be converted into binary
 * Retval	: @bcd in binary representation
 * Note		: N/A

static uint8_t BcdToBinary(uint8_t bcd)
{
	uint8_t tens, ones;

	tens = (uint8_t)((bcd >> 4) * 10);
	ones = bcd & (uint8_t)0xF;

	return tens + ones;
}  End of BcdToBinary */

/**
 * BinaryToBcd()
 * Brief	: Converts the passed binary value to Binary-Coded Decimal (BCD)
 * Param	: @binary - binary value to be converted into binary-coded decimal
 * Retval	: @binary in binary-coded decimal representation
 * Note		: N/A

uint8_t BinaryToBcd(uint8_t binary)
{
	uint8_t tens, ones;
	uint8_t bcd;

	bcd = binary;

	if (binary >= 10)
	{
		tens = binary / 10;
		ones = binary % 10;
		bcd = (tens << 4) | ones;
	}

	return bcd;
}  End of BinaryToBcd */
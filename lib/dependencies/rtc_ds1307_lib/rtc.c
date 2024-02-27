/*******************************************************************************
 * File		: rtc_ds1307.c
 * Brief	: Implementation of APIs for the DS1307 RTC module
 * Author	: Kyungjae Lee
 * Date		: Jun 25, 2023
 *
 * Note		: This code includes only the features that are necessary for my
 * 			  personal projects.
 ******************************************************************************/

#include <avr/io.h>
#include <stdio.h>
#include <string.h> 	/* memset() */
#include <stdint.h>

#include <rtc_ds1307_lib/rtc.h>

// read the eight registers from the DS1307 into the memory
void DS1307read(void)
{
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
void DS1307write(void)
{
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


/**
 * BcdToBinary()
 * Brief	: Converts the passed Binary-Coded Decimal (BCD) value to binary
 * Param	: @bcd - binary-coded decimal value to be converted into binary
 * Retval	: @bcd in binary representation
 * Note		: N/A
 */
static uint8_t BcdToBinary(uint8_t bcd)
{
	uint8_t tens, ones;

	tens = (uint8_t)((bcd >> 4) * 10);
	ones = bcd & (uint8_t)0xF;

	return tens + ones;
} /* End of BcdToBinary */

/**
 * BinaryToBcd()
 * Brief	: Converts the passed binary value to Binary-Coded Decimal (BCD)
 * Param	: @binary - binary value to be converted into binary-coded decimal
 * Retval	: @binary in binary-coded decimal representation
 * Note		: N/A
 */
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
} /* End of BinaryToBcd */
/*******************************************************************************
 * File		: rtc.h
 * Project	: For lunar phase clock
 ******************************************************************************/

#ifndef RTC_DS1307_H
#define RTC_DS1307_H

#include <avr/io.h>
#include <stdio.h>
#include <util/atomic.h>
#include "i2c_lib/i2cmaster.h"

/* Unsigned 8-bit BCD operations. */
// in BCD each half of an 8-bit unsigned integer represents one digit from 0-9
// similar to hexadecimal coding, but the higher bit combinations 1010 to 1111 
// are not used
#define fromBCD(x) (((x) >> 4) * 10 + ((x) & 0xF))
#define toBCD(x)   ((((x) / 10) << 4) | ((x) % 10))

/* Register addresses */
#define DSREGS 		0x08
#define dsMIN 		0x01
#define dsHOUR 		0x02
#define dsDOW 		0x03
#define dsDAY 		0x04
#define dsMONTH 	0x05
#define dsYEAR 		0x06
#define dsSTATUS 	0x07

// hardware I2C address
#define DS1307_ADDR  0xD0

// predefined values to set the real-time clock: seconds minutes hours day date  month  year control
//  14:05:30 on Wednesday (3) 24 November 2021
static const uint8_t DS1307_const[DSREGS] = { 0x30, 0x05, 0x14, 4, 0x24, 0x11, 0x21,  0b00000000};

// memory copy of the DS1307 registers
uint8_t DS1307_regs[DSREGS];


/* RTC date configuration structure */
typedef struct
{
	uint8_t date;
	uint8_t month;
	uint8_t year;
	uint8_t day;
} RTC_Date_TypeDef;

/* RTC time configuration structure */
typedef struct
{
	uint8_t seconds;
	uint8_t minutes;
	uint8_t hours;
	uint8_t	timeFormat;
} RTC_Time_TypeDef;

// read the eight registers from the DS1307 into the memory
void DS1307read(void);

// write the contents of the memory into the eight hardware registers of the DS1307
void DS1307write(void);

static uint8_t BcdToBinary(uint8_t bcd);
uint8_t BinaryToBcd(uint8_t binary);

#endif /* RTC_DS1307_H */
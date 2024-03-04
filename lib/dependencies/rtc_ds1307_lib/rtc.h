/*******************************************************************************
 * Author: Fazel Naser & Eugen Zovko
 * 
 * File		: rtc.h
 * Purpose	: The BSP (board support package) for our project.
 * Project	: For lunar phase clock
 ******************************************************************************/

#ifndef RTC_DS1307_H
#define RTC_DS1307_H

#include <avr/io.h>
#include <avr/eeprom.h>
#include <stdio.h>
#include <util/atomic.h>
#include <time.h>
#include <stdint.h>

#include "i2c_lib/i2cmaster.h"

// Addresses for storing uint32_t value
#define ADDRESS_0 0
#define ADDRESS_1 1
#define ADDRESS_2 2
#define ADDRESS_3 3

/* Unsigned 8-bit BCD operations. */
// in BCD each half of an 8-bit unsigned integer represents one digit from 0-9
// similar to hexadecimal coding, but the higher bit combinations 1010 to 1111 
// are not used
#define fromBCD(x) (((x) >> 4) * 10 + ((x) & 0xF))
#define toBCD(x)   ((((x) / 10) << 4) | ((x) % 10))

// hardware I2C address
#define DS1307_ADDR  0xD0

/* Register addresses */
#define DSREGS 		0x08
#define dsSEC 		0x00
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
//static const uint8_t DS1307_const[DSREGS] = { 0x30, 0x05, 0x14, 4, 0x24, 0x11, 0x21,  0b00000000};
static const uint8_t DS1307_const[DSREGS] = { 
    0x00,   // Seconds - Set to 00
    0x00,   // Minutes - Set to 00
    0x18,   // Hours (24-hour format) - Set to 18 (decimal) or 0x12 (hex)
    0x04,   // Day of the week (Thursday is 4)
    0x29,   // Day of the month - Set to 29
    0x02,   // Month of the year - Set to 2 (February)
    0x24,   // Year
    0b00000000
};


//struct tm *tm = localtime(&currentTime);

// memory copy of the DS1307 registers
uint8_t DS1307_regs[DSREGS];

// read the eight registers from the DS1307 into the memory
void DS1307read(void);

// write the contents of the memory into the eight hardware registers of the DS1307
void DS1307write(void);

unsigned long int dateTimeToSeconds(int year, int month, int day,
                                    int hour, int min, int seconds);

void getLocalTime(void);


void writeDataEEPROM(int address, uint32_t data);

uint32_t readDataEEPROM(uint8_t* address);


//static uint8_t BcdToBinary(uint8_t bcd);
//uint8_t BinaryToBcd(uint8_t binary);

#endif /* RTC_DS1307_H */
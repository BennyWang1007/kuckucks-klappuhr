/*
 * File:   ds1302.c
 * Author: laptop
 *
 * Created on 2025?1?13?, ?? 4:02
 */


#include <xc.h>
#include <stdint.h>
#include "ds1302.h"
#include "../threewire/threewire.h"

#define _BV(bit) (1 << (bit))

//DS1302 Register Addresses
const uint8_t DS1302_REG_TIMEDATE   = 0x80;
const uint8_t DS1302_REG_TIMEDATE_BURST = 0xBE;
const uint8_t DS1302_REG_TCR    = 0x90;
const uint8_t DS1302_REG_RAM_BURST = 0xFE;
const uint8_t DS1302_REG_RAMSTART   = 0xc0;
const uint8_t DS1302_REG_RAMEND     = 0xfd;
// ram read and write addresses are interleaved
const uint8_t DS1302RamSize = 31;


// DS1302 Trickle Charge Control Register Bits
enum DS1302TcrResistor
{
    DS1302TcrResistor_Disabled = 0,
    DS1302TcrResistor_2KOhm = 0b00000001,
    DS1302TcrResistor_4KOhm = 0b00000010,
    DS1302TcrResistor_8KOhm = 0b00000011,
    DS1302TcrResistor_MASK  = 0b00000011,
};

enum DS1302TcrDiodes
{
    DS1302TcrDiodes_None = 0,
    DS1302TcrDiodes_One      = 0b00000100,
    DS1302TcrDiodes_Two      = 0b00001000,
    DS1302TcrDiodes_Disabled = 0b00001100,
    DS1302TcrDiodes_MASK     = 0b00001100,
};

enum DS1302TcrStatus
{
    DS1302TcrStatus_Enabled  = 0b10100000,
    DS1302TcrStatus_Disabled = 0b01010000,
    DS1302TcrStatus_MASK     = 0b11110000,
};

// const uint8_t DS1302Tcr_Disabled = static_cast<uint8_t>(DS1302TcrStatus_Disabled) | 
//                                    static_cast<uint8_t>(DS1302TcrDiodes_Disabled) | 
//                                    static_cast<uint8_t>(DS1302TcrResistor_Disabled);

const uint8_t DS1302Tcr_Disabled = DS1302TcrStatus_Disabled | 
                                   DS1302TcrDiodes_Disabled | 
                                   DS1302TcrResistor_Disabled;


// DS1302 Clock Halt Register & Bits
const uint8_t DS1302_REG_CH = 0x80; // bit in the seconds register
const uint8_t DS1302_CH     = 7;


// Write Protect Register & Bits
const uint8_t DS1302_REG_WP = 0x8E; 
const uint8_t DS1302_WP = 7;


static uint8_t DS1302_ConvertDowToRtc(uint8_t dow)
{
    if (dow == 0)
    {
        dow = 7;
    }
    return dow;
}


uint8_t Uint8ToBcd(uint8_t val)
{
    return val + 6 * (val / 10);
}

uint8_t BcdToUint8(uint8_t val)
{
    return val - 6 * (val >> 4);
}


uint8_t BcdToBin24Hour(uint8_t bcdHour)
{
    uint8_t hour;
    if (bcdHour & 0x40)
    {
        // 12 hour mode, convert to 24
        char isPm = ((bcdHour & 0x20) != 0);

        hour = BcdToUint8(bcdHour & 0x1f);
        if (isPm)
        {
           hour += 12;
        }
    }
    else
    {
        hour = BcdToUint8(bcdHour);
    }
    return hour;
}

void DS1302_Begin()
{
    ThreeWire_Init();
    ThreeWire_Begin();
}


// bool GetIsWriteProtected()
// {
//     uint8_t wp = getReg(DS1302_REG_WP);
//     return !!(wp & _BV(DS1302_WP));
// }

char DS1302_GetIsWriteProtected()
{
    uint8_t wp = DS1302_GetReg(DS1302_REG_WP);
    return !!(wp & _BV(DS1302_WP));
}


// void SetIsWriteProtected(bool isWriteProtected)
// {
//     uint8_t wp = getReg(DS1302_REG_WP);
//     if (isWriteProtected)
//     {
//         wp |= _BV(DS1302_WP);
//     }
//     else
//     {
//         wp &= ~_BV(DS1302_WP);
//     }
//     setReg(DS1302_REG_WP, wp);
// }

void DS1302_SetIsWriteProtected(char isWriteProtected)
{
    uint8_t wp = DS1302_GetReg(DS1302_REG_WP);
    if (isWriteProtected)
    {
        wp |= _BV(DS1302_WP);
    }
    else
    {
        wp &= ~_BV(DS1302_WP);
    }
    DS1302_SetReg(DS1302_REG_WP, wp);
}

// uint8_t getReg(uint8_t regAddress)
// {
//     _wire.beginTransmission(regAddress | THREEWIRE_READFLAG);
//     uint8_t regValue = _wire.read();
//     _wire.endTransmission();
//     return regValue;
// }

// is date time valid, TODO


// bool GetIsRunning()
// {
//     uint8_t ch = getReg(DS1302_REG_CH);
//     return !(ch & _BV(DS1302_CH));
// }

char DS1302_GetIsRunning()
{
    uint8_t ch = DS1302_GetReg(DS1302_REG_CH);
    return !(ch & _BV(DS1302_CH));
}


// void SetIsRunning(bool isRunning)
// {
//     uint8_t ch = getReg(DS1302_REG_CH);
//     if (isRunning)
//     {
//         ch &= ~_BV(DS1302_CH);
//     }
//     else
//     {
//         ch |= _BV(DS1302_CH);
//     }
//     setReg(DS1302_REG_CH, ch);
// }

void DS1302_SetIsRunning(char isRunning)
{
    uint8_t ch = DS1302_GetReg(DS1302_REG_CH);
    if (isRunning)
    {
        ch &= ~_BV(DS1302_CH);
    }
    else
    {
        ch |= _BV(DS1302_CH);
    }
    DS1302_SetReg(DS1302_REG_CH, ch);
}


// uint8_t GetTrickleChargeSettings()
// {
//     uint8_t setting = getReg(DS1302_REG_TCR);
//     return setting;
// }

uint8_t DS1302_GetTrickleChargeSettings()
{
    uint8_t setting = DS1302_GetReg(DS1302_REG_TCR);
    return setting;
}

void DS1302_SetDateTime(const DS1302_DateTime_t* dateTime)
{
    ThreeWire_BeginTransmission(DS1302_REG_TIMEDATE_BURST);

    ThreeWire_Write(Uint8ToBcd(dateTime->second), 0);
    ThreeWire_Write(Uint8ToBcd(dateTime->minute), 0);
    ThreeWire_Write(Uint8ToBcd(dateTime->hour), 0);
    ThreeWire_Write(Uint8ToBcd(dateTime->dayOfMonth), 0);
    ThreeWire_Write(Uint8ToBcd(dateTime->month), 0);

    uint8_t rtcDow = DS1302_ConvertDowToRtc(dateTime->dayOfWeek);

    ThreeWire_Write(Uint8ToBcd(rtcDow), 0);
    ThreeWire_Write(Uint8ToBcd(dateTime->yearFrom2000), 0);
    ThreeWire_Write(0, 0);

    ThreeWire_EndTransmission();

}

DS1302_DateTime_t DS1302_GetDateTime()
{
    ThreeWire_BeginTransmission(DS1302_REG_TIMEDATE_BURST | THREEWIRE_READFLAG);

    DS1302_DateTime_t dateTime;
    dateTime.second = BcdToUint8(ThreeWire_Read() & 0x7F);
    dateTime.minute = BcdToUint8(ThreeWire_Read());
    dateTime.hour = BcdToBin24Hour(ThreeWire_Read());
    dateTime.dayOfMonth = BcdToUint8(ThreeWire_Read());
    dateTime.month = BcdToUint8(ThreeWire_Read());

    ThreeWire_Read();  // throwing away day of week as we calculate it
    
    dateTime.yearFrom2000 = BcdToUint8(ThreeWire_Read());
    
    ThreeWire_Read();  // throwing away write protect flag

    ThreeWire_EndTransmission();
    return dateTime;
}

// void SetMemory(uint8_t memoryAddress, uint8_t value)
// {
//     // memory addresses interleaved read and write addresses
//     // so we need to calculate the offset
//     uint8_t address = memoryAddress * 2 + DS1302_REG_RAMSTART;
//     if (address <= DS1302_REG_RAMEND)
//     {
//         setReg(address, value);
//     }
// }

void DS1302_SetMemory(uint8_t memoryAddress, uint8_t value)
{
    // memory addresses interleaved read and write addresses
    // so we need to calculate the offset
    uint8_t address = memoryAddress * 2 + DS1302_REG_RAMSTART;
    if (address <= DS1302_REG_RAMEND)
    {
        DS1302_SetReg(address, value);
    }
}

// uint8_t GetMemory(uint8_t memoryAddress)
// {
//     uint8_t value = 0;
//     // memory addresses interleaved read and write addresses
//     // so we need to calculate the offset
//     uint8_t address = memoryAddress * 2 + DS1302_REG_RAMSTART;
//     if (address <= DS1302_REG_RAMEND)
//     {
//         value = getReg(address);
//     }
//     return value;
// }

uint8_t DS1302_GetMemory(uint8_t memoryAddress)
{
    uint8_t value = 0;
    // memory addresses interleaved read and write addresses
    // so we need to calculate the offset
    uint8_t address = memoryAddress * 2 + DS1302_REG_RAMSTART;
    if (address <= DS1302_REG_RAMEND)
    {
        value = DS1302_GetReg(address);
    }
    return value;
}


// void SetTrickleChargeSettings(uint8_t setting)
// {
//     if ((setting & DS1302TcrResistor_MASK) == DS1302TcrResistor_Disabled) 
//     {
//         // invalid resistor setting, set to disabled
//         setting = DS1302Tcr_Disabled;
//     }
//     else if ((setting & DS1302TcrDiodes_MASK) == DS1302TcrDiodes_Disabled ||
//         (setting & DS1302TcrDiodes_MASK) == DS1302TcrDiodes_None) 
//     {
//         // invalid diode setting, set to disabled
//         setting = DS1302Tcr_Disabled;
//     }
//     else if ((setting & DS1302TcrStatus_MASK) != DS1302TcrStatus_Enabled) 
//     {
//         // invalid status setting, set to disabled
//         setting = DS1302Tcr_Disabled;
//     }

//     setReg(DS1302_REG_TCR, setting);
// }

void DS1302_SetTrickleChargeSettings(uint8_t setting)
{
    if ((setting & DS1302TcrResistor_MASK) == DS1302TcrResistor_Disabled) 
    {
        // invalid resistor setting, set to disabled
        setting = DS1302Tcr_Disabled;
    }
    else if ((setting & DS1302TcrDiodes_MASK) == DS1302TcrDiodes_Disabled ||
        (setting & DS1302TcrDiodes_MASK) == DS1302TcrDiodes_None) 
    {
        // invalid diode setting, set to disabled
        setting = DS1302Tcr_Disabled;
    }
    else if ((setting & DS1302TcrStatus_MASK) != DS1302TcrStatus_Enabled) 
    {
        // invalid status setting, set to disabled
        setting = DS1302Tcr_Disabled;
    }

    DS1302_SetReg(DS1302_REG_TCR, setting);
}

uint8_t DS1302_GetReg(uint8_t regAddress)
{
    ThreeWire_BeginTransmission(regAddress | THREEWIRE_READFLAG);
    uint8_t regValue = ThreeWire_Read();
    ThreeWire_EndTransmission();
    return regValue;
}

// void setReg(uint8_t regAddress, uint8_t regValue)
// {
//     _wire.beginTransmission(regAddress);
//     _wire.write(regValue);
//     _wire.endTransmission();
// }

void DS1302_SetReg(uint8_t regAddress, uint8_t regValue)
{
    ThreeWire_BeginTransmission(regAddress);
    ThreeWire_Write(regValue, 0);
    ThreeWire_EndTransmission();
}
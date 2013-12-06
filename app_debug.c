/******************************************************************************
 *  Copyright (C) Cambridge Silicon Radio Limited 2013
 *
 *  FILE
 *      app_debug.c
 *
 *  DESCRIPTION
 *      This file contains implementation of the debug functions
 *
 *****************************************************************************/

/*============================================================================*
 *  SDK Header Files
 *============================================================================*/

#include <debug.h>
#include <types.h>
#include <gatt_uuid.h>

/*============================================================================*
 *  Local Header File
 *============================================================================*/

#include "app_debug.h"

#if DEBUG_OUTPUT_ENABLED

/*============================================================================*
 *  Private Function Prototypes
 *============================================================================*/

static uint8 itoa(int16 value, char *str);

/*============================================================================*
 *  Private Function Implementations
 *============================================================================*/

/*----------------------------------------------------------------------------*
 *  NAME
 *      itoa
 *
 *  DESCRIPTION
 *      This function converts the signed integer(decimal) value
 *      into a string.
 *
 *  RETURNS/MODIFIES
 *      uint8 : returns the lenth of the string.
 *
 *---------------------------------------------------------------------------*/

uint8 itoa(int16 value, char *str)
{   
    const char *digits = "0123456789abcdefghijklmnopqrstuvwxyz";
    uint8 len = 0;
    uint16 ulvalue = value;
    char *p = str, *q = str;
    char temp;

    if ( value < 0)
    {
        *p++ = '-';
        q = p;
        ulvalue =-value;
    }
      
    do {
        *p++ = digits[ulvalue % 10];
        ulvalue /= 10;
        len++;
    } while (ulvalue > 0);

    *p-- = '\0';

    while (q < p) 
    {
        temp = *q;
        *q++ = *p;
        *p-- = temp;
    }
    
    return len;
}

/*============================================================================*
 *  Public Function Implementations
 *============================================================================*/

/*----------------------------------------------------------------------------*
 *  NAME
 *      AppDebugInit
 *
 *  DESCRIPTION
 *      This function is used to intialise debug output.
 *
 *  RETURNS
 *      Nothing.
 *
 *---------------------------------------------------------------------------*/
void AppDebugInit(void)
{
    /* Initialise debug output */
    DebugInit(0, NULL, NULL);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      AppDebugWriteBdAddress
 *
 *  DESCRIPTION
 *      This function prints out the Bluetooth address.
 *
 *  RETURNS
 *      Nothing.
 *
 *---------------------------------------------------------------------------*/
void AppDebugWriteBdAddress(TYPED_BD_ADDR_T* address)
{
    DebugWriteUint8(WORD_MSB(address->addr.nap));
    DebugWriteString(":");
    DebugWriteUint8(WORD_LSB(address->addr.nap));
    DebugWriteString(":");
    DebugWriteUint8(address->addr.uap);
    DebugWriteString(":");
    DebugWriteUint8(WORD_LSB(address->addr.lap >> 16));
    DebugWriteString(":");
    DebugWriteUint8(WORD_MSB(address->addr.lap));
    DebugWriteString(":");
    DebugWriteUint8(WORD_LSB(address->addr.lap));
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      AppDebugWriteInt
 *
 *  DESCRIPTION
 *      This function prints out the integer value
 *
 *  RETURNS
 *      Nothing.
 *
 *---------------------------------------------------------------------------*/
void AppDebugWriteInt(int value)
{
    char valueString[6];

    itoa(value, valueString);
    
    DebugWriteString(valueString);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      AppDebugWriteuuid128
 *
 *  DESCRIPTION
 *      This function prints out the 128-bit UUID
 *
 *  RETURNS
 *      Nothing.
 *
 *---------------------------------------------------------------------------*/
void AppDebugWriteUuid128(uint8* uuid)
{
    int i;
    
    for(i = 0; i < 16; i++)
    {
        DebugWriteUint8(uuid[i]);
    }
}

#endif /* DEBUG_OUTPUT_ENABLED */
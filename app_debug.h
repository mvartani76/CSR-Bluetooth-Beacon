/******************************************************************************
 *  Copyright (C) Cambridge Silicon Radio Limited 2012
 *
 *  FILE
 *      app_debug.h
 *
 *  DESCRIPTION
 *      This file defines a debug output routines for the application
 *
 *****************************************************************************/

#ifndef __APP_DEBUG_H__
#define __APP_DEBUG_H__

/*============================================================================*
 *  SDK Header Files
 *============================================================================*/

#include <bluetooth.h>
#include <debug.h>

/*============================================================================*
 *  Public Definitions
 *============================================================================*/

/* Debug settings */
#define DEBUG_OUTPUT_ENABLED            (TRUE)

/*============================================================================*
 *  Public Function Prototypes
 *============================================================================*/

#if DEBUG_OUTPUT_ENABLED

/* map the macros to the debug functions */
#define AppDebugWriteString(a)          DebugWriteString(a)
#define AppDebugWriteUint8(a)           DebugWriteUint8(a)
#define AppDebugWriteUint16(a)          DebugWriteUint16(a)
#define AppDebugWriteUint32(a)          DebugWriteUint32(a)


/* This function intialises the debug output */
extern void AppDebugInit(void);

/* Write the Bluetooth address */
extern void AppDebugWriteBdAddress(TYPED_BD_ADDR_T* address);

/* Write integer */
extern void AppDebugWriteInt(int value);

/* Write an error message with a status code */
#define AppDebugWriteErrorMessage(msg, error)   \
    DebugWriteString(msg);                      \
    DebugWriteString(" (0x");                   \
    DebugWriteUint16((uint16)error);            \
    DebugWriteString(")\r\n")

/* Write 128-bit UUID */
extern void AppDebugWriteUuid128(uint8* uuid);
    
#else

#define AppDebugWriteString(a)
#define AppDebugWriteUint8(a)
#define AppDebugWriteUint16(a)
#define AppDebugWriteUint32(a)
#define AppDebugInit()
#define AppDebugWriteBdAddress(a)
#define AppDebugWriteInt(a)
#define AppDebugWriteErrorMessage(msg, error)
#define AppDebugWriteUuid128(a)

#endif /* DEBUG_OUTPUT_ENABLED */

#endif /* __APP_DEBUG_H__ */


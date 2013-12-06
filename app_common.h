/******************************************************************************
 *  Copyright (C) Cambridge Silicon Radio Limited 2013
 *
 *  FILE
 *      app_common.h
 *
 *  DESCRIPTION
 *      Header definitions for common application attributes
 *
 ******************************************************************************/

#ifndef __APP_COMMON_H__
#define __APP_COMMON_H__

/*============================================================================*
 *  SDK Header Files
 *============================================================================*/

#include <types.h>

/*============================================================================*
 *  Public Definitions
 *============================================================================*/

/* Invalid UCID indicating we are not currently connected */
#define GATT_INVALID_UCID                   (0xFFFF)

/* Invalid UUID */
#define GATT_INVALID_UUID                   (0x0000)

/* Invalid Attribute Handle */
#define INVALID_ATT_HANDLE                  (0x0000)

/* AD Type for Appearance */
#define AD_TYPE_APPEARANCE                  (0x19)

/* Maximum Length of Device Name 
 * Note: Do not increase device name length beyond (DEFAULT_ATT_MTU -3 = 20) 
 * octets as GAP service at the moment doesn't support handling of Prepare 
 * write and Execute write procedures.
 */
#define DEVICE_NAME_MAX_LENGTH              (20)

#endif /* __APP_COMMON_H__ */

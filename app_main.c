/******************************************************************************
 *  Copyright (C) Cambridge Silicon Radio Limited, 2013
 *
 *  FILE
 *      app_main.c
 *
 *  DESCRIPTION
 *      This file defines a Beacon implementation
 *
 *****************************************************************************/

/*============================================================================*
 *  SDK Header Files
 *============================================================================*/

#include <main.h>
#include <mem.h>

#include <gatt.h>
#include <gatt_prim.h>
#include <gatt_uuid.h>
#include <ls_app_if.h>
#include <gap_app_if.h>
#include <config_store.h>

/*============================================================================*
 *  Local Header File
 *============================================================================*/

#include "app_common.h"
#include "app_debug.h"
#include "user_config.h"
#include "gap_conn_params.h"

/*=============================================================================*
 *  Private Definitions
 *============================================================================*/

/* Beacon advert size */
#define BEACON_ADVERT_SIZE              (26)

/* user key indices */
#define BEACON_UUID_MSW_USER_KEY_IDX    (0)     /* MSW of the beacon UUID */
#define BEACON_MAJOR_USER_KEY_IDX       (1)     /* Beacon major */
#define BEACON_MINOR_USER_KEY_IDX       (2)     /* Beacon minor */
#define BEACON_TX_POWER_USER_KEY_IDX    (3)     /* Beacon TX power */

#define BEACON_USER_KEY_DEFAULT_VALUE   (0)     /* default value */

/*============================================================================*
 *  Private Data Types
 *============================================================================*/

typedef struct {
    /* Beacon UUID */
    uint8 uuid[16];

    /* Beacon major */
    uint16 major;

    /* Beacon minor */
    uint16 minor;
    
    /* Beacon TX power */
    int8 txPower;
} APP_DATA_T;

/*============================================================================*
 *  Private Data
 *============================================================================*/

/* application data holder */
static APP_DATA_T g_app_data;

/*============================================================================*
 *  Private Function Prototypes
 *============================================================================*/

static void startBeaconing(void);
static void initBeacon(void);

/*============================================================================*
 *  Private Function Implementations
 *============================================================================*/

/*----------------------------------------------------------------------------*
 *  NAME
 *      initBeacon
 *
 *  DESCRIPTION
 *      This function initialises beacon data
 *
 *  RETURNS
 *      Nothing.
 *
 *---------------------------------------------------------------------------*/
void initBeacon(void)
{
    /* read the config values from CsKeys */
    uint16 beaconUuidMsw = CSReadUserKey(BEACON_UUID_MSW_USER_KEY_IDX);
    uint16 beaconMajor = CSReadUserKey(BEACON_MAJOR_USER_KEY_IDX);
    uint16 beaconMinor = CSReadUserKey(BEACON_MINOR_USER_KEY_IDX);
    uint16 txPower = CSReadUserKey(BEACON_TX_POWER_USER_KEY_IDX);
    
    /* set up beacon UUID */
    if(beaconUuidMsw == BEACON_USER_KEY_DEFAULT_VALUE)
    {
        g_app_data.uuid[0] = BEACON_UUID_00;
        g_app_data.uuid[1] = BEACON_UUID_01;
    }
    else
    {
        g_app_data.uuid[0] = WORD_MSB(beaconUuidMsw);
        g_app_data.uuid[1] = WORD_LSB(beaconUuidMsw);
    }
    
    g_app_data.uuid[2] = BEACON_UUID_02;
    g_app_data.uuid[3] = BEACON_UUID_03;
    g_app_data.uuid[4] = BEACON_UUID_04;
    g_app_data.uuid[5] = BEACON_UUID_05;
    g_app_data.uuid[6] = BEACON_UUID_06;
    g_app_data.uuid[7] = BEACON_UUID_07;
    g_app_data.uuid[8] = BEACON_UUID_08;
    g_app_data.uuid[9] = BEACON_UUID_09;
    g_app_data.uuid[10] = BEACON_UUID_10;
    g_app_data.uuid[11] = BEACON_UUID_11;
    g_app_data.uuid[12] = BEACON_UUID_12;
    g_app_data.uuid[13] = BEACON_UUID_13;
    g_app_data.uuid[14] = BEACON_UUID_14;
    g_app_data.uuid[15] = BEACON_UUID_15;

    /* set up beacon major */
    if(beaconMajor == BEACON_USER_KEY_DEFAULT_VALUE)
    {
        g_app_data.major = BEACON_DEFAULT_MAJOR;
    }
    else
    {
        g_app_data.major = beaconMajor;
    }
    
    /* set up beacon minor */
    if(beaconMinor == BEACON_USER_KEY_DEFAULT_VALUE)
    {
        g_app_data.minor = BEACON_DEFAULT_MINOR;
    }
    else
    {
        g_app_data.minor = beaconMinor;
    }

    /* set up the TX power */
    if(txPower == BEACON_USER_KEY_DEFAULT_VALUE)
    {
        g_app_data.txPower = BEACON_DEFAULT_TX_POWER;
    }
    else
    {
        g_app_data.txPower = txPower;
    }
}


/*----------------------------------------------------------------------------*
 *  NAME
 *      startBeaconing
 *
 *  DESCRIPTION
 *      This function is called to start beacon advertisements
 *
 *  RETURNS
 *      Nothing.
 *
 *---------------------------------------------------------------------------*/
void startBeaconing(void)
{
    uint8 advData[BEACON_ADVERT_SIZE];
    uint16 offset = 0;
    
    /* set the GAP Broadcaster role */
    GapSetMode(gap_role_broadcaster,
               gap_mode_discover_no,
               gap_mode_connect_no,
               gap_mode_bond_no,
               gap_mode_security_none);
    
    /* clear the existing advertisement data */
    LsStoreAdvScanData(0, NULL, ad_src_advertise);

    /* set the advertisement interval */
    GapSetAdvInterval(BEACON_ADVERTISING_INTERVAL_MIN, BEACON_ADVERTISING_INTERVAL_MAX);
    
    /* store the manufacturing data */
    advData[offset++] = AD_TYPE_MANUF;

    /* Apple company code, little endian */
    advData[offset++] = 0x4C;
    advData[offset++] = 0x00;
    
    advData[offset++] = 2;         /* Magic number */
    advData[offset++] = 0x15;      /* Length of the beacon payload */
    
    /* Beacon UUID */
    MemCopy(advData + offset, g_app_data.uuid, sizeof(g_app_data.uuid));
    offset += sizeof(g_app_data.uuid);

    /* Beacon major */
    advData[offset++] = WORD_MSB(g_app_data.major);
    advData[offset++] = WORD_LSB(g_app_data.major);
    
    /* Beacon minor */
    advData[offset++] = WORD_MSB(g_app_data.minor);
    advData[offset++] = WORD_LSB(g_app_data.minor);

    /* Beacon TX Power */
    advData[offset++] = g_app_data.txPower;

    /* store the advertisement data */
    LsStoreAdvScanData(BEACON_ADVERT_SIZE, advData, ad_src_advertise);
    
    /* Start broadcasting */
    LsStartStopAdvertise(TRUE, whitelist_disabled, ls_addr_type_random);
}


/*============================================================================*
 *  Public Function Implementations
 *============================================================================*/

/*----------------------------------------------------------------------------*
 *  NAME
 *      AppPowerOnReset
 *
 *  DESCRIPTION
 *      This function is called just after a power-on reset (including after
 *      a firmware panic).
 *
 *  RETURNS
 *      Nothing.
 *
 *---------------------------------------------------------------------------*/

void AppPowerOnReset(void)
{
    /* empty */
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      AppInit
 *
 *  DESCRIPTION
 *      This function is called after a power-on reset (including after a
 *      firmware panic) or after an HCI Reset has been requested.
 *
 *      NOTE: In the case of a power-on reset, this function is called
 *      after AppPowerOnReset().
 *
 *  RETURNS
 *      Nothing.
 *
 *---------------------------------------------------------------------------*/

void AppInit(sleep_state last_sleep_state)
{
    /* initialise application debug */
    AppDebugInit();

    AppDebugWriteString("\r\n\r\n*****************\r\n");
    AppDebugWriteString("Beacon example\r\n");
    
    /* Initialise GATT entity */
    GattInit();
    
    /* Initialise beacon data */
    initBeacon();
    
    /* Start beaconing */
    startBeaconing();   
}


/*----------------------------------------------------------------------------*
 *  NAME
 *      AppProcessSystemEvent
 *
 *  DESCRIPTION
 *      This user application function is called whenever a system event, such
 *      as a battery low notification, is received by the system.
 *
 *  RETURNS
 *      Nothing.
 *
 *---------------------------------------------------------------------------*/

void AppProcessSystemEvent(sys_event_id id, void *data)
{
    /* empty */
}


/*----------------------------------------------------------------------------*
 *  NAME
 *      AppProcessLmEvent
 *
 *  DESCRIPTION
 *      This user application function is called whenever a LM-specific event is
 *      received by the system.
 *
 *  RETURNS
 *      Nothing.
 *
 *---------------------------------------------------------------------------*/

extern bool AppProcessLmEvent(lm_event_code event_code, 
                              LM_EVENT_T *p_event_data)
{
    return TRUE;
}

//=====[Libraries]=============================================================

#include "mbed.h"
#include "arm_book_lib.h"

#include "light_system.h"
#include "bright_control.h"
#include "light_level_control.h"
#include "ldr_sensor.h"
#include "pc_serial_com.h"

//=====[Declaration of private defines]========================================

#define LDR_DATA_UPDATE   1s //1 second
//=====[Declaration of private data types]=====================================

//=====[Declaration and initialization of public global objects]===============

//=====[Declaration and initialization of private global objects]===============

//=====[Declaration of external public global variables]=======================

static float lightSystemLoopGain = 0.01;

//=====[Declaration and initialization of public global variables]=============

//=====[Declaration and initialization of private global variables]============

static float dutyCycle = 0.5f;
static bool brightnessRGBLedRedChangeEnabled = true;
static bool brightnessRGBLedGreenChangeEnabled = true;
static bool brightnessRGBLedBlueChangeEnabled = true;

//=====[Implementations of public functions]===================================

void lightSystemInit()
{
    brightControlInit();
}

void lightSystemUpdate()
{
    char str[20];
    static int i = 0;
    static float filteredLDR = 0.5f; // Initial value for low pass filter

    // Low pass filter on LDR reading (alpha = 0.1 for smoothing and 0.9 as 1 - alpha)
    // A low pass filter would process the LDR data 
    // to remove high-frequency noise before it’s used in calculations.
    filteredLDR = 0.9 * filteredLDR + 0.1 * ldrSensorRead();

    dutyCycle = filteredLDR;
                          

    if ( brightnessRGBLedRedChangeEnabled ) setDutyCycle( RGB_LED_RED, dutyCycle );
    if ( brightnessRGBLedGreenChangeEnabled ) setDutyCycle( RGB_LED_GREEN, dutyCycle );
    if ( brightnessRGBLedBlueChangeEnabled ) setDutyCycle( RGB_LED_BLUE, dutyCycle );
    
    
    pcSerialComStringWrite("Raw LDR: ");
    sprintf( str, "%0.4f", ldrSensorRead() );
    pcSerialComStringWrite( str );
    pcSerialComStringWrite("\r\n");
    pcSerialComStringWrite("Filtered LDR: ");
    sprintf( str, "%0.4f", filteredLDR );
    pcSerialComStringWrite( str );
    pcSerialComStringWrite("\r\n");
    pcSerialComStringWrite("Duty Cycle: ");
    sprintf( str, "%0.4f", dutyCycle );
    pcSerialComStringWrite( str );
    pcSerialComStringWrite("\r\n\n");
    ThisThread::sleep_for(LDR_DATA_UPDATE);

    i++;
}

void lightSystemBrightnessChangeEnable( lightSystem_t light, bool state )
{
    switch( light ) {
        case RGB_LED_RED:
            brightnessRGBLedRedChangeEnabled = state;
        break;
        case RGB_LED_GREEN:
            brightnessRGBLedGreenChangeEnabled = state;
        break;
        case RGB_LED_BLUE:
            brightnessRGBLedBlueChangeEnabled = state;
        break;
        default:
        break;
    }
}

//=====[Implementations of private functions]==================================

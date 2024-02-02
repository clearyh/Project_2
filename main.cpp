//=====[Libraries]=============================================================

#include "mbed.h"
#include "arm_book_lib.h"

//=====[Defines]===============================================================

#define DAY_TH    0.70
#define DSK_TH    0.25
#define LDR_HYST   0.1
#define HL_OFF_TH  0.3
#define HL_ON_TH   0.7
#define HL_HYST    0.1
#define DELAY_MS    10
#define DEBOUNCE_MS 50
#define HB_ON      0.9

//=====[Declaration of public data types]======================================

AnalogIn potentiometer(A0);
AnalogIn lightSensor(A1);

DigitalIn ignition(BUTTON1);
DigitalIn driverSeat(D4);
DigitalIn highBeamSwitch(D3);

DigitalOut lowBeam(D5);
DigitalOut highBeam(D6);
DigitalOut engine(LED2);

typedef enum HLMode {
    HL_ON,
    HL_AUTO,
    HL_OFF,
} HLMode_t;

typedef enum ButtonState {
    B_ON,
    B_RISING,
    B_OFF,
    B_FALLING,
} ButtonState_t;

typedef enum LDRState {
    L_DAY,
    L_BETWEEN,
    L_DUSK,
} LDRState_t;

HLMode_t headlightMode;
ButtonState_t ignitionState;
LDRState_t LDRState;

int debounce_accumulated_time_ms = 0;
int headlight_accumulated_time_ms = 0;

//=====[Declarations (prototypes) of public functions]=========================

void inputsInit();
void outputsInit();
void delayAccumulate();
void HLModeUpdate();
void ignitionStateUpdate();
void headlightUpdate();
void LDRStateUpdate();
void highBeamUpdate();

//=====[Main function, the program entry point after power on or reset]========

int main()
{
    headlightMode = HL_OFF;
    ignitionState = B_OFF;
    LDRState = L_DAY;
    inputsInit();
    outputsInit();

    while (true) {
        delayAccumulate();
        ignitionStateUpdate();
        headlightUpdate();
        highBeamUpdate();
    }
}

//=====[Implementations of public functions]===================================

void inputsInit()
{
    ignition.mode(PullDown);
    driverSeat.mode(PullDown);
}

void outputsInit()
{
    engine.write(OFF);
    lowBeam.write(ON);
    highBeam.write(ON);
}

void delayAccumulate() {
    delay(DELAY_MS);
    debounce_accumulated_time_ms += 10;
    headlight_accumulated_time_ms += 10;
}

void HLModeUpdate() {
    float f = potentiometer.read();
    if (headlightMode == HL_OFF) {
        if (f > HL_OFF_TH + HL_HYST) {
            headlightMode = HL_AUTO;
        }
        return;
    }
    if (headlightMode == HL_AUTO) {
        if (f < HL_OFF_TH) {
            headlightMode = HL_OFF;
        }
        if (f > HL_ON_TH) {
            headlightMode = HL_ON;
        }
        return;
    }
    if (headlightMode == HL_ON) {
        if (f < HL_ON_TH - HL_HYST) {
            headlightMode = HL_AUTO;
        }
        return;
    }
}

void ignitionStateUpdate () {
    bool ds = driverSeat.read();
    bool ig = ignition.read();
    if (ignitionState == B_OFF && ig == ON) {
        ignitionState = B_RISING;
        debounce_accumulated_time_ms = 0;
    }
    if (ignitionState == B_ON && ig == OFF) {
        ignitionState = B_FALLING;
        debounce_accumulated_time_ms = 0;
    }
    if ((ignitionState == B_RISING) && (debounce_accumulated_time_ms >= DEBOUNCE_MS)) {
        if (ig == ON) {
            ignitionState = B_ON;
        }
        else {
            ignitionState = B_OFF;
        }
    }
    if ((ignitionState == B_FALLING) && (debounce_accumulated_time_ms >= DEBOUNCE_MS)) {
        if (ig == ON) {
            ignitionState = B_ON;
        }
        else {
            ignitionState = B_OFF;
            if (ds == ON && engine == OFF) {
                engine.write(ON);
                return;
            }
            if (engine == ON) {
                engine.write(OFF);
            }
        }
    }
}

void headlightUpdate() {
    if (engine == OFF) {
        lowBeam.write(ON);
    }
    else {
        HLModeUpdate();
        LDRStateUpdate();
        if (headlightMode == HL_OFF) {
            lowBeam.write(ON);
        }
        if (headlightMode == HL_ON) {
            lowBeam.write(OFF);
        }
        if (headlightMode == HL_AUTO) {
            if (headlight_accumulated_time_ms == 1000 && LDRState == L_DUSK) {
                lowBeam.write(OFF);
            }
            if (headlight_accumulated_time_ms == 2000 && LDRState == L_DAY) {
                lowBeam.write(ON);
            }
        }
    }
}

void LDRStateUpdate() {
    float f = lightSensor.read();
    if (LDRState == L_DUSK) {
        if (f > DSK_TH + LDR_HYST) {
            LDRState = L_BETWEEN;
        }
        return;
    }
    if (LDRState == L_BETWEEN) {
        if (f < DSK_TH) {
            LDRState = L_DUSK;
            headlight_accumulated_time_ms = 0;
        }
        if (f > DAY_TH) {
            LDRState = L_DAY;
            headlight_accumulated_time_ms = 0;
        }
        return;
    }
    if (LDRState == L_DAY) {
        if (f < DAY_TH - LDR_HYST) {
            LDRState = L_BETWEEN;
        }
        return;
    }
}

void highBeamUpdate()
{
    if (highBeamSwitch.read() == ON || engine == OFF) {
    highBeam = ON;
    }
    else
    {
    highBeam = OFF;
    }
}

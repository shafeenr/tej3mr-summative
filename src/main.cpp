#include "Arduino.h"
#include "SevSeg.h"
#include "ShiftRegister74HC595.h"

//define instead of constant integers to save on memory
#define shiftRegisterAmount 1

#define clockPin 6
#define serialDataPin 7
#define latchPin 8

#define redOne 0
#define redTwo 1

#define yellowOne 2
#define yellowTwo 3

#define greenOne 4
#define greenTwo 5

#define piezo 9

// pedestrian object initialized here
SevSeg ped;

// traffic light timer shift register initialized here
ShiftRegister74HC595 trafficLight(shiftRegisterAmount, serialDataPin, clockPin, latchPin);

void trafficTimer(int timeLimit);

void setup()
{
    byte hardwareConfig = COMMON_CATHODE;
    byte numDigits = 1;
    byte digitPins[] = {};
    byte segmentPins[] = {6, 5, 2, 3, 4, 7, 8, 9};
    bool resistorsOnSegments = true;

    ped.begin(hardwareConfig, numDigits, digitPins, segmentPins, resistorsOnSegments);
    ped.setBrightness(90);
}

void loop()
{
    // signals both directions to stop for two seconds)
    trafficLight.set(redOne, HIGH);
    trafficLight.set(redTwo, HIGH);
    trafficTimer(2);

    // signals direction one to proceed, turns on buzzer, calls ped countdown timer function
    trafficLight.set(redOne, LOW);
    trafficLight.set(greenOne, HIGH);
    tone(piezo, 500);
    trafficTimer(5);
    noTone(piezo); // turns off buzzer

    // signals direction one to slow down for three seconds
    trafficLight.set(greenOne, LOW);
    trafficLight.set(yellowOne, HIGH);
    trafficTimer(3);

    // both directions told to stop for two seconds (direction two is already red)
    trafficLight.set(redOne, HIGH);
    trafficTimer(2);

    // signals direction two to proceed
    trafficLight.set(redTwo, LOW);
    trafficLight.set(greenTwo, HIGH);
    tone(piezo, 200);
    trafficTimer(5);
    noTone(piezo);

    trafficLight.set(greenTwo, LOW);
    trafficLight.set(yellowTwo, HIGH);
    trafficTimer(3);
}

void trafficTimer(int timeLimit)
{
    // function that simplifies the pedestrian timer countdown process

    for (int i = timeLimit; i >= 0; i--)
    {
        ped.setNumber(i);
        ped.refreshDisplay();
        delay(i != 0 ? 1000 : 2000);
    }
}
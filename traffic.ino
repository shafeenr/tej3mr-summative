#include <SevSeg.h>

#include <ShiftRegister74HC595.h> // not my library, used to simplify shift register usage
//#include <BCDDecoderDriver74LS47.h> // my library, incredibly primitive but allows for each IC to be used as an object

//define instead of constant integers to save on memory
#define shiftRegisterAmount 1

#define pinA 9
#define pinB 10
#define pinC 11
#define pinD 12
#define pinRBI 13

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

// pedestrian IC controller initialized here
//BCDDecoderDriver74LS47 ped(pinA, pinB, pinC, pinD, pinRBI);
SevSeg ped;

// traffic light timer shift register initialized here
ShiftRegister74HC595 trafficLight(shiftRegisterAmount, serialDataPin, clockPin, latchPin);

void setup()
{
    //ped.display(true);
    byte numDigits = 1;
    byte digitPins[] = {};
    byte segmentPins[] = {6, 5, 2, 3, 4, 7, 8, 9};
    bool resistorsOnSegments = true;

    byte hardwareConfig = COMMON_CATHODE; 
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

    for (int i = timeLimit; i <= 1; i--)
    {
        ped.setNumber(i);
        ped.refreshDisplay();
        delay(1000);
    }
    ped.setNumber(0);
    ped.refreshDisplay();
}

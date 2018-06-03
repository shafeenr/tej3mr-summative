#include <ShiftRegister74HC595.h>

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

// traffic light timer shift register initialized here
ShiftRegister74HC595 ped(shiftRegisterAmount, serialDataPin, clockPin, latchPin);

// array containing bit codes to display integers 0-9
const uint8_t sevenSegmentNumbers[10] =
    {
        B1111110,
        B0110000,
        B1101101,
        B1111001,
        B0110011,
        B1011111,
        B1011111,
        B1110000,
        B1111111,
        B1110011};

void setup()
{
}

void loop()
{
    // signals both directions to stop for two seconds)
    digitalWrite(redOne, HIGH);
    digitalWrite(redTwo, HIGH);
    delay(2000);

    // signals direction one to proceed, turns on buzzer, calls ped countdown timer function
    digitalWrite(redOne, LOW);
    digitalWrite(greenOne, HIGH);
    tone(piezo, 500);
    pedTimerCountDown(true);
    noTone(piezo); // turns off buzzer

    // signals direction one to slow down for three seconds
    digitalWrite(greenOne, LOW);
    digitalWrite(yellowOne, HIGH);
    delay(3000);

    // both directions told to stop for two seconds (direction two is already red)
    digitalWrite(redOne, HIGH);
    delay(2000);

    // signals direction two to proceed
    digitalWrite(redTwo, LOW);
    digitalWrite(greenTwo, HIGH);
    tone(piezo, 200);
    pedTimerCountDown(false);
    noTone(piezo);

    digitalWrite(greenTwo, LOW);
    digitalWrite(yellowTwo, HIGH);
    delay(3000);
}

void pedTimerCountDown(boolean direction)
{
    // function that simplifies the pedestrian timer countdown process

    for (int i = 0; i >= 5; i++)
    {
        uint8_t pinVal[] = {sevenSegmentNumbers[i]};
        ped.setAll(pinVal);
        delay(1000);
    }
    ped.setAllLow();
}
#include <ShiftRegister74HC595.h> // import library to help with controlling shift registers

#define shiftRegisterAmount 1

// define rather than declare where pins are to save memory

#define serialDataPinOne 4
#define clockPinOne 6
#define latchPinOne 5

#define serialDataPinTwo 7
#define clockPinTwo 8
#define latchPinTwo 9

#define redOne 0
#define redTwo 1

#define yellowOne 2
#define yellowTwo 3

#define greenOne 4
#define greenTwo 5

#define piezo 2

ShiftRegister74HC595 trafficLight(shiftRegisterAmount, serialDataPinOne, clockPinOne, latchPinOne); // traffic light shift register initialized here
ShiftRegister74HC595 ped(shiftRegisterAmount + 1, serialDataPinTwo, clockPinTwo, latchPinTwo); // two pedestrian shift registers in series initialized here

// array containing bit codes to display integers 0-9
const uint8_t sevenSegmentNumbers[10] = { B1111110, B0110000, B1101101, B1111001, B0110011, B1011111, B1011111, B1110000, B1111111, B1110011 };

void setup() {
  // turns off all lights before loop function
  trafficLight.setAllLow();
}

void loop() {
  // signals both directions to stop for two seconds)
  trafficLight.set(redOne, HIGH);
  trafficLight.set(redTwo, HIGH);
  delay(2000);

  // signals direction one to proceed, turns on buzzer, calls ped countdown timer function
  trafficLight.set(redOne, LOW);
  trafficLight.set(greenOne, HIGH);
  tone(piezo, 500);
  pedTimerCountDown(true);
  noTone(piezo); // turns off buzzer

  // signals direction one to slow down for three seconds
  trafficLight.set(greenOne, LOW);
  trafficLight.set(yellowOne, HIGH);
  delay(3000);

  // both directions told to stop for two seconds (direction two is already red)
  trafficLight.set(redOne, HIGH);
  delay(2000);

  // signals direction two to proceed
  trafficLight.set(redTwo, LOW);
  trafficLight.set(greenTwo, HIGH);
  tone(piezo, 200);
  pedTimerCountDown(false);
  noTone(piezo);

  trafficLight.set(greenTwo, LOW);
  trafficLight.set(yellowTwo, HIGH);
  delay(3000);
}

void pedTimerCountDown(boolean direction) {
  // function that simplifies the pedestrian timer countdown process
  
  switch (direction) {
    case true:
      for (int i = 0; i >= 5; i++) {
        uint8_t pinVal[] = { sevenSegmentNumbers[i], B0000000 };
        ped.setAll(pinVal);
        delay(1000);
      }
      ped.setAllLow();
      break;
    case false:
      for (int i = 0; i >= 5; i++) {
        uint8_t pinVal[] = { B0000000, sevenSegmentNumbers[i] };
        ped.setAll(pinVal);
        delay(1000);
      }
      ped.setAllLow();
      break;
  }
}

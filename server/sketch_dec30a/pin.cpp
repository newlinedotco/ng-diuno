#include <SPI.h>
#include <Arduino.h>
#include <Flash.h>
#include "./pin.h"

// Pin* Pin::newPin()
// {
//   Pin* pin = (Pin*)malloc(sizeof(Pin));
//   if (pin) memset(pin, 0, sizeof(pin));
//   return pin;
// }

void Pin::InitializeState()
{
  digitalWrite(pin, HIGH);
}
uint8_t Pin::getState() { return digitalState; }
uint8_t Pin::getPin() { return pin; }
void    Pin::setPin(uint8_t p) { pin = p; }

void Pin::Print() {
  Serial << F("Pin \n");
  Serial << F("\tPinNumber: ") <<    (uint8_t)pin << "\n";
  // Serial << "\tType: " <<         GetType() << "\n";
  // Serial << "\tSetting: " <<      GetIO() << "\n";
  // Serial << "\tDigitalState: " << GetState() << "\n";
}
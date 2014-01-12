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

Pin::Pin()             { _pin = 0; };
Pin::Pin(uint8_t num, PinType type)  { 
  _pin = num; 
  _type = type;
};
Pin::~Pin()            {};

void Pin::InitializeState()
{
  digitalWrite(_pin, HIGH);
}
uint8_t Pin::getState()         {
  _digitalState = digitalRead(_pin);
  return _digitalState; 
}
uint8_t Pin::setState(uint8_t state) {
  digitalWrite(_pin, state);
  delay(50);
  return getState();
}
uint8_t Pin::getPin()           { return _pin; }
void    Pin::setPin(uint8_t p)  { _pin = p; }

void Pin::Print() {
  Serial << F("Pin \n");
  Serial << F("\tPinNumber: ") <<    _pin << "\n";
  // Serial << "\tType: " <<         GetType() << "\n";
  // Serial << "\tSetting: " <<      GetIO() << "\n";
  // Serial << "\tDigitalState: " << GetState() << "\n";
}
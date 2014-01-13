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
  pinMode(_pin, OUTPUT);
  digitalWrite(_pin, LOW);
}
uint8_t Pin::getState(){
  return _digitalState = digitalRead(_pin);
}
uint8_t Pin::setState(uint8_t state) {
  digitalWrite(_pin, state);
  return getState();
}
uint8_t Pin::getPin()           { return _pin; }
void    Pin::setPin(uint8_t p)  { _pin = p; }

void    Pin::setMode(uint8_t t) {
  _mode = t;
  pinMode(_pin, t == 0 ? INPUT : OUTPUT);
}

void Pin::Print() {
  Serial << F("Pin \n");
  Serial << F("\tPinNumber: ") <<    _pin << "\n";
  Serial << F("\tDigitalState: ") << _digitalState << "\n";
  // Serial << "\tType: " <<         GetType() << "\n";
  // Serial << "\tSetting: " <<      GetIO() << "\n";
}
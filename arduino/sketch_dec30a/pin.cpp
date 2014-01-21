#include <SPI.h>
#include <Arduino.h>
#include <Flash.h>
#include <OneWire.h>
#include "./pin.h"

Pin::Pin()             { _pin = 0; };
Pin::Pin(uint8_t num, PinType type)  { 
  _pin = num; 
  _type = type;
  _currentValue = NULL;
};
Pin::~Pin()            {};

void Pin::InitializeState()
{
  switch(_type) {
    case INPUT:
      pinMode(_pin, INPUT);
      break;
    case ONEWIRE:
      pinMode(_pin, INPUT);
      _currentValue = 0;
      break;
    default:
      pinMode(_pin, OUTPUT);
      digitalWrite(_pin, LOW);
  }
}
float Pin::getState(){
  switch(_type) {
    case ONEWIRE:
      return _currentValue;
      break;
    default:
      _digitalState = digitalRead(_pin);
      return _digitalState;
  }
}
uint8_t Pin::setState(uint8_t state) {
  digitalWrite(_pin, state);
  _digitalState = state;
  return _digitalState;
}
uint8_t Pin::getPin()           { return _pin; }
void    Pin::setPin(uint8_t p)  { _pin = p; }

void    Pin::setMode(uint8_t t) {
  _mode = t;
  pinMode(_pin, t == 0 ? INPUT : OUTPUT);
}

void  Pin::setCurrentValue(float f) {
  _currentValue = f;
}

float Pin::getCurrentValue() {
  return _currentValue;
}

void Pin::Print() {
  Serial << F("Pin \n");
  Serial << F("\tPinNumber: ") <<    _pin << "\n";
  Serial << F("\tDigitalState: ") << _digitalState << "\n";
}
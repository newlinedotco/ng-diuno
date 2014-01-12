#ifndef _PIN_H
#define _PIN_H

// PINS
enum PinType {DIGITAL, ANALOG};
enum PinSetting { INPUT_TYPE, OUTPUT_TYPE};
enum PinState { HIGH_STATE, LOW_STATE };

class Pin {
private:
  uint8_t pin;
  uint8_t digitalState;
  // PinType type;
  // PinSetting setting;
public:
  Pin()             { pin = 0; };
  Pin(uint8_t num)  { pin = num; };
  ~Pin()            {};

  float analogValue;
  uint8_t getPin();
  void    setPin(uint8_t);
  uint8_t getState();

  void InitializeState();
  void Print();
  // Pin *newPin();
};

#endif
#ifndef _PIN_H
#define _PIN_H

// PINS
enum PinType {DIGITAL, ANALOG};
enum PinSetting { INPUT_TYPE, OUTPUT_TYPE};
enum PinState { HIGH_STATE, LOW_STATE };

class Pin {
private:
  uint8_t _pin;
  uint8_t _digitalState;
  PinType _type;
  // PinSetting setting;
public:
  Pin();
  Pin(uint8_t num, PinType type);
  ~Pin();

  float analogValue;
  uint8_t getPin();
  void    setPin(uint8_t);
  uint8_t getState();
  uint8_t setState(uint8_t state);

  void InitializeState();
  void Print();
  // Pin *newPin();
};

#endif
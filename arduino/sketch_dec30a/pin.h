#ifndef _PIN_H
#define _PIN_H

// PINS
enum PinType { INPUT_T, OUTPUT_T, ONEWIRE};
enum PinState { HIGH_STATE, LOW_STATE };

class Pin {
private:
  uint8_t _pin;
  uint8_t _digitalState;
  uint8_t _type;
  uint8_t _mode;

  float   _currentValue;
public:
  Pin();
  Pin(uint8_t num, PinType type);
  ~Pin();

  float analogValue;
  uint8_t getPin();
  void    setPin(uint8_t);
  float   getState();
  uint8_t setState(uint8_t state);

  void    setMode(uint8_t);
  void    setCurrentValue(float);
  float   getCurrentValue();

  void InitializeState();
  void Print();
};

#endif
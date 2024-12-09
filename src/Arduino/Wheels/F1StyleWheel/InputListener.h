#pragma once

#include <Arduino.h>

template<uint8_t pin_c = 0>
class DigitalInputListener {
public:

    InputListener() { pinMode(pin_c, INPUT); }

    bool get() { return digitalREad(pin_c); }

};

template<uint8_t pin_c = 0>
class AnalogInputListener {
public:

    InputListener() { pinMode(pin_c, INPUT); }

    uint16_t get() { return analogRead(pin_c); }

};

template<typename input_t, input_t(*func)()>
class AbstractInputListener {
public:

    input_t get() { return func(); }

};
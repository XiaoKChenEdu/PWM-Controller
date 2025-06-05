#ifndef ONBOARD_PWM_H
#define ONBOARD_PWM_H

#include "PWMBackend.h"
#include <Arduino.h>

class OnboardPWM : public PWMBackend {
private:
    float currentFrequency;
    int resolution;

public:
    OnboardPWM(int pwmResolution = 255) 
        : currentFrequency(490), resolution(pwmResolution) {}

    void begin() override {
        // Arduino PWM is ready by default
    }

    void setPWMFreq(float frequency) override {
        currentFrequency = frequency;
        
    }

    void setPWM(int pin, int dutyCycle) override {
        dutyCycle = constrain(dutyCycle, 0, resolution);
        
        // Ensure pin is set to output mode
        pinMode(pin, OUTPUT);
        
        // Use standard Arduino PWM
        analogWrite(pin, dutyCycle);
    }

    void stop(int pin) override {
        analogWrite(pin, 0);
    }

    int getResolution() override {
        return resolution;
    }

    int getMaxDutyCycle() override {
        return resolution;
    }

    PWMBackendType getBackendType() const override {
        return PWMBackendType::ONBOARD_PWM;
    }
};

#endif 
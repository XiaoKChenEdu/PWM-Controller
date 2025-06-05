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
        // Pin 3 uses Timer2, pin 9&10 use Timer1, pin 5&6 use Timer0
    }

    void setPWMFreq(float frequency) override {
        currentFrequency = frequency;
        
        // For servo applications, we'll use Arduino's default PWM frequency
        // Pin 3 (Timer2) default frequency is ~490Hz which works for servos
        // Note: Custom frequency control on Arduino requires complex timer manipulation
        // For servo control, the pulse width timing is more important than frequency
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
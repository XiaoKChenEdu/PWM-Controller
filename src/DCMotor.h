#ifndef DC_MOTOR_H
#define DC_MOTOR_H

#include "Motor.h"

#ifdef ARDUINO
    #include <Arduino.h>
#else
    #include <algorithm>
    
    // Arduino-like functions for Raspberry Pi
    inline float constrain(float x, float a, float b) {
        return std::max(a, std::min(x, b));
    }
#endif

class DCMotor : public Motor {
private:
    bool invertDirection;

public:
    DCMotor(int pwmPin,
            PWMBackend* backend,
            float minSpeed = 20.0f,        // Minimum speed percentage where motor starts
            float maxSpeed = 100.0f,       // Maximum speed percentage
            bool invert = false)           // Invert direction
        : Motor(pwmPin, backend, minSpeed, maxSpeed),
          invertDirection(invert) {
        
        pwmBackend->begin();
        pwmBackend->setPWMFreq(1000);  // 1kHz for DC motors
        #ifdef ARDUINO
            pinMode(pin, OUTPUT);
        #endif
        stop();
    }

    void setSpeed(float percentage) override {
        currentSpeed = constrain(percentage, 0.0f, 100.0f);
        
        // Apply direction inversion if needed
        float adjustedSpeed = invertDirection ? (100.0f - currentSpeed) : currentSpeed;
        
        // Map speed to duty cycle
        float dutyCycle = mapSpeedToDutyCycle(adjustedSpeed);
        
        // Convert to backend-specific value
        int pwmValue = (dutyCycle / 100.0f) * pwmBackend->getMaxDutyCycle();
        
        pwmBackend->setPWM(pin, pwmValue);
    }

    void stop() override {
        currentSpeed = 0.0f;
        pwmBackend->stop(pin);
    }

    // DC Motor specific methods
    void setDirection(bool invert) {
        invertDirection = invert;
        // Re-apply current speed with new direction
        setSpeed(currentSpeed);
    }

    bool getDirection() const {
        return invertDirection;
    }
};

#endif 
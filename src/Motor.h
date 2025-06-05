#ifndef MOTOR_H
#define MOTOR_H

#include "PWMBackend.h"

class Motor {
protected:
    float minDutyCycle;  // Minimum duty cycle where motor starts moving
    float maxDutyCycle;  // Maximum duty cycle
    float currentSpeed;  // Current speed as a percentage (0-100)
    int pin;            // PWM pin/channel number
    PWMBackend* pwmBackend;  // PWM backend to use

public:
    Motor(int pwmPin, PWMBackend* backend, float minDuty = 0.0f, float maxDuty = 100.0f)
        : minDutyCycle(minDuty), maxDutyCycle(maxDuty), currentSpeed(0.0f), pin(pwmPin), pwmBackend(backend) {}
    
    virtual ~Motor() = default;

    // Set speed as a percentage (0-100)
    virtual void setSpeed(float percentage) = 0;
    
    // Get current speed as a percentage
    float getSpeed() const { return currentSpeed; }
    
    // Stop the motor
    virtual void stop() = 0;

protected:
    // Convert percentage to actual duty cycle considering deadzone
    float mapSpeedToDutyCycle(float percentage) {
        if (percentage <= 0.0f) return 0.0f;
        if (percentage >= 100.0f) return maxDutyCycle;
        
        return minDutyCycle + (maxDutyCycle - minDutyCycle) * (percentage / 100.0f);
    }
};

#endif 
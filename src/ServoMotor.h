#ifndef SERVO_MOTOR_H
#define SERVO_MOTOR_H

#include "Motor.h"
#include <Arduino.h>

class ServoMotor : public Motor {
private:
    unsigned long minPulseWidth;    // Minimum pulse width in microseconds
    unsigned long maxPulseWidth;    // Maximum pulse width in microseconds
    unsigned long period;           // Period in microseconds (1/frequency)
    float minAngle;                // Minimum angle in degrees
    float maxAngle;                // Maximum angle in degrees

public:
    ServoMotor(int pwmPin,
               PWMBackend* backend,
               float frequency = 50,           // Default 50Hz
               float minAngle = 0,            // Default 0 degrees
               float maxAngle = 180,          // Default 180 degrees
               unsigned long minPulse = 544,   // Default 544 microseconds
               unsigned long maxPulse = 2400)  // Default 2400 microseconds
        : Motor(pwmPin, backend, minAngle, maxAngle),
          minPulseWidth(minPulse),
          maxPulseWidth(maxPulse),
          period(1000000/frequency),  // Convert frequency to period in microseconds
          minAngle(minAngle),
          maxAngle(maxAngle) {
        
        pwmBackend->begin();
        pwmBackend->setPWMFreq(frequency);
        pinMode(pin, OUTPUT);
        stop();
    }

    // Set angle directly (in degrees)
    void setAngle(float angle) {
        float percentage = mapAngleToPercentage(angle);
        setSpeed(percentage);
    }

    void setSpeed(float percentage) override {
        currentSpeed = constrain(percentage, 0.0f, 100.0f);
        
        // Map percentage (0-100) to pulse width
        unsigned long pulseWidth = map(currentSpeed, 0, 100, 
                                     minPulseWidth, maxPulseWidth);
        
        // Check if we're using OnboardPWM backend
        if (pwmBackend->getBackendType() == PWMBackendType::ONBOARD_PWM) {
            // Use direct pin control for precise timing with OnboardPWM
            generateServoPulse(pulseWidth);
        } else {
            // Use PWM duty cycle for other backends (like PCA9685)
            float dutyCycleFloat = (float(pulseWidth) / float(period)) * pwmBackend->getMaxDutyCycle();
            int dutyCycle = round(dutyCycleFloat);
            pwmBackend->setPWM(pin, dutyCycle);
        }
    }

    void stop() override {
        currentSpeed = 0.0f;
        
        // Check if we're using OnboardPWM backend
        if (pwmBackend->getBackendType() == PWMBackendType::ONBOARD_PWM) {
            // Use direct pin control for OnboardPWM
            digitalWrite(pin, LOW);
        } else {
            // Use PWM backend stop method for other backends
            pwmBackend->stop(pin);
        }
    }

    // Getters for servo parameters
    float getMinAngle() const { return minAngle; }
    float getMaxAngle() const { return maxAngle; }
    unsigned long getMinPulseWidth() const { return minPulseWidth; }
    unsigned long getMaxPulseWidth() const { return maxPulseWidth; }
    float getFrequency() const { return 1000000.0f/period; }

private:
    void generateServoPulse(unsigned long pulseWidth) {
        // Generate a few servo pulses for smooth movement (OnboardPWM only)
        for (int i = 0; i < 10; i++) {
            digitalWrite(pin, HIGH);
            delayMicroseconds(pulseWidth);
            digitalWrite(pin, LOW);
            delayMicroseconds(period - pulseWidth);
        }
    }

    float mapAngleToPercentage(float angle) {
        angle = constrain(angle, minAngle, maxAngle);
        return ((angle - minAngle) / (maxAngle - minAngle)) * 100.0f;
    }
};

#endif 
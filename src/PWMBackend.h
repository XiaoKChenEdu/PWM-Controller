#ifndef PWM_BACKEND_H
#define PWM_BACKEND_H

#include <Arduino.h>

// Enum for PWM backend types
enum class PWMBackendType {
    ONBOARD_PWM,
    PCA9685_PWM,
    OTHER
};

// Abstract base class for PWM backends
class PWMBackend {
public:
    virtual ~PWMBackend() = default;
    
    // Initialize the PWM backend
    virtual void begin() = 0;
    
    // Set PWM frequency for a pin/channel
    virtual void setPWMFreq(float frequency) = 0;
    
    // Set PWM duty cycle (0-4095 for 12-bit, 0-255 for 8-bit)
    virtual void setPWM(int pin, int dutyCycle) = 0;
    
    // Stop PWM output on a pin/channel
    virtual void stop(int pin) = 0;
    
    // Get the resolution of this PWM backend
    virtual int getResolution() = 0;
    
    // Get the maximum duty cycle value
    virtual int getMaxDutyCycle() = 0;
    
    // Get the backend type (instead of using dynamic_cast)
    virtual PWMBackendType getBackendType() const = 0;
};

#endif 
#ifndef RASPBERRY_PI_PWM_H
#define RASPBERRY_PI_PWM_H

#include "PWMBackend.h"
#include <pigpio.h>
#include <map>
#include <stdexcept>

class RaspberryPiPWM : public PWMBackend {
private:
    float currentFrequency;
    int resolution;
    std::map<int, bool> activePins;  // Track active PWM pins
    bool pigpioInitialized;

public:
    RaspberryPiPWM(int pwmResolution = 255) 
        : currentFrequency(50), resolution(pwmResolution), pigpioInitialized(false) {}

    ~RaspberryPiPWM() {
        // Stop all active PWM pins before cleanup
        for (auto& pin : activePins) {
            if (pin.second) {
                gpioSetMode(pin.first, PI_INPUT);
            }
        }
        
        if (pigpioInitialized) {
            gpioTerminate();
        }
    }

    void begin() override {
        if (gpioInitialise() < 0) {
            throw std::runtime_error("Failed to initialize pigpio. Make sure to run with sudo privileges.");
        }
        pigpioInitialized = true;
        
        // Set default PWM frequency
        setPWMFreq(currentFrequency);
    }

    void setPWMFreq(float frequency) override {
        currentFrequency = frequency;
        
        // Set PWM frequency for all active pins
        for (auto& pin : activePins) {
            if (pin.second) {
                gpioSetPWMfrequency(pin.first, (unsigned)frequency);
            }
        }
    }

    void setPWM(int pin, int dutyCycle) override {
        // Validate pin number (GPIO pins 0-31 are valid on most RPi models)
        if (pin < 0 || pin > 31) {
            throw std::invalid_argument("Invalid GPIO pin number. Valid range: 0-31");
        }
        
        // Constrain duty cycle
        dutyCycle = std::max(0, std::min(dutyCycle, resolution));
        
        // Initialize pin if not already active
        if (activePins.find(pin) == activePins.end() || !activePins[pin]) {
            gpioSetMode(pin, PI_OUTPUT);
            gpioSetPWMfrequency(pin, (unsigned)currentFrequency);
            activePins[pin] = true;
        }
        
        // Convert duty cycle to pigpio range (0-255)
        int pigpioDutyCycle = (dutyCycle * 255) / resolution;
        gpioPWM(pin, pigpioDutyCycle);
    }

    void stop(int pin) override {
        if (activePins.find(pin) != activePins.end() && activePins[pin]) {
            gpioPWM(pin, 0);
            gpioSetMode(pin, PI_INPUT);
            activePins[pin] = false;
        }
    }

    int getResolution() override {
        return resolution;
    }

    int getMaxDutyCycle() override {
        return resolution;
    }

    PWMBackendType getBackendType() const override {
        return PWMBackendType::RASPBERRY_PI_PWM;
    }
    
    // Raspberry Pi specific methods
    
    // Set PWM range (alternative to resolution)
    void setPWMRange(int pin, int range) {
        if (activePins.find(pin) != activePins.end() && activePins[pin]) {
            gpioSetPWMrange(pin, range);
        }
    }
    
    // Get actual PWM frequency (may differ from requested)
    int getActualFrequency(int pin) {
        return gpioGetPWMfrequency(pin);
    }
    
    // Check if pigpio is properly initialized
    bool isInitialized() const {
        return pigpioInitialized;
    }
};

#endif 
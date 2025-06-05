#ifndef PCA9685_LIBRARY_PWM_H
#define PCA9685_LIBRARY_PWM_H

#include "PWMBackend.h"
#include <Arduino.h>
#include <PCA9685.h>

class PCA9685LibraryPWM : public PWMBackend {
private:
    PCA9685 pca9685;
    static constexpr int RESOLUTION_BITS = 12;
    static constexpr int MAX_DUTY_CYCLE = 4095;

public:
    PCA9685LibraryPWM(uint8_t address = 0x40) 
        : pca9685(address) {}

    void begin() override {
        // Don't call Wire.begin() here - it's handled in main setup
        // Just initialize the PCA9685 device
        pca9685.resetDevices();       // Reset all PCA9685 devices on i2c line
        pca9685.init();               // Initialize with default settings
        pca9685.setPWMFreqServo();    // Set 50Hz for servo control
    }

    void setPWMFreq(float frequency) override {
        if (frequency == 50.0f) {
            pca9685.setPWMFreqServo();  // Use optimized servo frequency
        } else {
            pca9685.setPWMFrequency(frequency);
        }
    }

    void setPWM(int channel, int dutyCycle) override {
        if (channel < 0 || channel > 15) return; // PCA9685 has 16 channels (0-15)
        
        dutyCycle = constrain(dutyCycle, 0, MAX_DUTY_CYCLE);
        pca9685.setChannelPWM(channel, dutyCycle);
    }

    void stop(int channel) override {
        if (channel < 0 || channel > 15) return;
        pca9685.setChannelPWM(channel, 0);
    }

    int getResolution() override {
        return RESOLUTION_BITS;
    }

    int getMaxDutyCycle() override {
        return MAX_DUTY_CYCLE;
    }

    PWMBackendType getBackendType() const override {
        return PWMBackendType::PCA9685_PWM;
    }

    // Additional methods specific to this library
    void printModuleInfo() {
        #ifdef PCA9685_ENABLE_DEBUG_OUTPUT
        pca9685.printModuleInfo();
        #endif
    }
};

#endif 
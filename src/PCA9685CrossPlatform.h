#ifndef PCA9685_CROSS_PLATFORM_H
#define PCA9685_CROSS_PLATFORM_H

#include "PWMBackend.h"
#include <pigpio.h>
#include <cmath>
#include <stdexcept>
#include <unistd.h>

// Arduino-like functions for Raspberry Pi
inline int constrain(int x, int a, int b) {
    return std::max(a, std::min(x, b));
}

// PCA9685 Register definitions
#define PCA9685_MODE1 0x00
#define PCA9685_MODE2 0x01
#define PCA9685_PRESCALE 0xFE
#define PCA9685_LED0_ON_L 0x06
#define PCA9685_LED0_ON_H 0x07
#define PCA9685_LED0_OFF_L 0x08
#define PCA9685_LED0_OFF_H 0x09
#define PCA9685_ALLLED_ON_L 0xFA
#define PCA9685_ALLLED_ON_H 0xFB
#define PCA9685_ALLLED_OFF_L 0xFC
#define PCA9685_ALLLED_OFF_H 0xFD

// Mode1 register bits
#define PCA9685_RESTART 0x80
#define PCA9685_SLEEP 0x10
#define PCA9685_ALLCALL 0x01
#define PCA9685_EXTCLK 0x40
#define PCA9685_AI 0x20

class PCA9685CrossPlatform : public PWMBackend {
private:
    static constexpr int RESOLUTION_BITS = 12;
    static constexpr int MAX_DUTY_CYCLE = 4095;
    static constexpr float OSCILLATOR_FREQ = 25000000.0f;  // 25MHz internal oscillator
    
    uint8_t deviceAddress;
    float currentFrequency;
    
    #ifndef ARDUINO
    int i2cHandle;
    bool pigpioInitialized;
    #endif

public:
    PCA9685CrossPlatform(uint8_t address = 0x40) 
        : deviceAddress(address), currentFrequency(50.0f) {
        #ifndef ARDUINO
        i2cHandle = -1;
        pigpioInitialized = false;
        #endif
    }

    ~PCA9685CrossPlatform() {
        #ifndef ARDUINO
        if (i2cHandle >= 0) {
            i2cClose(i2cHandle);
        }
        #endif
    }

    void begin() override {
        // Raspberry Pi I2C initialization
        if (gpioInitialise() < 0) {
            throw std::runtime_error("Failed to initialize pigpio for PCA9685");
        }
        pigpioInitialized = true;
        
        i2cHandle = i2cOpen(1, deviceAddress, 0);  // I2C bus 1, device address
        if (i2cHandle < 0) {
            throw std::runtime_error("Failed to open I2C connection to PCA9685");
        }
        
        reset();
        init();

    }

    void setPWMFreq(float frequency) override {
        currentFrequency = frequency;
        
        // Calculate prescale value
        // prescale = round(osc_clock / (4096 * freq)) - 1
        float prescaleFloat = (OSCILLATOR_FREQ / (4096.0f * frequency)) - 1.0f;
        uint8_t prescale = (uint8_t)(prescaleFloat + 0.5f);  // Round to nearest
        
        // Read current mode
        uint8_t oldMode = readRegister(PCA9685_MODE1);
        
        // Go to sleep to change prescaler
        uint8_t newMode = (oldMode & 0x7F) | PCA9685_SLEEP;
        writeRegister(PCA9685_MODE1, newMode);
        
        // Write prescaler
        writeRegister(PCA9685_PRESCALE, prescale);
        
        // Restore original mode
        writeRegister(PCA9685_MODE1, oldMode);
        
        // Wait for oscillator to stabilize
        usleep(5000);  // 5ms delay
        
        // Restart PWM
        writeRegister(PCA9685_MODE1, oldMode | PCA9685_RESTART);
    }

    void setPWM(int channel, int dutyCycle) override {
        if (channel < 0 || channel > 15) return; // PCA9685 has 16 channels (0-15)
        
        dutyCycle = constrain(dutyCycle, 0, MAX_DUTY_CYCLE);
        
        if (dutyCycle == 0) {
            // Full OFF
            setPWMRaw(channel, 0, 4096);
        } else if (dutyCycle == MAX_DUTY_CYCLE) {
            // Full ON
            setPWMRaw(channel, 4096, 0);
        } else {
            // Normal PWM
            setPWMRaw(channel, 0, dutyCycle);
        }
    }

    void stop(int channel) override {
        if (channel < 0 || channel > 15) return;
        setPWMRaw(channel, 0, 4096); // Full OFF
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

    // Additional utility methods
    void setAllPWM(int dutyCycle) {
        dutyCycle = constrain(dutyCycle, 0, MAX_DUTY_CYCLE);
        
        if (dutyCycle == 0) {
            writeRegister(PCA9685_ALLLED_ON_L, 0);
            writeRegister(PCA9685_ALLLED_ON_H, 0);
            writeRegister(PCA9685_ALLLED_OFF_L, 0);
            writeRegister(PCA9685_ALLLED_OFF_H, 0x10); // bit 4 = full OFF
        } else if (dutyCycle == MAX_DUTY_CYCLE) {
            writeRegister(PCA9685_ALLLED_ON_L, 0);
            writeRegister(PCA9685_ALLLED_ON_H, 0x10); // bit 4 = full ON
            writeRegister(PCA9685_ALLLED_OFF_L, 0);
            writeRegister(PCA9685_ALLLED_OFF_H, 0);
        } else {
            writeRegister(PCA9685_ALLLED_ON_L, 0);
            writeRegister(PCA9685_ALLLED_ON_H, 0);
            writeRegister(PCA9685_ALLLED_OFF_L, dutyCycle & 0xFF);
            writeRegister(PCA9685_ALLLED_OFF_H, dutyCycle >> 8);
        }
    }

    float getCurrentFrequency() const {
        return currentFrequency;
    }

private:
    void reset() {
        // Software reset
        writeRegister(PCA9685_MODE1, PCA9685_RESTART);
        usleep(10000);  // 10ms delay
    }

    void init() {
        // Set default mode: auto increment, normal mode, respond to all call
        writeRegister(PCA9685_MODE1, PCA9685_AI | PCA9685_ALLCALL);
        writeRegister(PCA9685_MODE2, 0x04); // Output logic high when OE high
        
        usleep(5000);  // 5ms delay
        
        // Set default frequency (50Hz for servos)
        setPWMFreq(50.0f);
        
        // Turn off all channels
        setAllPWM(0);
    }

    void setPWMRaw(int channel, uint16_t onTime, uint16_t offTime) {
        uint8_t baseReg = PCA9685_LED0_ON_L + 4 * channel;
        
        writeRegister(baseReg, onTime & 0xFF);
        writeRegister(baseReg + 1, onTime >> 8);
        writeRegister(baseReg + 2, offTime & 0xFF);
        writeRegister(baseReg + 3, offTime >> 8);
    }

    void writeRegister(uint8_t reg, uint8_t value) {
        #ifdef ARDUINO
            Wire.beginTransmission(deviceAddress);
            Wire.write(reg);
            Wire.write(value);
            Wire.endTransmission();
        #else
            if (i2cHandle >= 0) {
                i2cWriteByteData(i2cHandle, reg, value);
            }
        #endif
    }

    uint8_t readRegister(uint8_t reg) {
        #ifdef ARDUINO
            Wire.beginTransmission(deviceAddress);
            Wire.write(reg);
            Wire.endTransmission();
            Wire.requestFrom(deviceAddress, (uint8_t)1);
            return Wire.read();
        #else
            if (i2cHandle >= 0) {
                int result = i2cReadByteData(i2cHandle, reg);
                return (result >= 0) ? (uint8_t)result : 0;
            }
            return 0;
        #endif
    }
};

#endif 
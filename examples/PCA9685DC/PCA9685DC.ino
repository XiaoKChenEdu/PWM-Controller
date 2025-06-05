#include "./src/DCMotor.h"
#include "./src/PCA9685LibraryPWM.h"
#include <Wire.h>

// Channel definition
const int MOTOR_CHANNEL = 0;  // PCA9685 channel (0-15)

// Create PCA9685 PWM backend
PCA9685LibraryPWM pca9685PWM(0x40);  // I2C address 0x40

// Create DC motor instance
DCMotor motor(MOTOR_CHANNEL, &pca9685PWM, 
              15.0f,  // min speed (15% - lower deadzone with PCA9685)
              100.0f, // max speed (100%)
              false); // direction not inverted

void setup() {
    Serial.begin(9600);
    delay(1000);
    
    Serial.println("PCA9685 DC Motor Controller");
    
    // Initialize I2C
    Wire.begin();
    
    // Test PCA9685 connection
    Wire.beginTransmission(0x40);
    if (Wire.endTransmission() == 0) {
        Serial.println("PCA9685 found at 0x40");
        Serial.println("Commands: 0-100 (speed %), stop, reverse, help");
    } else {
        Serial.println("ERROR: PCA9685 not found!");
        Serial.println("Check wiring: SDA->A4, SCL->A5, VCC->5V, GND->GND");
    }
}

void loop() {
    if (Serial.available()) {
        String command = Serial.readStringUntil('\n');
        command.trim();
        
        if (command == "stop") {
            motor.stop();
            Serial.println("Motor stopped");
        }
        else if (command == "reverse") {
            motor.setDirection(!motor.getDirection());
            Serial.print("Direction: ");
            Serial.println(motor.getDirection() ? "Reversed" : "Forward");
        }
        else if (command == "help") {
            Serial.println("Enter speed (0-100), 'stop', 'reverse'");
        }
        else {
            float speed = command.toFloat();
            if (speed >= 0 && speed <= 100) {
                motor.setSpeed(speed);
                Serial.print("Motor speed: ");
                Serial.print(speed);
                Serial.println("%");
            } else {
                Serial.println("Invalid speed. Use 0-100");
            }
        }
    }
} 
#include "./src/ServoMotor.h"
#include "./src/PCA9685LibraryPWM.h"
#include <Wire.h>

// PCA9685 PWM backend and servo motor pointers
PCA9685LibraryPWM* pwmBackend = nullptr;
ServoMotor* servo = nullptr;

void setup() {
    Serial.begin(9600);
    Serial.println("Simple PCA9685 Servo Controller");
    
    // Initialize I2C and create instances
    Wire.begin();
    pwmBackend = new PCA9685LibraryPWM(0x40);
    servo = new ServoMotor(0, pwmBackend, 50, 0, 270, 544, 2400);
    
    Serial.println("Commands: angle (0-270), stop, help");
}

void loop() {
    if (Serial.available()) {
        String command = Serial.readStringUntil('\n');
        command.trim();
        
        if (command == "stop") {
            servo->stop();
            Serial.println("Servo stopped");
        }
        else if (command == "help") {
            Serial.println("Commands: angle (0-270), stop");
        }
        else {
            float angle = command.toFloat();
            if (angle >= 0 && angle <= 270) {
                servo->setAngle(angle);
                Serial.print("Angle: ");
                Serial.println(angle);
            } else {
                Serial.println("Use 0-270");
            }
        }
    }
} 
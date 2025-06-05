#include "./src/DCMotor.h"
#include "./src/OnboardPWM.h"

// Pin definition
const int MOTOR_PIN = 6;  // PWM pin for DC motor

// Create onboard PWM backend
OnboardPWM onboardPWM;

// Create DC motor instance
DCMotor motor(MOTOR_PIN, &onboardPWM, 
              25.0f,  // min speed (25% - motor deadzone)
              100.0f, // max speed (100%)
              false); // direction not inverted

void setup() {
    Serial.begin(9600);
    Serial.println("Onboard DC Motor Controller");
    Serial.println("Commands: 0-100 (speed %), stop, reverse, help");
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
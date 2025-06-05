#include "./src/ServoMotor.h"
#include "./src/OnboardPWM.h"

// Pin definition
const int SERVO_PIN = 9;  // PWM pin for servo (use pin 9 or 10 for best results)

// Create onboard PWM backend
OnboardPWM onboardPWM;

// Create servo motor instance
ServoMotor servo(SERVO_PIN, &onboardPWM, 
                50,    // 50Hz frequency
                0,     // min angle
                270,   // max angle
                544,   // min pulse width (μs)
                2400); // max pulse width (μs)

void setup() {
    Serial.begin(9600);
    Serial.println("Onboard Servo Controller");
    Serial.println("Commands: 0-180 (angle), stop, help");
}

void loop() {
    if (Serial.available()) {
        String command = Serial.readStringUntil('\n');
        command.trim();
        
        if (command == "stop") {
            servo.stop();
            Serial.println("Servo stopped");
        }
        else if (command == "help") {
            Serial.println("Enter angle (0-270) or 'stop'");
        }
        else {
            float angle = command.toFloat();
            if (angle >= 0 && angle <= 270) {
                servo.setAngle(angle);
                Serial.print("Servo set to: ");
                Serial.print(angle);
                Serial.println(" degrees");
            } else {
                Serial.println("Invalid angle. Use 0-270");
            }
        }
    }
} 
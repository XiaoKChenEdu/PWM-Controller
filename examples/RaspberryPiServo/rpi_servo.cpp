#include "../../src/ServoMotor.h"
#include "../../src/RaspberryPiPWM.h"
#include <iostream>
#include <string>
#include <thread>
#include <chrono>

// GPIO pin definition for servo (use GPIO pin numbers, not physical pin numbers)
const int SERVO_PIN = 18;  // GPIO18 (Physical pin 12) - Hardware PWM pin

// Create Raspberry Pi PWM backend
RaspberryPiPWM rpiPWM;

// Create servo motor instance
ServoMotor servo(SERVO_PIN, &rpiPWM, 
                50,    // 50Hz frequency (standard for servos)
                0,     // min angle
                270,   // max angle
                544,   // min pulse width (μs)
                2400); // max pulse width (μs)

void printHelp() {
    std::cout << "\nRaspberry Pi Servo Controller" << std::endl;
    std::cout << "Commands:" << std::endl;
    std::cout << "  0-270    : Set servo angle" << std::endl;
    std::cout << "  sweep    : Perform sweep demo" << std::endl;
    std::cout << "  stop     : Stop servo" << std::endl;
    std::cout << "  help     : Show this help" << std::endl;
    std::cout << "  quit     : Exit program" << std::endl;
    std::cout << "\nNOTE: Make sure to run with sudo privileges for GPIO access!" << std::endl;
    std::cout << "Usage: sudo ./rpi_servo" << std::endl;
}

void sweepDemo() {
    std::cout << "Performing sweep demo..." << std::endl;
    
    // Sweep from 0 to 270 degrees
    for (int angle = 0; angle <= 270; angle += 10) {
        servo.setAngle(angle);
        std::cout << "Angle: " << angle << " degrees" << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
    
    // Sweep back from 270 to 0 degrees
    for (int angle = 270; angle >= 0; angle -= 10) {
        servo.setAngle(angle);
        std::cout << "Angle: " << angle << " degrees" << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
    
    std::cout << "Sweep demo completed!" << std::endl;
}

int main() {
    try {
        std::cout << "Initializing Raspberry Pi Servo Controller..." << std::endl;
        
        // Initialize the servo (this will call rpiPWM.begin())
        // Note: This is automatically called in the ServoMotor constructor
        
        std::cout << "Servo initialized successfully!" << std::endl;
        std::cout << "GPIO Pin: " << SERVO_PIN << std::endl;
        std::cout << "Frequency: " << servo.getFrequency() << " Hz" << std::endl;
        std::cout << "Angle Range: " << servo.getMinAngle() << " - " << servo.getMaxAngle() << " degrees" << std::endl;
        
        printHelp();
        
        std::string command;
        while (true) {
            std::cout << "\nEnter command: ";
            std::getline(std::cin, command);
            
            if (command.empty()) {
                continue;
            }
            
            if (command == "quit" || command == "exit") {
                break;
            }
            else if (command == "stop") {
                servo.stop();
                std::cout << "Servo stopped" << std::endl;
            }
            else if (command == "help") {
                printHelp();
            }
            else if (command == "sweep") {
                sweepDemo();
            }
            else {
                try {
                    float angle = std::stof(command);
                    if (angle >= 0 && angle <= 270) {
                        servo.setAngle(angle);
                        std::cout << "Servo set to: " << angle << " degrees" << std::endl;
                    } else {
                        std::cout << "Invalid angle. Use 0-270" << std::endl;
                    }
                }
                catch (const std::exception& e) {
                    std::cout << "Invalid command. Type 'help' for available commands." << std::endl;
                }
            }
        }
        
        std::cout << "Stopping servo and cleaning up..." << std::endl;
        servo.stop();
        
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        std::cerr << "Make sure to run with sudo privileges: sudo ./rpi_servo" << std::endl;
        return 1;
    }
    
    std::cout << "Goodbye!" << std::endl;
    return 0;
} 
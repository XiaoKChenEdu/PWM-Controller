#include "../../src/DCMotor.h"
#include "../../src/RaspberryPiPWM.h"
#include <iostream>
#include <string>
#include <thread>
#include <chrono>

// GPIO pin definition for DC motor (use GPIO pin numbers, not physical pin numbers)
const int MOTOR_PIN = 18;  // GPIO18 (Physical pin 12) - Hardware PWM pin

// Create Raspberry Pi PWM backend
RaspberryPiPWM rpiPWM;

// Create DC motor instance
DCMotor motor(MOTOR_PIN, &rpiPWM, 
              20.0f,     // Minimum speed percentage (deadzone)
              100.0f,    // Maximum speed percentage
              false);    // Don't invert direction

void printHelp() {
    std::cout << "\nRaspberry Pi DC Motor Controller" << std::endl;
    std::cout << "Commands:" << std::endl;
    std::cout << "  0-100    : Set motor speed (0-100%)" << std::endl;
    std::cout << "  ramp     : Perform ramp demo" << std::endl;
    std::cout << "  invert   : Toggle direction inversion" << std::endl;
    std::cout << "  stop     : Stop motor" << std::endl;
    std::cout << "  help     : Show this help" << std::endl;
    std::cout << "  quit     : Exit program" << std::endl;
    std::cout << "\nNOTE: Make sure to run with sudo privileges for GPIO access!" << std::endl;
    std::cout << "Usage: sudo ./rpi_dc" << std::endl;
}

void rampDemo() {
    std::cout << "Performing ramp demo..." << std::endl;
    
    // Ramp up from 0 to 100%
    std::cout << "Ramping up..." << std::endl;
    for (int speed = 0; speed <= 100; speed += 10) {
        motor.setSpeed(speed);
        std::cout << "Speed: " << speed << "%" << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
    }
    
    std::this_thread::sleep_for(std::chrono::seconds(1));
    
    // Ramp down from 100 to 0%
    std::cout << "Ramping down..." << std::endl;
    for (int speed = 100; speed >= 0; speed -= 10) {
        motor.setSpeed(speed);
        std::cout << "Speed: " << speed << "%" << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
    }
    
    std::cout << "Ramp demo completed!" << std::endl;
}

int main() {
    try {
        std::cout << "Initializing Raspberry Pi DC Motor Controller..." << std::endl;
        
        // Initialize the motor (this will call rpiPWM.begin())
        // Note: This is automatically called in the DCMotor constructor
        
        std::cout << "DC Motor initialized successfully!" << std::endl;
        std::cout << "GPIO Pin: " << MOTOR_PIN << std::endl;
        std::cout << "Speed Range: 0-100%" << std::endl;
        std::cout << "Direction Inverted: " << (motor.getDirection() ? "Yes" : "No") << std::endl;
        
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
                motor.stop();
                std::cout << "Motor stopped" << std::endl;
            }
            else if (command == "help") {
                printHelp();
            }
            else if (command == "ramp") {
                rampDemo();
            }
            else if (command == "invert") {
                motor.setDirection(!motor.getDirection());
                std::cout << "Direction inverted: " << (motor.getDirection() ? "Yes" : "No") << std::endl;
            }
            else {
                try {
                    float speed = std::stof(command);
                    if (speed >= 0 && speed <= 100) {
                        motor.setSpeed(speed);
                        std::cout << "Motor speed set to: " << speed << "%" << std::endl;
                        std::cout << "Current speed: " << motor.getSpeed() << "%" << std::endl;
                    } else {
                        std::cout << "Invalid speed. Use 0-100" << std::endl;
                    }
                }
                catch (const std::exception& e) {
                    std::cout << "Invalid command. Type 'help' for available commands." << std::endl;
                }
            }
        }
        
        std::cout << "Stopping motor and cleaning up..." << std::endl;
        motor.stop();
        
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        std::cerr << "Make sure to run with sudo privileges: sudo ./rpi_dc" << std::endl;
        return 1;
    }
    
    std::cout << "Goodbye!" << std::endl;
    return 0;
} 
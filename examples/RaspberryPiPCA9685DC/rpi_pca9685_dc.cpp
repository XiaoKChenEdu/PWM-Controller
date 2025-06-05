#include "../../src/DCMotor.h"
#include "../../src/PCA9685CrossPlatform.h"
#include <iostream>
#include <string>
#include <thread>
#include <chrono>

// PCA9685 I2C address (default 0x40)
const uint8_t PCA9685_ADDRESS = 0x40;

// PCA9685 channels for DC motors (0-15 available)
const int MOTOR1_CHANNEL = 0;
const int MOTOR2_CHANNEL = 1;
const int MOTOR3_CHANNEL = 2;
const int MOTOR4_CHANNEL = 3;

// Create PCA9685 PWM backend
PCA9685CrossPlatform pca9685(PCA9685_ADDRESS);

// Create DC motor instances
DCMotor motor1(MOTOR1_CHANNEL, &pca9685, 20.0f, 100.0f, false);
DCMotor motor2(MOTOR2_CHANNEL, &pca9685, 20.0f, 100.0f, false);
DCMotor motor3(MOTOR3_CHANNEL, &pca9685, 20.0f, 100.0f, false);
DCMotor motor4(MOTOR4_CHANNEL, &pca9685, 20.0f, 100.0f, false);

void printHelp() {
    std::cout << "\nRaspberry Pi PCA9685 DC Motor Controller" << std::endl;
    std::cout << "Commands:" << std::endl;
    std::cout << "  1 <speed>  : Set motor 1 speed (0-100%)" << std::endl;
    std::cout << "  2 <speed>  : Set motor 2 speed (0-100%)" << std::endl;
    std::cout << "  3 <speed>  : Set motor 3 speed (0-100%)" << std::endl;
    std::cout << "  4 <speed>  : Set motor 4 speed (0-100%)" << std::endl;
    std::cout << "  all <speed>: Set all motors to speed" << std::endl;
    std::cout << "  ramp       : Perform ramp demo on all motors" << std::endl;
    std::cout << "  wave       : Perform wave speed demo" << std::endl;
    std::cout << "  invert <n> : Toggle direction inversion for motor n (1-4)" << std::endl;
    std::cout << "  stop       : Stop all motors" << std::endl;
    std::cout << "  status     : Show motor status" << std::endl;
    std::cout << "  help       : Show this help" << std::endl;
    std::cout << "  quit       : Exit program" << std::endl;
    std::cout << "\nI2C Address: 0x" << std::hex << (int)PCA9685_ADDRESS << std::dec << std::endl;
    std::cout << "Channels: " << MOTOR1_CHANNEL << ", " << MOTOR2_CHANNEL << ", " << MOTOR3_CHANNEL << ", " << MOTOR4_CHANNEL << std::endl;
    std::cout << "\nNOTE: Make sure to run with sudo privileges for I2C access!" << std::endl;
    std::cout << "Connect motors through appropriate motor drivers!" << std::endl;
}

void showStatus() {
    std::cout << "\nMotor Status:" << std::endl;
    std::cout << "  Motor 1: " << motor1.getSpeed() << "% (Dir: " << (motor1.getDirection() ? "Inverted" : "Normal") << ")" << std::endl;
    std::cout << "  Motor 2: " << motor2.getSpeed() << "% (Dir: " << (motor2.getDirection() ? "Inverted" : "Normal") << ")" << std::endl;
    std::cout << "  Motor 3: " << motor3.getSpeed() << "% (Dir: " << (motor3.getDirection() ? "Inverted" : "Normal") << ")" << std::endl;
    std::cout << "  Motor 4: " << motor4.getSpeed() << "% (Dir: " << (motor4.getDirection() ? "Inverted" : "Normal") << ")" << std::endl;
    std::cout << "PCA9685 Frequency: " << pca9685.getCurrentFrequency() << " Hz" << std::endl;
}

void rampDemo() {
    std::cout << "Performing ramp demo on all motors..." << std::endl;
    
    // Ramp up from 0 to 100%
    std::cout << "Ramping up..." << std::endl;
    for (int speed = 0; speed <= 100; speed += 10) {
        motor1.setSpeed(speed);
        motor2.setSpeed(speed);
        motor3.setSpeed(speed);
        motor4.setSpeed(speed);
        std::cout << "All motors: " << speed << "%" << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(400));
    }
    
    std::this_thread::sleep_for(std::chrono::seconds(1));
    
    // Ramp down from 100 to 0%
    std::cout << "Ramping down..." << std::endl;
    for (int speed = 100; speed >= 0; speed -= 10) {
        motor1.setSpeed(speed);
        motor2.setSpeed(speed);
        motor3.setSpeed(speed);
        motor4.setSpeed(speed);
        std::cout << "All motors: " << speed << "%" << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(400));
    }
    
    std::cout << "Ramp demo completed!" << std::endl;
}

void waveDemo() {
    std::cout << "Performing wave speed demo..." << std::endl;
    
    for (int cycle = 0; cycle < 2; cycle++) {
        // Wave pattern: motors turn on/off in sequence
        for (int speed = 0; speed <= 80; speed += 20) {
            motor1.setSpeed(speed);
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
            
            motor2.setSpeed(speed);
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
            
            motor3.setSpeed(speed);
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
            
            motor4.setSpeed(speed);
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
        }
        
        // Reverse wave
        for (int speed = 80; speed >= 0; speed -= 20) {
            motor4.setSpeed(speed);
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
            
            motor3.setSpeed(speed);
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
            
            motor2.setSpeed(speed);
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
            
            motor1.setSpeed(speed);
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
        }
    }
    
    std::cout << "Wave demo completed!" << std::endl;
}

int main() {
    try {
        std::cout << "Initializing Raspberry Pi PCA9685 DC Motor Controller..." << std::endl;
        
        // Initialize the PCA9685 (this will call pca9685.begin())
        // Note: This is automatically called in the DCMotor constructors
        
        std::cout << "PCA9685 initialized successfully!" << std::endl;
        std::cout << "I2C Address: 0x" << std::hex << (int)PCA9685_ADDRESS << std::dec << std::endl;
        std::cout << "Frequency: " << pca9685.getCurrentFrequency() << " Hz" << std::endl;
        std::cout << "Resolution: " << pca9685.getResolution() << " bits (" << pca9685.getMaxDutyCycle() << " max)" << std::endl;
        
        printHelp();
        showStatus();
        
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
                motor1.stop();
                motor2.stop();
                motor3.stop();
                motor4.stop();
                std::cout << "All motors stopped" << std::endl;
            }
            else if (command == "help") {
                printHelp();
            }
            else if (command == "status") {
                showStatus();
            }
            else if (command == "ramp") {
                rampDemo();
            }
            else if (command == "wave") {
                waveDemo();
            }
            else if (command.substr(0, 4) == "all ") {
                try {
                    float speed = std::stof(command.substr(4));
                    if (speed >= 0 && speed <= 100) {
                        motor1.setSpeed(speed);
                        motor2.setSpeed(speed);
                        motor3.setSpeed(speed);
                        motor4.setSpeed(speed);
                        std::cout << "All motors set to: " << speed << "%" << std::endl;
                    } else {
                        std::cout << "Invalid speed. Use 0-100" << std::endl;
                    }
                }
                catch (const std::exception& e) {
                    std::cout << "Invalid speed format" << std::endl;
                }
            }
            else if (command.substr(0, 7) == "invert ") {
                try {
                    int motorNum = std::stoi(command.substr(7));
                    if (motorNum >= 1 && motorNum <= 4) {
                        DCMotor* motors[] = {&motor1, &motor2, &motor3, &motor4};
                        DCMotor* motor = motors[motorNum - 1];
                        motor->setDirection(!motor->getDirection());
                        std::cout << "Motor " << motorNum << " direction inverted: " << (motor->getDirection() ? "Yes" : "No") << std::endl;
                    } else {
                        std::cout << "Invalid motor number. Use 1-4" << std::endl;
                    }
                }
                catch (const std::exception& e) {
                    std::cout << "Invalid motor number format" << std::endl;
                }
            }
            else if (command.length() >= 3 && (command[0] >= '1' && command[0] <= '4') && command[1] == ' ') {
                try {
                    int motorNum = command[0] - '0';
                    float speed = std::stof(command.substr(2));
                    
                    if (speed >= 0 && speed <= 100) {
                        DCMotor* motors[] = {&motor1, &motor2, &motor3, &motor4};
                        motors[motorNum - 1]->setSpeed(speed);
                        std::cout << "Motor " << motorNum << " set to: " << speed << "%" << std::endl;
                    } else {
                        std::cout << "Invalid speed. Use 0-100" << std::endl;
                    }
                }
                catch (const std::exception& e) {
                    std::cout << "Invalid command format" << std::endl;
                }
            }
            else {
                std::cout << "Invalid command. Type 'help' for available commands." << std::endl;
            }
        }
        
        std::cout << "Stopping all motors and cleaning up..." << std::endl;
        motor1.stop();
        motor2.stop();
        motor3.stop();
        motor4.stop();
        
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        std::cerr << "Make sure to:" << std::endl;
        std::cerr << "  - Run with sudo privileges: sudo ./rpi_pca9685_dc" << std::endl;
        std::cerr << "  - Check I2C is enabled: sudo raspi-config -> Interface Options -> I2C" << std::endl;
        std::cerr << "  - Check PCA9685 is connected to I2C bus 1 (GPIO2=SDA, GPIO3=SCL)" << std::endl;
        std::cerr << "  - Verify PCA9685 address (default 0x40)" << std::endl;
        std::cerr << "  - Connect motors through appropriate motor drivers (H-bridge, etc.)" << std::endl;
        return 1;
    }
    
    std::cout << "Goodbye!" << std::endl;
    return 0;
} 
#include "../../src/ServoMotor.h"
#include "../../src/PCA9685CrossPlatform.h"
#include <iostream>
#include <string>
#include <thread>
#include <chrono>

// PCA9685 I2C address (default 0x40)
const uint8_t PCA9685_ADDRESS = 0x40;

// PCA9685 channels for servos (0-15 available)
const int SERVO1_CHANNEL = 0;
const int SERVO2_CHANNEL = 1;
const int SERVO3_CHANNEL = 2;

// Create PCA9685 PWM backend
PCA9685CrossPlatform pca9685(PCA9685_ADDRESS);

// Create servo motor instances
ServoMotor servo1(SERVO1_CHANNEL, &pca9685, 50, 0, 270, 544, 2400);
ServoMotor servo2(SERVO2_CHANNEL, &pca9685, 50, 0, 180, 544, 2400);
ServoMotor servo3(SERVO3_CHANNEL, &pca9685, 50, 0, 180, 544, 2400);

void printHelp() {
    std::cout << "\nRaspberry Pi PCA9685 Servo Controller" << std::endl;
    std::cout << "Commands:" << std::endl;
    std::cout << "  1 <angle>  : Set servo 1 angle (0-270)" << std::endl;
    std::cout << "  2 <angle>  : Set servo 2 angle (0-180)" << std::endl;
    std::cout << "  3 <angle>  : Set servo 3 angle (0-180)" << std::endl;
    std::cout << "  all <angle>: Set all servos to angle" << std::endl;
    std::cout << "  sweep      : Perform sweep demo on all servos" << std::endl;
    std::cout << "  stop       : Stop all servos" << std::endl;
    std::cout << "  help       : Show this help" << std::endl;
    std::cout << "  quit       : Exit program" << std::endl;
    std::cout << "\nI2C Address: 0x" << std::hex << (int)PCA9685_ADDRESS << std::dec << std::endl;
    std::cout << "Channels: " << SERVO1_CHANNEL << ", " << SERVO2_CHANNEL << ", " << SERVO3_CHANNEL << std::endl;
    std::cout << "\nNOTE: Make sure to run with sudo privileges for I2C access!" << std::endl;
}

void sweepDemo() {
    std::cout << "Performing sweep demo on all servos..." << std::endl;
    
    for (int angle = 0; angle <= 270; angle += 10) {
        servo1.setAngle(angle);
        std::cout << "servo 1: " << angle << " degrees" << std::endl;
        if (angle < 180) {
            servo2.setAngle(angle);
            servo3.setAngle(angle);
            std::cout << "servos 2 and 3: " << angle << " degrees" << std::endl;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
    }
    
    for (int angle = 270; angle >= 0; angle -= 10) {
        servo1.setAngle(angle);
        std::cout << "servo 1: " << angle << " degrees" << std::endl;
        if (angle < 180) {
            servo2.setAngle(angle);
            servo3.setAngle(angle);
            std::cout << "servos 2 and 3: " << angle << " degrees" << std::endl;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
    }
    
    std::cout << "Sweep demo completed!" << std::endl;
}

int main() {
    try {
        std::cout << "Initializing Raspberry Pi PCA9685 Servo Controller..." << std::endl;
        
        // Initialize the PCA9685 (this will call pca9685.begin())
        // Note: This is automatically called in the ServoMotor constructors
        
        std::cout << "PCA9685 initialized successfully!" << std::endl;
        std::cout << "I2C Address: 0x" << std::hex << (int)PCA9685_ADDRESS << std::dec << std::endl;
        std::cout << "Frequency: " << pca9685.getCurrentFrequency() << " Hz" << std::endl;
        std::cout << "Resolution: " << pca9685.getResolution() << " bits (" << pca9685.getMaxDutyCycle() << " max)" << std::endl;
        
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
                servo1.stop();
                servo2.stop();
                servo3.stop();
                std::cout << "All servos stopped" << std::endl;
            }
            else if (command == "help") {
                printHelp();
            }
            else if (command == "sweep") {
                sweepDemo();
            }
            else if (command.substr(0, 4) == "all ") {
                try {
                    float angle = std::stof(command.substr(4));
                    if (angle >= 0 && angle <= 270) {
                        servo1.setAngle(angle);
                        std::cout << "servos 1 set to: " << angle << " degrees" << std::endl;
                        if (angle <= 180) {
                            servo2.setAngle(angle);
                            servo3.setAngle(angle);
                            std::cout << "servos 2 and 3 set to: " << angle << " degrees" << std::endl;
                        }
                    } else {
                        std::cout << "Invalid angle. Use 0-270" << std::endl;
                    }
                }
                catch (const std::exception& e) {
                    std::cout << "Invalid angle format" << std::endl;
                }
            }
            else if (command.length() >= 3 && (command[0] == '1' || command[0] == '2' || command[0] == '3') && command[1] == ' ') {
                try {
                    int servoNum = command[0] - '0';
                    float angle = std::stof(command.substr(2));
                    
                    if (angle >= 0 && angle <= 270) {
                        switch (servoNum) {
                            case 1:
                                servo1.setAngle(angle);
                                break;
                            case 2:
                                if (angle <= 180) {
                                    servo2.setAngle(angle);
                                }
                                break;
                            case 3:
                                if (angle <= 180) {
                                    servo3.setAngle(angle);
                                }
                                break;
                        }
                        std::cout << "Servo " << servoNum << " set to: " << angle << " degrees" << std::endl;
                    } else {
                        std::cout << "Invalid angle. Use 0-270" << std::endl;
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
        
        std::cout << "Stopping all servos and cleaning up..." << std::endl;
        servo1.stop();
        servo2.stop();
        servo3.stop();
        
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        std::cerr << "Make sure to:" << std::endl;
        std::cerr << "  - Run with sudo privileges: sudo ./rpi_pca9685_servo" << std::endl;
        std::cerr << "  - Check I2C is enabled: sudo raspi-config -> Interface Options -> I2C" << std::endl;
        std::cerr << "  - Check PCA9685 is connected to I2C bus 1 (GPIO2=SDA, GPIO3=SCL)" << std::endl;
        std::cerr << "  - Verify PCA9685 address (default 0x40)" << std::endl;
        return 1;
    }
    
    std::cout << "Goodbye!" << std::endl;
    return 0;
} 
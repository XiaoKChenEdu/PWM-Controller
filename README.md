# PWM Motor Controller Library

## Project Structure

```
PWM_Controller/
├── src/                          # Library source files
│   ├── Motor.h                   # Base motor class
│   ├── PWMBackend.h              # PWM backend interface
│   ├── RaspberryPiPWM.h          # Raspberry Pi PWM backend
│   ├── ServoMotor.h              # Servo motor implementation
│   └── DCMotor.h                 # DC motor implementation
├── examples/                     # Example sketches/programs
│   ├── RaspberryPiServo/         # Raspberry Pi hardware PWM servo control
│   ├── RaspberryPiDC/            # Raspberry Pi hardware PWM DC motor control
│   ├── RaspberryPiPCA9685Servo/  # Raspberry Pi PCA9685 servo control
│   └── RaspberryPiPCA9685DC/     # Raspberry Pi PCA9685 DC motor control
```
## Dependencies

### Raspberry Pi Dependencies
- **pigpio**: Hardware PWM and I2C control library
- **build-essential**: C++ compiler and build tools
- **i2c-tools**: I2C device detection and debugging utilities

## Installation

### Raspberry Pi Installation
1. **Clone the repository**:
   ```bash
   git clone <repository-url>
   cd PWM_Controller/examples/RaspberryPI*
   ```

2. **Make Help**
   ```bash
   make help
   ```
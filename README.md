# PWM Motor Controller Library

## Project Structure

```
PWM_Controller/
├── src/                          # Library source files
│   ├── Motor.h                   # Base motor class
│   ├── PWMBackend.h              # PWM backend interface
│   ├── OnboardPWM.h              # Arduino native PWM backend
│   ├── PCA9685LibraryPWM.h       # PCA9685 I2C PWM backend
│   ├── ServoMotor.h              # Servo motor implementation
│   └── DCMotor.h                 # DC motor implementation
└── examples/                     # Example sketches
    ├── OnboardServo/             # Arduino PWM servo control
    ├── OnboardDC/                # Arduino PWM DC motor control
    ├── PCA9685Servo/             # PCA9685 servo control
    └── PCA9685DC/                # PCA9685 DC motor control
```

## Dependencies

### Required Libraries
- **Arduino Core**: Standard Arduino libraries
- **Wire.h**: For I2C communication (PCA9685 examples)

### External Library (for PCA9685)
- **PCA9685-Arduino by NachtRaveVL**: [Install from Library Manager](https://github.com/NachtRaveVL/PCA9685-Arduino)

## Installation

1. **Download/Clone** this repository to your Arduino libraries folder
2. **Install PCA9685 Library** (if using PCA9685 examples):
   - Open Arduino IDE
   - Go to `Tools` → `Manage Libraries`
   - Search for "PCA9685-Arduino by NachtRaveVL"
   - Click Install

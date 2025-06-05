CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -O2
LDFLAGS = -lpigpio -lrt -lpthread

TARGET = rpi_pca9685_servo
SOURCE = rpi_pca9685_servo.cpp

.PHONY: all clean install

all: $(TARGET)

$(TARGET): $(SOURCE)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SOURCE) $(LDFLAGS)

clean:
	rm -f $(TARGET)

install:
	@echo "Installing required libraries..."
	sudo apt update
	sudo apt install -y libpigpio-dev i2c-tools
	@echo "Enabling I2C interface..."
	@echo "Please run 'sudo raspi-config' and enable I2C under Interface Options if not already enabled"

test-i2c:
	@echo "Scanning for I2C devices..."
	sudo i2cdetect -y 1

help:
	@echo "Available targets:"
	@echo "  all       - Build the PCA9685 servo example"
	@echo "  clean     - Remove built files"
	@echo "  install   - Install required dependencies"
	@echo "  test-i2c  - Scan for I2C devices"
	@echo "  help      - Show this help"
	@echo ""
	@echo "Usage:"
	@echo "  make install   # Install dependencies first"
	@echo "  make           # Build the example"
	@echo "  make test-i2c  # Check if PCA9685 is detected"
	@echo "  sudo ./rpi_pca9685_servo  # Run the example"
	@echo ""
	@echo "Wiring (I2C):"
	@echo "  PCA9685 VCC -> RPi 5V"
	@echo "  PCA9685 GND -> RPi GND"
	@echo "  PCA9685 SDA -> RPi GPIO2 (SDA)"
	@echo "  PCA9685 SCL -> RPi GPIO3 (SCL)" 
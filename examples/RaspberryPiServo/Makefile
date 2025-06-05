CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -O2
LDFLAGS = -lpigpio -lrt -lpthread

TARGET = rpi_servo
SOURCE = rpi_servo.cpp

.PHONY: all clean install

all: $(TARGET)

$(TARGET): $(SOURCE)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SOURCE) $(LDFLAGS)

clean:
	rm -f $(TARGET)

install:
	@echo "Installing pigpio library (requires sudo)..."
	sudo apt update
	sudo apt install -y libpigpio-dev

help:
	@echo "Available targets:"
	@echo "  all      - Build the servo example"
	@echo "  clean    - Remove built files"
	@echo "  install  - Install required dependencies (pigpio)"
	@echo "  help     - Show this help"
	@echo ""
	@echo "Usage:"
	@echo "  make install  # Install dependencies first"
	@echo "  make          # Build the example"
	@echo "  sudo ./rpi_servo  # Run the example" 
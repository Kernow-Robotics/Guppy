# Guppy

A small low-cost robot platform for research, education, and entertainment.

This library provides a simple Arduino interface for the Guppy robot from Kernow Robotics, including motor control, onboard LED feedback, battery monitoring, and nRF24 radio communication.

## Features

- Smooth DC motor speed control with safe power ramping
- Background motor update loop using Pico multicore support
- nRF24 radio initialization, send, and receive functions
- Onboard LED heartbeat and error indicators
- Battery voltage measurement and state-of-charge conversion

## Requirements

- Arduino-compatible board with RP2040/Pico multicore support
- `SPI` library
- `RF24` library
- `nRF24L01` library
- `Ticker` library

## Installation

Install the library into the Arduino IDE using the standard library installation workflow, or clone this repository to the Arduino libraries folder using `git clone https://github.com/Kernow-Robotics/Guppy`

## Quick Start

```cpp
#include <Guppy.h>

Guppy guppy;

void setup() {
  guppy.begin();
  guppy.startBackgroundServices();
}

void loop() {
  guppy.motorDrive(3.0, -3.0);
  delay(1000);
  guppy.motorDrive(0.0, 0.0);
  delay(1000);
}
```

## API Reference

### Motor

The `Motor` class is used internally by `Guppy` and can also be used directly if custom motor wiring is required.

#### Motor(int pinA, int pinB)

Constructor.
- `pinA`: PWM/drive pin for forward motor direction
- `pinB`: PWM/drive pin for reverse motor direction

The constructor configures both motor pins as outputs.

#### void power(float power)

Sets the target motor power level.
- `power`: value between `-5.0` and `5.0`

The motor will not jump to the target power immediately. Instead, the internal update loop ramps the actual output smoothly toward the setpoint.

### Guppy

The main library class for controlling the Guppy robot.

#### Guppy()

Constructor.
Initializes the robot pins, motor objects, radio object, and onboard peripherals.

#### void begin()

Initializes SPI, serial output, and performs an initial battery voltage read.
Also flashes a brief LED heartbeat.

#### void startBackgroundServices()

Launches the secondary core and starts the periodic motor update timer.
This method must be called after `begin()` to enable smooth motor ramping.

#### void motorDrive(float power0, float power1)

Sets target power for both drive motors.
- `power0`: target power for motor `m0`
- `power1`: target power for motor `m1`

Values are clamped to the valid range `-5.0` to `5.0`.

#### void initRadio()

Initializes the nRF24 radio transceiver.

#### void startListening(uint8_t address[6])

Begins radio receive mode on the specified 6-byte address.

#### void stopListening()

Stops the radio from listening for incoming packets.

#### void send(String message, uint8_t address[6])

Sends a text message to a remote radio address.
- `message`: Arduino `String` to transmit
- `address`: 6-byte radio address

#### String receive()

Returns the latest received radio message, or an empty string if no packet is available.

#### void lightOn()

Turns the onboard LED on.

#### void lightOff()

Turns the onboard LED off.

#### void heartbeat()

Plays a short LED blink pattern to indicate normal operation.

#### void errorState()

Plays a longer LED blink pattern to indicate an error condition.

#### float updateVbatt()

Reads the analog battery voltage pin, filters the readings, and updates the internal battery voltage estimate.
Returns the filtered battery voltage in volts.

#### int voltageToSOC(float voltage)

Converts a battery voltage value into a state-of-charge percentage using a predefined lookup table and linear interpolation.
Returns a value from `0` to `100`.

## Example

See `examples/Minimal/Minimal.ino` for a complete motor drive example. That sketch demonstrates initializing the library, starting background services, and smoothly driving the two motors.

## Notes

- The library uses RP2040 multicore support to run the motor update loop on the second core.
- Motor power is applied gradually to avoid sudden voltage changes.
- Radio packet payloads are fixed to 32 bytes internally.

## License

This library is released into the public domain.

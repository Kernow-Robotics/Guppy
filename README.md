# Guppy

A small low-cost robot platform for research, education, and entertainment.

This library provides a simple Arduino interface for the Guppy robot from Kernow Robotics, including motor control, onboard LED feedback, battery monitoring, and nRF24 radio communication.

## Contents

- [Features](#features)
- [Requirements](#requirements)
- [Installation](#installation)
- [Quick Start](#quick-start)
- [API Reference](#api-reference)
  - [Motor](#motor)
  - [Guppy](#guppy)
- [Example](#example)
- [Notes](#notes)
- [License](#license)

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

Install the library into the Arduino IDE using the standard library installation workflow, or clone this repository to the Arduino libraries folder using:

```bash
git clone https://github.com/Kernow-Robotics/Guppy
```

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

The `Motor` class is used internally by `Guppy` and can also be used directly for custom motor wiring.

> #### `Motor(int pinA, int pinB)`
> **Purpose:** Create a motor controller for a pair of drive pins.
> 
> **Parameters:**
> - `pinA` — PWM/drive pin for forward motor direction
> - `pinB` — PWM/drive pin for reverse motor direction
> 
> **Behavior:** Configures both motor pins as outputs.

> #### `void power(float power)`
> **Purpose:** Set the target motor power.
> 
> **Parameters:**
> - `power` — motor power between `-5.0` and `5.0`
> 
> **Behavior:** The value is stored as a setpoint, and the actual motor output ramps smoothly toward that target.

### Guppy

The main library class for controlling the Guppy robot.

> #### `Guppy()`
> **Purpose:** Construct the robot controller.
> 
> **Behavior:** Initializes motor objects, radio support, and onboard GPIO pins.

> #### `void begin()`
> **Purpose:** Initialize SPI, serial logging, and battery input.
> 
> **Behavior:** Performs initial hardware setup and flashes a startup heartbeat.

> #### `void startBackgroundServices()`
> **Purpose:** Start the secondary core and periodic motor update loop.
> 
> **Behavior:** Must be called after `begin()` to enable smooth motor ramping.

> #### `void motorDrive(float power0, float power1)`
> **Purpose:** Set the target power for both drive motors.
> 
> **Parameters:**
> - `power0` — target power for `m0`
> - `power1` — target power for `m1`
> 
> **Behavior:** Power values are clamped to `-5.0`..`5.0`.

> #### `void initRadio()`
> **Purpose:** Initialize the nRF24 radio transceiver.

> #### `void startListening(uint8_t address[6])`
> **Purpose:** Begin listening for radio packets on a 6-byte address.

> #### `void stopListening()`
> **Purpose:** Stop radio receive mode.

> #### `void send(String message, uint8_t address[6])`
> **Purpose:** Transmit a text message over the radio.
> 
> **Parameters:**
> - `message` — Arduino `String` payload
> - `address` — 6-byte radio address

> #### `String receive()`
> **Purpose:** Read a received radio message.
> 
> **Returns:** Received message or an empty string when no packet is available.

> #### `void lightOn()`
> **Purpose:** Turn the onboard LED on.

> #### `void lightOff()`
> **Purpose:** Turn the onboard LED off.

> #### `void heartbeat()`
> **Purpose:** Play a short LED blink pattern to show normal operation.

> #### `void errorState()`
> **Purpose:** Play a longer LED blink pattern to show an error.

> #### `float updateVbatt()`
> **Purpose:** Read and filter the battery voltage.
> 
> **Returns:** Filtered battery voltage in volts.

> #### `int voltageToSOC(float voltage)`
> **Purpose:** Convert a voltage reading to battery state-of-charge.
> 
> **Returns:** Percentage from `0` to `100`.

## Example

See `examples/Minimal/Minimal.ino` for a complete motor drive example. That sketch demonstrates initializing the library, starting background services, and smoothly driving both motors.

## Notes

- The library uses RP2040 multicore support to run the motor update loop on the second core.
- Motor power is applied gradually to avoid sudden voltage changes.
- Radio packet payloads are fixed to 32 bytes internally.

## License

This library is released into the public domain.

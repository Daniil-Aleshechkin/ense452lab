# LAB 2: STM32 Micro CLI Application

This is a CLI (Command Line Interface) application designed for STM32 microcontrollers. It allows you to control an LED with four available commands: `help`, `turnon`, `turnoff`, and `status`. Command text is case insensitive.

## Prerequisites

Before using this CLI application, make sure you have the following prerequisites:

- [PuTTY](https://www.putty.org/) installed on your computer.
- PuTTY configured with the following settings:
  - Baud rate: 115200
  - Parity: None
  - Stop bits: 1
- Support for VT100 terminal commands enabled in PuTTY:
  1. Open PuTTY.
  2. Go to `Terminal` in the left sidebar.
  3. Under the `Keyboard` section, click the `VT100+` option.

## Usage

1. Connect your STM32 microcontroller to your computer using a serial communication interface (e.g., USB-to-Serial adapter).
2. Open PuTTY and configure it with the prerequisites mentioned above.
3. Connect to the STM32 microcontroller's serial port.
4. Once connected, you can use the following commands:

### Commands

- `help`: Displays help text with information about available commands.
- `turnon`: Turns on the LED.
- `turnoff`: Turns off the LED.
- `status`: Prints the status of the LED.

Example usage:

```
> help
> turnon
> turnoff
> stats
```

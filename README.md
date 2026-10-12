# PIC-EEPROM-Energy-Logger
# Smart Meter Power-Loss Recovery System (PIC16F877A)

A bare-metal C firmware implementation demonstrating non-volatile data retention and hardware interrupt handling for smart metrology applications. 

## Overview
In the event of a sudden power failure, smart meters must instantly save the customer's accumulated energy data before the system's capacitors drain. This project simulates that critical sequence using an external hardware interrupt to trigger an emergency EEPROM write cycle.

## Technical Highlights
* **Hardware Interrupts (ISR):** Configured the PIC16F877A `RB0/INT` pin to trigger on a falling edge. The Interrupt Service Routine (ISR) safely pauses the main processing loop to execute the high-priority memory save function.
* **Non-Volatile Memory (EEPROM):** Engineered a data-handling routine to split 32-bit energy integers into four sequential 8-bit bytes for EEPROM storage (`make8`), and successfully reconstruct them on system reboot (`make32`).
* **Custom Hardware Drivers:** Utilized a custom-written 4-bit LCD driver to maintain strict timing control during asynchronous interrupt events, avoiding the busy-flag polling crashes common in standard libraries.

## Files Included
* `main.c`: Core firmware featuring the ISR, EEPROM logic, and custom LCD driver.
* `simulation.pdsprj`: Proteus schematic featuring the PIC, LCD, and interrupt trigger button.
* `emergency_save.png`: Screenshot of the active hardware interrupt seizing the CPU.

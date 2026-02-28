# Real-Time Digital Clock Controller (ATmega32)

## Project Overview
This project features a fully functional Digital Clock system designed using an AVR Microcontroller (ATmega32). The system maintains an accurate 24-hour time format (HH:MM:SS) and outputs the data to an LCD display (LM016L). It combines precise firmware timing with a professional hardware simulation in Proteus.

## Key Features
* **Precise Timekeeping**: Utilizes internal Microcontroller Timers to ensure 1-second accuracy for real-time tracking.
* **24-Hour Format**: Automatically handles time overflows (60 seconds to 1 minute, 60 minutes to 1 hour, 24 hours to reset).
* **Interactive Controls**: Includes hardware reset and manual adjustment capabilities via push-buttons.
* **Visual Interface**: Displays real-time clock and date data on a 16x2 character LCD.

## Technical Stack
* **Hardware**: ATmega32 Microcontroller (AVR Architecture).
* **Software/Firmware**: C Language (main.c).
* **Simulation Tool**: Proteus Design Suite.

## Logic Flowchart & System Operations
The system logic is divided into three main stages to ensure stable performance:

### 1. Initialization Phase
* **I/O Configuration**: Sets Port B and Port D as output ports for the LCD interface.
* **LCD Setup**: Configures the LM016L screen, clears the buffer, and sets the starting cursor position.
* **Time Variables**: Initializes variables for Seconds (SS), Minutes (MM), and Hours (HH).

### 2. The Main Processing Loop
The clock operates on a continuous cycle:
* **Time Increment**: The system waits for a 1-second trigger (Interrupt or Delay) then increments the `Seconds` count.
* **Carry-over Logic**:
    * If `SS == 60`: Reset `SS` to 0 and increment `MM`.
    * If `MM == 60`: Reset `MM` to 0 and increment `HH`.
    * If `HH == 24`: Reset all values to `00:00:00`.
* **User Input**: Checks for button presses to manually reset or adjust the time.

### 3. Display Update
* The current time values are converted from integers to ASCII format.
* The LCD is updated with the formatted string (HH:MM:SS) every second to reflect the new time.

## Project Structure
* **main.c**: The core C source code containing the timing logic.
* **digital_clock.pdsprj**: The Proteus simulation file for hardware verification.
* **Clock_report.pdf**: Detailed technical documentation and circuit analysis.
* **Flowchart.png**: Visual representation of the system logic.

## How to Run
1. **Simulation**: Open `digital_clock.pdsprj` in Proteus and click the 'Play' button.
2. **Firmware**:
   * Open `main.c` in an AVR compiler (e.g., Microchip Studio).
   * Compile the code to generate the `.hex` file.
   * Load the `.hex` file into the ATmega32 component within the Proteus environment.

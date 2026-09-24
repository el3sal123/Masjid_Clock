# Masjid_Clock1
Digital masjid clock with 6x 7-segment display, LCD prayer times, 4x4 keypad, and LM35 temperature sensor
 Masjid Clock

A digital masjid clock displaying the current time on six 7-segment digits
and the five daily prayer times on an LCD, configurable via a 4x4 keypad.
Includes ambient temperature readout from an LM35 sensor.

## Features
- Real-time clock (HH:MM:SS) on 6 multiplexed 7-segment displays
- Prayer times (Fajr, Dhuhr, Asr, Maghrib, Isha) shown on a 16x2 LCD
- Keypad-based configuration of the time and each prayer time
- Temperature measurement with LM35 (ADC)
- Dedicated push button for setting the clock

## Hardware
| Component | Qty | Purpose |
|---|---|---|
| Microcontroller (<MCU>) | 1 | Main controller |
| 7-segment display | 6 | HH:MM:SS |
| 16x2 LCD | 1 | Prayer times / temperature |
| 4x4 keypad | 1 | User input |
| LM35 | 1 | Temperature sensor |
| Push button | 1 | Enter time-setting mode |

## Keypad Mapping
| Key | Action |
|---|---|
| `D` | Show Fajr time |
| `/` | Show Dhuhr time |
| `x` | Show Asr time |
| `-` | Show Maghrib time |
| `+` | Show Isha time |
| `=` | Show temperature (LM35) |
| `0-9` | While a prayer time is displayed: edit that time |

## Setting the Clock
1. Press the push button.
2. Enter 2 digits for hours, 2 for minutes, 2 for seconds.
3. The clock resumes from the entered time.

## Setting a Prayer Time
1. Press the prayer key (e.g. `D` for Fajr).
2. While it is displayed, type the new time using the digit keys.

## Project Structure
See the folder layout: `src/`, `hardware/`, `build/`, `docs/`, `media/`.

## How to Run
- **Simulation:** open `hardware/schematic/<file>.pdsprj` in Proteus, load `build/firmware.hex` into the MCU.
- **Hardware:** flash `build/firmware.hex` using <programmer>.

## Technical Notes
- Display refresh: multiplexing at <X> Hz
- Timekeeping: <Timer / interrupt / RTC>
- LM35: 10 mV/°C, read via <ADC resolution>-bit ADC

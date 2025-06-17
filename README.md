# PIC Digital Alarm Clock

**Created by Sari Jokinen**  
*December 2023 - February 2024*  
[github.com/sarix1](https://github.com/sarix1)

## Table of Contents
- [Hardware Architecture](#hardware-architecture)
- [Software Architecture](#software-architecture)
- [Code Structure](#code-structure)
- [Core Modules](#core-modules)
- [Build System](#build-system)
- [Hardware Documentation](#hardware-documentation)

## Overview

A digital alarm clock using a PIC16F690 microcontroller, featuring:
- 4-digit 7-segment LED display
- Push-buttons and a rotary encoder for setting time, alarm, tune, volume, display brightness
- PWM-driven speaker
- DHT11 temperature/humidity sensor

## Hardware Architecture

### System Design Philosophy
The design maximizes the limited I/O capabilities of the PIC16F690 by using:
- **SIPO (Serial-In, Parallel-Out)** shift registers (CD4094B) for display output
- **PISO (Parallel-In, Serial-Out)** shift registers (CD4021B) for button input
- Shared clock and latch signals to minimize pin usage

### Key Components
- **Microcontroller**: PIC16F690 (20-pin, 8-bit MCU)
- **Display**: Four 7-segment LED displays
- **Input**: 10 buttons + 1 rotary encoder with push button
- **Sensors**: DHT11 temperature/humidity sensor
- **Audio**: PWM-driven speaker on RC5/P1A
- **Shift Registers**: 4x CD4094B (output), 2x CD4021B (input)

## Software Architecture

```
picclock/
├── src/
│   ├── main.c              # Main program entry and state machine
│   ├── config.h            # Configuration bits and system defines
│   ├── hardware/
│   │   ├── shift_reg.c     # SIPO/PISO shift register drivers
│   │   ├── shift_reg.h
│   │   ├── display.c       # 7-segment display control
│   │   ├── display.h
│   │   ├── buttons.c       # Button input handling
│   │   ├── buttons.h
│   │   ├── encoder.c       # Rotary encoder driver
│   │   ├── encoder.h
│   │   ├── dht11.c         # Temperature/humidity sensor
│   │   └── dht11.h
│   ├── time/
│   │   ├── rtc.c           # Real-time clock implementation
│   │   ├── rtc.h
│   │   ├── alarm.c         # Alarm functionality
│   │   └── alarm.h
│   └── audio/
│       ├── pwm.c           # PWM audio generation
│       └── pwm.h
├── include/
│   └── pic16f690.h         # MCU-specific definitions
├── build/
│   └── Makefile            # Build configuration
└── README.md               # This file
```

## Core Modules

### 1. Shift Register Interface (`shift_reg.c/h`)
Manages serial communication with shift registers:

```c
// Expected function signatures
void shift_init(void);
void shift_write_byte(uint8_t data);
uint16_t shift_read_buttons(void);
void shift_latch_output(void);
```

**Key Implementation Details:**
- Clock signal on RC0 (PISO) and RC7 (SIPO)
- Data lines: RC6 (SIPO out), RB5 (PISO in)
- Latch signals: RC1 (PISO), RC4 (SIPO)

### 2. Display Driver (`display.c/h`)
Controls 4-digit 7-segment display through SIPO registers:

```c
// 7-segment encoding (common cathode assumed)
const uint8_t seg_digits[10] =
{
    0x3F, // 0: 0111111
    0x06, // 1: 0000110
    0x5B, // 2: 1011011
    // ...
};

void display_update(uint8_t digit[4]);
void display_set_brightness(uint8_t level);
```

**Display Mapping:**
- Each CD4094B controls one digit
- Segments A-G mapped to Q1-Q7
- RA2 controls global display enable (PWM for brightness)

### 3. Button Input Handler (`buttons.c/h`)
Processes 10 button inputs through PISO registers:

```c
typedef enum {
    BTN_TIME_SET    = 0x01,
    BTN_ALARM_SET   = 0x02,
    BTN_SEC_SET     = 0x04,
    BTN_ENCODER     = 0x08,
    BTN_ALM_TMR_ON  = 0x10,
    BTN_SENSOR      = 0x20,
    BTN_SNOOZE      = 0x40,
    BTN_BRIGHT_SET  = 0x80,
    BTN_TUNE_SET    = 0x100,
    BTN_VOLUME_SET  = 0x200
} button_t;

uint16_t buttons_read(void);
bool button_is_pressed(button_t btn);
```

### 4. Rotary Encoder Driver (`encoder.c/h`)
Handles quadrature decoding for smooth user input:

```c
typedef struct {
    int8_t position;
    bool button_pressed;
} encoder_state_t;

void encoder_init(void);
encoder_state_t encoder_read(void);
```

**Pin Connections:**
- Encoder A: RB6
- Encoder B: RB7
- Button integrated with main button matrix

### 5. DHT11 Sensor Interface (`dht11.c/h`)
Implements 1-wire protocol for temperature/humidity:

```c
typedef struct {
    uint8_t temperature;
    uint8_t humidity;
    bool valid;
} dht11_data_t;

dht11_data_t dht11_read(void);
```

**Protocol Implementation:**
- Single data line on RB4/AN10
- Requires precise timing for 1-wire communication
- 40-bit data transmission (8-bit integral RH + 8-bit decimal RH + 8-bit integral T + 8-bit decimal T + 8-bit checksum)

### 6. Real-Time Clock (`rtc.c/h`)
Software RTC using Timer1 and crystal oscillator:

```c
typedef struct {
    uint8_t hours;
    uint8_t minutes;
    uint8_t seconds;
} time_t;

void rtc_init(void);
void rtc_set_time(time_t time);
time_t rtc_get_time(void);
```

**Implementation Notes:**
- Uses 32.768 kHz crystal on RA4/RA5
- Timer1 configured for 1-second interrupts
- Handles 24-hour and 12-hour formats

### 7. PWM Audio (`pwm.c/h`)
Generates alarm tones using hardware PWM:

```c
void pwm_init(void);
void pwm_set_frequency(uint16_t freq);
void pwm_play_tone(uint16_t freq, uint16_t duration_ms);
void pwm_play_melody(const uint16_t *notes, uint8_t count);
```

**PWM Configuration:**
- Output on RC5/P1A
- Uses Timer2 and ECCP module

### Configuration Bits
```c
__code uint16_t __at(_CONFIG) config = 
    _INTRC_OSC_NOCLKOUT &  // Internal oscillator, I/O on RA4/RA5
    _WDT_OFF &             // Watchdog timer disabled
    _PWRTE_ON &            // Power-up timer enabled
    _MCLRE_OFF &           // RA3/MCLR pin function is digital I/O
    _CP_OFF &              // Code protection off
    _CPD_OFF &             // Data code protection off
    _BOR_OFF &             // Brown-out reset disabled
    _IESO_OFF &            // Internal/External switch-over disabled
    _FCMEN_OFF;            // Fail-safe clock monitor disabled
```

## Hardware Documentation

### Microcontroller Unit Pinout

#### PIC16F690
```
                    _______     _______
                   |       \___/       |
             +5V --|1 VDD        VSS 20|-- 0V
                   |                   |
          Xtal A ->|2 RA5        RA0 19|-- Unused
                   |   osc1            |
          Xtal B ->|3 RA4        RA1 18|-- Unused
                   |   osc2            |
           Reset ->|4 RA3        RA2 17|-> SIPO Output Enable
                   |   mclr       in   |
PWM/Speaker Sig. <~|5 RC5        RC0 16|-> PISO Clock
                   |   p1a        out  |
      SIPO Latch <-|6 RC4        RC1 15|-> PISO Latch
                   |   out        out  |
          Unused --|7 RC3        RC2 14|-- Unused
                   |   p1c        p1d  |
       SIPO Data <-|8 RC6        RB4 13|<-> DHT11
                   |   out        an10 |
      SIPO Clock <-|9 RC7        RB5 12|<- PISO Data
                   |   out        in   |
  Rot. Encoder B ->|10 RB7       RB6 11|<- Rot. Encoder A
                   |    in        in   |
                   '-------------------'
                         PIC16F690
```

### Pin Connections Summary

#### PIC16F690 Pin Assignments
- **Pin 1 (VDD)**: +5V Power Supply
- **Pin 2 (RA5/OSC1)**: Crystal Oscillator A
- **Pin 3 (RA4/OSC2)**: Crystal Oscillator B
- **Pin 4 (RA3/MCLR)**: Reset Input
- **Pin 5 (RC5/P1A)**: PWM/Speaker Signal Output
- **Pin 6 (RC4)**: SIPO Latch Output
- **Pin 7 (RC3/P1C)**: Unused
- **Pin 8 (RC6)**: SIPO Data Output
- **Pin 9 (RC7)**: SIPO Clock Output
- **Pin 10 (RB7)**: Rotary Encoder B Input
- **Pin 11 (RB6)**: Rotary Encoder A Input
- **Pin 12 (RB5)**: PISO Data Input
- **Pin 13 (RB4/AN10)**: DHT11 Temperature/Humidity Sensor
- **Pin 14 (RC2/P1D)**: Unused
- **Pin 15 (RC1)**: PISO Latch Output
- **Pin 16 (RC0)**: PISO Clock Output
- **Pin 17 (RA2)**: SIPO Output Enable
- **Pin 18 (RA1)**: Unused
- **Pin 19 (RA0)**: Unused
- **Pin 20 (VSS)**: Ground (0V)

### Rotary Encoder Pinout

```
       ______    
  A <-|  __  |-> Button: Min/Hour
Gnd --| (__) |
  B <-|______|-- Gnd
```

### Shift Register Details

#### Parallel-In, Serial-Out Shift Registers (PISO)

##### CD4021B 1
```
                 ____   ____
 Button Input ->|PI8 \_/ VDD|-- +5V
       Unused <-|Q6      PI7|<- Unused 
    PISO Data <-|Q8      PI6|<- BTN_SENSOR
 BTN_TIME_SET ->|PI4     PI5|<- BTN_SEC_SET
BTN_ALARM_SET ->|PI3      Q7|-> Unused
  BTN_ENCODER ->|PI2     SER|<- CD4021B 2 Q8
BTN_ALM_TMR_ON->|PI1     CLK|<- PISO Clock
           0V --|VSS     LAT|<- PISO Latch
                '-----------'
                  CD4021B 1
```

##### CD4021B 2
```
                 ____   ____
       Unused ->|PI8 \_/ VDD|-- +5V
       Unused <-|Q6      PI7|<- Unused
 CD4021B 1 SER<-|Q8      PI6|<- Unused
   BTN_SNOOZE ->|PI4     PI5|<- Unused
BTN_BRIGHT_SET->|PI3      Q7|-> Unused
 BTN_TUNE_SET ->|PI2     SER|-> Unused
BTN_VOLUME_SET->|PI1     CLK|<- PISO Clock
           0V --|VSS     LAT|<- PISO Latch
                '-----------'
                  CD4021B 2
```

#### Serial-In, Parallel-Out Shift Registers (SIPO)

##### CD4094B 1-4 (Display Drivers)
Each CD4094B drives one 7-segment display digit:
- Q1-Q7: Segments A-G
- Daisy-chained via QS outputs
- Common latch and clock signals
- RA2 provides global output enable

### Button Inputs (via PISO)
- BTN_TIME_SET
- BTN_ALARM_SET
- BTN_SEC_SET
- BTN_ENCODER
- BTN_ALARM_TIMER_ON
- BTN_SENSOR
- BTN_SNOOZE
- BTN_BRIGHT_SET
- BTN_TUNE_SET
- BTN_VOLUME_SET

### Display Outputs (via SIPO)
- Four 7-segment displays
- Each display uses segments A-G (Q1-Q7)
- All displays share common control signals:
  - SIPO Latch
  - SIPO Clock
  - LED Enable

## Program Operation

### Power-On
1. Initialize oscillator (internal 8MHz or external crystal)
2. Configure I/O pins according to pin assignments
3. Initialize shift registers
4. Read stored settings from EEPROM
5. Start RTC
6. Enter main display loop

### UI States
- **Normal Display**: Shows current time
- **Time Set Mode**: Adjust hours/minutes with encoder
- **Alarm Set Mode**: Configure alarm time
- **Temperature Display**: Show DHT11 readings
- **Brightness Adjust**: PWM-based display dimming
- **Volume Setting**: Adjust alarm volume

### Notes
- Power-saving mode?
- Checksum for DHT11 data
- Save user settings in EEPROM

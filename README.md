# PIC Digital Alarm Clock

A digital alarm clock using a PIC16F690 MCU

Created by [Sari Jokinen](https://github.com/sarix1) in December 2023 - February 2024


## Components

- PIC16F690 (20-pin, 8-bit MCU)
- 4 x 7-segment LED displays
- 10 x push-buttons
- 1 rotary encoder with push button
- DHT11 temperature/humidity sensor
- PWM-driven speaker on RC5/P1A
- 4 x SIPO shift registers (CD4094B) for display output
- 2 x PISO shift registers (CD4021B) for button input

## Pinout

### PIC16F690

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
PWM / Speaker Signal <~|5 RC5        RC0 16|-> PISO Clock
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

### Rotary Encoder
```
       ______    
  A <-|  __  |-> Button: Min/Hour
Gnd --| (__) |
  B <-|______|-- Gnd
```

### Parallel-In, Serial-Out Shift Registers
```
                     ____   ____
     Button Input ->|PI8 \_/ VDD|-- +5V
           Unused <-|Q6      PI7|<- Unused 
        PISO Data <-|Q8      PI6|<- BTN_SENSOR
     BTN_TIME_SET ->|PI4     PI5|<- BTN_SEC_SET
    BTN_ALARM_SET ->|PI3      Q7|-> Unused
      BTN_ENCODER ->|PI2     SER|<- CD4021B 2 Q8 <-.
TN_ALARM_TIMER_ON ->|PI1     CLK|<- PISO Clock     |
               0V --|VSS     LAT|<- PISO Latch     |
                    '-----------'                  |
                      CD4021B 1                    |
.--------------------------------------------------'
|                    ____   ____
|          Unused ->|PI8 \_/ VDD|-- +5V
|          Unused <-|Q6      PI7|<- Unused
'-- CD4021B 1 SER <-|Q8      PI6|<- Unused
       BTN_SNOOZE ->|PI4     PI5|<- Unused
   BTN_BRIGHT_SET ->|PI3      Q7|-> Unused
     BTN_TUNE_SET ->|PI2     SER|-> Unused
   BTN_VOLUME_SET ->|PI1     CLK|<- PISO Clock
               0V --|VSS     LAT|<- PISO Latch
                    '-----------'
                      CD4021B 2
```

### Serial-In, Parallel-Out Shift Registers
```
                     ____   ____
       SIPO Latch ->|LAT \_/ VDD|-- +5V
        SIPO Data ->|DAT      OE|<- LED Enable
       SIPO Clock ->|CLK      Q5|-> 7-Seg 1 E
        7-Seg 1 A <-|Q1       Q6|-> 7-Seg 1 F
        7-Seg 1 B <-|Q2       Q7|-> 7-Seg 1 G
        7-Seg 1 C <-|Q3       Q8|-> Unused
        7-Seg 1 D <-|Q4      Q'S|-> Unused
               0V --|VSS      QS|-> CD4094 2 DAT --.
                    '-----------'                  |
                      CD4094B 1                    |
.--------------------------------------------------'
|                    ____   ____
|      SIPO Latch ->|LAT \_/ VDD|-- +5V
'--> CD4094B 1 QS ->|DAT      OE|<- LED Enable
       SIPO Clock ->|CLK      Q5|-> 7-Seg 2 E
        7-Seg 2 A <-|Q1       Q6|-> 7-Seg 2 F
        7-Seg 2 B <-|Q2       Q7|-> 7-Seg 2 G
        7-Seg 2 C <-|Q3       Q8|-> Unused
        7-Seg 2 D <-|Q4      Q'S|-> Unused
               0V --|VSS      QS|-> CD4094 3 DAT --.
                    '-----------'                  |
                      CD4094B 2                    |
.--------------------------------------------------'
|                    ____   ____
|      SIPO Latch ->|LAT \_/ VDD|-- +5V
'--> CD4094B 2 QS ->|DAT      OE|<- LED Enable
       SIPO Clock ->|CLK      Q5|-> 7-Seg 3 E
        7-Seg 3 A <-|Q1       Q6|-> 7-Seg 3 F
        7-Seg 3 B <-|Q2       Q7|-> 7-Seg 3 G
        7-Seg 3 C <-|Q3       Q8|-> Unused
        7-Seg 3 D <-|Q4      Q'S|-> Unused
               0V --|VSS      QS|-> CD4094 4 DAT --.
                    '-----------'                  |
                      CD4094B 3                    |
.--------------------------------------------------'
|                    ____   ____
|      SIPO Latch ->|LAT \_/ VDD|-- +5V
'--> CD4094B 3 QS ->|DAT      OE|<- LED Enable
       SIPO Clock ->|CLK      Q5|-> 7-Seg 4 E
        7-Seg 4 A <-|Q1       Q6|-> 7-Seg 4 F
        7-Seg 4 B <-|Q2       Q7|-> 7-Seg 4 G
        7-Seg 4 C <-|Q3       Q8|-> Unused
        7-Seg 4 D <-|Q4      Q'S|-> Unused
               0V --|VSS      QS|-> Unused
                    '-----------'                  
                      CD4094B 4
```

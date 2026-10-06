# Raspberi Pi Zero OS Monitor

## Table of Content


### Short description
API that display output and interract with touch screen TFT display to. waht to display can be configured via YAML file.

### Technology stack
- C language: main implementation of logic and low level interraction with hardware
- Python: to read configuration and transmit it to C language level on startup

### Setup Details

- Raspberi Pi Zero W with Raspbian OS image
- 3.5" Touch screen TFT display (IC Driver ILI9488-display ,XPT2046-touch; resolution 480x320 )

# Application Demo Goal
Display RAM usage, CPU LOAD, NETWORK LOAD, Top 5 processes by resources usage.
# SOLAR-DIVERTER
The s.i. is a means of storing excess solar production in a hot-water cylinder (HWC) instead of deliver it to the grid, where it is unwanted and doesn't make any money. The HWC essentially functions as a cheap solar battery (3 times cheaper than a real home battery). For a handy person this is a very simple diy project.

Check out the [wiki](https://github.com/patience4711/SOLAR-DIVERTER/wiki) for more info

## the system
The software currently runs on a esp32c3 (optional with oled display). All settings can be done via the webui.<br>
![settings_1](https://github.com/user-attachments/assets/042de2c8-96dc-4ca5-92d3-14469ccc7f19)

## b.o.m < 20$
- ESP32C3 (with oled display)
- a thyrisor module (or ssr and zcd)
- 5v dc power supply
  
## downloads
- sept 3 2025 [esp32-solar-diverter-v0.0](https://github.com/patience4711/SOLAR-DIVERTER/blob/main/ESP32C3_SOLAR_DIVERTER.ino.bin)

## in practice
This AC Power Controller can work with thyristors and also with solid state relais. Complete with cooling body they are not very expensive. Using a SSR means you also need a zero cross detector. 

The picture below is an example of a very simple but smart solar thermic battery.
![DIVERTER](https://github.com/user-attachments/assets/24d69c4e-29d5-4c90-af08-9f484eec1a6b)


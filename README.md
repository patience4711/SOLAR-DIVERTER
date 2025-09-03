# SOLAR-DIVERTER
The s.i. is a means of storing excess solar production in a hot-water cylinder (HWC) instead of deliver it to the grid, where it is unwanted and doesn't make any money. The HWC essentially functions as a cheap solar battery. (total costs < 20 Euro for the electronics)

Check out the [wiki](https://github.com/patience4711/SOLAR-DIVERTER/wiki) for more info

In this meter we can put a dongle (home wizzard or diy [look here](https://github.com/patience4711/ESP32-C3-READ-P1-METER) that we can request for data via our wifi network. Now we know exactly how much energy we have excess. 
The s.i. now drives a thyristor (or ssr) open so that exactly the amount of excess flows into our HWC. The HWC we can put in series with our normal hot water supply. 
As the HWC normally has to heat cold water, the water is already (partially) warmed up, so here can save energy. 

This dimmer can work with thyristors and also with solid state relais. Complete with cooling body they are not very expensive. <br>

The picture below is an example of a very simple but smart solar thermic battery.
![DIVERTER](https://github.com/user-attachments/assets/24d69c4e-29d5-4c90-af08-9f484eec1a6b)


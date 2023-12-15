# flipTurn


<img align="right" src="images/10E8D329-E4C9-4EC1-8C14-E52BE17179A1.png" width="150"/> 






ESP-32 based BLE foot operated page-turner designed to work with iPad music apps like Unreal Book

## Operation

the flipTurn footswitch emulates a BT-105 series Airturn device. 

It uses BLE (Bluetooth Low Energy) to pair the foot-switch peripheral with central device (in my case, an iPad or iPhone).  To connect the peripheral in IOS: Settings -> Bluetooth -> find flipTurn and click to pair.

Once foot-switch is paired to iPad device, in Unreal Book App, go to Settings -> Airturn -> Airturn setup.  If flipTurn is not listed as connected, click Add Keyboard Device.

Once paired, the peripheral will automatically re-pair upon startup.

|Footswitch Action| Meaning|
|-------|---|
| Single Press| Page Music Down (Down Arrow)
| Double Press | Page Music Back (Up Arrow)
| Hold Press | 1) If on text entry search screen, open Virtual onscreen keyboard, and 2) Show Battery Status (for four seconds)



|RGB LED Colour| Meaning|
|---|---|
|flashing Blue| Looking for Bluetooth Connection
| solid Blue| Bluetooth Connected
| Green| High Battery charge (~3.7 - 4.2V)
| Magenta| Charge Now Warning (~3.2 to 3.7V)
| Red| Low Battery Warning (~ 3 to 3.2V)
|Flashing Red| Preparing to auto shutdown (< 3V).  Charge battery to reset

On power-up, RGB LED shows battery status for four seconds before indicating Bluetooth connection status.




## Hardware

* DFRobot Firebeetle ESP-32 [DFR0478 module](https://www.dfrobot.com/product-1590.html?tracking=5bb8307c07f05&gclid=CjwKCAjw5P2aBhAlEiwAAdY7dJ-QGfzxyblFBYGZIF1oe9mBKURqpCOLsVRYEaOLaDYofkDmhq_u7BoCOLoQAvD_BwE)
* Rechargeable LiPo battery
* Slide Switch

#flipTurn Case
3D-printed footswitch case, inspired by 

See project at Printables for 3D print files.


## Licence
***Software:*** [GPL v2 Licence](https://github.com/cwgstreet/flipTurn/blob/main/Licence-software.md)

[![License: GPL v2](https://img.shields.io/badge/License-GPLv2-blue.svg)](https://www.gnu.org/licenses/gpl-2.0)


<br>

***Hardware:*** [CC-BY-SA-4.0](https://github.com/cwgstreet/flipTurn/blob/main/Licence-hardware.md)

 [![CC BY 4.0][cc-by-shield]][cc-by]

Hardware (circuit and 3D print) is licensed under a
[Creative Commons Attribution 4.0 International License][cc-by].

[![CC BY 4.0][cc-by-image]][cc-by]

[cc-by]: http://creativecommons.org/licenses/by/4.0/
[cc-by-image]: https://i.creativecommons.org/l/by/4.0/88x31.png
[cc-by-shield]: https://img.shields.io/badge/License-CC%20BY%204.0-lightgrey.svg


-----
<img align="right" src="*.png#gh-light-mode-only" width="250"/> 
<img align="right" src="*.jpg#gh-dark-mode-only" width="2500"/>  

<img align="Left" src="images/flipTurnLogo.png" width="250"/>


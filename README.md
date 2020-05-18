# Ignition-panel-to-wake-on-lan
This project, is entirely to turn on a PC in a totally overkill fashion; nobody should make this, everybody should make this.
Before we get into any of these, I don't pretend to be an expert on any of this at all, so I might not know everything I'm talking about but I will try my best to give you correct information! I have a bunch of store links here, none of them are affiliate links or anything.

I take no ownership of the components in the code, I found them online and cobbled them together to make my program. Schematic file, bill of materials, and code are named appropriately.
The underlying logic is that a nodemcu ESP8266 module which is one of these ![nodemcu_esp8266](https://i0.wp.com/www.abstractotech.com/wp-content/uploads/2019/02/IMG_20190209_144805-2.jpg?resize=1024%2C773&ssl=1)

An ESP8266 is an IOT module which is similar to an arduino except it comes built in with wireless networking capabilities and operates on a 3.3V logic level as opposed to the arduino's 5V. The nodemcu board is an ESP8266 module with pinouts, and a USB input for easier acces/powering abilities. (I'm sure there's more to the board but that's really all I know). There's no reason this project wont work with the new ESP32 module as well. 

The underlying logic is to take inputs from an ignition panel and on the final switch press, send a wake on lan packet to startup the PC.
The ignition panel looks something like this ![igniton panel](https://i.imgur.com/HezUzRw.jpg)
![igniton panel2](https://i.imgur.com/JDkIAPW.jpg)
you get the idea. It's for race cars, all the better for our end use. 

**A note on the ignition module:** 

1. The ignition panel may claim to be 12V, but it's not strictly 12V. In my testing, none of the components need 12V to function, everything runs fine with just 3.3V, saving you the need for transistors and separate 12V power lanes.
2. The lights in the module are just bulbs, not LEDs so they do work with 3.3V. Better yet, feel free to snip these off and solder on LEDs for even brighter lights.
3. The panel I purchased came with wires, loop clips, and shrink wrap tubing to help with the soldering. Your results may vary.

## Stuff you need
These prices may vary but this is based on time of creation. 
[Bill of materials here](https://github.com/Srcodesalittle/Ignition-panel-to-wake-on-lan/blob/master/Bill%20of%20materials.xlsx)
I really don't want to make a table here, so bear with me and actually check out the excel file. 

**Points to note in the items**
1. You can get a soldering or plug and use soddering board depending on whether or not you want to solder everything together. If you do go the soldering route, you will need a soldering iron and some good solder. *Eye protection is probably a good idea as well*
2. You will need an ethernet cable to execute the wake on lan, this wont work over wifi, I have tried. So the length of cable you need will depend on your house setup. If it's unfeasible, consider a ![powerline adapter](https://www.amazon.com/TP-Link-AV600-Powerline-Ethernet-Adapter/dp/B00AWRUICG).

## The wiring
Here's the glorious guts ![internal tangle of wiring](https://github.com/Srcodesalittle/Ignition-panel-to-wake-on-lan/blob/master/Back%20of%20panel.jpg)
The wired up nodemcu on a soldering breadboard ![wired up board](https://github.com/Srcodesalittle/Ignition-panel-to-wake-on-lan/blob/master/Wiring%20with%20esp.jpg)
and the battery pack with a push button micro usb cable. ![Battery pack+cable](https://github.com/Srcodesalittle/Ignition-panel-to-wake-on-lan/blob/master/battery%20pack.jpg)

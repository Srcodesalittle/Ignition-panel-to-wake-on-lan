# Ignition-panel-to-wake-on-lan
This project, is entirely to turn on a PC in a totally overkill fashion; nobody should make this, everybody should make this.
Before we get into any of these, I don't pretend to be an expert on any of this at all, so I might not know everything I'm talking about but I will try my best to give you correct information! I have a bunch of store links here, none of them are affiliate links or anything.

I take no ownership of the components in the code, I found them online and cobbled them together to make my program. Schematic file, bill of materials, and code are named appropriately.
The underlying logic is that a nodemcu ESP8266 module which is one of these ![nodemcu_esp8266](https://i0.wp.com/www.abstractotech.com/wp-content/uploads/2019/02/IMG_20190209_144805-2.jpg?resize=1024%2C773&ssl=1)


The underlying logic is to take inputs from an ignition panel and on the final switch press, send a wake on lan packet to startup the PC.
The ignition panel looks something like this ![igniton panel](https://i.imgur.com/HezUzRw.jpg)
![igniton panel2](https://i.imgur.com/JDkIAPW.jpg)
you get the idea. It's for race cars, all the better for our end use. 

**A note on the ignition module:** 

1. The ignition panel may claim to be 12V, but it's not strictly 12V. In my testing, none of the components need 12V to function, everything runs fine with just 3.3V, saving you the need for transistors and separate 12V power lanes.
2. The lights in the ignition panel are just bulbs, not LEDs so they do work with 3.3V. Better yet, feel free to snip these off and solder on LEDs for even brighter lights.
3. The panel I purchased came with wires, loop clips, and shrink wrap tubing to help with the soldering. Your results may vary.

## Stuff you need
These prices may vary but this is based on time of creation. 
[Bill of materials here](https://github.com/Srcodesalittle/Ignition-panel-to-wake-on-lan/blob/master/Bill%20of%20materials.xlsx)
I really don't want to make a table here, so bear with me and actually check out the excel file. 

**Points to note in the items**
1. You can get a soldering or plug and use soddering board depending on whether or not you want to solder everything together. If you do go the soldering route, you will need a soldering iron and some good solder. *Eye protection is probably a good idea as well*
2. You will need an ethernet cable to execute the wake on lan, this wont work over wifi, I have tried. So the length of cable you need will depend on your house setup. If it's unfeasible, consider a ![powerline adapter](https://www.amazon.com/TP-Link-AV600-Powerline-Ethernet-Adapter/dp/B00AWRUICG).

## The brains of the operation, the NodeMCU ESP8266 module:
An ESP8266 is an IOT module which is similar to an arduino except it comes built in with wireless networking capabilities and operates on a 3.3V logic level as opposed to the arduino's 5V. The nodemcu board is an ESP8266 module with pinouts, and a USB input for easier acces/powering abilities. (I'm sure there's more to the board but that's really all I know). There's no reason this project wont work with the new ESP32 module as well. 
I refer to "pins" throughout this guide, if you're unaware of what a pin is, it's the pokey bits that come out of the chipset. Seen here looking like metal teeth ![metal pins](https://cdn.instructables.com/FNA/7UD5/JH8JBT8A/FNA7UD5JH8JBT8A.LARGE.jpg)

## Powering the ignition panel
- The simplest way to power your box is using the microusb input on the board. This will take 5V and draw between 1-2A of current. So if you get any cheap powerbank you should be good to go. Plugging it into your PC's USB port while uploading code etc will also be enough to power the module and troublehsoot. 

- Another more complicated method uses the Vin+ground pins to supply power to your nodemcu board. This method allows you to supply a wide range of voltages, sometimes up to 12V. Refer to your product spec sheet for exact values. *However* providing a high voltage will make the board have to step it down to 3.3V since all the internal logic of the board operates at 3.3V. This will cause a lot of heat. You will also need to solder power to the Vin/Ground pins. 

- Since the ESP8266 needs to connect to your Wifi for this to work, it will chew through your battery very very quickly. That is why it's a good idea to incorporate a microUSB cable with a switch. So you can physically cut power to the ignition panel when you're not using it and can turn it on when needed.

- If you do go the route of using a battery, consider purchasing the always on battery that I have linked in the bill of materials. This is because most other battery packs will turn off rather quickly if they don't detect any power drawn. So you will have to open the whole box, turn on the battery pack, screw the box back on, and make your cool video for internet points. Alternatively you can incorporate a very small load that constantly draws power and thus keeps the battery on, but I didn't go that route.


## The wiring
Here's the glorious guts ![internal tangle of wiring](https://github.com/Srcodesalittle/Ignition-panel-to-wake-on-lan/blob/master/Back%20of%20panel.jpg)
The wired up nodemcu on a soldering breadboard ![wired up board](https://github.com/Srcodesalittle/Ignition-panel-to-wake-on-lan/blob/master/Wiring%20with%20esp.jpg)
and the battery pack with a push button micro usb cable. ![Battery pack+cable](https://github.com/Srcodesalittle/Ignition-panel-to-wake-on-lan/blob/master/battery%20pack.jpg)

Obviously there is very little you can get from those images, here is the circuit diagram for the project
![Circuit_diagram](https://github.com/Srcodesalittle/Ignition-panel-to-wake-on-lan/blob/master/Circtuit%20diagram.PNG)
**Schematic Legend**
1. SW1 Ignition_key is the first key turn.
2. SW2, SW3, SW4 are all the shielded toggle switches.
3. LED1, 2, 3 are the LEDs underneath the toggle switches
4. SW5 WOL_Button is the engine start button that functions to send the wake on lan signal.
5. WOL_LED is the LED that lights up inside the start engine button.
**Notes about wiring**
The wiring is such that the only way to send the wake on lan signal is to flip all the preceeding switches in that particular order. They are all in series and powered by the previous switch. All the switches are also pulled down to ground using a 10K resistor. All this means is, the output of the switch is wired to the ESP8266 *and* the ground connection. This forces the switch to stay off unless it is explicitly set to on. **If you don't have these resistors, then really weird shit will happen** where the switches wont know if they're on or not. The actual explanation is that, the esp8266 works on very low currents, so low that, the electromagnetic induction produced in the wires from just random radiowaves, hands waving, etc will generate enough current for the pins to register an input and think that the switch is active. To avoid all these, we run the output to ground through a resistor so that any tiny charges that develop in the wires are all fed to ground. However, when the switch is properly activated, the current will rather flow down the path of least resistance to the chip and register an actual *active* state than force its way through a heavy 10K resistor to reach ground.
All of that to say - use ground resistors on your switches. They will save you a lot of pain.

I chose these pins on the nodemcu board since not all pins are accessible for input output, some are reserved for clocks and such. Here is the pinout diagram
![pinout](https://cdn.instructables.com/FJC/J4AP/JH8JBTHA/FJCJ4APJH8JBTHA.LARGE.jpg)
and a [link](https://www.instructables.com/id/NodeMCU-ESP8266-Details-and-Pinout/) to the pins you can and cannot use. 

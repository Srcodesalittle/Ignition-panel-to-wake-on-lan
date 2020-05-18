# Guide to start your PC like the battlestation it is!
## [Video of functioning panel here](https://i.imgur.com/bVzd4uq.mp4).

## The sole purpose of this project to startup your PC in a totally overkill fashion; nobody should make this, everybody should make this.

Before we get into any of these, I don't pretend to be an expert on any of this at all, so I might not know everything I'm talking about but I will try my best to give you correct information! I have a bunch of store links here, none of them are affiliate links or anything.

I take no ownership of the components in the code, I found them online and cobbled them together to make my program. Schematic file, bill of materials, and code are named appropriately.
The underlying logic is that a nodemcu ESP8266 module which is one of these ![nodemcu_esp8266](https://i0.wp.com/www.abstractotech.com/wp-content/uploads/2019/02/IMG_20190209_144805-2.jpg?resize=1024%2C773&ssl=1)


takes inputs from an ignition panel and on the final switch press, send a wake on lan packet to startup the PC.

The ignition panel looks something like this ![igniton panel](https://i.imgur.com/JDkIAPW.jpg)
![igniton panel2](https://i.imgur.com/HezUzRw.jpg)
you get the idea. It's for race cars, all the better for our end use. 

**A note on the ignition module:** 

1. The ignition panel may claim to be 12V, but it's not strictly 12V. In my testing, none of the components need 12V to function, everything runs fine with just 3.3V, saving you the need for transistors and separate 12V power lanes.
2. The lights in the ignition panel are just bulbs, not LEDs so they do work with 3.3V. Better yet, feel free to snip these off and solder on LEDs for even brighter lights.
3. The panel I purchased came with wires, loop clips, and shrink wrap tubing to help with the soldering. Your results may vary.

## Stuff you need
These prices may vary but this is based on time of creation. 
[Bill of materials here](Final bill of materials.xlsx)
I really don't want to make a table here, so bear with me and actually check out the excel file. 

**Points to note in the items**
1. You can get a soldering or plug and use soddering board depending on whether or not you want to solder everything together. If you do go the soldering route, you will need a soldering iron and some good solder. *Eye protection is probably a good idea as well*
2. You will need an ethernet cable to execute the wake on lan, this wont work over wifi, I have tried. So the length of cable you need will depend on your house setup. If it's unfeasible, consider a [powerline adapter](https://www.amazon.com/TP-Link-AV600-Powerline-Ethernet-Adapter/dp/B00AWRUICG).

## The brains of the operation, the NodeMCU ESP8266 module:
An ESP8266 is an IOT module which is similar to an arduino except it comes built in with wireless networking capabilities and operates on a 3.3V logic level as opposed to the arduino's 5V. The nodemcu board is an ESP8266 module with pinouts, and a USB input for easier acces/powering abilities. (I'm sure there's more to the board but that's really all I know). 
I refer to "pins" throughout this guide, if you're unaware of what a pin is, it's the pokey bits that come out of the chipset. Seen here looking like metal teeth ![metal pins](https://cdn.instructables.com/FNA/7UD5/JH8JBT8A/FNA7UD5JH8JBT8A.LARGE.jpg)

They sit in the breadboard and allow access to the various input/outputs of the board. **If you're unsure of what a breadboard is**, give [this](https://www.youtube.com/watch?v=6WReFkfrUIk) a watch to learn how to use a breadboard. 
People much smarter than I have created all sorts of useful functions for the ESP8266 including the ability to connect to wifi, send wake on lan packets, and communicate with other ESP8266 devices. We will use these three functions in this project. There's no reason this project wont work with the new ESP32 module as well. 

## If this is on WiFi, how secure is it?:
A valid question. This code only creates a small server within your local network. It is not accessible by devices outside your network. It also, does not respond to any oustide inputs thus it is very secure. You can test this by noting the IP address (described a few steps down) and trying to ping it through an external computer like a work computer. It should not respond. Also, if you keep the device powered off untill necessary, there is nothing to even connect to. 

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

So go ahead and hook up your components as shown in the circuit diagram and we can move into programming this.

## Programming the ESP8266 in the Arduino IDE
To actually program the ESP8266 to do what we want, we need to write a program in a special environment known as the arduino IDE. This was initially designed for programming arduinos but the language is the same and it should work fine. 
1. Download the IDE [here](https://www.arduino.cc/en/Main/Software) Hit just download or download and donate if you're feeling generous. Install the IDE once download completes.
2. Next, open the application, you should see a blank window like this 
![Fresh IDE install](https://github.com/Srcodesalittle/Ignition-panel-to-wake-on-lan/blob/master/Arduino%20IDE.PNG)
3. Goto >> Files >> Preferences and paste following Link in "Additional board manager URLs http://arduino.esp8266.com/stable/package_esp8266com_index.json and hit OK. This tells the IDE that you are going to need specific instructions for the ESP8266.
4. Now Goto >> Tools >> Board >> Board Manager.
5. Scroll down to find ESP8266 and click on install. If not, type ESP8266 in the search bar and install ESP8266 by ESP8266 community. This will install the required files to begin working.
6. Finally, Tools >> Board >>Node MCU 1.0 (ESP -12E Module). You are now good to go. 
7. Plug in your nodemcu ESP8266 via USB and it should auto detect. This can be verified by navigating to Tools>>Port>> here you should see it on one of the coms. 
8. Feel free to go to File>>Examples>>Esp8266>>Blink to load up a basic sketch(name of programs in this IDE) that makes the onboard LED blink on and off. Hit the right arrow button next to the checkmark button that says "Upload". The sketch  should compile, upload, and the onboard LED should flash on and off. If you get an error saying could not open port, you chose the wrong port. Simply go to Tools>>Port>> and choose a different port and try uploading again. 
9. Bonafide ESP8266 programmer!

## The actual wake on lan code
I have attached the code here with the name WOL_LED_SecOTA.ino. Download this and open it up. It should open up in the arduino IDE. Don't upload just yet, you need to give it your credentials. The name  ThisWOL_LED_SecOTA refers to the three main functions executed by the code:

1. The Wake on LAN which powers on your PC.
2. The ability to control LEDs (or really whatever you want) by using a slave ESP8266 to respond to the switches being flipped. This is part of phase 2 of the project and will be updated in the future.
3. SecOTA - This is secure over the air updates. Since you're going to be sealing your module inside the project box, it might be a pain to actually wire into it every time you want to update your code. So this module just adds the ability to update code wirelessly. 
To implement the ability to update wirelessly, simply note the hostname and password you set and then upload the code. Once uploaded, reboot using the reset switch and now, even if you're powering your module using a battery pack, if you navigate to Tools>>Port>>  you should see the nodemcu module show up with the hostname you specified. Attempting to upload code will be met with a password prompt. **Make sure that whatever code you upload to this board in the future has the OTA code in it with the same hostname and password, if the OTA code is omitted, you will lose the ability to update over the air in the future.** 

### Adding libraries to the code:
The code for the most part is decently well commented and should be self explanatory. You will need to add two libraries to the IDE to enable all the required features. Do this by hitting Ctrl+Shift+I or navigate to Skethc>>Include libraries>> Manage libraries. 
The first library you need is arduinojson. Simply type arduinojson into library manager and click install.
![library manager](https://i.imgur.com/agAoYgD.png)
The second library you will need is the wake on lan library. **Do not search wake on lan in the manager, you will install the wrong library**. Instead, head [here](https://github.com/koenieee/WakeOnLan-ESP8266). Once you reach the github page, click on clone/download> then download as zip 
![download as zip](https://i.imgur.com/VX14qjN.png)

Save your zip file somewhere, there is no need to unzip this file. 
Next, navigate to Sketch>>Include library>> Add .Zip library
![Path](https://i.imgur.com/HO1eIOX.png).
This will bring up a dialog box, navigate to your .zip library and select it. This will add the library to your IDE.
Next, close the IDE and reopn the WOL_LED_SecOTA.ino file just to ensure all libraries are loaded correctly. Ensure your board is selected correctly as shown below
![Boards](https://i.imgur.com/ZuISm7J.png)

### Adding credentials to the code:
With our libraries installed, we are almost done. The code just needs credentials to connect to wifi and to receive over the air updates. 
**Wifi credentials:** Locate the sections shown below and replace "WIFI name here" with your 2.4Ghz wifi name. Make sure you retain the quotes. i.e. if your network name is Thebatcave, the code should look like 

``const char* ssid = "Thebatcave";`` Next, do the same with your wifi password. 
![wifi credentials](https://i.imgur.com/uXu9n4r.png)
Hit the upload button. If you have chosen the port correctly, it should compile and beging uploading. The first time you compile may take time, be patient. 
After uploading, your ESP8266 should be on the wifi. This can be tested by opening serial monitor: 
![monitor](https://i.imgur.com/jcNDv07.png).
Set your baud rate to 115200 from the dropdown and then hit the reset button on your nodemcu board. You should see (maybe) a few .6s and then you should see the message 

Connected to <your SSID>
IP address: <your IP address>

MDNS responder started
HTTP server started

If you dont see this message in the serial monitor, try hitting the reset button while the window is open, and if still no, check your SSID+password and firewall permissions. 
To visually ensure that the module has connected to wifi and is ready to go, the code instructs the panel to flash the LED1 two times when connection to wifi has been established. So once you build your whole module, when you turn it on, after a few seconds the LED1 should flash twice.

**OTA credentials:** More or less the same deal. Navigate to this section of the code:
![OTA section](https://i.imgur.com/khnjZPa.png)

and replace hostname with any name you want to give to your module and a memorable passowrd to update the module wirelessly in the future. Make sure you retain the "". Again, if you are updating code, ensure that the new code also has OTA enabled so that you don't lose this functionality. Ideally, you just want to keep adding/removing to the same code so you never risk removing OTA. One note though, serial monitor cannot be accessed if you are connecting purely wirelessly. Since there is no way of knowing if OTA is done updating, the code will flash LED1 on and off while over the air updates are happening. Wait till it finishes flashing before unplugging to avoid data corruption. 

## Enable wake on LAN on your PC:
The last step is to ensure that the PC is setup to receive wake on lan signals. This requires making changes at the BIOS level and in your ethernet adapter. There are multiple excellent guides, however, try to follow this
https://www.howtogeek.com/70374/how-to-geek-explains-what-is-wake-on-lan-and-how-do-i-enable-it/
and you should be successful more often than not. If your PC doesn't turn on, open up the IDE, connect your module, and fire up the serial monitor. For debug purposes, when you hit the wake on lan button, the serial monitor should print WOL_EXEC. Since this entire series is in circuit, if you are seeing this message when you press the button, it means the code is functioning as expected. The error lies in the PC's ability to receive and respond to the packets. Google your particular motherboard along with wake on lan not working and there might be MOBO specific changes that need to be made (due to powersaving implementations by different vendors).

## How it gets put together and functions:
Get a large enough project box, cut out an outline using a saw/power tools and insert the ignition panel. Scren in the panel using the provided screws. Make a hole in the side of the box and attach the button of the microUSB power cable into the hole. Connect the batter to the nodemcu via the microusb cable. This way, when you hit the button through the hole, the module will power up... saving the battery.
The order of operations is as follows:
1. On powerup - connect to wifi, denoted by LED1 blinking twice.
2. Turn the ignition key - LED1,2, and 3, should light up and turn off in a wave. This powers switch 1.
3. Turn on switch 1 - LED 1 should turn on and switch 2 is now powered.
4. Turn on switch 2 - LED 2 should turn on and switch 3 is now powered.
5. Turn on switch 3 - LED 3 and Start engine LEDs should turn on. Start engine aka Wake on lan button is now powered.
6. Press Start engine button - Send the wake on LAN signal and hopefully turn on your PC.

Good luck! I hope it works out for you as well!

## Future - what are these "This is the sending part comments"?
The future work of this project involves ESP now to communicate with a slave ESP8266 inside the PC case to run LEDs and or sounds in response to the switches. Any parts of the code that are commented with ``// this is the sending part for ESP now if necessary.`` are regions that will be activated to send packets to the slave. As of now, these are not executed.

## acknowledgements
1. [Jeremy blum](https://www.jeremyblum.com/) and his excellent [tutorial series](https://www.youtube.com/watch?v=fCxzA9_kg6s&list=PLA567CE235D39FA84) that helped me start with IoT.
2. [Rui Santos](https://makeradvisor.com/author/ruisantos/) for ESP8266 tutorials
3. [keoniee](https://github.com/koenieee) for wake on lan library and initial code.
4. [Good ol' google-fu](https://www.google.com/)
5. Ya'll for reading!

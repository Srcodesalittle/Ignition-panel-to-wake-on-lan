

/*********
  Rui Santos
  Complete project details at http://randomnerdtutorials.com
  adapted for WOL and LED. This is the sending module attached to the ignition panel.
  Refer to circuit diagram titled ignition panel.
*********/

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <WiFiClientSecure.h>
#include <WiFiUDP.h>
#include <espnow.h> 
#include <WakeOnLan.h>
#include <ArduinoJson.h>
#include <ArduinoOTA.h> // for OTA updates


MDNSResponder mdns;

// Replace with your network credentials
const char* ssid = "WIFI name here";
const char* password = "WIFI password here";

byte mac[] = {}; // your pc mac address here
//Defining switch pins//
int igntion_key = 14; // D5(gpio14)
int toggle_1 = 12; // D6(gpio12)
int toggle_2 = 13; // D7(gpio13)
int toggle_3 = 15; // D8(gpio15)
int WOL_button = 2; //D4(gpio2)
// LED for WOL will be created in hardware refer to schematic
int led1_gate = 5; // D1(gpio5)
int led2_gate = 4; // D2(gpio4)
int led3_gate = 0; // D3(gpio0)
int key_state = 0; // declare state for key to be used when executing the led series flash
int sw1_state = 0;// declare state for switch 1 to be used when executing the LED on and sending ESP now signal
int sw2_state = 0;// declare state for switch 1 to be used when executing the LED on and sending ESP now signal
int sw3_state = 0;// declare state for switch 1 to be used when executing the LED on and sending ESP now signal

WiFiUDP UDP;
/**
   This will brodcast the Magic packet on your entire network range.
*/
IPAddress computer_ip(255, 255, 255, 255);

void sendWOL();


ESP8266WebServer server(80);



// Callback when data is sent for LED control in tower only
void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) {
  Serial.print("Last Packet Send Status: ");
  if (sendStatus == 0) {
    Serial.println("Delivery success");
  }
  else {
    Serial.println("Delivery fail");

  }
}

String webPage = "";

void setup(void) {
  // pins and defitions
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  pinMode(igntion_key, INPUT);
  pinMode(toggle_1, INPUT);
  pinMode(toggle_2, INPUT);
  pinMode(toggle_3, INPUT);
  pinMode(WOL_button, INPUT);
  pinMode(led1_gate, OUTPUT);
  pinMode(led2_gate, OUTPUT);
  pinMode(led3_gate, OUTPUT);
  digitalWrite(led1_gate, LOW);
  digitalWrite(led2_gate, LOW);
  digitalWrite(led3_gate, LOW);
  digitalWrite(WOL_button, LOW);
  // Initialize WiFi
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println(WiFi.status());
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println("");
  digitalWrite(led1_gate, HIGH); // flash an LED just to know wifi connected properly.
  delay(200);
  digitalWrite(led1_gate, LOW);
  delay(200);
  digitalWrite(led1_gate, HIGH);
  delay(200);
  digitalWrite(led1_gate, LOW);
  delay(50);
  if (mdns.begin("esp8266", WiFi.localIP())) {
    Serial.println("MDNS responder started");
  }
  // Init ESP-NOW

  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  server.begin();
  Serial.println("HTTP server started");

  // OTA code
  ArduinoOTA.setHostname("HOSTNAMEHERE"); //Replace HOSTNAMEHERE with the name you want to give your module
  ArduinoOTA.setPassword((const char *)"PASSWORDHERE");//REPLACE PASSWORDHERE with whatever password you want for your OTA updates
  ArduinoOTA.begin();
  ArduinoOTA.onStart([]() {

  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    digitalWrite(led1_gate, HIGH); // flash an LED just to know OTA is progressing.
    delay(200);
    digitalWrite(led1_gate, LOW);
    delay(200); 
  });
  ArduinoOTA.onEnd([]() {

  });
}
// functions to be executed when switches are pressed
void ign_on()
{
  key_state = 1; // change state to indicate series flash has been called
  digitalWrite(led1_gate, HIGH);
  delay(50);
  digitalWrite(led2_gate, HIGH);
  delay(50);
  digitalWrite(led3_gate, HIGH);
  delay(500);
  digitalWrite(led3_gate, LOW);
  delay(50);
  digitalWrite(led2_gate, LOW);
  delay(50);
  digitalWrite(led1_gate, LOW);
  //esp_now_send(broadcastAddress, (uint8_t *) &sw1_send, sizeof(sw1_send)); // this is the sending part

}

void sw1_exec()
{
  sw1_state = 1; // record that switch has been turned on so that function executes only once.
  digitalWrite(led1_gate, HIGH);


  //esp_now_send(broadcastAddress, (uint8_t *) &sw2_send, sizeof(sw2_send)); // this is the sending part

}

void sw2_exec()
{
  sw2_state = 1; // record that switch has been turned on so that function executes only once.
  digitalWrite(led2_gate, HIGH);

  //esp_now_send(broadcastAddress, (uint8_t *) &sw3_send, sizeof(sw3_send)); // this is the sending part
}

void sw3_exec()
{
  sw3_state = 1;// record that switch has been turned on so that function executes only once.
  digitalWrite(led3_gate, HIGH);
  //esp_now_send(broadcastAddress, (uint8_t *) &sw4_send, sizeof(sw4_send)); // this is the sending part
}

void WOL_exec()
{
Serial.println("WOL_EXEC");
  WakeOnLan::sendWOL(computer_ip, UDP, mac, sizeof mac);// send WOL packet
  delay(10000);//add a delay just to avoid flooding the data packets
  //esp_now_send(broadcastAddress, (uint8_t *) &sw5_send, sizeof(sw5_send)); // this is the sending part for ESP now if necessary.

}
void loop(void) {
  server.handleClient();// fpr server stuff
  ArduinoOTA.handle();// to respond to OTA commands
  // This is the actual meat of the code
  if ((digitalRead(igntion_key) == 1) && key_state == 0) // If key is turned , flash the LEDS in series to look like initialization only once based on the key state
  {
    ign_on(); // function to run series flash

  }
  else if (digitalRead(igntion_key) == 0)
  {
    key_state = 0; // reset keystate to 0 when key is turned off
  }
  if ((digitalRead(toggle_1) == 1) &&  sw1_state == 0)// Check to make sure this is first time turning on switch_1. Then turn on just the first LED. Since all buttons will be held down at all times, this should give appearance of each turning on in series.
  {
    sw1_exec();// pass control off to fuction.

  }
  else if ((digitalRead(toggle_1) == 0))
  {
    sw1_state = 0; //record that switch is off so that it can be turned on again.
    digitalWrite(led1_gate, LOW);
  }

  if ((digitalRead(toggle_2) == 1) &&  sw2_state == 0) // Check to make sure this is first time turning on switch_2. Then turn on just the second LED
  {

    sw2_exec();// pass control off to fuction.


  }
  else if ((digitalRead(toggle_2) == 0))
  {
    sw2_state = 0; //record that switch is off so that it can be turned on again.
    digitalWrite(led2_gate, LOW);
  }
  if ((digitalRead(toggle_3) == 1) &&  sw3_state == 0) // Check to make sure this is first time turning on switch_3. Then turn on third LED
  {

    sw3_exec();// pass control off to fuction.


  }
  else if ((digitalRead(toggle_3) == 0))
  {
    sw3_state = 0; //record that switch is off so that it can be turned on again.
    digitalWrite(led3_gate, LOW);
  }

  if (digitalRead(WOL_button) == 1) // Check to make sure this is first time turning on WOL_button. Then send WOL
  {

    WOL_exec();

  }
  else if (digitalRead(WOL_button) == 0)
  {

  }


  else {
  }
}

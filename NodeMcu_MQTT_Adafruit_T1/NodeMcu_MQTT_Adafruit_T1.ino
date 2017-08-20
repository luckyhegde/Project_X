#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <Adafruit_NeoPixel.h>
//needed for library
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>          //https://github.com/tzapu/WiFiManager
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
//for LED status
#include <Ticker.h>
#define WLAN_SSID       "lucky"
#define WLAN_PASS    "laxmish53"
#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT 8883 // 8883 for MQTTS
#define AIO_USERNAME   "luckyhegde"
#define AIO_KEY          "b518ecfbe32f422aad8e1f2c0f795175"
// WiFiFlientSecure for SSL/TLS support
WiFiClientSecure client;

// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

// io.adafruit.com SHA1 fingerprint
const char* fingerprint = "26 96 1C 2A 51 07 FD 15 80 96 93 AE F7 32 CE B9 0D 01 55 C4";


/****************************** Feeds ***************************************/

// Setup a feed called 'photocell' for publishing.
// Notice MQTT paths for AIO follow the form: <username>/feeds/<feedname>
Adafruit_MQTT_Publish Motion_detect = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/Motion_detect");

// Setup a feed called 'onoff' for subscribing to changes.
Adafruit_MQTT_Subscribe onoffbutton = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/onoff");

/*************************** Sketch Code ************************************/
Ticker ticker;
#define PIN 15//D8 NodeMCU
// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      9
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
const char* ssid = "lucky";
const char* password = "laxmish53";

#define OLED_RESET LED_BUILTIN
Adafruit_SSD1306 display(OLED_RESET);
#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif
static int RED_LED = 12;  //D6
static int GREEN_LED = 13;  //D7
static int BLUE_LED = 14;  //D5
int button = 16;            //D0
int LED = LED_BUILTIN;
int buttonState = LOW;
int value = 1;
int DL_check;
const int sleepTimeS = 10;
int delayval = 500; // delay for half a second
int i=0;
int j=0;
int sensorValue;
// for some reason (only affects ESP8266, likely an arduino-builder bug).
void MQTT_connect();
void verifyFingerprint();

void tick()
{
  //toggle state
  int state = digitalRead(LED_BUILTIN);  // get the current state of GPIO1 pin
  digitalWrite(LED_BUILTIN, !state);     // set pin to the opposite state
}

//gets called when WiFiManager enters configuration mode
void configModeCallback (WiFiManager *myWiFiManager) {
  Serial.println("Entered config mode");
  Serial.println(WiFi.softAPIP());
  //if you used auto generated SSID, print it
  Serial.println(myWiFiManager->getConfigPortalSSID());
  //entered config mode, make led toggle faster
  ticker.attach(0.2, tick);
}

void setup() {
   pixels.begin();
  pixels.show(); // Initialize all pixels to 'off'
  Serial.begin(115200);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D (for the 128x64)
  // init done
  // Show image buffer on the display hardware.
  // Since the buffer is intialized with an Adafruit splashscreen
  // internally, this will display the splashscreen.
  display.display();
  delay(2000);
  // Clear the buffer.
  display.clearDisplay();
 
  // wait for serial monitor to open
  while(! Serial);
 
  // connect to io.adafruit.com
  Serial.println("Booting");
   // text display tests
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,10);
  display.println("Booting");
  display.display();
  delay(500);
  display.clearDisplay();
  /*WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);*/
  //set led pin as output
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(RED_LED, OUTPUT);pinMode(GREEN_LED, OUTPUT);pinMode(BLUE_LED, OUTPUT);
  // start ticker with 0.5 because we start in AP mode and try to connect
  ticker.attach(0.6, tick);

  //WiFiManager
  //Local intialization. Once its business is done, there is no need to keep it around
  WiFiManager wifiManager;
  //reset settings - for testing
  //wifiManager.resetSettings();

  //set callback that gets called when connecting to previous WiFi fails, and enters Access Point mode
  wifiManager.setAPCallback(configModeCallback);

  //fetches ssid and pass and tries to connect
  //if it does not connect it starts an access point with the specified name
  //here  "AutoConnectAP"
  //and goes into a blocking loop awaiting configuration
  if (!wifiManager.autoConnect()) {
    Serial.println("failed to connect and hit timeout");
    //reset and try again, or maybe put it to deep sleep
    ESP.reset();
    delay(1000);
  }

  //if you get here you have connected to the WiFi
  Serial.println("Connected to WiFi");
   display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,10);
  display.println("Connected to WiFi");
  display.display();
  delay(500);
  display.clearDisplay();
  ticker.detach();
  //keep LED on
  digitalWrite(LED_BUILTIN, LOW);
  // Port defaults to 8266
  // ArduinoOTA.setPort(8266);

  // Hostname defaults to esp8266-[ChipID]
  ArduinoOTA.setHostname("Esp8266_project_X");

  // No authentication by default
  // ArduinoOTA.setPassword((const char *)"123");

  ArduinoOTA.onStart([]() {
    Serial.println("Start");
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
  ArduinoOTA.begin();
  // we are connected
  // check the fingerprint of io.adafruit.com's SSL cert
void verifyFingerprint();
  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,10);
  display.println("IP address: ");
  display.println(WiFi.localIP());
  display.display();
  delay(2000);
  display.drawPixel(40, 40, WHITE);
  // Show the display buffer on the hardware.
  // NOTE: You _must_ call display after making any drawing commands
  // to make them visible on the display hardware!
  display.display();
  delay(1000);
  display.clearDisplay();
  
  // Setup MQTT subscription for onoff feed.
mqtt.subscribe(&onoffbutton);
}
uint32_t x=0;


void loop() {
  // put your main code here, to run repeatedly:
  ArduinoOTA.handle();
  // function definition further below.
  MQTT_connect();

  // this is our 'wait for incoming subscription packets' busy subloop
  // try to spend your time here

  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(1000))) {
    if (subscription == &onoffbutton) {
      Serial.print(F("Got_Input: "));
      Serial.println((char *)onoffbutton.lastread);
      display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,10);
  display.println(F("Got_Input: "));
  
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0,30);
  display.println("value:");
  display.println((char *)onoffbutton.lastread);
  display.display();
    delay(100);
  display.clearDisplay();
      for(int i=0;i<NUMPIXELS;i++){
  pixels.setPixelColor(i,255,0,255);
    pixels.show();
  }
    }
  }
// ping the server to keep the mqtt connection alive
  // NOT required if you are publishing once every KEEPALIVE seconds
   /* if(! mqtt.ping()) {
    mqtt.disconnect();
  }*/
  
  pixels.setBrightness(50);
//  pinMode(RED_LED, HIGH);pinMode(GREEN_LED, LOW);pinMode(BLUE_LED, HIGH);
//  delay(2000);
//  pinMode(RED_LED, LOW);pinMode(GREEN_LED, HIGH);pinMode(BLUE_LED, HIGH);
//  delay(2000);
//  pinMode(RED_LED, HIGH);pinMode(GREEN_LED, HIGH);pinMode(BLUE_LED, LOW);
sensorValue = analogRead(A0);
  DL_check = digitalRead(16);
  if(DL_check==1)
  {
//    pinMode(GREEN_LED, HIGH);
//    delay(500);
//      pinMode(BLUE_LED, LOW);
pixels.setPixelColor(0,255,0,0);
    delay(100);
  for(int i=0;i<NUMPIXELS;i++){
  pixels.setPixelColor(i,0,150,0);
    pixels.show();
  }
    live();
    DL_check = digitalRead(16);
  display.clearDisplay();
    }
  else
  {
   
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,10);
  display.println("Sensor Testing:");
  
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0,30);
  display.println("Sensor:");
  display.println(sensorValue);
  display.display();
    delay(100);
  display.clearDisplay();
  
  // apply the calibration to the sensor reading
  int sensorVal = map(sensorValue, 0, 1024, 0, 255);

  // in case the sensor value is outside the range seen during calibration
  sensorVal = constrain(sensorValue, 0, 255);
  for(int i=0;i<NUMPIXELS;i++){
    
  pixels.setPixelColor(i,sensorVal,sensorVal,255-sensorVal);
  delay(100);
    pixels.show();
  }
  }

  // Clear the buffer.
  display.clearDisplay();
}
void defconn()
{
     Serial.println(WiFi.status());
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setCursor(0, 10);
      display.println("WiFi status ");
      display.println(WiFi.status());
      display.display();
      delay(500);
      display.clearDisplay();
      if(WiFi.status() == WL_CONNECTED) {
    Serial.println("WiFi Connected.");
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setCursor(0, 10);
      display.println("WiFi Connected...");
      display.display();
      delay(500);
      display.clearDisplay();
      Serial.print("IP Address: ");
      Serial.println(WiFi.localIP());
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setCursor(0, 10);
      display.println("IP Address:");
      display.println(WiFi.localIP());
      display.display();
      delay(2000);
      display.clearDisplay();
      }
      else{
   WiFi.mode(WIFI_AP);
       WiFi.begin(ssid, password); // connecting to wifi
       }
}

void live()
{
sensorValue = analogRead(A0);

  // print out the value you read:
  Serial.println(sensorValue);
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0,10);
  display.println("Sensor:");
  display.println(sensorValue);
  display.display();
    delay(100);
    // Clear the buffer.
  display.clearDisplay();
 
    while(sensorValue>500)
    {
      digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                       // wait for a second
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);                       // wait for a second
      Serial.println("Sensor Value High");
       // Now we can publish stuff!
  Serial.print(F("\nSending Motion_sensor val "));
  display.setTextSize(1);
        display.setTextColor(WHITE);
        display.setCursor(0, 10);
   display.println("\nSending Motion_sensor val ");
        display.display();
        delay(1000);
        display.clearDisplay();
  Serial.print(sensorValue);
  Serial.print("...");
  if (! Motion_detect.publish(sensorValue)) {
    Serial.println(F("Failed"));
  } else {
    Serial.println(F("OK!"));
}
      if(i<9)
  {while (j<255)
{pixels.setBrightness(50); //The overall brightness of all the LEDs can be adjusted using setBrightness(). This takes a single argument, a number in the range 0 (off) to 255 (max brightness). For example, to set a pixels to 1/4 brightness:
pixels.setPixelColor(i, j,255-j, 255-j);
pixels.setPixelColor(i+1, j,255-j, 255-j);
pixels.setPixelColor(i+2, j,255-j, 255-j);
pixels.setPixelColor(i+3, j,255-j, 255-j);
pixels.setPixelColor(i+4, j,255-j, 255-j);
pixels.setPixelColor(i+5, j,255-j, 255-j);
pixels.setPixelColor(i+6, j,255-j, 255-j);
pixels.setPixelColor(i+7, j,255-j, 255-j);
j++;
 delay(100);
 pixels.show(); 
    }
     
  }
    else
{
  i=0;
  pixels.setPixelColor(i,255,255,255);
}
  
  // Sleep
  Serial.println("ESP8266 in sleep mode");
  display.setTextSize(1);
        display.setTextColor(WHITE);
        display.setCursor(0, 10);
   display.println("ESP8266 in sleep mode");
        display.display();
        delay(1000);
        display.clearDisplay();
        digitalWrite(LED_BUILTIN, HIGH); // LED turns Off
    ESP.deepSleep(sleepTimeS * 100);
    ESP.restart();
      
 }
}
// Function to connect and reconnect as necessary to the MQTT server.
// Should be called in the loop function and it will take care if connecting.
void MQTT_connect() {
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");
display.setTextSize(1);
        display.setTextColor(WHITE);
        display.setCursor(0, 10);
   display.println("Connecting to MQTT... ");
        display.display();
        delay(1000);
        display.clearDisplay();
  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
       Serial.println(mqtt.connectErrorString(ret));
       Serial.println("Retrying MQTT connection in 5 seconds...");
       mqtt.disconnect();
       delay(5000);  // wait 5 seconds
       retries--;
       if (retries == 0) {
         // basically die and wait for WDT to reset me
         while (1);
       }
  }
  Serial.println("MQTT Connected!");
display.setTextSize(1);
        display.setTextColor(WHITE);
        display.setCursor(0, 10);
   display.println("MQTT Connected!");
        display.display();
        delay(1000);
        display.clearDisplay();
}

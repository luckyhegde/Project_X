/*
 *  This sketch sends data via HTTP GET requests to IFTTT Maker channel service.
 *
 *  You need to  privateKey from the Maker channel on IFTTT and paste it
 *  below. 
 *
 * Based on the example code for WiFi client and button debounce example from Arduno IDE
 https://bigjungle.net/blog/2015/6/26/connect-maker-to-anything-ifttt
 */

#include <ESP8266WiFi.h>

void send_event(const char *event);

// constants won't change. They're used here to
// set pin numbers:

// Wifi setup
const char *ssid     = "WiFi_SSID";
const char *password = "Password";

// IFTTT setup
const char *host = "maker.ifttt.com";
const char *privateKey = "xxxxxxxxxxxxxxxxxxxxxx";

// Hardware setup
const int buttonPin = 2;     // the number of the pushbutton pin
const int ledPin = 0;        // the number of the LED pin

// Variables will change:
int buttonState;             // the current reading from the input pin
int lastButtonState = LOW;   // the previous reading from the input pin

// the following variables are long's because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
long lastDebounceTime = 0;  // the last time the output pin was toggled
long debounceDelay = 50;    // the debounce time; increase if the output flickers

void setup() 
{
  // Set your pin modes
  pinMode(buttonPin, INPUT);
  pinMode(ledPin, OUTPUT);
  
  // Bring up the serial for a bit of debugging
  Serial.begin(115200);
  delay(10);

  // We start by connecting to a WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  // Wait for the connection, flashing the LED while we wait
  int led = HIGH;  
  while (WiFi.status() != WL_CONNECTED) {
    delay(200);
    digitalWrite(ledPin, led);
    led = !led;
  }
  digitalWrite(ledPin, LOW);

  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() 
{
  // read the state of the switch into a local variable:
  int reading = digitalRead(buttonPin);

  // check to see if you just pressed the button
  // (i.e. the input went from LOW to HIGH),  and you've waited
  // long enough since the last press to ignore any noise:

  // If the switch changed, due to noise or pressing:
  if (reading != lastButtonState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) 
  {
    // whatever the reading is at, it's been there for longer
    // than the debounce delay, so take it as the actual current state:

    // if the button state has changed:
    if (reading != buttonState) 
    {
      Serial.print("Button now ");
      Serial.println(HIGH == reading ? "HIGH" : "LOW");
      buttonState = reading;

      // When the button is in the LOW state (pulled high) the button 
      // has been pressed so send the event.
      if (buttonState == LOW) {
        send_event("button_pressed");
      }
    }
  }

  // save the reading.  Next time through the loop,
  // it'll be the lastButtonState:
  lastButtonState = reading;
}

void send_event(const char *event)
{
  // set the LED on whle we are sending the event
  digitalWrite(ledPin, HIGH);

  Serial.print("Connecting to ");
  Serial.println(host);
  
  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("Connection failed");
    return;
  }
  
  // We now create a URI for the request
  String url = "/trigger/";
  url += event;
  url += "/with/key/";
  url += privateKey;
  
  Serial.print("Requesting URL: ");
  Serial.println(url);
  
  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");

  // Read all the lines of the reply from server and print them to Serial,
  // the connection will close when the server has sent all the data.
  while(client.connected())
  {
    if(client.available())
    {
      String line = client.readStringUntil('\r');
      Serial.print(line);
    } else {
      // No data yet, wait a bit
      delay(50);
    };
  }
  
  // All done
  Serial.println();
  Serial.println("closing connection");

  client.stop();
  
  // Finished sending the message, turn off the LED
  digitalWrite(ledPin, LOW);
}

/*
  This a simple example of the aREST UI Library for the ESP8266.
  See the README file for more details.

  Written in 2014-2016 by Marco Schwartz under a GPL license.
*/

// Import required libraries
#include <ESP8266WiFi.h>
#include <aREST.h>
#include <aREST_UI.h>
#include <Adafruit_NeoPixel.h>
// Create aREST instance
aREST_UI rest = aREST_UI();

// WiFi parameters
const char* ssid = "lucky";
const char* password = "laxmish53";
#define PIN 2
#define NUMPIXELS      16
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
// The port to listen for incoming TCP connections
#define LISTEN_PORT           80

// Create an instance of the server
WiFiServer server(LISTEN_PORT);

// Variables to be exposed to the API
int Analog;
int delayval = 500; // delay for half a second

int ledControl(String command);

void setup(void) {
    pixels.begin(); // This initializes the NeoPixel library.
    pixels.show();
  // Start Serial
  Serial.begin(115200);

  // Set the title
  rest.title("aREST Neo Demo");

  // Create button to control pin 5
  rest.slider(2);

  // Init variables and expose them to REST API
  Analog = analogRead(2);
  rest.variable("Analog", &Analog);

  // Labels
  rest.label("Analog");


  // Function to be exposed
  rest.function("led", ledControl);

  // Give name and ID to device
  rest.set_id("1");
  rest.set_name("esp8266");

  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  // Start the server
  server.begin();
  Serial.println("Server started");

  // Print the IP address
  Serial.println(WiFi.localIP());

}

void loop() {
  // Handle REST calls
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  while (!client.available()) {
    delay(1);
  }
  rest.handle(client);

}

int ledControl(String command) {
  // Print command
  Serial.println(command);

  // Get state from command
  int value = command.toInt();
  Analog = value;
  rest.variable("Analog", &Analog);
int pixelValue = map(value, 0, 1024, 0, 255);

  // in case the sensor value is outside the range seen during calibration
  pixelValue = constrain(pixelValue, 0, 255);
  for(int i=0;i<NUMPIXELS;i++){

    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
    pixels.setPixelColor(i, pixels.Color(pixelValue,255-pixelValue,255-pixelValue)); // Moderately bright green color.

    pixels.show(); // This sends the updated pixel color to the hardware.

    delay(delayval); // Delay for a period of time (in milliseconds).
  analogWrite(2, value);}
  return 1;
}

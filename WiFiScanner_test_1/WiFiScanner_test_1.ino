
#include <ESP8266WiFi.h>

const char* ssid     = "lucky_home";
const char* password = "laxmish53";

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
    pinMode(2, OUTPUT);
  Serial.begin(115200);
  delay(10);

  // We start by connecting to a WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
}
void loop()
{
  if (WiFi.status() != WL_CONNECTED)
     {
     Serial.println("scan start");
  // WiFi.scanNetworks will return the number of networks found
  int n = WiFi.scanNetworks();
  Serial.println("scan done");
  if (n == 0)
    Serial.println("no networks found");
  else
  {
    Serial.print(n);
    Serial.println(" networks found");
    for (int i = 0; i < n; ++i)
    {
      // Print SSID and RSSI for each network found
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));
      Serial.print(")");
      Serial.println((WiFi.encryptionType(i) == ENC_TYPE_NONE)?" ":"*");
      delay(10);
    }
  }
  Serial.println("");

}
     
   if (WiFi.status() == WL_CONNECT_FAILED)
  {
    for (int i=0;i<5;i++)
    {
      digitalWrite(2, LOW);   // Turn the LED on (Note that LOW is the voltage level
      delay(1000);                      // Wait for a second
      digitalWrite(2, HIGH);  // Turn the LED off by making the voltage HIGH3
      delay(2000);
  }
  }
  if(WiFi.status() == WL_CONNECTED)
  {
    digitalWrite(LED_BUILTIN, LOW);   // Turn the LED on (Note that LOW is the voltage level
    delay(500);
     digitalWrite(LED_BUILTIN, HIGH);  // Turn the LED off by making the voltage HIGH
      delay(500);
      digitalWrite(LED_BUILTIN, LOW);   // Turn the LED on (Note that LOW is the voltage level
   Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.print("You're connected to the network");
  Serial.print(ssid);
  }
  }
 

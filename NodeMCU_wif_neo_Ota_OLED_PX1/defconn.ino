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

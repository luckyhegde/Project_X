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
  

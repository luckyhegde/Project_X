void setup()
{
  pinMode(12, OUTPUT); 
  pinMode(13, OUTPUT); 
  pinMode(14, OUTPUT); 
  pinMode(A0, INPUT);
  Serial.begin(9600);
}

// the loop function runs over and over again forever
void loop() 
{
   int sV = analogRead(A0);
    Serial.println(sV);
  if (sV >0 && sV<800)
 { digitalWrite(12, LOW);   
  delay(500);                     
  digitalWrite(12, HIGH); 
  delay(500);
  digitalWrite(12, LOW);
  delay(500);
}
if (sV >800 && sV<1000)
 { digitalWrite(13, LOW);   
  delay(500);                     
  digitalWrite(13, HIGH); 
  delay(500);
  digitalWrite(13, LOW);
  delay(500);
}
 /* digitalWrite(14, LOW);   
  delay(500);                     
  digitalWrite(14, HIGH); 
  delay(500);
  digitalWrite(14, LOW);
  delay(500);*/
}


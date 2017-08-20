void setup()
{
  pinMode(12, OUTPUT); 
  pinMode(13, OUTPUT); 
  pinMode(14, OUTPUT); 
}

// the loop function runs over and over again forever
void loop() 
{
  digitalWrite(12, LOW);   
  delay(500);                     
  digitalWrite(12, HIGH); 
  delay(500);
  digitalWrite(12, LOW);
  delay(500);
  digitalWrite(13, LOW);   
  delay(500);                     
  digitalWrite(13, HIGH); 
  delay(500);
  digitalWrite(13, LOW);
  delay(500);
  digitalWrite(14, LOW);   
  delay(500);                     
  digitalWrite(14, HIGH); 
  delay(500);
  digitalWrite(14, LOW);
  delay(500);
  }


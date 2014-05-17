
void setup()
{
  for(int i =1 ; i < 10; i++)
    pinMode(i, OUTPUT);   // sets the pin as output


}

void loop()
{
  digitalWrite(1,1);
  digitalWrite(2,0);
  analogWrite(3, 155);
  digitalWrite(4,0);  // analogRead values go from 0 to 1023, analogWrite values from 0 to 255
  analogWrite(5, 255);
  analogWrite(6, 155); 
  digitalWrite(7,1);
  analogWrite(9, 155);

}



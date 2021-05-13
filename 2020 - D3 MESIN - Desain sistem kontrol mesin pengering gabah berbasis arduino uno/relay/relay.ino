void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  delay(1000);

  /*_____________GPIO_____________*/
  pinMode(5, OUTPUT);                
  /*_____________GPIO_____________*/

}

void loop() {
  // put your main code here, to run repeatedly:

  digitalWrite(5,LOW);
  Serial.print("GPIO OFF");
  delay(5000);
  digitalWrite(5,HIGH);
  Serial.print("GPIO ON");
  delay(5000);
  
  
}

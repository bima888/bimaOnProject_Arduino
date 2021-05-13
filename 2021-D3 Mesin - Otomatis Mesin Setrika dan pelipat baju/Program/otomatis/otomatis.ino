int data  = 0;
int data1 = 0;
void setup() {
  Serial.begin(9600);
  pinMode(6,INPUT_PULLUP);
  pinMode(7,INPUT_PULLUP);
  pinMode(12,INPUT_PULLUP);
  pinMode(13,INPUT_PULLUP);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(A0, OUTPUT);
  pinMode(A1, OUTPUT);
  digitalWrite(10, HIGH);
  digitalWrite(11, LOW);
  digitalWrite(A0, HIGH);
  digitalWrite(A1, HIGH);
  data = 0;
}

void loop() {
  Serial.print("data = ");
  delay(500);
  Serial.println(data);
  Serial.print("data 1 = ");
  delay(500);
  Serial.println(data1);

   if (digitalRead(6)==LOW)
  {
    digitalWrite(11, HIGH);
    }

if(digitalRead(12)==LOW)
{
  if (digitalRead(6)==LOW)
  {
    digitalWrite(10, LOW);
    digitalWrite(11, HIGH);
    data=1;
    }
  if (digitalRead(7)==LOW)
  {
    digitalWrite(10, HIGH);
    digitalWrite(11,LOW);
    data=2;
    }
  if (data == 2)
   {
    data1 = 1;
    }
   if (data1 == 1 && data == 1)
    {
      data  = 0;
      data1 = 0;
    digitalWrite(10,HIGH);
    digitalWrite(11,HIGH);
    delay(500);
    
    digitalWrite(A0, LOW);
    delay(1000);
    digitalWrite(A0, HIGH);
  
    digitalWrite(A1, LOW);
    delay(1000);
    digitalWrite(A1, HIGH);
    }
  
}
else if (digitalRead(13)==HIGH)
{
  digitalWrite(10, HIGH);
  digitalWrite(11, HIGH);
  digitalWrite(A0, HIGH);
  digitalWrite(A1, HIGH);
  }
  delay(2000);
}

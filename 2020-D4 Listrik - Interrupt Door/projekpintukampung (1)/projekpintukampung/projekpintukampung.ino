//A infrared, B PIR sensor ,jika  B detect maka OUT aktif, A detect OUT Nonaktif
//selanjutnya jika A detect OUT nonaktif.

int OUT = 6;
int PIR = 2;  
int IRED = 3;         
int data = 0; //berisi data PIR sensor
int status = LOW;

void setup() {

  pinMode(PIR, INPUT);
  pinMode(IRED, INPUT_PULLUP); 
  pinMode(OUT, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(3),RED,CHANGE); 
  digitalWrite(OUT, HIGH);
  Serial.begin(9600);  
}

void loop(){
  
  data = digitalRead(PIR); 
  //kondisi 1
  if (data == HIGH) 
  {     
    if (status == LOW)
    {
        Serial.println("OUTPUT OFF");
        status = HIGH; //interlock ON PIR
        digitalWrite(OUT, LOW);
        Serial.println("kondisi B");
        Serial.print("data = "); 
        Serial.println(data); 
        Serial.println("status HIGH");
        Serial.println("OUTPUT ON");
        delay(500);
      }
    } 

    else 
    {
      if(status == HIGH)
      {
        delay(500);
        status = LOW; //interlock OFF PIR
        Serial.println("kondisi B");
        Serial.print("data = "); 
        Serial.println(data);
        Serial.println("status LOW");
      } 
    }         
    
  //kondisi 2     
   
}


void RED ()
{
   status = LOW; //interlock OFF PIR
   Serial.println("kondisi A");
   Serial.print("data = "); 
   Serial.println(data);
   digitalWrite(OUT, HIGH);
   Serial.println("status LOW");
   Serial.println("OUTPUT OFF");
   
  }

int PIR1 = 2;
int PIR2 = 3;              
int data1 = 0;     
int data2 = 0;
int status1 = LOW;
int status2 = LOW;

void setup() {

  pinMode(PIR1, INPUT);    
  pinMode(PIR2, INPUT);
  pinMode(6, OUTPUT);
  //pinMode(7, OUTPUT); //spare jika butuh

  digitalWrite(6, LOW);
  Serial.begin(9600);  
}

void loop(){
  data1 = digitalRead(PIR1);
  data2 = digitalRead(PIR2);
  
  //kondisi 1
  if (data1 == HIGH) 
  {     
    if (status1 == LOW)
    {
        Serial.println("OUTPUT OFF");
        delay(4000);
        status1 = HIGH;
        digitalWrite(6, HIGH);
        Serial.println("kondisi B");
        Serial.print("data 1= "); 
        Serial.println(data1);
        Serial.print("data 2= ");
        Serial.println(data2); 
        delay(500);
       
      }
    } 

    else 
    {
      
      if(status1 == HIGH)
      {
        delay(500);
        status1 = LOW;
        Serial.println("kondisi B");
        Serial.print("data 1= "); 
        Serial.println(data1);
        Serial.print("data 2= ");
        Serial.println(data2);
        delay(500);         
      } 
    }         
  //kondisi 2
    if (data2 == HIGH) 
    { 
      
      if (status2 == LOW)
      {
        
        Serial.println("OUTPUT ON");
        delay(500);    
        status2 = HIGH;
        status1 = LOW;
        delay(2000);
        digitalWrite(6, LOW);
        Serial.println("kondisi A");
        Serial.print("data 1= "); 
        Serial.println(data1);
        Serial.print("data 2= ");
        Serial.println(data2); 
        delay(500);       
      }
    }
   else 
   {
      
      if(status2 == HIGH)
      {
        delay(500);
        status2 = LOW;
        Serial.println("kondisi A");
        Serial.print("data 1= "); 
        Serial.println(data1);
        Serial.print("data 2= ");
        Serial.println(data2);
        delay(500);         
       }          
    }

     
}

#include <Keypad_I2C.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>
#include <Wire.h>

#define DHTPIN 2
#define DHTTYPE DHT11
#define buzzer 13
#define ledON 12
#define ledOFF 11

/*PID*/
float set_temperature = 0;          
float temperature_read = 0.0;
float humidty_read = 0.0;
float PID_error = 0;
float previous_error = 0;
float elapsedTime, Time, timePrev;
float PID_value = 0;
float PID_value_hum = 0; 
float last_set_temperature = 0;

int PID_p = 0;    int PID_i = 0;    int PID_d = 0;
/*PID*/

int start_status = 0;
float SP_temp = 0;
int SP_hum = 0;
int SP_Kp = 0;
int SP_Ki = 0;
int SP_Kd = 0;
int data = 0;
int data1 = 0;
int celcius_1 = 0;
int humidity_1 = 0;
double output;
double input;


const byte ROWS = 4; 
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {0, 1, 2, 3}; 
byte colPins[COLS] = {4, 5, 6, 7}; 
Keypad_I2C keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS, 0x20, PCF8574 );
LiquidCrystal_I2C lcd(0x27,16,2);

char stringAngka[17];
int indexKeypad = 0;
DHT dht(DHTPIN,DHTTYPE);

void(*reset_all) (void) = 0;

void menu1 ()
{
  digitalWrite(ledOFF, HIGH);
 //Pembacaan untuk data kelembaban
  humidity_1 = dht.readHumidity();
  //Pembacaan dalam format celcius (c)
  celcius_1 = dht.readTemperature();
 lcd.clear();
 lcd.setCursor(0,0);
 lcd.print("Fermentasi Tempe");
 lcd.setCursor(0,1);
 lcd.print("T:");
 lcd.setCursor(2,1);
 lcd.print(celcius_1);
 lcd.setCursor(4,1);
 lcd.print("/");
 lcd.setCursor(5,1);
 lcd.print(SP_temp);
 lcd.setCursor(8,1);
 lcd.print("H:");
 lcd.setCursor(10,1);
 lcd.print(humidity_1);
 lcd.setCursor(12,1);
 lcd.print("/");
 lcd.setCursor(13,1);
 lcd.print(SP_hum);
 
}

void menustart()
{         
  //Pembacaan untuk data kelembaban
  humidity_1 = dht.readHumidity();
  //Pembacaan dalam format celcius (c)
  celcius_1 = dht.readTemperature();
 lcd.clear();
 lcd.setCursor(3,0);
 lcd.print("SISTEM ON");
 lcd.setCursor(0,1);
 lcd.print("T:");
 lcd.setCursor(2,1);
 lcd.print(celcius_1);
 lcd.setCursor(4,1);
 lcd.print("/");
 lcd.setCursor(5,1);
 lcd.print(SP_temp);
 lcd.setCursor(8,1);
 lcd.print("H:");
 lcd.setCursor(10,1);
 lcd.print(humidity_1);
 lcd.setCursor(12,1);
 lcd.print("/");
 lcd.setCursor(13,1);
 lcd.print(SP_hum);
}

int getTemp()
{
  int valtemp = 0;
  char key = keypad.getKey();   
  while(key != 'C')
   {
      dht.begin();
      //Pembacaan untuk data kelembaban
  humidity_1 = dht.readHumidity();
  //Pembacaan dalam format celcius (c)
  celcius_1 = dht.readTemperature();
      switch (key)
      {
         case NO_KEY:break;
         case '0': case '1': case '2': case '3': case '4':
         case '5': case '6': case '7': case '8': case '9':
          if (!indexKeypad)
          {
          lcd.setCursor(1,1);
          lcd.blink();
          }
          stringAngka[indexKeypad++] = key;
          lcd.print(key);
          //num = key - '0';
          valtemp = valtemp * 10 + (key - '0');
          digitalWrite(buzzer, HIGH);
          delay(100);
          digitalWrite(buzzer, LOW);
          break;
         case 'A':
          digitalWrite(buzzer, HIGH);
          delay(100);
          digitalWrite(buzzer, LOW);
          data = data+1;
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Set Temperrature");
          lcd.setCursor(0,1);
          lcd.print(":");
          lcd.setCursor(1,1);
          lcd.blink();
          break; 
         case '*':
         menu1();
//          lcd.clear();
//          lcd.setCursor(0,0);
//          lcd.print("Set Temperrature");
//          lcd.setCursor(0,1);
//          lcd.print(":");
//          lcd.setCursor(1,1);
//          lcd.blink();
          valtemp = 0;
         case 'D':;
//          lcd.clear();
//          lcd.setCursor(0,0);
//          lcd.print("Fermentasi Tempe");
//          lcd.setCursor(0,1);
//          lcd.print("T: 1");
//          lcd.setCursor(8,1);
//          lcd.print("H:");
            

      }
      key = keypad.getKey();
   }
   return valtemp;
}


int getHum()
{
  if(data >= 1)
         {
          data = 0;
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Set Humidity");
          lcd.setCursor(0,1);
          lcd.print(":");
         }
  int valhum = 0;
  char key = keypad.getKey();
  while(key != 'C')
   {
      switch (key)
      {
         case NO_KEY:break;
         case '0': case '1': case '2': case '3': case '4':
         case '5': case '6': case '7': case '8': case '9':
          if (!indexKeypad)
          {
          lcd.setCursor(1,1);
          lcd.blink();
          }
          stringAngka[indexKeypad++] = key;
          lcd.print(key);
          //num = key - '0';
          valhum = valhum * 10 + (key - '0');
          digitalWrite(buzzer, HIGH);
          delay(100);
          digitalWrite(buzzer, LOW);
          break;
         case '*': 
         menu1();
//          lcd.clear();
//          lcd.setCursor(0,0);
//          lcd.print("Set Humidity");
//          lcd.setCursor(0,1);
//          lcd.print(":");
//          lcd.setCursor(1,1);
//          lcd.blink();
          valhum = 0;
         case 'D':;
//          lcd.clear();
//          lcd.setCursor(0,0);
//          lcd.print("Fermentasi Tempe");
//          lcd.setCursor(0,1);
//          lcd.print("T: 2");
//          lcd.setCursor(8,1);
//          lcd.print("H:");
//          break;  

      }
      key = keypad.getKey();
   }
   return valhum;
}

int getKp()
{
  int valKp = 0;
  char key = keypad.getKey();   
  while(key != 'C')
   {
      switch (key)
      {
         case NO_KEY:break;
         case '0': case '1': case '2': case '3': case '4':
         case '5': case '6': case '7': case '8': case '9':
          if (!indexKeypad)
          {
          lcd.setCursor(1,1);
          lcd.blink();
          }
          stringAngka[indexKeypad++] = key;
          lcd.print(key);
          //num = key - '0';
          valKp = valKp * 10 + (key - '0');
          digitalWrite(buzzer, HIGH);
          delay(100);
          digitalWrite(buzzer, LOW);
          break;
         case 'B':
          digitalWrite(buzzer, HIGH);
          delay(100);
          digitalWrite(buzzer, LOW);
          data1 = data1+1;
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Set Kp, Ki, Kd");
          delay(2000);
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Set Kp");
          lcd.setCursor(0,1);
          lcd.print(":");
          lcd.setCursor(1,1);
          lcd.blink();
          break; 
         case '*': 
         menu1();
//          lcd.clear();
//          lcd.setCursor(0,0);
//          lcd.print("Set Kp");
//          lcd.setCursor(0,1);
//          lcd.print(":");
//          lcd.setCursor(1,1);
//          lcd.blink();
          valKp = 0;

      }
      key = keypad.getKey();
   }
   return valKp;
}

int getKi() 
{
  if(data1 == 1)
         {
          data1 = data1+1;
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Set Ki");
          lcd.setCursor(0,1);
          lcd.print(":");
          lcd.setCursor(1,1);
          lcd.blink(); 
         }
  int valKi = 0;
  char key = keypad.getKey();
  while(key != 'C')
   {
      switch (key)
      {
         case NO_KEY:break;
         case '0': case '1': case '2': case '3': case '4':
         case '5': case '6': case '7': case '8': case '9':
          if (!indexKeypad)
          {
          lcd.setCursor(1,1);
          lcd.blink();
          }
          stringAngka[indexKeypad++] = key;
          lcd.print(key);
          //num = key - '0';
          valKi = valKi * 10 + (key - '0');
          digitalWrite(buzzer, HIGH);
          delay(100);
          digitalWrite(buzzer, LOW);
          break;
         case '*':
         menu1();
//          lcd.clear();
//          lcd.setCursor(0,0);
//          lcd.print("Set Ki");
//          lcd.setCursor(0,1);
//          lcd.print(":");
//          lcd.setCursor(1,1);
//          lcd.blink();
          valKi = 0;

      }
      key = keypad.getKey();
   }
   return valKi;
}

int getKd()
{
  if(data1 == 2)
         {
          data1 = 0;
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Set Kd");
          lcd.setCursor(0,1);
          lcd.print(":");
          lcd.setCursor(1,1);
          lcd.blink(); 
         }
  int valKd = 0;
  char key = keypad.getKey();
  while(key != 'C')
   {
      switch (key)
      {
         case NO_KEY:break;
         case '0': case '1': case '2': case '3': case '4':
         case '5': case '6': case '7': case '8': case '9':
          if (!indexKeypad)
          {
          lcd.setCursor(1,1);
          lcd.blink();
          }
          stringAngka[indexKeypad++] = key;
          lcd.print(key);
          //num = key - '0';
          valKd = valKd * 10 + (key - '0');
          digitalWrite(buzzer, HIGH);
          delay(100);
          digitalWrite(buzzer, LOW);
          break;
         case '*': 
         menu1();
//          lcd.clear();
//          lcd.setCursor(0,0);
//          lcd.print("Set Kd");
//          lcd.setCursor(0,1);
//          lcd.print(":");
//          lcd.setCursor(1,1);
//          lcd.blink();
          valKd = 0;

      }
      key = keypad.getKey();
   }
   return valKd;
}

int getstart() {
  if (data1 = 3)
  {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("D Menu For Start");
    lcd.setCursor(6,1);
    lcd.print(">>>");
  }
int start = 0;
char key = keypad.getKey();
while(key != '#')
{
  
  if (key == 'D')
  {
    start = 1;
    menu1();
           digitalWrite(buzzer, HIGH);
          delay(100);
          digitalWrite(buzzer, LOW);
           digitalWrite(buzzer, HIGH);
          delay(100);
          digitalWrite(buzzer, LOW);
  }
  key = keypad.getKey();
}
return start;
}


void setup()
{
 Serial.begin(9600); 
 lcd.init();
 lcd.backlight();
 keypad.begin();
 dht.begin();
 
 delay(200);
 pinMode(ledON, OUTPUT);
 pinMode(ledOFF, OUTPUT);
 pinMode(buzzer, OUTPUT);
 pinMode(6 , OUTPUT);
 pinMode(A3, OUTPUT);
 pinMode(DHTPIN, INPUT);
 digitalWrite(DHTPIN, HIGH);

  //Pembacaan untuk data kelembaban
  humidity_1 = dht.readHumidity();
  //Pembacaan dalam format celcius (c)
  celcius_1 = dht.readTemperature();
 menu1();
}

void loop()
{
   menu1();
   analogWrite(6,55);
   SP_temp = getTemp();
   delay(500);
   lcd.setCursor(0,1);
   lcd.print("set T");
   lcd.setCursor(6,1);
   lcd.print(SP_temp);
          digitalWrite(buzzer, HIGH);
          delay(100);
          digitalWrite(buzzer, LOW);
   delay(1000);
   Serial.print("temp : ");
   Serial.println(SP_temp);

   SP_hum = getHum();
   lcd.setCursor(0,1);
   lcd.print("set H");
   lcd.setCursor(6,1);
   lcd.print(SP_hum);
   delay(1000);
   lcd.clear();
   lcd.setCursor(3,0);
   lcd.print("Temp & Hum");
   lcd.setCursor(4,1);
   lcd.print("all Set.");
          digitalWrite(buzzer, HIGH);
          delay(100);
          digitalWrite(buzzer, LOW);
           digitalWrite(buzzer, HIGH);
          delay(100);
          digitalWrite(buzzer, LOW);
           digitalWrite(buzzer, HIGH);
          delay(100);
          digitalWrite(buzzer, LOW);
   Serial.print("hum : ");
   Serial.println(SP_hum);

   SP_Kp = getKp();
   lcd.setCursor(0,1);
   lcd.print("set Kp");
   lcd.setCursor(7,1);
   lcd.print(SP_Kp);
   delay(800);
//          digitalWrite(buzzer, HIGH);
//          delay(100);
//          digitalWrite(buzzer, LOW);
   Serial.print("Kp : ");
   Serial.println(SP_Kp);   

   SP_Ki = getKi();
   lcd.setCursor(0,1);
   lcd.print("set Ki");
   lcd.setCursor(7,1);
   lcd.print(SP_Ki);
   delay(800);
//          digitalWrite(buzzer, HIGH);
//          delay(100);
//          digitalWrite(buzzer, LOW);
   Serial.print("Ki : ");
   Serial.println(SP_Ki);

   SP_Kd = getKd();
   lcd.setCursor(0,1);
   lcd.print("set Kd");
   lcd.setCursor(7,1);
   lcd.print(SP_Kd);
   delay(800);
//          digitalWrite(buzzer, HIGH);
//          delay(100);
//          digitalWrite(buzzer, LOW);
   Serial.print("Kd : ");
   Serial.println(SP_Kd);

   start_status = getstart();
   
   delay(800); 
   Serial.print("temp : ");
   Serial.println(SP_temp);
   delay(80);
   Serial.print("hum : ");
   Serial.println(SP_hum);
   delay(80);
   Serial.print("Kp : ");
   Serial.println(SP_Kp);
   delay(80); 
   Serial.print("Ki : ");
   Serial.println(SP_Ki);
   delay(80);
   Serial.print("Kd : ");
   Serial.println(SP_Kd);
   analogWrite(6,55);
   
   char key = keypad.getKey();
   while(SP_temp>5 && SP_hum>5 && start_status == 1)
   {
    delay(800);
    menustart();
    digitalWrite(ledON, HIGH);
    digitalWrite(ledOFF, LOW);
    Serial.print("Looping Monitor");
    char key = keypad.getKey();
    

    temperature_read = dht.readTemperature();
    PID_error = SP_temp - temperature_read + 3;
    PID_p = 0.01*SP_Kp * PID_error;
    PID_i = 0.01*PID_i + (SP_Ki * PID_error);
    timePrev = Time;
    Time = millis();
    elapsedTime = (Time - timePrev) / 1000;
    PID_d = 0.01*SP_Kd*((PID_error - previous_error)/elapsedTime);
    PID_value = PID_p + PID_i + PID_d; 

    Serial.print(celcius_1);
    Serial.print(" ");
    Serial.println(PID_value);
    Serial.print(" ");  
    Serial.println(SP_temp);
    Serial.print(" ");

    humidty_read = dht.readHumidity();
    PID_error = SP_hum - humidty_read + 3;
    PID_p = 0.01*SP_Kp * PID_error;
    PID_i = 0.01*PID_i + (SP_Ki * PID_error);
    timePrev = Time;
    Time = millis();
    elapsedTime = (Time - timePrev) / 1000;
    PID_d = 0.01*SP_Kd*((PID_error - previous_error)/elapsedTime);
    PID_value_hum = PID_p + PID_i + PID_d; 
    

    Serial.print(humidity_1);
    Serial.print(" ");
    Serial.println(PID_value_hum);
    Serial.print(" ");  
    Serial.println(SP_hum);
    Serial.print(" ");
    
    
   if( SP_temp > temperature_read)
   {
    if(PID_value < 0)
    {    PID_value = 0;    }
    if(PID_value > 255)  
    {    PID_value = 180;  }

    //analogWrite(6,255-PID_value); //Output PID Temperature di PIN 6 ke MOSFET (INVERTING OUTPUT)
    analogWrite(6,PID_value);
    previous_error = PID_error;
   }

   if( SP_temp < temperature_read)
   {
    if(PID_value < 0)
    {    PID_value = 0;    }
    if(PID_value > 255)  
    {    PID_value = 180;  }
    
    analogWrite(6,PID_value-180);
    previous_error = PID_error;
   }
   
    if(SP_hum < humidty_read)
    {
      digitalWrite(A3, LOW);
      //delay(1000);
    }
    if(SP_hum >= humidty_read)
    {
      digitalWrite(A3, HIGH);
    }
    
    if(celcius_1 == SP_temp)
    {
      digitalWrite(buzzer, LOW);
    }
    else {
          digitalWrite(buzzer, HIGH);
          delay(100);
          digitalWrite(buzzer, LOW);
    }
    
    if (key == '*')
    {
      SP_temp = 0;
      SP_hum = 0;
      SP_Kp = 0;
      SP_Ki = 0;
      SP_Kd = 0;
      start_status = 0;
      reset_all();
      delay(1000);
      PID_value = 0;
      analogWrite(6,55);
      menu1();
    }
    
   }
   
}

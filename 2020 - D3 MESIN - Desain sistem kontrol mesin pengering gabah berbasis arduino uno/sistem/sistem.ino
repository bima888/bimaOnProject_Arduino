/*______HEADER______*/
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
//#include <Keypad.h>
#include <Keypad_I2C.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#define ONE_WIRE_BUS 12
/*______HEADER______*/

/*______MAPPING SUHU______*/
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
/*______MAPPING SUHU______*/

/*______MAPPING KEYPAD______*/ 
const byte rows = 4;
const byte cols = 4;
char keys[rows][cols] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte rowPins[rows] = {0,1,2,3};
byte colPins[cols] = {4,5,6,7};

Keypad_I2C keypad(makeKeymap(keys), rowPins, colPins, rows, cols,0x20,PCF8574);
LiquidCrystal_I2C lcd(0x27,16,2);  //LCD address 0x27
/*______MAPPING KEYPAD______*/

/*______TIPE DATA______*/ 
int MIN = 0;
int MAX = 0;
int pemantik = 0;
int durasi = 0;
char stringAngka[17];
int indexKeypad = 0;
int Celcius=0;
int Fahrenheit=0;
/*______TIPE DATA______*/ 

/*______MOTOR______*/
//const int selenoid = A0;
//const int pemantik = A1;
//const int motorAC = A2;
//const int blower = A3;
//const int motor1 = A4;
/*______MOTOR______*/


int GetNumber()
{
   int num = 0;
   char key = keypad.getKey();
   while(key != '#')
   {
      switch (key)
      {
         case NO_KEY:
            break;

         case '0': case '1': case '2': case '3': case '4':
         case '5': case '6': case '7': case '8': case '9':
          if (!indexKeypad)
          {
          lcd.setCursor(8,1);
          }
          stringAngka[indexKeypad++] = key;
               
    
            lcd.print(key);
            //num = key - '0';
            num = num * 10 + (key - '0');
            break;

         case '*':
            num = 0;
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("SET NILAI SUHU");
            lcd.setCursor(0,1);
            lcd.print("Nilai :");
            break;
         case 'A':
            lcd.clear();
            for(int i=0;i<=10;i++)
            {
            bacasensor();
            delay(500);
            lcd.setCursor(0,0);
            lcd.print("Status suhu : ");
            lcd.setCursor(0,1);
            lcd.print(Celcius);
            lcd.setCursor(3,1);
            lcd.print("C");
            lcd.setCursor(5,1);
            lcd.print(Fahrenheit);
            lcd.setCursor(8,1);
            lcd.print("F");
            lcd.setCursor(10,1);
            lcd.print("Cy:");
            lcd.setCursor(14,1);
            lcd.print(i);
            
            if (Celcius > 50 && Fahrenheit > 50)
            {
              lcd.setCursor(0,1);
              lcd.print("value hunting");}
              else
             {
              lcd.setCursor(0,0);
              lcd.print("Status suhu  : ");
              lcd.setCursor(0,1);
              lcd.print(Celcius);
              lcd.setCursor(3,1);
              lcd.print("C");
              lcd.setCursor(5,1);
              lcd.print(Fahrenheit);
              lcd.setCursor(8,1);
              lcd.print("F");}
              lcd.setCursor(10,1);
              lcd.print("Cy :");
              lcd.setCursor(14,1);
              lcd.print(i);
            
            }
         case 'B' :
         lcd.setCursor(0,0);
         lcd.print("Durasi Pemantik : ");
         lcd.setCursor(0,1);
         lcd.print("Nilai :");

         case 'C' :
         lcd.setCursor(0,0);
         lcd.print("Durasi gabah : ");
         lcd.setCursor(0,1);
         lcd.print("Nilai :");

      }

      key = keypad.getKey();
   }

   return num;
}


void setup() {
 
  Serial.begin(9600);
  sensors.begin();
  keypad.begin();
  
  pinMode (A0, OUTPUT);//selenoid
  pinMode (A1, OUTPUT);//pemantik
  pinMode (A2, OUTPUT);//motorAC
  pinMode (A3, OUTPUT);//blower
  pinMode (A4, OUTPUT);//motor1
  /*_____GPIO_____*/
   
  Serial.println("Input Nilai Suhu");
  Serial.println();

  /*______KOMUNIKASI GPIO______*/ 
  lcd.begin();//LCD ON
  lcd.backlight();//Display ON
  lcd.setCursor(0,0);
  lcd.print("SET NILAI SUHU");//Display awal di LCD
  lcd.setCursor(0,1);
  lcd.print("Nilai :");
/*_____KOMUNIKASI GPIO______*/

}
 
 
void loop() {

  /*______FUNGSI KEYPAD______*/ 
   MIN = GetNumber();
   lcd.setCursor(0,1);
   lcd.print("set min ");
   lcd.setCursor(8,1);
   lcd.print(MIN);
   Serial.print ("Data Suhu Minimal = ");
   Serial.print (MIN);
   MAX = GetNumber();
   lcd.setCursor(0,1);
   lcd.print("set max ");
   lcd.setCursor(8,1);
   lcd.print(MAX);
   Serial.print ("Data Suhu Maximal = ");
   Serial.print (MAX);

   pemantik = GetNumber();
   lcd.setCursor(0,1);
   lcd.print("set pemantik ");
   lcd.setCursor(8,1);
   lcd.print(durasi);   
   Serial.print ("Durasi pemantik = ");
   Serial.print (pemantik);
   
   durasi = GetNumber();
   lcd.setCursor(0,1);
   lcd.print("set durasi ");
   lcd.setCursor(8,1);
   lcd.print(durasi);
   Serial.print ("Durasi gabah = ");
   Serial.print (durasi);

if (MAX >= MIN)
{
  
  bacasensor();
  if (Celcius <= MAX)
  { 
    digitalWrite(A0,HIGH);//Selenoid ON
    digitalWrite(A1,HIGH);//Pemantik ON
    digitalWrite(A2,HIGH);//Blower ON
    digitalWrite(A3,LOW);//motor AC OFF
    digitalWrite(A4, HIGH);//motor DC 1 OFF (VALVE OPEN OFF)
  }
  else if (Celcius >= MAX )
  {
    digitalWrite(A0,LOW);//Selenoid OFF
    digitalWrite(A1,LOW);//Pemantik OFF
    digitalWrite(A2,HIGH);//Blower ON
    digitalWrite(A3,HIGH);//motor AC ON
    digitalWrite(A4, LOW);//motor DC 1 ON (VALVE OPEN)
    delay(3000);
    digitalWrite(A4, HIGH);//motor DC 1 OFF (VALVE OPEN OFF)
      }
    
  }

}  
        
void bacasensor() 
{

  sensors.requestTemperatures(); 
  Celcius=sensors.getTempCByIndex(0);
  Fahrenheit=sensors.toFahrenheit(Celcius);
  Serial.print(" C = ");
  Serial.print(Celcius);
  Serial.print(" F = ");
  Serial.println(Fahrenheit);
  delay(1000);
  
}

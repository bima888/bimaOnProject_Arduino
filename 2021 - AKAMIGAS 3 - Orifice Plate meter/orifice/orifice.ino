#include <ThingerEthernet.h>
#include <Ethernet.h>
#include <SPI.h>
#include <Wire.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <LiquidCrystal_I2C.h>
#define ONE_WIRE_BUS 12
#define USERNAME "akamigas"
#define DEVICE_ID "akamigas2021"
#define DEVICE_CREDENTIAL "akamigas2021"

ThingerEthernet thing(USERNAME, DEVICE_ID, DEVICE_CREDENTIAL);
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire); 
LiquidCrystal_I2C lcd(0x27,16, 2);

const float  OffSet =  0.48340; //nilai tegangan out sensor tanpa beban
const float OffSet2 =  0.48340; //nilai tegangan out sensor tanpa beban
const float OffSet3 =  0.48340; //nilai tegangan out sensor tanpa beban
float A,B,C,D,E; 
float V, P;
float V2, P2;
float V3, P3;
float DIF;
float Celcius;
float Fahrenheit;
float previousPressureStatic, previousPressureHigh, previousPressureLow, previousPressureDifferential, previousTemperature;
float currentPressureStatic, currentPressureHigh, currentPressureLow, currentPressureDifferential, currentTemperature;

void setup()
{
  Serial.begin(9600);        // open serial port, set the baud rate to 9600 bps
  pinMode(A1,INPUT);
  pinMode(A2,INPUT);
  pinMode(A3,INPUT);
  pinMode(3,OUTPUT);
  pinMode(4,OUTPUT);
  
  lcd.init();
  lcd.backlight();
  
  thing["PRESSURE_STATIC"] >> [](pson& out){     
    out = A;//data pressure statis
  };
  thing["PRESSURE_HIGH"] >> [](pson& out){     
    out = B;//data high stream pressure
  };
  thing["PRESSURE_LOW"] >> [](pson& out){     
    out = C;//data low stream pressure
  };
  thing["DIFFERENTIAL_PRESSURE"] >> [](pson& out){     
    out = D;//data differenntial pressure
  };
  thing["TEMPERATURE"] >> [](pson& out){     
    out = E;//data suhu fluida
  };
}
 
void loop()
{
    lcd.setCursor(0,0);
    lcd.print("PS:");
    lcd.setCursor(3,0);
    lcd.print(P,1); // dua di belakang koma

    lcd.setCursor(8,0);
    lcd.print("DIF:");
    lcd.setCursor(12,0);
    lcd.print(D,1);
 
    lcd.setCursor(4,1);
    lcd.print("TEM :");
    lcd.setCursor(9,1);
    lcd.print(E,1);
    
    thing.handle();
    baca_pressure();
    baca_temperature();
    A = P;
    B = P2;
    C = P3;
    D = DIF;
    E = Celcius;

    if(Celcius > 35 ){
      digitalWrite(3,HIGH);
    } else {
      digitalWrite(3,LOW);
    }
    if(Celcius < 35){
      digitalWrite(4,HIGH);
    } else {
      digitalWrite(4,LOW);
    }

//    Serial.print("Pressure Statis = ");
//    Serial.print(P,2);
//    Serial.print(" BAR | ");
//    Serial.print("Pressure 2 = ");
//    Serial.print(P2,2);
//    Serial.print(" BAR | ");
//    Serial.print("Pressure 3 = ");
//    Serial.print(P3,2);
    
    float kPa1 = P/100;
//    Serial.println(" ");
//    Serial.print("Pressure 1 = ");
//    Serial.print(kPa1,5);
//    Serial.print(" Kpa | ");
    
    float kPa2 = P2/100;
//    Serial.print("Pressure 2 = ");
//    Serial.print(kPa2,5);
//    Serial.print(" Kpa | ");

    float kPa3 = P3/100;
//    Serial.print("Pressure 3 = ");
//    Serial.print(kPa3,5);
//    Serial.print(" Kpa");
//    Serial.println(" ");
    
    delay(500);
    
    lcd.clear();

      currentPressureStatic = A;
      if(abs(currentPressureStatic-previousPressureStatic)>=1.0f){
    thing.stream(thing["PRESSURE_STATIC"]);
    previousPressureStatic=currentPressureStatic;
      }

      currentPressureHigh = B;
        if(abs(currentPressureHigh-previousPressureHigh)>=1.0f){
    thing.stream(thing["PRESSURE_HIGH"]);
    previousPressureHigh=currentPressureHigh;
      }
      currentPressureLow = C;
        if(abs(currentPressureLow-previousPressureLow)>=1.0f){
    thing.stream(thing["PRESSURE_LOW"]);
    previousPressureLow=currentPressureLow;
      }
      currentPressureDifferential = D;
        if(abs(currentPressureDifferential-previousPressureDifferential)>=1.0f){
    thing.stream(thing["DIFFERENTIAL_PRESSURE"]);
    previousPressureDifferential=currentPressureDifferential;
      }
      currentTemperature = E;
        if(abs(currentTemperature-previousTemperature)>=1.0f){
    thing.stream(thing["TEMPERATURE"]);
    previousTemperature=currentTemperature;
      }

}
 
void baca_pressure(){

    V = analogRead(A1) * 5.00 / 1024;     //Sensor output voltage
    P = (V - OffSet) * 400;             //Calculate water pressure((float)V/1024*175);
    if ( P < 0)P = 0;
       
    V2 = analogRead(A2) * 5.00 / 1024;
    P2 = (V2 - OffSet2) * 400;
    if ( P2 < 0)P2 = 0;

    V3 = analogRead(A3) * 5.00 / 1024;
    P3 = (V3 - OffSet3) * 400;
    if ( P2 < 0)P2 = 0;

    DIF = P2-P3;
}
void baca_temperature(){
  sensors.requestTemperatures(); 
  Celcius=sensors.getTempCByIndex(0);
}

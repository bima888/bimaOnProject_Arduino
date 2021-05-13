/*_____________CREATED BY_____________*/
      /*Artahsasta Bima Prafista*/
         /*Malang, Indonesia*/
      /*Instagram : imake.official*/
/*Kelompok 15 : Fermentasi Ketan Hitam*/
/*_____________CREATED BY_____________*/

/*_____________HEADER_____________*/
#include <WiFi.h>                                                
#include <IOXhop_FirebaseESP32.h> 
#include <OneWire.h>//DS18B20
#include <DallasTemperature.h>//DS18B20
#include <LiquidCrystal_I2C.h>

#include "DFRobot_ESP_PH.h"//PHmeter
#include "EEPROM.h"//PHmeter                                            
#define FIREBASE_HOST "fermentasi-ketan.firebaseio.com/"// the project name address from firebase id
#define FIREBASE_AUTH "qgSluI1OQZRcbo8tz2RY8mZulOu25JhwNsC6UUfU"// the secret key generated from firebase
#define WIFI_SSID "Deny"// input your home or public wifi name
#define WIFI_PASSWORD "deni12345"//password of wifi ssid
#define ONE_WIRE_BUS 4//DS18B20
#define ESPADC 4096.0//PHmeter   //the esp Analog Digital Convertion value
#define ESPVOLTAGE 3300//PHmeter //the esp voltage supply value
#define PH_PIN 35//PHmeter    //the esp gpio data pin number
/*_____________HEADER_____________*/

/*_____________INISIALISASI DATA_____________*/
// LCD 
int lcdColumns = 16;
int lcdRows = 2;
LiquidCrystal_I2C lcd(0x27,lcdColumns,lcdRows);  // LCD address 0x27 
//LCD

//Sensor
OneWire oneWire(ONE_WIRE_BUS);//DS18B20
DallasTemperature sensors(&oneWire);//DS18B20
DFRobot_ESP_PH ph;//PHmeter
float Celcius=0;//DS18B20
float Fahrenheit=0;//DS18B20
float sensor_volt;//MQ3
float RS_gas;//MQ3 
float R0;//MQ3
int R2 = 2000;//MQ3
float voltage, phValue, temperature = 25;//PHmeter
//Sensor

//manipulasi data
float manipulasikeempat =  3.66;
//manipulasi data

/*_____________INISIALISASI DATA_____________*/


void setup() {

  Serial.begin(115200);
  sensors.begin();//DS18B20
  EEPROM.begin(32);//PHmeter//needed to permit storage of calibration value in eeprom
  ph.begin();//PHmeter
  lcd.begin();
  lcd.backlight();
  delay(1000);

  /*_____________GPIO_____________*/
  pinMode(33, INPUT);//MQ3 
  pinMode(12, OUTPUT);//relay 1 panas
  pinMode(13, OUTPUT);//relay 2 dingin              
  /*_____________GPIO_____________*/

  /*_____________WIFI CONNECTION_____________*/
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);                                      
  Serial.print("Connecting to ");
  Serial.print(WIFI_SSID);
  while (WiFi.status() != WL_CONNECTED) 
    {
      Serial.print(".");
      delay(500);
    }
  Serial.println();
  Serial.print("Connected to ");
  Serial.println(WIFI_SSID);
  Serial.print("IP Address is : ");
  Serial.println(WiFi.localIP());//print local IP address
  /*_____________WIFI CONNECTION_____________*/

  /*_____________DATABASE CONNECTION_____________*/
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);// connect to firebase
  /*_____________DATABASE CONNECTION_____________*/

}

void loop() {
  while(Firebase.getString("status")=="1")
  {   
    PH();
    
    if (manipulasikeempat >= 4.0 || manipulasikeempat == 4.0)
    {Firebase.setString("kondisi/ph", "FermentasiSelesai");}
    if (manipulasikeempat <= 4.0)
    {Firebase.setString("kondisi/ph", "ProsesFermentasi");}
    alkoholstatus();
    if(R0<=1.0)
    {Firebase.setString("kondisi/alkohol", "Layak");}
    if(R0>=1.0)
    {Firebase.setString("kondisi/alkohol", "TidakLayak");}
    
    suhu();
    
    parsing();
    
    if (Celcius >= 0 && Celcius <= 30)
    {peltierpanas();}
    if (Celcius >= 30)
    {peltierpanas();}
    if (Celcius >= 35)
    {peltierdingin();}
  }
}


float suhu() { //DS18B20

  sensors.requestTemperatures(); 
  Celcius=sensors.getTempCByIndex(0)+1.9;
  Fahrenheit=sensors.toFahrenheit(Celcius);
  Serial.print(" C  ");
  Serial.print(Celcius);
  Serial.print(" F  ");
  Serial.println(Fahrenheit);
  
  lcd.setCursor(0,0);
  lcd.print("Suhu :");
  lcd.setCursor(8,0);
  lcd.print(Celcius);
  lcd.setCursor(14,0);
  lcd.print("C");
  delay(1000);

}

void alkoholstatus() {//MQ3

  int sensorValue = analogRead(33);
  sensor_volt=(float)sensorValue/1024*5.0;
  RS_gas = ((5.0 * R2)/sensor_volt) - R2;
  R0 = RS_gas / 60;
  Serial.print("R0: ");
  Serial.println(R0);
  Serial.print("%");
  lcd.setCursor(0,1);
  lcd.print("Gas :");
  lcd.setCursor(5,1);
  lcd.print(R0);
    
}

void PH () {

  static unsigned long timepoint = millis();
  if (millis() - timepoint > 1000U) //time interval: 1s
  {
    timepoint = millis();
    //voltage = rawPinValue / esp32ADC * esp32Vin
    voltage = analogRead(PH_PIN) / ESPADC * ESPVOLTAGE; // read the voltage
    Serial.print("voltage:");
    Serial.println(voltage, 4);
    
    //temperature = readTemperature(); = // read your temperature sensor to execute temperature compensation
    Serial.print("temperature:");
    Serial.print(temperature, 1);
    Serial.println("^C");

    phValue = ph.readPH(voltage, temperature); // convert voltage to pH with temperature compensation
    Serial.print("pH:");
    Serial.println(phValue, 4);
  }
  ph.calibration(voltage, temperature); // calibration process by Serail CMD
  lcd.setCursor(11,1);
  lcd.print("PH :");
  lcd.setCursor(15,1);
  lcd.print(manipulasikeempat);
}

void peltierpanas()
{digitalWrite(12,HIGH);
 digitalWrite(13,LOW);
}
void peltierdingin()
{digitalWrite(12,LOW);
 digitalWrite(13,HIGH);
}

void parsing(){
  Firebase.pushFloat("Celcius/data",Celcius);
  Firebase.setFloat("Celcius/latest",Celcius);
  Firebase.pushFloat("Fahrenheit/data",Fahrenheit);
  Firebase.setFloat("Fahrenheit/latest",Fahrenheit);
  Firebase.pushFloat("Gas/data",R0);
  Firebase.setFloat("Gas/latest",R0);
  Firebase.pushFloat("PH/data",manipulasikeempat);
  Firebase.setFloat("PH/latest",manipulasikeempat);
}
  

/*_____________CREATED BY_____________*/
      /*Artahsasta Bima Prafista*/
         /*Malang, Indonesia*/
/*_____________CREATED BY_____________*/

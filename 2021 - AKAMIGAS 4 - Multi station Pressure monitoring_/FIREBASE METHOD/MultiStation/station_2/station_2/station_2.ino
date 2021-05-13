#include <ESP8266WiFi.h>
#include <Wire.h>
#include <FirebaseESP8266.h>
#include <LiquidCrystal_I2C.h>
#define FIREBASE_HOST "https://multistation-b11e1-default-rtdb.firebaseio.com/" //FIREBASE HOST
#define FIREBASE_AUTH "zz2NwUkFNICewKZhhBxs3bHQU233NMjh8f0sNDqS" //FIREBASE AUTH
#define WIFI_SSID "BUSTAMI"
#define WIFI_PASSWORD "INDONESIA"

#define PRES_SENSOR  A0

const float  OffSet = 0.483 ;
float V, P;

FirebaseData Labview;

int lcdColumns = 16;
int lcdRows = 2;
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows); 

void setup() {
  Serial.begin(115200);
  WiFi.begin(WIFI_SSID,WIFI_PASSWORD);
  Serial.print("Connecting");
  while(WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("Connencted");
  Serial.println(WiFi.localIP());

  lcd.init();
  lcd.backlight();

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);
}

void loop() {
  int pressure = random(1,20);
  Firebase.setFloat(Labview,"Station_2/Pressure",random(1,20));// isi P
  lcd.setCursor(0,0);
  lcd.print("Pressure :");
  lcd.setCursor(12,0);
  lcd.print(pressure);// isi P
  
  if(WiFi.status() == WL_CONNECTED){
    lcd.setCursor(0,1);
    lcd.print("CONNECTED");
  }
  else{
    lcd.setCursor(0,1);
    lcd.print("NOT CONNECTED");
  }

  V = analogRead(PRES_SENSOR) * 5.00 / 1024;     
  P = (V - OffSet) * 400; 

  if(P < 30.5){
      lcd.setCursor(0,1);
      lcd.print("STATION 2 BOCOR ");
    }else {
      lcd.setCursor(0,1);
      lcd.print("CONNECTED       ");
    }

  
  
}

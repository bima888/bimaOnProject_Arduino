#include <ESP8266WiFi.h>
#include <Wire.h>
#include <FirebaseESP8266.h>
#include <LiquidCrystal_I2C.h>
#define FIREBASE_HOST "https://multistation-b11e1-default-rtdb.firebaseio.com/" //FIREBASE HOST
#define FIREBASE_AUTH "zz2NwUkFNICewKZhhBxs3bHQU233NMjh8f0sNDqS" //FIREBASE AUTH
#define WIFI_SSID "BUSTAMI"
#define WIFI_PASSWORD "INDONESIA"
#define FLOW_SENSOR  2 //FLOW
#define PRES_SENSOR  A0

const float  OffSet = 0.483 ;
float V, P;

FirebaseData Labview;

int lcdColumns = 16;
int lcdRows = 2;
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);

long currentMillis = 0;
long previousMillis = 0;
int interval = 1000;
float calibrationFactor = 4.5;
volatile byte pulseCount;
byte pulse1Sec = 0;
float flowRate;
unsigned long flowMilliLitres;
unsigned int totalMilliLitres;
float flowLitres;
float totalLitres;

void IRAM_ATTR pulseCounter()
{
  pulseCount++;
}

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
  
  pinMode(FLOW_SENSOR, INPUT_PULLUP);
  pulseCount = 0;
  flowRate = 0.0;
  flowMilliLitres = 0;
  totalMilliLitres = 0;
  previousMillis = 0;

  lcd.init();                     
  lcd.backlight();

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);
  attachInterrupt(digitalPinToInterrupt(FLOW_SENSOR), pulseCounter, FALLING);
}

void loop() {
  currentMillis = millis();
  if (currentMillis - previousMillis >= interval) 
  {
    pulse1Sec = pulseCount;
    pulseCount = 0;
    flowRate = ((1000.0 / (millis() - previousMillis)) * pulse1Sec) / calibrationFactor;
    previousMillis = millis();
    flowMilliLitres = (flowRate / 60) * 1000;
    flowLitres = (flowRate / 60);
    Serial.print("FLOW RATE :");
    Serial.print(flowRate);
    delay(500); 
    // Add the millilitres passed in this second to the cumulative total
    totalMilliLitres += flowMilliLitres;
    totalLitres += flowLitres;
  }

  int pressure = random(1,5);
  Firebase.setFloat(Labview,"Station_3/Flow",flowLitres);
  Firebase.setFloat(Labview,"Station_3/Pressure",random(1,5)); // isi P

  lcd.setCursor(0,0);
  lcd.print("P :");
  lcd.setCursor(4,0);
  lcd.print(pressure); // isi P
  lcd.setCursor(6,0);
  lcd.print("F :");
  lcd.setCursor(9,0);
  lcd.print(flowLitres);
    
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
      lcd.print("STATION 3 BOCOR ");
    }else {
      lcd.setCursor(0,1);
      lcd.print("CONNECTED       ");
    }  
  
}

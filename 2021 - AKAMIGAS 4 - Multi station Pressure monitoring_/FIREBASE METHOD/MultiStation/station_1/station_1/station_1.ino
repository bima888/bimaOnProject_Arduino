#include <ESP8266WiFi.h>
#include <Wire.h>
#include <FirebaseESP8266.h>
#include <LiquidCrystal_I2C.h> //GPIO 4 (D2)- SDA || GPIO 5 (D1) - SCL
#define FIREBASE_HOST "https://multistation-b11e1-default-rtdb.firebaseio.com/" //FIREBASE HOST
#define FIREBASE_AUTH "zz2NwUkFNICewKZhhBxs3bHQU233NMjh8f0sNDqS" //FIREBASE AUTH
#define WIFI_SSID "BUSTAMI"
#define WIFI_PASSWORD "INDONESIA"
#define FLOW_SENSOR  2 //FLOW
#define PRES_SENSOR  A0

const float  OffSet = 0.483 ;
float V, P;

FirebaseData Labview;

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

int datapass1 = 0;
int datapass2 = 0;
int getdatapass1,getdatapass2;

float pressuretotal;
float flowtotal;
float SP_pressure = 30.50;
float SP_flow = 0.50;

double bocorpressure,
      P_prosentase,P_prosentasehasil,
      P_getpros;

float F_prosentase,
      F_prosentasehasil,
      bocorflow,
      F_getpros;
      
float pressure1,pressure2,pressure3;

float flow3;
float flow1;

int lcdColumns = 16;
int lcdRows = 2;

LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows); 

void IRAM_ATTR pulseCounter()
{
  pulseCount++;
}

void setup() {
  Serial.begin(115200);
  pinMode(FLOW_SENSOR, INPUT_PULLUP);
  pinMode(16,OUTPUT); //Relay
  pinMode(0,OUTPUT);  //Buzzer

  datapass1 = 0;
  datapass2 = 0;
  
  WiFi.begin(WIFI_SSID,WIFI_PASSWORD);
  Serial.print("Connecting");
  while(WiFi.status() != WL_CONNECTED){
    //Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("Connencted");
  Serial.println(WiFi.localIP());
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
    totalMilliLitres += flowMilliLitres;
    totalLitres += flowLitres;
  }
  
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
  
  getdata();
 
  lcd.setCursor(0,0);
  lcd.print("P :");
  lcd.setCursor(4,0);
  lcd.print(pressure1);
  lcd.setCursor(6,0);
  lcd.print("F :");
  lcd.setCursor(9,0);
  lcd.print(flowLitres);

  Serial.println(pressure1);
  Serial.println(pressure2);
  Serial.println(pressure3);
  Serial.println(flow3);
  Serial.println(flow1);
  Serial.println(bocorflow);
  Serial.println(bocorpressure);
  Serial.println(P_getpros);
  Serial.println(F_getpros);
  Serial.println(getdatapass1);
  Serial.println(getdatapass2);
  //Serial.println("data1 : " + String(datapass1)+" | data2 : " + String(datapass2));

  if(pressure1 && pressure2 && pressure3 >= SP_pressure){
    datapass1 = 1;
  }
  if(flow1 && flow3 >= SP_flow){
    datapass2 = 1;
  }

  if(pressure1 && pressure2 && pressure3 < 5){
    datapass1 = 0;
  }

  if(flow1 && flow3 <= 0.1){
    datapass2 = 0;
  }

  pressuretotal = pressure1 + pressure2 + pressure3 ;
  flowtotal = flow1 + flow3;
  P_prosentase = (pressuretotal/91.5)*100;
  P_prosentasehasil = 100 - P_prosentase;
  F_prosentase = (flowtotal/1)*100;
  F_prosentasehasil = 100 - F_prosentase;
  
  
  if(datapass1 && datapass2 == 1){
    
    Firebase.setDouble(Labview,"Prosentase/Pressure",P_prosentasehasil);
    Firebase.setDouble(Labview,"Prosentase/Flow",F_prosentasehasil);
    
    if(pressuretotal < 91.5){
      digitalWrite(16,HIGH);
      digitalWrite(0,HIGH);
    }else{
      digitalWrite(16,LOW);
      digitalWrite(0,LOW);
    }

    if(P < 30.5){
      lcd.setCursor(0,1);
      lcd.print("STATION 1 BOCOR ");
    }else {
      lcd.setCursor(0,1);
      lcd.print("CONNECTED       ");
    }

    if(pressure2 < 30.5){
      lcd.setCursor(0,1);
      lcd.print("STATION 2 BOCOR ");
    }else {
      lcd.setCursor(0,1);
      lcd.print("CONNECTED       ");
    }

    if(pressure3 < 30.5){
      lcd.setCursor(0,1);
      lcd.print("STATION 3 BOCOR ");
    }else {
      lcd.setCursor(0,1);
      lcd.print("CONNECTED       ");
    }
    
  }

  

  //Firebase.setFloat(Labview,"Station_1/Flow",flowLitres); 
  //Firebase.setFloat(Labview,"Station_1/Pressure",pressure1);// isi P
  Firebase.setFloat(Labview,"Kebocoran/Flow",flowtotal);
  Firebase.setFloat(Labview,"Kebocoran/Pressure",pressuretotal);
  Firebase.setInt(Labview,"datapass/datapass1",datapass1);
  Firebase.setInt(Labview,"datapass/datapass2",datapass2);
  
}

void getdata(){
  //station 2
  if (Firebase.getFloat(Labview,"Station_2/Pressure")){
         if (Labview.dataType() == "float")
         pressure2 = (Labview.floatData());         
  }
  
  //station 3
  if (Firebase.getFloat(Labview,"Station_3/Pressure")){
         if (Labview.dataType() == "float")
         pressure3 = (Labview.floatData());
  }
  
  if (Firebase.getFloat(Labview,"Station_3/Flow")){
         if (Labview.dataType() == "float")
         flow3 = (Labview.floatData());
  }
  
  //TES station 1
  if (Firebase.getFloat(Labview,"Station_1/Flow")){
         if (Labview.dataType() == "float")
         flow1 = (Labview.floatData());
  }  
  
  if (Firebase.getFloat(Labview,"Station_1/Pressure")){
         if (Labview.dataType() == "float")
         pressure1 = (Labview.floatData());
  }  

  //Data bocor
   if (Firebase.getFloat(Labview,"Kebocoran/Flow")){
         if (Labview.dataType() == "float")
         bocorflow = (Labview.floatData());
  }  
  
   if (Firebase.getFloat(Labview,"Kebocoran/Pressure")){
         if (Labview.dataType() == "float")
         bocorpressure = (Labview.floatData());
  }  

  //Data prosentase
  if (Firebase.getInt(Labview,"Prosentase/Flow")){
         if (Labview.dataType() == "int")
         F_getpros = (Labview.intData());
  }  
  
   if (Firebase.getDouble(Labview,"Prosentase/Pressure")){
         if (Labview.dataType() == "double")
         P_getpros = (Labview.doubleData());
  }  

  if (Firebase.getInt(Labview,"datapass/datapass1")){
         if (Labview.dataType() == "int")
         getdatapass1 = (Labview.intData());
  }
  
  if (Firebase.getInt(Labview,"datapass/datapass2")){
         if (Labview.dataType() == "int")
         getdatapass2 = (Labview.intData());
  }
}

#include <OneWire.h>
#include <Servo.h>
#include <DallasTemperature.h>
#define ONE_WIRE_BUS 8

Servo a; //class servo
OneWire oneWire(ONE_WIRE_BUS); //class temperature sensor

// DEKLARASI TEMPERATURE SENSOR
DallasTemperature sensors(&oneWire);
DeviceAddress sensor1 = { 0x28, 0x90, 0x98, 0x07, 0xD6, 0x01, 0x3C, 0xB1 };
DeviceAddress sensor2 = { 0x28, 0x1E, 0x80, 0x07, 0xD6, 0x01, 0x3C, 0x0A };
DeviceAddress sensor3 = { 0x28, 0x04, 0x3A, 0x07, 0xD6, 0x01, 0x3C, 0x6C };
// DEKLARASI TEMPERATURE SENSOR

// DEKLARASI FLOW 
float flowRate = 0.0;
float calibrationFactor = 7.5;
volatile byte pulseCount =0;  
unsigned int flowMilliLitres =0;
unsigned long totalMilliLitres = 0;
unsigned long oldTime = 0;
int sensorInterrupt = 0;  // interrupt 0
int flowsensorPin       = 2;  // Digital Pin 2 flow sensor
// DEKLARASI FLOW

// DEKLARASI TEMP
int   temp_read1,
      temp_read2,
      temp_read3;
// DEKLARASI TEMP

// DEKLARASI MAP CASCADE
int mapTemp2Flow,
    flowError;
// DEKLARASI MAP CASCADE

/*_____CASCADE_____*/
/*PID FLOW*/         
int flow_read = 0.0;
int F_PID_error = 0;
int F_previous_error = 0;
int F_elapsedTime, F_Time, F_timePrev;
int F_PID_value = 0;

int F_PID_p = 0;    int F_PID_i = 0;    int F_PID_d = 0;

int SP_flow = 20; //SET POINT FLOW RATE
int F_degree = 0;
int F_SP_Kp = 450;
int F_SP_Ki = F_SP_Kp/100;
int F_SP_Kd = 1000;
int degreemapVariable;
/*PID FLOW*/
/*__________________*/
/*PID TEMP*/     
const int peltier = 9; //Variable PIN OUTPUT peltier pendingin     
float temp_read = 0.0;
float T_PID_error = 0;
float T_previous_error = 0;
float T_elapsedTime, T_Time, T_timePrev;
float T_PID_value = 0;

int T_PID_p = 0;    int T_PID_i = 0;    int T_PID_d = 0;

float SP_temp = 20; //SET POINT TEMPERATURE 
int T_degree = 0;
int T_SP_Kp = 60;
int T_SP_Ki = 30;
int T_SP_Kd = 20;
/*PID TEMP*/
int setTempCascade = 50; //SET POINT TEMP CASCADE
int setFlowCascade = 20; //SET POINT FLOW CASCADE
/*_____CASCADE_____*/


/*_____ON-OFF_____*/
int set_temp_H = 80;
const int hotpump  = 4;
const int coldpump = 5;
const int heatexchangepump = 6;
const int heater = 7;
/*_____ON-OFF_____*/

void setup()
{

  Serial.begin(9600);
  sensors.begin();
  a.attach(3); // OUTPUT PID SERVO VALVE
  
  pinMode(hotpump,OUTPUT);
  pinMode(coldpump,OUTPUT);
  pinMode(heatexchangepump,OUTPUT);
  pinMode(heater,OUTPUT);
  pinMode(peltier , OUTPUT);
  pinMode(flowsensorPin, INPUT);

  digitalWrite(flowsensorPin, HIGH);
  attachInterrupt(sensorInterrupt, pulseCounter, FALLING);
}

void loop()
{
  dataTemp();
  if((millis() - oldTime) >= 1000) 
  { 
    detachInterrupt(sensorInterrupt);
    flowRate = ((1000.0 / (millis() - oldTime)) * pulseCount) / calibrationFactor;
    oldTime = millis();
    flowMilliLitres = (flowRate / 60) * 1000;
    totalMilliLitres += flowMilliLitres;

    temp_read1 =  sensors.getTempC(sensor1);
    Serial.println("TEMP_1: " + String(temp_read1));
    
    // flow rate liter / menit
    Serial.print("Flow rate: ");
    Serial.print(flowRate, DEC);  // Print the integer part of the variable
    Serial.print("");
    Serial.print(" mL/Minute");
    Serial.print("\t");
    
    // flowmili liter / second
    Serial.print("Flow mili: ");
    Serial.print(flowMilliLitres, DEC);  // Print the integer part of the variable
    Serial.print("");
    Serial.print(" mL/Second");
    Serial.print("\t");           

    // akumulasi total mili
    Serial.print("TOTAL VOLUME: ");        
    Serial.print(totalMilliLitres,DEC);
    Serial.println(" mL"); 
    Serial.print("\t");      
    
    // Reset the pulse counter so we can start incrementing again
    pulseCount = 0;
    //totalMilliLitres =0;
    attachInterrupt(sensorInterrupt, pulseCounter, FALLING);
  }
  
//____________________________CASCADE control____________________________________//

  //MONITOR  
  temp_read2 =  sensors.getTempC(sensor2);
  temp_read3 =  sensors.getTempC(sensor3);
    Serial.println("TEMP_2: " + String(temp_read2) + " | TEMP_3: " + String(temp_read3));
    Serial.println("");
  //MONITOR
  
  //CASCADE TEMP to FLOW
  mapTemp2Flow = map(temp_read1,30,setTempCascade,0,setFlowCascade);
  flowError = 20 - mapTemp2Flow;
  //CASCADE TEMP to FLOW
  
  //__________________________PID FLOW
  F_PID_error = SP_flow - flowError;
  F_PID_p = 0.01*F_SP_Kp * F_PID_error;
  F_PID_i = 0.01*F_PID_i + (F_SP_Ki * F_PID_error);
  F_Time = millis();
  if((F_Time- F_timePrev) >= 1000){
  F_timePrev = F_Time;
  F_elapsedTime = (F_Time - F_timePrev) / 1000;
  F_PID_d = 0.01*F_SP_Kd*((F_PID_error - F_previous_error)/F_elapsedTime);}
  F_PID_value = F_PID_p ;/*+ F_PID_i + F_PID_d;*/  

//  int b = map(F_PID_value,0,100,0,90);
//  a.write(90-b);
  //__________________________PID FLOW
  //__________________________PID TEMP
  temp_read2 =  sensors.getTempC(sensor2);
  T_PID_error = SP_temp - temp_read2;
  T_PID_p = 0.01*T_SP_Kp * T_PID_error;
  T_PID_i = 0.01*T_PID_i + (T_SP_Ki * T_PID_error);
  T_timePrev = T_Time;
  T_Time = millis();
  T_elapsedTime = (T_Time - T_timePrev) / 1000;
  T_PID_d = 0.01*T_SP_Kd*((T_PID_error - T_previous_error)/T_elapsedTime);
  T_PID_value = T_PID_p + T_PID_i + T_PID_d; 
  
  //TEMP SET POINT
  if(T_PID_value > 255)
      T_PID_value == 255;
  if(T_PID_value < 0)
      T_PID_value == 0;
  
  analogWrite(peltier,T_PID_value);
  //TEMP SET POINT

  //__________________________PID TEMP

//____________________________CASCADE control____________________________________//

  //ON-OFF CONTROL//
  digitalWrite(hotpump, HIGH);
  
  //HOT water control
  if(temp_read3 < set_temp_H) {
  digitalWrite(heater,HIGH);  
  } else { digitalWrite(heater,LOW); }
  
  //Cold water control
  digitalWrite(heatexchangepump, HIGH);   
  digitalWrite(coldpump, HIGH);  
  
  //ON-OFF CONTROL//
}

void pulseCounter()
{
  pulseCount++;
}

void printTemperature(DeviceAddress deviceAddress)
{
  float tempC = sensors.getTempC(deviceAddress);
  Serial.print(tempC);
  Serial.print((char)176);
  Serial.print("C  |  ");
  Serial.print(DallasTemperature::toFahrenheit(tempC));
  Serial.print((char)176);
  Serial.println("F");
}

void dataTemp()
{
  sensors.requestTemperatures();
  
//  Serial.print("Sensor 1: ");
  printTemperature(sensor1);
  
//  Serial.print("Sensor 2: ");
  printTemperature(sensor2);
  
//  Serial.print("Sensor 3: ");
  printTemperature(sensor3);
  
//  Serial.println();
  delay(1000);
}

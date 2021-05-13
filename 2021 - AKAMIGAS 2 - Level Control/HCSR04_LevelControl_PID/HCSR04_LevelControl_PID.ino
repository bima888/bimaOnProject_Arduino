#include <Servo.h>
Servo servoValve;

/*PID*/         
int volume_read = 0.0;
float PID_error = 0;
float previous_error = 0;
float elapsedTime, Time_, timePrev;
int PID_value = 0;

int PID_p = 0;    int PID_i = 0;    int PID_d = 0;

int degree = 0;
int lastdegree = 0;

int SP_level = 3000;
float SP_Kp = 3;
int SP_Ki = SP_Kp/100;
int SP_Kd = 1000;
/*PID*/

 const int TriggerPin = 3;    //trig pin
 const int EchoPin = 2;       //Echo pin
 float Duration = 0;
 int full = 0;
 int mL = 0;
 int pos = 0;
   
 void setup() {
  
  Serial.begin(9600);      
  servoValve.attach(7);
  pinMode(TriggerPin,OUTPUT); 
  pinMode(EchoPin,INPUT); 
  
}

void loop() {

  digitalWrite(TriggerPin,LOW);
  delayMicroseconds(2);
  digitalWrite(TriggerPin,HIGH);
  delayMicroseconds(10);
  digitalWrite(TriggerPin,LOW);
  
  Duration = pulseIn(EchoPin,HIGH);
  float Distance_cm = Distance(Duration); //0-18 cm
 
  mL=Distance_cm*200; //0-3600 ml
  volume_read = 3600 - mL;
  
  PID_error = SP_level - volume_read;
  PID_p = 0.01*SP_Kp * PID_error + ;
  PID_i = 0.01*PID_i + (SP_Ki * PID_error);
  Time_ = millis();
  if((Time_- timePrev) >= 1000){
  timePrev = Time_;
  elapsedTime = (Time_ - timePrev) / 1000;
  previous_error = PID_error;
  PID_d = 0.01*SP_Kd*((PID_error - previous_error)/elapsedTime);}
  PID_value = PID_p - PID_i + PID_d;

  //full = map(PID_value,-2070,1250,0,SP_level);
  
  Serial.print(" | PID Value = ");
  Serial.println(90-PID_value);
  Serial.print(" ");
  delay(50);

 
    //degree = map(full,10,SP_level,0,90);
    //pos = 180-degree;    
    servoValve.write(90-PID_value);

  delay(80);
  Serial.print("Distance: ");
  Serial.print(previous_error);
  Serial.print(" cm");
  Serial.print(" | volume: ");
  Serial.print(volume_read);
  Serial.print(" mL");
  Serial.print(" | degree: ");
  Serial.print(PID_d);

  
}

long Distance(long time)
{
  long DistanceCalc;

  DistanceCalc = ((time * 0.034)/2);
  return DistanceCalc;
}

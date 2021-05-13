//Deklarasi Input / Output
const byte saklar_ON = 2;
const byte saklar_OFF = 3;
const int motor_FWD = 10;
const int motor_RVR = 11;

//Deklarasi Kondisi Output
volatile byte FWD = LOW;
volatile byte RVR = LOW;

void setup() 
{
Serial.begin(9600);
  pinMode(saklar_ON,INPUT_PULLUP);
  pinMode(saklar_OFF,INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(saklar_ON), FORWARD, CHANGE);  
  attachInterrupt(digitalPinToInterrupt(saklar_OFF), REVERSE, CHANGE);
  pinMode(motor_FWD, OUTPUT);
  pinMode(motor_RVR, OUTPUT);
}

void loop() 
{
  digitalWrite(motor_FWD, FWD);
  digitalWrite(motor_RVR, RVR);
  if (FWD = HIGH)
  {Serial.print("FWD ON");} else {Serial.println("FWD OFF");}
  delay(500);
  if (RVR = HIGH)
  {Serial.print("RVR ON");} else {Serial.println("RVR OFF");}
  delay(500);
}

void FORWARD()
{
  FWD = !FWD;
  RVR = RVR;
}

void REVERSE()
{
  RVR = !RVR;
  FWD = FWD;
}

// Thermopile test

// pins used
#define THERMOPILE A0
#define THERMISTOR A1

// constants for the thermistor calculations
const float a = -412.6;
const float b = 140.41;
const float c = 0.00764;
const float d = -0.0000000000000000625;
const float e = -0.00000000000000000000000115;

// constants for the thermopile calculation
const float k = 0.004313; 
const float delta = 2.468;

const float reftemp = 25; // reference temperature is 25C
const float shiftv = 0.6; // amount to shift thermopile voltage for negative V values in range
const float verr = 0.6;  // voltage error introduced to thermopile by circuit

void setup() {
  Serial.begin(9600);
}

void loop() {
  float thermopileValue = analogRead(THERMOPILE);
  float thermistorValue = analogRead(THERMISTOR);
  
  // work out thermistor temp from reading
  float v1 = (thermistorValue / 1024) * 5; // source voltage is 5v so reading is fraction of that
  float r = -(v1*1000)/(v1-5); // to get the resistance
  float ambtemp = a + b * sqrt(1+c*r) + d*pow(r,5) + e*pow(r,7); // ambient temp
  
  float comp = k * (pow(ambtemp,4-delta)-pow(reftemp,4-delta));  // equivalent thermopile V for amb temp
  
  // calculate the thermopile temp
  float v2 = (thermopileValue / 1024) * 5 + comp - verr - shiftv; // thermopile voltage
  float objtemp = pow((v2+k*pow(ambtemp,4-delta))/k, 1/(4-delta)); // object temp
  
  Serial.print("thermopile value = ");
  Serial.println(thermopileValue);
  Serial.print("thermistor value = ");
  Serial.println(thermistorValue);
  Serial.print("ambient temp = ");
  Serial.println(ambtemp);
  Serial.print("object temp = ");
  Serial.println(objtemp);
  Serial.println();

  delay(2000);
}  

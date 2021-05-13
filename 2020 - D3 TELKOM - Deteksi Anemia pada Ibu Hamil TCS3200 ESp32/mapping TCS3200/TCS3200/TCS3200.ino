// TCS230 or TCS3200 pins wiring to Arduino
#define S0 4
#define S1 5
#define S2 6
#define S3 7
#define sensorOut 8

// Stores frequency read by the photodiodes
int redFrequency = 0;
int greenFrequency = 0;
int blueFrequency = 0;

// Stores the red. green and blue colors
int redColor = 270;
int greenColor = 183;
int blueColor = 206;

int redcon;
int greencon;
int bluecon;

void setup() {
  // Setting the outputs
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);

  // Setting the sensorOut as an input
  pinMode(sensorOut, INPUT);
  
  // Setting frequency scaling to 20%
  digitalWrite(S0,HIGH);
  digitalWrite(S1,LOW);
  
  // Begins serial communication
  Serial.begin(9600);
}

  void loop() {
  // Setting RED (R) filtered photodiodes to be read
  digitalWrite(S2,LOW);
  digitalWrite(S3,LOW);
  
  // Reading the output frequency
  redFrequency = pulseIn(sensorOut, LOW);
  // Remaping the value of the RED (R) frequency from 0 to 255
  // You must replace with your own values. Here's an example: 
  // redColor = map(redFrequency, 70, 120, 255,0);
  redColor = map(redFrequency, 70, 120, 255, 0);
  redcon = redColor*0.511;
  // Printing the RED (R) value
  Serial.print("R = ");
  Serial.print(redcon);
  delay(100);
  
  // Setting GREEN (G) filtered photodiodes to be read
  digitalWrite(S2,HIGH);
  digitalWrite(S3,HIGH);
  
  // Reading the output frequency
  greenFrequency = pulseIn(sensorOut, LOW);
  // Remaping the value of the GREEN (G) frequency from 0 to 255
  // You must replace with your own values. Here's an example: 
  // greenColor = map(greenFrequency, 100, 199, 255, 0);
  greenColor = map(greenFrequency, 100, 199, 255, 0);
  greencon = greenColor*0.511;
  // Printing the GREEN (G) value  
  Serial.print(" G = ");
  Serial.print(greencon);
  delay(100);
 
  // Setting BLUE (B) filtered photodiodes to be read
  digitalWrite(S2,LOW);
  digitalWrite(S3,HIGH);

   // Reading the output frequency
  blueFrequency = pulseIn(sensorOut, LOW);
  // Remaping the value of the BLUE (B) frequency from 0 to 255
  // You must replace with your own values. Here's an example: 
  // blueColor = map(blueFrequency, 38, 84, 255, 0);
  blueColor = map(blueFrequency, 38, 84, 255, 0);
  bluecon = blueColor*0.511;
  // Printing the BLUE (B) value 
  Serial.print(" B = ");
  Serial.print(bluecon);
  delay(100);

  // Checks the current detected color and prints
  // a message in the serial monitor
  if(redcon > greencon && redcon > bluecon){
      Serial.println(" - RED detected!");
  }
  if(greencon > redcon && greencon > bluecon){
    Serial.println(" - GREEN detected!");
  }
  if(bluecon > redcon && bluecon > greencon){
    Serial.println(" - BLUE detected!");
  }
  if (redcon > 270)
  {
    Serial.println("- ACCES GRANTED RED -");
    } else {Serial.println("- ACCES FRAUD -");}
    
  
  if (bluecon > 183)
  {
    Serial.println("- ACCES GRANTED BLUE -");
    } else {Serial.println("- ACCES FRAUD -");}
  
  if (greencon > 206)
  {
    Serial.println("- ACCES GRANTED GREEN -");
    } else {Serial.println("- ACCES FRAUD -");}
}

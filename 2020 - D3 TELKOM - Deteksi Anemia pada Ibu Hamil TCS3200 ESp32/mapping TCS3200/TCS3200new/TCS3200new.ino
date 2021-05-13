// TCS230 or TCS3200 pins wiring to Arduino
#define S0 9
#define S1 10
#define S2 11
#define S3 12
#define sensorOut1 2
#define S00 8
#define S11 7
#define S22 6
#define S33 5
#define sensorOut2 3

/*_________________INISIALISASI FREKUENSI______________*/
// Stores frequency read by the photodiodes
int redFrequency1 = 0;
int greenFrequency1 = 0;
int blueFrequency1 = 0;
int redFrequency2 = 0;
int greenFrequency2 = 0;
int blueFrequency2 = 0;
/*________________________________________________*/

/*_________________INISIALISASI DATA______________*/
// Stores the red. green and blue colors
//int redColor1 = 270;
//int greenColor1 = 183;
//int blueColor1 = 206;
//int redColor2 = 270;
//int greenColor2 = 183;
//int blueColor2 = 206;

int redColor1 = 0;
int greenColor1 = 0;
int blueColor1 = 0;
int redColor2 = 0;
int greenColor2 = 0;
int blueColor2 = 0;
/*________________________________________________*/

/*_________________INISIALISASI DATA FORMULA______________*/
int redcon1;
int greencon1;
int bluecon1;
int redcon2;
int greencon2;
int bluecon2;
float redstat1;
float result;
/*________________________________________________*/

void setup() {
  // Setting the outputs
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(S00, OUTPUT);
  pinMode(S11, OUTPUT);
  pinMode(S22, OUTPUT);
  pinMode(S33, OUTPUT);

  // Setting the sensorOut as an input
  pinMode(sensorOut1, INPUT);
  pinMode(sensorOut2, INPUT);
  
  // Setting frequency scaling to 100%
  digitalWrite(S0,HIGH);
  digitalWrite(S1,LOW);
  digitalWrite(S00,HIGH);
  digitalWrite(S11,LOW);
  
  // Begins serial communication
  Serial.begin(9600);
}

  void loop() {
  /*_________________RED 1______________*/
  // Setting RED (R) filtered photodiodes to be read
  digitalWrite(S2,LOW);
  digitalWrite(S3,LOW);
  // Reading the output frequency
  redFrequency1 = pulseIn(sensorOut1, LOW);
  redColor1 = map(redFrequency1, 70, 120, 255, 0);
  /*___RUMUS___*/
  redcon1 = redColor1*0.511;
  //DATA 1
  redstat1 = 1.14/100*263;
  resultred1 = 263-redstat1;
  /*___RUMUS___*/
  // Printing the RED (R) value
  Serial.print("R = ");
  Serial.print(redcon1);
  //Serial.print(redColor1);
  delay(100);
  /*_________________RED 1______________*/

  /*_________________GREEN 1______________*/
  // Setting GREEN (G) filtered photodiodes to be read
  digitalWrite(S2,HIGH);
  digitalWrite(S3,HIGH); 
  // Reading the output frequency
  greenFrequency1 = pulseIn(sensorOut1, LOW);
  greenColor1 = map(greenFrequency1, 100, 199, 255, 0);
  /*___RUMUS___*/
  greencon1 = greenColor1*0.511;
  /*___RUMUS___*/
  // Printing the GREEN (G) value  
  Serial.print(" G = ");
  Serial.print(greencon1);
  //Serial.print(greenColor1);
  delay(100);
  /*_________________GREEN 1______________*/

  /*_________________BLUE 1______________*/
  // Setting BLUE (B) filtered photodiodes to be read
  digitalWrite(S2,LOW);
  digitalWrite(S3,HIGH);
  // Reading the output frequency
  blueFrequency1 = pulseIn(sensorOut1, LOW);
  blueColor1 = map(blueFrequency1, 38, 84, 255, 0);
  /*___RUMUS___*/
  bluecon1 = blueColor1*0.511;
  /*___RUMUS___*/
  // Printing the BLUE (B) value 
  Serial.print(" B = ");
  Serial.print(bluecon1);
  //Serial.print(blueColor1);
  delay(100);
  /*_________________BLUE 1______________*/

  /*_________________RED 2______________*/
  // Setting RED (R) filtered photodiodes to be read
  digitalWrite(S2,LOW);
  digitalWrite(S3,LOW);
  // Reading the output frequency
  redFrequency2 = pulseIn(sensorOut2, LOW);
  redColor2 = map(redFrequency2, 70, 120, 255, 0);
  /*___RUMUS___*/
  redcon2 = redColor2*0.511;
  /*___RUMUS___*/
  // Printing the RED (R) value
  Serial.print("R = ");
  Serial.print(redcon2);
  //Serial.print(redColor2);
  delay(100);
  /*_________________RED 2______________*/

  /*_________________GREEN 2______________*/
  // Setting GREEN (G) filtered photodiodes to be read
  digitalWrite(S2,HIGH);
  digitalWrite(S3,HIGH); 
  // Reading the output frequency
  greenFrequency2 = pulseIn(sensorOut2, LOW);
  greenColor2 = map(greenFrequency2, 100, 199, 255, 0);
  /*___RUMUS___*/
  greencon2 = greenColor2*0.511;
  /*___RUMUS___*/
  // Printing the GREEN (G) value  
  Serial.print(" G = ");
  Serial.print(greencon2);
  //Serial.print(greenColor2);
  delay(100);
  /*_________________GREEN 2______________*/

  /*_________________BLUE 2______________*/
  // Setting BLUE (B) filtered photodiodes to be read
  digitalWrite(S2,LOW); 
  digitalWrite(S3,HIGH);
  // Reading the output frequency
  blueFrequency2 = pulseIn(sensorOut2, LOW);
  blueColor2 = map(blueFrequency2, 38, 84, 255, 0);
  /*___RUMUS___*/
  bluecon2 = blueColor2*0.511;
  /*___RUMUS___*/
  // Printing the BLUE (B) value 
  Serial.print(" B = ");
  Serial.print(bluecon2);
  //Serial.print(blueColor2);
  delay(100);
  /*_________________BLUE 2______________*/
  
  /*_________________VALIDASI______________*/
  /*_____________BASED on FORMULA______________*/
  if(redcon1 > greencon1 && redcon1 > bluecon1){
      Serial.println(" - 1 RED detected!");
  }
  if(greencon1 > redcon1 && greencon1 > bluecon1){
    Serial.println(" - 1 GREEN detected!");
  }
  if(bluecon1 > redcon1 && bluecon1 > greencon1){
    Serial.println(" - 1 BLUE detected!");
  }
  if(redcon2> greencon2 && redcon2> bluecon2){
      Serial.println(" - 2 RED detected!");
  }
  if(greencon2 > redcon2 && greencon2 > bluecon2){
    Serial.println(" - 2 GREEN detected!");
  }
  if(bluecon2 > redcon2 && bluecon2 > greencon2){
    Serial.println(" - 2 BLUE detected!");
  }
  /*_____________BASED on FORMULA______________*/

  /*_____________NORMAL______________*/
//  if(redColor1 > greenColor1 && redColor1 > blueColor1){
//      Serial.println(" - 1 RED detected!");
//  }
//  if(greenColor1 > redColor1 && greenColor1 > blueColor1){
//    Serial.println(" - 1 GREEN detected!");
//  }
//  if(blueColor1 > redColor1 && blueColor1 > greenColor1){
//    Serial.println(" - 1 BLUE detected!");
//  }
//
//  if(redColor2 > greenColor2 && redColor2 > blueColor2){
//      Serial.println(" - 2 RED detected!");
//  }
//  if(greenColor2 > redColor2 && greenColor2 > blueColor2){
//    Serial.println(" - 2 GREEN detected!");
//  }
//  if(blueColor2 > redColor2 && blueColor2 > greenColor2){
//    Serial.println(" - 2 BLUE detected!");
//  }
  /*_____________NORMAL______________*/
  
  /*____________BASED on DATA______________*/
//  if (redcon > 270)
//  {
//    Serial.println("- ACCES GRANTED RED -");
//    } else {Serial.println("- ACCES FRAUD -");}
//    
//  
//  if (bluecon > 183)
//  {
//    Serial.println("- ACCES GRANTED BLUE -");
//    } else {Serial.println("- ACCES FRAUD -");}
//  
//  if (greencon > 206)
//  {
//    Serial.println("- ACCES GRANTED GREEN -");
//    } else {Serial.println("- ACCES FRAUD -");}
  /*____________BASED on DATA______________*/

  /*____________BASED on TABLE______________*/
  if(redColor1 > 183){Serial.print ("RED");}
  else if (redColor1 == 280 ) {Serial.print ("DATA RED MATCH");}
//  else if () {}
//  else if () {}
//  else if () {}
//  else if () {}
  /*____________BASED on TABLE______________*/
  
    
  /*_________________VALIDASI______________*/

}

#define S0 4
#define S1 5
#define S2 6
#define S3 7
#define setupGPIO 8

void setup ()
{
  //GPIO SETTING INPUT
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  //SETTING sensor sebagai Input
  pinMode (setupGPIO, INPUT);
  //frekuensi scalling ke 20 %
  digitalWrite(S0,HIGH);
  digitalWrite(S1,LOW);
  Serial.begin(9600);
  }

void loop(){}

void scanning ()
{
  int redfrek = 0;
  int greenfrek = 0;
  int bluefrek = 0;

  int red = 0;
  int green = 0;
  int blue = 0;
  
  digitalWrite (S2,LOW);
  digitalWrite (S3,LOW);
  redfrek = pulseIn (setupGPIO, LOW);
  red = map(redfrek, 00,00,255,0);
  
  digitalWrite (S2,HIGH);
  digitalWrite (S3,HIGH);
  greenfrek = pulseIn (setupGPIO, LOW);
  green = map(greenfrek, 00,00,255,0);

  digitalWrite (S2,LOW);
  digitalWrite (S3,HIGH);
  bluefrek = pulseIn (setupGPIO, LOW);
  blue = map(bluefrek, 00,00,255,0);

  if (red > green && red > blue){Serial.println("Warna Merah");}
  if (green > red && green > blue){Serial.println("Warna Hijau");}
  if (blue > red && blue > green){Serial.println("Warna Biru");}
  }

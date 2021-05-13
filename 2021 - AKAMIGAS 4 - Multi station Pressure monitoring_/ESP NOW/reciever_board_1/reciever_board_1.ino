#include <ESP8266WiFi.h>
#include <espnow.h>
#include <Wire.h>
#define SERVER_PORT 80   
const char* ssid = "MultiStation";     
const char* password = "123456789";

IPAddress ip(192,168,16,2);
IPAddress gateway(192,168,16,1);
IPAddress subnet(255,255,255,0);

WiFiServer server(SERVER_PORT);    

#define SENSOR  2

long currentMillis = 0;
long previousMillis = 0;
int interval  = 1000;
int interval2 = 1000;
float calibrationFactor = 4.5;
volatile byte pulseCount;
byte pulse1Sec = 0;
float flowRate;
unsigned long flowMilliLitres;
unsigned int totalMilliLitres;
float flowLitres;
float totalLitres;

// Structure example to receive data
// Must match the sender structure
typedef struct struct_message {
    int id;
    int x;
    int y;
} struct_message;

// Create a struct_message called myData
struct_message myData;

// Create a structure to hold the readings from each board
struct_message board2;
struct_message board3;

// Create an array with all the structures
struct_message boardsStruct[2] = {board2, board3};

// Callback function that will be executed when data is received
void OnDataRecv(uint8_t * mac_addr, uint8_t *incomingData, uint8_t len) {
  char macStr[18];
  Serial.print("Data diterima : ");
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.println(macStr);
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.printf("Station ID %u: %u bytes\n", myData.id, len);
  // Update the structures with the new incoming data
  boardsStruct[myData.id-1].x = myData.x;
  boardsStruct[myData.id-1].y = myData.y;
  Serial.printf("Data 1 value: %d \n", boardsStruct[myData.id-1].x);
  Serial.printf("Data 2 value: %d \n", boardsStruct[myData.id-1].y);
  Serial.println();
}

void IRAM_ATTR pulseCounter()
{
  pulseCount++;
}

void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);
  
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_AP_STA);
  WiFi.config(ip,gateway,subnet);
  WiFi.begin(ssid,password);
   while (WiFi.status() != WL_CONNECTED)  
     { Serial.print("...");
       delay(200);
     }
  Serial.println("");
     Serial.println("Multi Station WiFi Connected");   
     Serial.print("Multi Station IP address: "); 
     Serial.println(WiFi.localIP());     
     server.begin(); 
  //WiFi.disconnect();

  // Init ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
  esp_now_register_recv_cb(OnDataRecv);

  pinMode(SENSOR, INPUT_PULLUP);
  pulseCount = 0;
  flowRate = 0.0;
  flowMilliLitres = 0;
  totalMilliLitres = 0;
  previousMillis = 0;
 
  attachInterrupt(digitalPinToInterrupt(SENSOR), pulseCounter, FALLING);

}

void loop(){
  WiFiClient client = server.available(); 
  // Access the variables for each board
  /*int board1X = boardsStruct[0].x;
  int board1Y = boardsStruct[0].y;
  int board2X = boardsStruct[1].x;
  int board2Y = boardsStruct[1].y;
  */
  currentMillis = millis();
  if (currentMillis - previousMillis >= interval) 
  {
    
    pulse1Sec = pulseCount;
    pulseCount = 0;
    flowRate = ((1000.0 / (millis() - previousMillis)) * pulse1Sec) / calibrationFactor;
    previousMillis = millis();
    flowMilliLitres = (flowRate / 60) * 1000;
    flowLitres = (flowRate / 60);
//    Serial.print("FLOW RATE :");
//    Serial.print(flowRate);
    // Add the millilitres passed in this second to the cumulative total
    totalMilliLitres += flowMilliLitres;
    totalLitres += flowLitres;
  }

  float oke = 0.5;
  Serial.print("Station ID 1 | ");
  Serial.print("Data Pressure : ");
  Serial.print(oke);
  Serial.print(" | Data Flow : ");
  Serial.print(flowRate);
  Serial.println("");
  delay (1000);
  
if (client)                              
 {   
  while(1) 
  {
     while(client.available())   
     {  

       int board2X = boardsStruct[0].x;
       int board3X = boardsStruct[1].x;
       int board3y = boardsStruct[1].x;
         
       
       currentMillis = millis();
         if (currentMillis - previousMillis >= interval) 
             {
    
                pulse1Sec = pulseCount;
                pulseCount = 0;
                flowRate = ((1000.0 / (millis() - previousMillis)) * pulse1Sec) / calibrationFactor;
                previousMillis = millis();
                flowMilliLitres = (flowRate / 60) * 1000;
                flowLitres = (flowRate / 60);
                // Add the millilitres passed in this second to the cumulative total
                totalMilliLitres += flowMilliLitres;
                totalLitres += flowLitres;
              }
       uint8_t dataout =client.read(); 
       Serial.write(dataout);          
         switch (dataout)                 
          {
            case 'a':                 
            client.print(board2X);   
            Serial.print("Data Station 2 diakses");
            Serial.print(" | Data Pressure");       
            break;
            case'b':  
            client.print(board3X);
            Serial.print("Data Station 3 diakses");
            Serial.print(" | Data Flow");
            break;
            case 'c':                              
            client.print(board3y);  
            Serial.print("Data Station 3 diakses");
            Serial.print(" | Data Pressure");        
            break;
            case 'd':
            client.print(flowRate);
            break;
           }
           if(server.hasClient())  
             { 
              return;               
             }
      }
    }  
  }
}

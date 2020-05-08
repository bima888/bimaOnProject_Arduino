#include "REG_PM2200.h"
#include <ModbusMaster.h>
#include <SoftwareSerial.h>

#define DirectControl 3
int RX;
//#define SSerialRx 10
//#define SSerialTx 11
SoftwareSerial swSer(10, 11);
//SoftwareSerial swSer(0, 1); // RX, TX

ModbusMaster node;
//01 04 00 00 00 02 71 3F // Test 30001
//------------------------------------------------
// konversi data 32bit ke float
//------------------------------------------------
float HexTofloat(uint32_t x) 
{
  return (*(float*)&x);
}
//data yang bentuknya sudah float tetap ke float
uint32_t FloatTohex(float x) 
{
  return (*(uint32_t*)&x);
}
//------------------------------------------------

float Read_Meter_float(char addr , uint16_t  REG) 
{
  float i = 0;//data float mulai dari 0
  uint8_t j, result;//data 8 bit adalah sebagai hasil
  uint16_t data[2];//2 data berarti 16 bit
  uint32_t value = 0;//data 32bit sebagai variable value(hasil) mulai dari 0
  node.begin(addr, swSer);//serial komunikasi dimulai Tx Rx
  result = node.readHoldingRegisters (REG, 2); ///< Modbus function 0x03 Read Holding Registers, untuk pembacaan register general
  delay(500);
  if (result == node.ku8MBSuccess) 
  {
    for (j = 0; j < 2; j++)//2 data pembacaan
    {
      data[j] = node.getResponseBuffer(j);
    }
    value = data[0];//data ke..
    value = value << 16;//data selesai limit bit
    value = value + data[1];//print value 32bit ditambah data ke..
    i = HexTofloat(value);//konversi hasil ke float
    //Serial.println("Connect modbus Ok.");
    return i;
  } else 
  {
    Serial.print("Connect modbus fail. REG >>> "); Serial.println(REG); // Debug
    delay(1000); 
    return 0;
  }
}

void GET_METER() 
{     // Update read all data
  delay(1000);                            
    for (char i = 0; i < Total_of_Reg ; i++)//ambil data dari PM
    {
      DATA_METER [i] = Read_Meter_float(ID_meter, Reg_addr[i]);//masukan hasil rumus i dan terapkan 
    } 
}

//**************************************************************************************************************
void setup() 
{
  Serial.begin(9600);//komuikasi serial mulai
  swSer.begin(9600);//komunikasi RS485 Tx Rx mulai
  Serial.println();
  pinMode(DirectControl, OUTPUT);
  digitalWrite(DirectControl, LOW);
  Serial.println(F("RS485 Modbus RTU Test")); //Indikator
}

void loop() 
{
  //float x = Read_Meter_float(ID_meter,Reg_Volt);adalah hasil dari data ke 1 dan ke 2 yang sudah disetting
  if(swSer.available())
  {
    RX = swSer.read();
    }
  GET_METER();
  Serial.println();
  Serial.print("Volt L-N  = "); Serial.print(DATA_METER[0]);Serial.println(" VAC");
  Serial.print("Frequency =  "); Serial.print(DATA_METER[1]);Serial.println(" Hz");
  delay(3000);
}

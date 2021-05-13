#include <ThingerEthernet.h>
#include <Ethernet.h>
#include <SPI.h>
#include <avr/wdt.h>
#define USERNAME "fransisco"
#define DEVICE_ID "PM2230"
#define DEVICE_CREDENTIAL "polinema2020"
ThingerEthernet thing(USERNAME, DEVICE_ID, DEVICE_CREDENTIAL);

#include <SoftwareSerial.h>
SoftwareSerial serial(10, 11); // TX, RX

#include <SimpleModbusMaster.h>
float A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, AA, AB, AC, AD, AE, AF, AG, AH, AI, AJ;
int P, Q, R, S, T, U, V, W, X;

//////////////////// Port information ///////////////////
#define baud 38400
#define timeout 1000
#define polling 100 // the scan rate
#define retry_count 3
#define arSerial Serial1
#define TxEnablePin 2
#define TOTAL_NO_OF_REGISTERS 35


//void (*fungsi_reset) (void) = 0;

// This is the easiest way to create new packets
// Add as many as you want. TOTAL_NO_OF_PACKETS
// is automatically updated.
enum
{
  PACKET1,
  PACKET2,
  PACKET3,
  PACKET4,
  PACKET5,
  PACKET6,
  PACKET7,
  PACKET8,
  PACKET9,
  PACKET10,
  PACKET11,
  PACKET12,
  PACKET13,
  PACKET14,
  PACKET15,
  PACKET16,
  PACKET17,
  PACKET18,
  PACKET19,
  PACKET20,
  PACKET21,
  PACKET22,
  PACKET23,
  //  PACKETn,

  TOTAL_NO_OF_PACKETS 
};


Packet packets[TOTAL_NO_OF_PACKETS];

// Masters register array
unsigned int regs[TOTAL_NO_OF_REGISTERS];

void setup(void)
{
   
    uint8_t mac[6] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05};
 // Ethernet.begin(mac, IPAddress(125, 160, 77, 36));
   
   // Initialize each packet
   Serial.begin(38400); //READ_HOLDING_REGISTERS 43118
   delay(1000);
   wdt_enable(WDTO_8S);


//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  //  modbus_construct(&packets[PACKET1], 1(adres PM710 sampai ke n), READ_INPUT_REGISTERS, 4010(adress registry), 1(jenis data), 10(pengurutan data sampai ke n/TOTAL_NO_OF_REGISTERS));
  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

  //  modbus_construct(&packets[PACKET1], 1, READ_HOLDING_REGISTERS, 3109, 2, 10); //A-N                              //A

  modbus_construct(&packets[PACKET1], 1, READ_HOLDING_REGISTERS, 3027,  2, 10); //A-N  (VA)                        //R
  modbus_construct(&packets[PACKET2], 1, READ_HOLDING_REGISTERS, 3029,  2, 12); //B-N  (VB)                        //S
  modbus_construct(&packets[PACKET3], 1, READ_HOLDING_REGISTERS, 3031,  2, 14); //C-N  (VC)                        //T
  modbus_construct(&packets[PACKET4], 1, READ_HOLDING_REGISTERS, 3035,  2, 16); //Vavg L-N  (Vavg)                 //n
  modbus_construct(&packets[PACKET5], 1, READ_HOLDING_REGISTERS, 3051,  2, 18); //Vunb L-N  (Vunb)                 //unb
  
  modbus_construct(&packets[PACKET6], 1, READ_HOLDING_REGISTERS, 3019,  2, 20); //A-B  (vAB)                        //RS
  modbus_construct(&packets[PACKET7], 1, READ_HOLDING_REGISTERS, 3021,  2, 22); //B-C  (vBC)                        //ST
  modbus_construct(&packets[PACKET8], 1, READ_HOLDING_REGISTERS, 3023,  2, 24); //C-A  (vCA)                        //TR
  modbus_construct(&packets[PACKET9], 1, READ_HOLDING_REGISTERS, 3025,  2, 26); //Vavg L-L (Vavg)                   //TR
  modbus_construct(&packets[PACKET10], 1, READ_HOLDING_REGISTERS, 3043,  2, 28); //Vunb L-L (Vunb)                   //unb
 
  modbus_construct(&packets[PACKET11], 1, READ_HOLDING_REGISTERS, 3109,  2, 30); //Frekuency (Hz)                  //B
  modbus_construct(&packets[PACKET12], 1, READ_HOLDING_REGISTERS, 21329, 2, 32); //Voltage THD, phase 1 (V.THD)    //C
  modbus_construct(&packets[PACKET13], 1, READ_HOLDING_REGISTERS, 21299, 2, 34); //Current THD, phase 1 (I.THD)    //D
 
  modbus_construct(&packets[PACKET14], 1, READ_HOLDING_REGISTERS, 2999,  2, 36); //CurrentA,(IA)                     //E
  modbus_construct(&packets[PACKET15], 1, READ_HOLDING_REGISTERS, 3001,  2, 38); //CurrentB,(IB)                     //E
  modbus_construct(&packets[PACKET16], 1, READ_HOLDING_REGISTERS, 3003,  2, 40); //CurrentC,(IC)                     //E
  modbus_construct(&packets[PACKET17], 1, READ_HOLDING_REGISTERS, 3005,  2, 42); //CurrentN,(IN)                     //E
  modbus_construct(&packets[PACKET18], 1, READ_HOLDING_REGISTERS, 3009,  2, 44); //CurrentAvg,(Iavg)                     //E
  modbus_construct(&packets[PACKET19], 1, READ_HOLDING_REGISTERS, 3017,  2, 46); //CurrentUnb,(Iunb)                     //E
 
  modbus_construct(&packets[PACKET10], 1, READ_HOLDING_REGISTERS, 3075,  2, 28); //Apparent power, total (VA)       //F
  modbus_construct(&packets[PACKET20], 1, READ_HOLDING_REGISTERS, 3059,  2, 48); //Total active energy (Kw)        //G
  modbus_construct(&packets[PACKET21], 1, READ_HOLDING_REGISTERS, 3067,  2, 50); //Total Reactive energy (KVAR)        //G
  modbus_construct(&packets[PACKET22], 1, READ_HOLDING_REGISTERS, 3075,  2, 52); //Total Apparent energy (KVA)        //G
 
  modbus_construct(&packets[PACKET23], 1, READ_HOLDING_REGISTERS, 3191,  2, 54); //Power factor A(Pf)               //H

  //modbus_construct(&packets[PACKET9], 1, PRESET_MULTIPLE_REGISTERS, 0, 2, 0);
  /*
  3028 // 3027   == Line to neutral voltage (V.AN)
  3110 // 3109   == Frequency, (Hz)
  21330 // 21329 == Voltage THD, phase 1 (V.THD)
  21300 // 21299 == Current THD, phase 1 (I.THD)
  2700 // 2699   == Total active energy (Kwh)
  3070 // 3069   == Apparent power, total (VA)....//DAYA SEMU
  3000 // 2999   == Current, phase 1 (I)
  3192 // 3191   == Power factor A (Pf)
  */

  /*
  Valid modbus byte formats are:
  SERIAL_8N2: 1 start bit, 8 data bits, 2 stop bits
  SERIAL_8E1: 1 start bit, 8 data bits, 1 Even parity bit, 1 stop bit
  SERIAL_8O1: 1 start bit, 8 data bits, 1 Odd parity bit, 1 stop bit
  */

  // Initialize the Modbus Finite State Machine
    modbus_configure(&Serial3, baud, SERIAL_8N1, timeout, polling, retry_count, TxEnablePin, packets, TOTAL_NO_OF_PACKETS, regs);

  thing["frekuensi"] >> [](pson& out){out = K;
   };
  thing["RN"] >> [](pson& out){out = A;
   };  
  thing["SN"] >> [](pson& out){out = B;
   };
  thing["TN"] >> [](pson& out){out = C;
   };
  thing["RS"] >> [](pson& out){out = F;
   };
  thing["ST"] >> [](pson& out){out = G;
   };
  thing["TR"] >> [](pson& out){out = H;
   };
  thing["V_AVG"] >> [](pson& out){out = D;
   };
  thing["ARUS R"] >> [](pson& out){out = N;
   };
  thing["ARUS S"] >> [](pson& out){out = O;
   };
  thing["ARUS T"] >> [](pson& out){out = AA;
   };
  thing["ARUS N"] >> [](pson& out){out = AB;
   };
  thing["VOLTAGE THD"] >> [](pson& out){out = L;
   };
  thing["CURRENT THD"] >> [](pson& out){out = M;
   };
  thing["DAYA AKTIF"] >> [](pson& out){out = AE;
   };
  thing["DAYA REAKTIF"] >> [](pson& out){out = AF;
   };
  thing["FAKTOR DAYA"] >> [](pson& out){out = AH;
   };
  thing["DAYA NYATA"] >> [](pson& out){out = AG;
   };


}
//<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>
//<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>
void loop()
{ 
  thing.handle();   
  modbus_update();

  Serial.println("=======DATA MDPLV PSDKU===========");
  Serial.print("successful_requests \t : ");
  Serial.println(packets[PACKET1].successful_requests);
  Serial.print("failed_requests \t : ");
  Serial.println(packets[PACKET1].failed_requests);
  Serial.print("exception_errors \t : ");
  Serial.println(packets[PACKET1].exception_errors);
  Serial.print("connection \t\t : ");
  Serial.println(packets[PACKET1].connection);
  Serial.println("==================================");

//  J = *(float*)&regs[27]; Serial.print ("(VA)  : "); Serial.println (J);
  A = *(float*)&regs[ 9]; 
  B = *(float*)&regs[11]; 
  C = *(float*)&regs[13]; 
  D = *(float*)&regs[15]; 
  E = *(float*)&regs[17]; 
  
  F = *(float*)&regs[19]; 
  G = *(float*)&regs[21]; 
  H = *(float*)&regs[23]; 
  I = *(float*)&regs[25]; 
  J = *(float*)&regs[27]; 
  
  K = *(float*)&regs[29]; 
  L = *(float*)&regs[31]; 
  M = *(float*)&regs[33];
 
  N = *(float*)&regs[35]; 
  O = *(float*)&regs[37]; 
  AA = *(float*)&regs[39]; 
  AB = *(float*)&regs[41];
  AC = *(float*)&regs[43];
  AD = *(float*)&regs[45];
  
  AE = *(float*)&regs[47];
  AF = *(float*)&regs[49];
  AG = *(float*)&regs[51];

  AH = *(float*)&regs[53];
  
  Serial.print (" VA-N   : "); Serial.println (A);
  Serial.print (" VB-N   : "); Serial.println (B);
  Serial.print (" VC-N   : "); Serial.println (C); 
  Serial.print (" Vavg L-N   : "); Serial.println (D);
  Serial.print (" Vunb L-N   : "); Serial.println (E);Serial.println ("");
  Serial.print (" A-B   : "); Serial.println (F);
  Serial.print (" B-C   : "); Serial.println (G);
  Serial.print (" C-A   : "); Serial.println (H);
  Serial.print (" Vavg L-L  : "); Serial.println (I);
  Serial.print (" Vunb L-L   : "); Serial.println (J);Serial.println ("");
  Serial.print (" Frecq : "); Serial.println (K);
  Serial.print (" V_THD : "); Serial.println (L);
  Serial.print (" I_THD : "); Serial.println (M); Serial.println ("");
  Serial.print (" I-A   : "); Serial.println (N);
  Serial.print (" I-B   : "); Serial.println (O);
  Serial.print (" I-C   : "); Serial.println (AA); 
  Serial.print (" I-N   : "); Serial.println (AB);
  Serial.print (" Iavg   : "); Serial.println (AC);
  Serial.print (" Iunb   : "); Serial.println (AD); Serial.println ("");
  Serial.print (" kW   : "); Serial.println (AE);
  Serial.print (" kVA   : "); Serial.println (AF);
  Serial.print (" kVAR   : "); Serial.println(AG); Serial.println ("");
  Serial.print (" Pf    : "); Serial.println (AH);

  delay(1000);
  panggil_reset();
  delay(5000);
  
}

void panggil_reset()
{
   /*___________FUNGSI RESET____________*/
   
  wdt_reset();
  for(int i = 8; i > 0; i--) //delay hitung mundur 8 detik
            {
              Serial.print("RESET");
              Serial.print("dalam ");
              Serial.print(i);
              wdt_disable();
              delay(1000);
           
            }
         
  /*___________FUNGSI RESET____________*/
  }
  

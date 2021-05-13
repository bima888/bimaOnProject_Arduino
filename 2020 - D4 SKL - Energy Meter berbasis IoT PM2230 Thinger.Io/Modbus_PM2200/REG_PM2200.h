#define ID_meter  1
#define Total_of_Reg  2

#define Reg_LN                3035      //  1. MODBUS offset dari tegangan rata2 L-N (V) 
#define Reg_FQ                3109      //  2. MODBUS offset dari Frekuensi Tegangan (Hz)



uint16_t Reg_addr[2] = {
  Reg_LN,
  Reg_FQ,
};

float DATA_METER [Total_of_Reg] ;

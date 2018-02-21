#include <Wire.h>

#define Pwr_Stage1  9   // Se le dio este roden para identificar fisicamente en la placa las secciones activadas
#define Pwr_Stage2  A0  // Sin embargo su cambio solo implica movimientos estructurales en el codigo 
#define Pwr_Stage3  A1
#define Pwr_Stage4  8
#define Pwr_Stage5  4
#define Pwr_Stage6  3
#define Pwr_Stage7  A2
#define Pwr_Stage8  A3

float Volt_level[255];
int a = 0;
int i=0;
float Volt_Comp;
float Volt_Prev = 5;
float Vt;
int Vin;
byte Screen_Data;
int Data_Value;
byte Volt_Chosen;
int Ch_Out[8];
word Stage_Out[8];
byte Comp = 1;

void setup() {
  
  Serial.begin(9600);
  Wire.begin(9);
  Wire.onReceive(receiveEvent); // register event
  Serial.println(Vt);
  pinMode(Pwr_Stage1, OUTPUT);
  pinMode(Pwr_Stage2, OUTPUT);
  pinMode(Pwr_Stage3, OUTPUT);
  pinMode(Pwr_Stage4, OUTPUT);
  pinMode(Pwr_Stage5, OUTPUT);  
  pinMode(Pwr_Stage6, OUTPUT);
  pinMode(Pwr_Stage7, OUTPUT);
  pinMode(Pwr_Stage8, OUTPUT);
  
  
 
}

void loop() {
  Equation_L();
  Kind_Cutting();
  Voltage_Compare(Vt);
  Matrix_Out(Volt_Chosen);  
  Act_Out(); 
  Rst_DC(); 
}


void receiveEvent(int howMany) {
  while (0 < Wire.available()) { // loop through all but the last
    if(a == 0)
    {
      Vin = Wire.read(); // receive byte as a character
      a++;
      delay(10);
    } 
    else if(a == 1)
    { 
      Screen_Data = Wire.read(); // receive byte as a character
      a = 0;
      delay(10);      
    }  
  }
}

void Equation_L ()
{
  Vt = (Vin-0.8518)/0.8853;
  Serial.println(Vt); 
}

void Kind_Cutting ()
{
  if(Screen_Data == 1)
  {
    Limit_Voltage_Cut();
    Selection_Volt_Cut();
  }
  else if(Screen_Data == 2)
  {
    Limit_Voltage_Blend();
    Selection_Volt_Blend();
  } 
  else if(Screen_Data == 3)
  {
    Limit_Voltage_Coag();
    Selection_Volt_Coag();
  }
}

void Limit_Voltage_Cut()
{
  if(Vt <= 11)
  {
    Vt = 12; 
  }
  else if(Vt >= 51)
  {
    Vt = 50; 
  }
  else
  {
    Vt = Vt;  
  }
}

void Limit_Voltage_Blend()
{
  if(Vt <= 11)
  {
    Vt = 12; 
  }
  else if(Vt >= 61)
  {
    Vt = 60; 
  }
  else
  {
    Vt = Vt;  
  }
}

void Limit_Voltage_Coag()
{
  if(Vt <= 11)
  {
    Vt = 12; 
  }
  else if(Vt >= 71)
  {
    Vt = 70; 
  }
  else
  {
    Vt = Vt;  
  }
}

void Selection_Volt_Cut()
{
  Volt_level[6] = 49.80;
  Volt_level[8] = 45.96;
  Volt_level[7] = 44.68;    
  Volt_level[16] = 43.38;
  Volt_level[9] = 41.66;
  Volt_level[32] = 40.45;
  Volt_level[10] = 39.96;
  Volt_level[17] = 39.59;
  Volt_level[12] = 38.34;
  Volt_level[18] = 38.08;  
  Volt_level[33] = 37.21; 
  Volt_level[11] = 36.80;
  Volt_level[20] = 36.63;
  Volt_level[34] = 35.90;
  Volt_level[19] = 35.25;
  Volt_level[36] = 34.64;
  Volt_level[21] = 34.04;
  Volt_level[35] = 33.43;
  Volt_level[22] = 32.99;
  Volt_level[15] = 32.08;
  Volt_level[24] = 31.49;
  Volt_level[23] = 30.96;
  Volt_level[40] = 30.09;
  Volt_level[39] = 29.62;
  Volt_level[48] = 29.14;
  Volt_level[41] = 28.47;
  Volt_level[28] = 28.17;
  Volt_level[49] = 27.63;
  Volt_level[44] = 27.11;
  Volt_level[68] = 26.60;
  Volt_level[30] = 26.20;
  Volt_level[51] = 25.76;
  Volt_level[53] = 25.20;
  Volt_level[54] = 24.70;
  Volt_level[72] = 24.15;
  Volt_level[55] = 23.71;
  Volt_level[73] = 23.21;
  Volt_level[81] = 22.72;
  Volt_level[60] = 22.27;
  Volt_level[98] = 21.76;
  Volt_level[85] = 21.24;
  Volt_level[101] = 20.73;
  Volt_level[87] = 20.29;
  Volt_level[112] = 19.68;
  Volt_level[106] = 19.20;
  Volt_level[107] = 18.70;
  Volt_level[117] = 18.21;
  Volt_level[120] = 17.71;
  Volt_level[122] = 17.14;
  Volt_level[125] = 16.62;
  Volt_level[127] = 16.15;
  Volt_level[175] = 15.76;
  Volt_level[224] = 15.37;
  Volt_level[226] = 15.00;
  Volt_level[191] = 14.59;
  Volt_level[240] = 14.29;
  Volt_level[223] = 13.71;
  Volt_level[252] = 13.23;
  Volt_level[254] = 13.01;
  Volt_level[255] = 12.87;          
}

void Selection_Volt_Blend()
{
  Volt_level[4] = 60.36;
  Volt_level[3] = 56.13;
  Volt_level[5] = 52.67;;
  Volt_level[6] = 49.80;
  Volt_level[8] = 45.96;
  Volt_level[7] = 44.68;    
  Volt_level[16] = 43.38;
  Volt_level[9] = 41.66;
  Volt_level[32] = 40.45;
  Volt_level[10] = 39.96;
  Volt_level[17] = 39.59;
  Volt_level[12] = 38.34;
  Volt_level[18] = 38.08;  
  Volt_level[33] = 37.21; 
  Volt_level[11] = 36.80;
  Volt_level[20] = 36.63;
  Volt_level[34] = 35.90;
  Volt_level[19] = 35.25;
  Volt_level[36] = 34.64;
  Volt_level[21] = 34.04;
  Volt_level[35] = 33.43;
  Volt_level[22] = 32.99;
  Volt_level[15] = 32.08;
  Volt_level[24] = 31.49;
  Volt_level[23] = 30.96;
  Volt_level[40] = 30.09;
  Volt_level[39] = 29.62;
  Volt_level[48] = 29.14;
  Volt_level[41] = 28.47;
  Volt_level[28] = 28.17;
  Volt_level[49] = 27.63;
  Volt_level[44] = 27.11;
  Volt_level[68] = 26.60;
  Volt_level[30] = 26.20;
  Volt_level[51] = 25.76;
  Volt_level[53] = 25.20;
  Volt_level[54] = 24.70;
  Volt_level[72] = 24.15;
  Volt_level[55] = 23.71;
  Volt_level[73] = 23.21;
  Volt_level[81] = 22.72;
  Volt_level[60] = 22.27;
  Volt_level[98] = 21.76;
  Volt_level[85] = 21.24;
  Volt_level[101] = 20.73;
  Volt_level[87] = 20.29;
  Volt_level[112] = 19.68;
  Volt_level[106] = 19.20;
  Volt_level[107] = 18.70;
  Volt_level[117] = 18.21;
  Volt_level[120] = 17.71;
  Volt_level[122] = 17.14;
  Volt_level[125] = 16.62;
  Volt_level[127] = 16.15;
  Volt_level[175] = 15.76;
  Volt_level[224] = 15.37;
  Volt_level[226] = 15.00;
  Volt_level[191] = 14.59;
  Volt_level[240] = 14.29;
  Volt_level[223] = 13.71;
  Volt_level[252] = 13.23;
  Volt_level[254] = 13.01;
  Volt_level[255] = 12.87;    
}

void Selection_Volt_Coag()
{
  Volt_level[1] = 70.48;
  Volt_level[2] = 65.11;
  Volt_level[4] = 60.36;
  Volt_level[3] = 56.13;
  Volt_level[5] = 52.67;;
  Volt_level[6] = 49.80;
  Volt_level[8] = 45.96;
  Volt_level[7] = 44.68;    
  Volt_level[16] = 43.38;
  Volt_level[9] = 41.66;
  Volt_level[32] = 40.45;
  Volt_level[10] = 39.96;
  Volt_level[17] = 39.59;
  Volt_level[12] = 38.34;
  Volt_level[18] = 38.08;  
  Volt_level[33] = 37.21; 
  Volt_level[11] = 36.80;
  Volt_level[20] = 36.63;
  Volt_level[34] = 35.90;
  Volt_level[19] = 35.25;
  Volt_level[36] = 34.64;
  Volt_level[21] = 34.04;
  Volt_level[35] = 33.43;
  Volt_level[22] = 32.99;
  Volt_level[15] = 32.08;
  Volt_level[24] = 31.49;
  Volt_level[23] = 30.96;
  Volt_level[40] = 30.09;
  Volt_level[39] = 29.62;
  Volt_level[48] = 29.14;
  Volt_level[41] = 28.47;
  Volt_level[28] = 28.17;
  Volt_level[49] = 27.63;
  Volt_level[44] = 27.11;
  Volt_level[68] = 26.60;
  Volt_level[30] = 26.20;
  Volt_level[51] = 25.76;
  Volt_level[53] = 25.20;
  Volt_level[54] = 24.70;
  Volt_level[72] = 24.15;
  Volt_level[55] = 23.71;
  Volt_level[73] = 23.21;
  Volt_level[81] = 22.72;
  Volt_level[60] = 22.27;
  Volt_level[98] = 21.76;
  Volt_level[85] = 21.24;
  Volt_level[101] = 20.73;
  Volt_level[87] = 20.29;
  Volt_level[112] = 19.68;
  Volt_level[106] = 19.20;
  Volt_level[107] = 18.70;
  Volt_level[117] = 18.21;
  Volt_level[120] = 17.71;
  Volt_level[122] = 17.14;
  Volt_level[125] = 16.62;
  Volt_level[127] = 16.15;
  Volt_level[175] = 15.76;
  Volt_level[224] = 15.37;
  Volt_level[226] = 15.00;
  Volt_level[191] = 14.59;
  Volt_level[240] = 14.29;
  Volt_level[223] = 13.71;
  Volt_level[252] = 13.23;
  Volt_level[254] = 13.01;
  Volt_level[255] = 12.87;  
}

void Voltage_Compare(int Volt_Ref)
{
  for (int i=0; i <= 255; i++){
    Volt_Comp = abs(Volt_Ref - Volt_level[i]);
    if(Volt_Comp <= Volt_Prev)
    {
      Volt_Prev = Volt_Comp;
      Volt_Chosen = i; 
    }
  }  
}

void Matrix_Out (byte Selection_Out)
{
  for (int j=0; j <= 7; j++){
    Ch_Out[j] = (Selection_Out >> j)&(Comp); 
  }  
}

void Act_Out()
{
  if(Ch_Out[0] == 1)
  {
    digitalWrite(Pwr_Stage1, HIGH);
  }
  else if(Ch_Out[0] == 0)
  {
    digitalWrite(Pwr_Stage1, LOW);
  }
  if(Ch_Out[1] == 1)
  {
    digitalWrite(Pwr_Stage2, HIGH);
  }    
  else if(Ch_Out[1] == 0)
  {
    digitalWrite(Pwr_Stage2, LOW);
  }  
  if(Ch_Out[2] == 1)
  {
    digitalWrite(Pwr_Stage3, HIGH);
  }
  else if(Ch_Out[2] == 0)
  {
    digitalWrite(Pwr_Stage3, LOW);
  }
  if(Ch_Out[3] == 1)
  {
    digitalWrite(Pwr_Stage4, HIGH);
  }
  else if(Ch_Out[3] == 0)
  {
    digitalWrite(Pwr_Stage4, LOW);
  }    
  if(Ch_Out[4] == 1)
  {
    digitalWrite(Pwr_Stage5, HIGH);
  }
  else if(Ch_Out[4] == 0)
  {
    digitalWrite(Pwr_Stage5, LOW);
  }    
  if(Ch_Out[5] == 1)
  {
    digitalWrite(Pwr_Stage6, HIGH);
  }
  else if(Ch_Out[5] == 0)
  {
    digitalWrite(Pwr_Stage6, LOW);
  }    
  if(Ch_Out[6] == 1)
  {
    digitalWrite(Pwr_Stage7, HIGH);
  }
  else if(Ch_Out[6] == 0)
  {
    digitalWrite(Pwr_Stage7, LOW);
  }    
  if(Ch_Out[7] == 1)
  {
    digitalWrite(Pwr_Stage8, HIGH);
  } 
  else if(Ch_Out[7] == 0)
  {
    digitalWrite(Pwr_Stage8, LOW);
  }                           
}


void Rst_DC ()
{
  Volt_Prev = 5;
  Volt_Comp = 20;
  i=0;   
}









   

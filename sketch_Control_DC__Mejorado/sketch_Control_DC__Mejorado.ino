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
int a;
float Volt_Comp;
float Volt_Prev = 5;
byte Vt=25;
int Data_Value;
byte Volt_Chosen;
int Ch_Out[8];
word Stage_Out[8];
byte Comp = 1;

void setup() {
  
  Serial.begin(9600);
  //Wire.begin(9);
  //Wire.onReceive(receiveEvent); // register event
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
    Assignment_Out();
    Selection_Volt();
    Voltage_Compare(Vt);
    Matrix_Out(Volt_Chosen);  
    Act_Out();  
}


void receiveEvent(int howMany) {
    Vt = Wire.read(); // receive byte as a character
    Serial.print("Vt: "); 
    Serial.println(Vt);
    }


void Selection_Volt()
{
  Volt_level[1] = 49.44;
  Volt_level[2] = 47.72;
  Volt_level[4] = 45.81;
  Volt_level[8] = 43.66;
  Volt_level[16] = 41.24;
  Volt_level[32] = 38.49;
  Volt_level[3] = 35.72;
  Volt_level[5] = 34.74;    
  Volt_level[6] = 33.99;
  Volt_level[9] = 33.61;
  Volt_level[10] = 32.92;
  Volt_level[17] = 32.30;
  Volt_level[18] = 31.67;
  Volt_level[20] = 30.94;
  Volt_level[34] = 30.19;  
  Volt_level[36] = 29.54; 
  Volt_level[40] = 28.78;
  Volt_level[7] = 28.06;
  Volt_level[11] = 27.40;
  Volt_level[13] = 26.89;
  Volt_level[14] = 26.49;
  Volt_level[21] = 26.13;
  Volt_level[22] = 25.76;
  Volt_level[37] = 25.22;
  Volt_level[38] = 24.88;
  Volt_level[65] = 24.29;
  Volt_level[50] = 23.81;
  Volt_level[72] = 23.21;
  Volt_level[80] = 22.70;
  Volt_level[27] = 22.26;
  Volt_level[30] = 21.74;
  Volt_level[46] = 21.18;
  Volt_level[57] = 20.68;
  Volt_level[82] = 20.24;
  Volt_level[31] = 19.71;
  Volt_level[47] = 19.29;
  Volt_level[59] = 18.75;
  Volt_level[86] = 18.25;
  Volt_level[106] = 17.72;
  Volt_level[79] = 17.21;
  Volt_level[93] = 16.71;
  Volt_level[118] = 16.17;
  Volt_level[95] = 15.66;
  Volt_level[123] = 15.19;
  Volt_level[126] = 15.03;
  Volt_level[127] = 14.34;
  Volt_level[255] = 12.43;          
}

void Assignment_Out()
{
  Stage_Out[0] = Pwr_Stage1; 
  Stage_Out[1] = Pwr_Stage2; 
  Stage_Out[2] = Pwr_Stage3; 
  Stage_Out[3] = Pwr_Stage4; 
  Stage_Out[4] = Pwr_Stage5; 
  Stage_Out[5] = Pwr_Stage6; 
  Stage_Out[6] = Pwr_Stage7;
  Stage_Out[7] = Pwr_Stage8;          
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
//    Serial.println("Volt_Chosen");    
//    Serial.println(Volt_Chosen);
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
  for (int k=0; k <= 7; k++){
    if(Ch_Out[k] == 1)
    {
      digitalWrite(Stage_Out[k], HIGH); 
    }
    else if(Ch_Out[k] == 0)
    {
      digitalWrite(Stage_Out[k], LOW);        
    }
  }    
}


   

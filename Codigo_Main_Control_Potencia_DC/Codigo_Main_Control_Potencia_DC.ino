#include <Wire.h>

int Val;
int Vm;
int Vt;
int Frec_Util;
float C_Util;
int Pot_Val;
int Bio_Val;
int Modo_Corte_Val;

void setup() 
{
  Serial.begin(9600);
  Wire.begin();
}

void loop() 
{
  
Periodo_Util(Modo_Corte_Val);
Calc_Power(Pot_Val, Bio_Val, C_Util);
Transmition_Control_DC();

}

void Periodo_Util (int Signal_Type)                         // Obtencion del periodo del ciclo util de la onda de modulacion          
{
  if(Signal_Type == 1)
  {
    Frec_Util = 1;  
  }
  else if(Signal_Type == 2)
  {
    Frec_Util = 0.95;  
  }
  else if(Signal_Type == 3)
  {
    Frec_Util = 0.9;  
  }
  else if(Signal_Type == 4)
  {
    Frec_Util = 0.8;  
  }
  else if(Signal_Type == 5)
  {
    Frec_Util = 0.75;  
  }
  else if(Signal_Type == 6)
  {
    Frec_Util = 0.7;  
  } 
  C_Util = 1/Frec_Util;
}

void Calc_Power (long P_Value, long B_Value, float C_Val)
{  
  Val = B_Value * P_Value; 
  Vm = sqrt(2*Val*C_Val);
  Vt = (Vm*2)/13;                         // Los valores de Factor de uso Fac_Use se calcularon al obtener la formula Vrms = Vamp/(sqrt(2*Tciclo util))   
  Serial.println(Vt);
}

void Transmition_Control_DC ()
{
    Wire.beginTransmission(9); // transmit to device #9
    Wire.write(Vt);
    Wire.endTransmission();    // stop transmitting 
}


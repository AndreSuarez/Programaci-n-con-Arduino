#include <Wire.h>

// Todos los pines disponibles, por si necesita mas son: 8,9,14,15,16,17. Estan tambien las entradas al A/D Converter: A6,A7

#define ECG_Detection 16 
#define Alarma1 15
#define Alarma2 14

int Val;
long Vm;
long Vt;
int Frec_Util;
float C_Util;
int Pot_Val = 100;
int Bio_Val = 300;
int Modo_Corte_Val = 1;

void setup() 
{
  Serial.begin(9600);
  Wire.begin();
  pinMode(ECG_Detection, INPUT_PULLUP);
  pinMode(Alarma1, OUTPUT);
  pinMode(Alarma2, OUTPUT);
}

void loop() 
{

  digitalWrite(Alarma1, LOW);                             // Desactivaion Sonido Alarma 1
  digitalWrite(Alarma2, LOW);                             // Desactivaion Sonido Alarma 2
  Periodo_Util(Modo_Corte_Val);                           // Funcion para obteber el valor del periodo en calculo de potencia, a partir del tipo de corte          
  Calc_Power(Pot_Val, Bio_Val, C_Util);                   // Funcion de Calculo de Potencia     
  Transmition_Control_DC();                               // Transmision valor de tension del calculo de potencia           
  digitalWrite(Alarma1, HIGH);                            // Activaion Sonido Alarma 1   
  digitalWrite(Alarma2, HIGH);                            // Activaion Sonido Alarma 2 

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
  Vm = sqrt(2)*sqrt(Val)*sqrt(C_Val);
  Vt = (Vm*2)/13;                         // Se obtiene un unico valor a transmitir Vt que es la tension de salida a cuadrar   
  Serial.println(Vt);
}

void Transmition_Control_DC ()
{
  Wire.beginTransmission(9); // transmit to device #9
  Wire.write(Vt);               // Tension de Salida
  Wire.endTransmission();    // stop transmitting
}


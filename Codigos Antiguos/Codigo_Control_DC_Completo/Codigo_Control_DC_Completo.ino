
#include <Wire.h>

#define Pwr_Stage1  9              // Se le dio este roden para identificar fisicamente en la placa las secciones activadas
#define Pwr_Stage2  A0             // Sin embargo su cambio solo implica movimientos estructurales en el codigo 
#define Pwr_Stage3  A1
#define Pwr_Stage4  8
#define Pwr_Stage5  4
#define Pwr_Stage6  3
#define Pwr_Stage7  A2
#define Pwr_Stage8  A3 

long Pot_Value;              // *Valor de potencia en Watts traducida del valor asociado en la pantalla tactil requerido para calculo de Tension de Salida
int Ciclo_Value;              // Valor de Tipo de Corte o Coagulacion asignado desde la pantalla
float C_util;                       // Valor del Ciclo util asignado a partir del tipo de corte o coagulacion asignado
long Bio_Value;                   // Valor de bioimpedancia recibido para calcular la tension a colocar 
int Bio_ValF_Prev;                // Valor impedancia previo  
int Bio_ValF;                     // Bits mas significativos del valor de Bioimpedancia
int Bio_ValS;                     // Bits menos significativos del valor de Bioimpedancia
float Volt_Value;                 // *Valor de tension obtenido a partir de los datos de Bioimpedancia y Potencia desde  la pantalla requerido para calculo de Tension de Salida
int Tutil;                        // Valor extraido del dato recopilado de la frecuencia de Modulacion
int T_Value;                      // *Valor de periodo requerido para calculo de Tension de Salida   
int Volt_level[256];              // Matriz de valores de tension Elegibles   
int b;                            // Contador para la recopilacion de datos por I2C
int i = 0;                        // Contador de valor de referencia    
float Volt_Comp;                    // Valor de comparacion usado para determinar el dato de la Matriz de tensiones fijas mas cercano al valor de salida requerido 
int Volt_Prev = 5;                // Valor de inicial comparacion para obtener la tension de comparacion correcta  
byte Volt_Chosen;                 // Voltaje elegido en el proceso de comparacion hecho
int Ch_Out[8];                    // Matriz de conversion de la salida en bits a valores individuales
int Stage_Out[8];                 // Valores a colocar en alto o bajo para las salidas digitales/analogicas 
long Val;                          // Valor intermedio calculo de tension, solo de apoyo
long Vrms;                         // Valor Vrms calculado con los datos ingresados     
float Vamp;                         // Valor DC requerido a la salida de la fuente conmutada
float FrecC_Util;                    // Frecuencia de Modulacion elegida dependiendo del tipo de corte      
int Data_Value[5];                // Matriz de Datos obtenidos del sistema via I2C
int Comp = 1;
int Pass_Value = 0;

void setup() {                    

  Serial.begin(9600);
  Wire.begin(9);                // join i2c bus with address #8
  pinMode(Pwr_Stage1, OUTPUT);
  pinMode(Pwr_Stage2, OUTPUT);
  pinMode(Pwr_Stage3, OUTPUT);
  pinMode(Pwr_Stage4, OUTPUT);
  pinMode(Pwr_Stage5, OUTPUT);  
  pinMode(Pwr_Stage6, OUTPUT);
  pinMode(Pwr_Stage7, OUTPUT);
  pinMode(Pwr_Stage8, OUTPUT);
  Assignment_Out(); 
  Previous_State();
  Selection_Volt();     
}

void loop() { 
  
  Wire.onReceive(receiveEvent); // register event
  Bio_Function();
  Serial.println(Bio_Value);
  Serial.println(Pot_Value);
  Serial.println(Ciclo_Value);
  Periodo_Util(Ciclo_Value);                         // Funcion de calculo de ciclo util        
  Calc_Power(Pot_Value, Bio_Value, C_util);       // Funcion de calculo de tension para la salida previa de la fuente conmutada           
  Comparate_Stage(Volt_Value);                     // Funcion de calculo de resistencia y asignacion de canales activados para el valor de tension requerido 


}

void Selection_Volt ()
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

void receiveEvent(int howMany) {
  while (0 < Wire.available()) { // loop through all but the last

    Data_Value[b] = Wire.read(); // receive byte as a character
    if(b < 5)
    {
      if(b == 0)
      {
        Pot_Value = Data_Value[b]; 
      }
      if(b == 1)
      {
        Bio_ValF_Prev = Data_Value[b]; 
      }
      if(b == 2)
      {
        Bio_ValS = Data_Value[b];    
      }
      if(b == 3)
      {
        Ciclo_Value = Data_Value[b];    
      } 
      if(b == 4)
      {
        Pass_Value = Data_Value[b];    
      }                         
      b++;    
    }
    else if(b >=5)
    {
      b = 0;
    }
  }
}

void Bio_Function()
{
  Bio_ValF = (Bio_ValF_Prev << 8);
  Bio_Value = (Bio_ValF | Bio_ValS);    
}


void Periodo_Util (int Signal_Type)                         // Obtencion del periodo del ciclo util de la onda de modulacion          
{
  if(Signal_Type == 1)
  {
    FrecC_Util = 1;  
  }
  else if(Signal_Type == 2)
  {
    FrecC_Util = 0.95;  
  }
  else if(Signal_Type == 3)
  {
    FrecC_Util = 0.9;  
  }
  else if(Signal_Type == 4)
  {
    FrecC_Util = 0.8;  
  }
  else if(Signal_Type == 5)
  {
    FrecC_Util = 0.75;  
  }
  else if(Signal_Type == 6)
  {
    FrecC_Util = 0.7;  
  } 
  C_util = 1/FrecC_Util;
}

void Calc_Power (long P_Value, long B_Value, float Cycle_Value)
{  
  Val = Bio_Value * P_Value; 
  Vrms = sqrt(Val);
  Vamp = Vrms/(sqrt(2*Cycle_Value));                         // Los valores de Factor de uso Fac_Use se calcularon al obtener la formula Vrms = Vamp/(sqrt(2*Tciclo util))   
  Volt_Value = Vamp/13;
  Serial.println(Volt_Value);
}

void Comparate_Stage(int VoltIn)                          // Etapa de eleccion de tension correspodiente a colocar a la salida de la fuente conmutada
{    
  Serial.println(VoltIn);  
  Voltage_Compare(VoltIn);
  Matrix_Out(Volt_Chosen);
  Assignment_Out();               // Asignacion de los valores de la matriz de salida con los puertos fisicos de salida
  Act_Out();  
  Rst_DC();
}

void Voltage_Compare(int Volt_Ref)                    
{
  for (int i=0; i <= 100; i++)                  // Comparador de los valores disponibles de tension con el valor requerido
  { 
    Volt_Comp = abs(Volt_Ref - Volt_level[i]);
    if(Volt_Comp <= Volt_Prev)
    {
      Volt_Prev = Volt_Comp;                    // Valor elegido mas cercano
      Volt_Chosen = i;                    // Ubicacion del valor elegido en la matriz de valores disponibles

    }
  } 
}

void Matrix_Out (byte Selection_Out)           // Conversion de dato binario o bits a datos individual en una matriz
{
  for (int j=0; j <= 7; j++)
  {                      
    Ch_Out[j] = (Selection_Out >> j)&(Comp); 
  }  
}

void Act_Out()
{
  for (int k=0; k <= 7; k++)                    // Actuador para las salidas fisicas del Atmega
  {
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

void Rst_DC ()
{
  Volt_Prev = 5;
  Volt_Comp = 20;
  i=0;   
}

void Previous_State ()
{
    digitalWrite(Stage_Out[0], HIGH);
    digitalWrite(Stage_Out[1], HIGH);
    digitalWrite(Stage_Out[2], HIGH); 
    digitalWrite(Stage_Out[3], HIGH); 
    digitalWrite(Stage_Out[4], HIGH); 
    digitalWrite(Stage_Out[5], HIGH); 
    digitalWrite(Stage_Out[6], HIGH);
    digitalWrite(Stage_Out[7], HIGH); 
}

   

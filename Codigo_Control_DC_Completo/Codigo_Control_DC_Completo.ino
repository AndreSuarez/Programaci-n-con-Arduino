
#include <Wire.h>

#define Pwr_Stage1  9   // Se le dio este roden para identificar fisicamente en la placa las secciones activadas
#define Pwr_Stage2  A0  // Sin embargo su cambio solo implica movimientos estructurales en el codigo 
#define Pwr_Stage3  A1
#define Pwr_Stage4  8
#define Pwr_Stage5  4
#define Pwr_Stage6  3
#define Pwr_Stage7  A2
#define Pwr_Stage8  A3

int Power_Rcv = 0;                // Dato de potencia requerida por la pantalla
int Power_Value = 0;              // *Valor de potencia en Watts traducida del valor asociado en la pantalla tactil requerido para calculo de Tension de Salida
int Power_Data;                   // Valor de potencia para asignarlo a la etapa correcta    
int Bio_Value = 0;                // Valor de bioimpedancia recibido para calcular la tension a colocar      
int Volt_Value;                   // *Valor de tension obtenido a partir de los datos de Bioimpedancia y Potencia desde  la pantalla requerido para calculo de Tension de Salida
int Tutil;                        // Valor extraido del dato recopilado de la frecuencia de Modulacion
int T_Value;                      // *Valor de periodo requerido para calculo de Tension de Salida   
int Volt_level[256];              // Matriz de valores de tension Elegibles   
int b;                            // Contador para la recopilacion de datos por I2C
int Volt_Comp;                    // Valor de comparacion usado para determinar el dato de la Matriz de tensiones fijas mas cercano al valor de salida requerido 
int Volt_Prev = 5;                // Valor de inicial comparacion para obtener la tension de comparacion correcta  
byte Volt_Chosen;                 // Voltaje elegido en el proceso de comparacion hecho
int Ch_Out[8];                    // Matriz de conversion de la salida en bits a valores individuales
int Stage_Out[8];                 // Valores a colocar en alto o bajo para las salidas digitales/analogicas 
int Val;                          // Valor intermedio calculo de tension, solo de apoyo
int Vrms;                         // Valor Vrms calculado con los datos ingresados     
int Vamp;                         // Valor DC requerido a la salida de la fuente conmutada
int Frecuency;                    // Frecuencia de Modulacion       
int Data_Value[2];                // Matriz de Datos obtenidos del sistema via I2C

void setup() {                    

  Assignment_Out();               // Asignacion de los valores de la matriz de salida con los puertos fisicos de salida
  Selection_Volt();
  
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

  while(Power_Rcv == 0)            // Primera etapa, recopilacion de datos de Potencia proveniente de la Pantalla
  {
    Wire.onReceive(receiveEvent); // register event
    if(Power_Data > 0)
    {
      Power_Rcv = 1;
      b = 1;
    }
  }

  while(Power_Rcv == 1)            // Segunda etapa, recopilacion de datos del Bioimpedanciometro
  {
    Wire.onReceive(receiveEvent); // register event
    if(Bio_Value > 0)
    {
      Power_Rcv = 2;
      b = 0;
    }
  }

  while(Power_Rcv == 2)            // Tercera etapa, procesamiento de datos recopilados y posterior salidas fisicas
  {
    Power_Stage(Power_Data);                         // Funcion de asignacion de potencia en Watts         
    Periodo_Util(Frecuency);                         // Funcion de calculo de ciclo util        
    Calc_Power(Power_Value, Bio_Value, Tutil);       // Funcion de calculo de tension para la salida previa de la fuente conmutada           
    Comparate_Stage(Volt_Value);                     // Funcion de calculo de resistencia y asignacion de canales activados para el valor de tension requerido    
  }
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
  while (1 < Wire.available())    // loop through all but the last
  {                               
    Data_Value[b] = Wire.read();      // receive a byte as character
    Serial.print(Power_Value);              // print the character
  }
  if(b == 0)
  {
    Power_Data = Data_Value[b];
  }
  else if(b == 1)
  {
    Bio_Value = Data_Value[b];
  }
  else
  {
    Serial.println("Error de Serie");
  }
}

void Power_Stage (int P_Data)                            // Etapa de subdivision de los rangos de comparacion para los datos de la Pantalla
{
  if((P_Data > 0)&&(P_Data <= 20))
  {
    Choose_Pot1(P_Data);  
  }
  else if((P_Data > 20)&&(P_Data <= 40))
  {
    Choose_Pot2(P_Data);      
  }
  else if((P_Data > 40)&&(P_Data <= 60))
  {
    Choose_Pot3(P_Data);      
  }
  else if((P_Data > 60)&&(P_Data <= 80))
  {
    Choose_Pot4(P_Data);      
  }
  else if((P_Data > 80)&&(P_Data <= 100))
  {
    Choose_Pot5(P_Data);      
  }  
}

void Choose_Pot1 (int Pot_Comp)                           // Comparador #1  
{
  if((Pot_Comp == 1)||(Pot_Comp == 2))
  {
    Power_Value = 50;    
  }
  else if((Pot_Comp == 3)||(Pot_Comp == 4))
  {
    Power_Value = 51;    
  }
  else if((Pot_Comp == 5)||(Pot_Comp == 6))
  {
    Power_Value = 52;    
  }
  else if((Pot_Comp == 7)||(Pot_Comp == 8))
  {
    Power_Value = 53;    
  }
  else if((Pot_Comp == 9)||(Pot_Comp == 10))
  {
    Power_Value = 54;    
  }
  else if((Pot_Comp == 11)||(Pot_Comp == 12))
  {
    Power_Value = 55;    
  }
  else if((Pot_Comp == 13)||(Pot_Comp == 14))
  {
    Power_Value = 56;    
  }
  else if((Pot_Comp == 15)||(Pot_Comp == 16))
  {
    Power_Value = 57;    
  }
  else if((Pot_Comp == 17)||(Pot_Comp == 18))
  {
    Power_Value = 58;    
  }
  else if((Pot_Comp == 19)||(Pot_Comp == 20))
  {
    Power_Value = 59;    
  }  
}

void Choose_Pot2 (int Pot_Comp)                             // Comparador #2
{
  if((Pot_Comp == 21)||(Pot_Comp == 22))
  {
    Power_Value = 60;    
  }
  else if((Pot_Comp == 23)||(Pot_Comp == 24))
  {
    Power_Value = 61;    
  }
  else if((Pot_Comp == 25)||(Pot_Comp == 26))
  {
    Power_Value = 62;    
  }
  else if((Pot_Comp == 27)||(Pot_Comp == 28))
  {
    Power_Value = 63;    
  }
  else if((Pot_Comp == 29)||(Pot_Comp == 30))
  {
    Power_Value = 64;    
  }
  else if((Pot_Comp == 31)||(Pot_Comp == 32))
  {
    Power_Value = 65;    
  }
  else if((Pot_Comp == 33)||(Pot_Comp == 34))
  {
    Power_Value = 66;    
  }
  else if((Pot_Comp == 35)||(Pot_Comp == 36))
  {
    Power_Value = 67;    
  }
  else if((Pot_Comp == 37)||(Pot_Comp == 38))
  {
    Power_Value = 68;    
  }
  else if((Pot_Comp == 39)||(Pot_Comp == 40))
  {
    Power_Value = 69;    
  }   
}

void Choose_Pot3 (int Pot_Comp)                                   // Comparador #3
{
  if((Pot_Comp == 41)||(Pot_Comp == 42))
  {
    Power_Value = 70;    
  }
  else if((Pot_Comp == 43)||(Pot_Comp == 44))
  {
    Power_Value = 71;    
  }
  else if((Pot_Comp == 45)||(Pot_Comp == 46))
  {
    Power_Value = 72;    
  }
  else if((Pot_Comp == 47)||(Pot_Comp == 48))
  {
    Power_Value = 73;    
  }
  else if((Pot_Comp == 49)||(Pot_Comp == 50))
  {
    Power_Value = 74;    
  }
  else if((Pot_Comp == 51)||(Pot_Comp == 52))
  {
    Power_Value = 75;    
  }
  else if((Pot_Comp == 53)||(Pot_Comp == 54))
  {
    Power_Value = 76;    
  }
  else if((Pot_Comp == 55)||(Pot_Comp == 56))
  {
    Power_Value = 77;    
  }
  else if((Pot_Comp == 57)||(Pot_Comp == 58))
  {
    Power_Value = 78;    
  }
  else if((Pot_Comp == 59)||(Pot_Comp == 60))
  {
    Power_Value = 79;    
  }   
}

void Choose_Pot4 (int Pot_Comp)                                 // Comparador #4    
{
  if((Pot_Comp == 61)||(Pot_Comp == 62))
  {
    Power_Value = 80;    
  }
  else if((Pot_Comp == 63)||(Pot_Comp == 64))
  {
    Power_Value = 81;    
  }
  else if((Pot_Comp == 65)||(Pot_Comp == 66))
  {
    Power_Value = 82;    
  }
  else if((Pot_Comp == 67)||(Pot_Comp == 68))
  {
    Power_Value = 83;    
  }
  else if((Pot_Comp == 69)||(Pot_Comp == 70))
  {
    Power_Value = 84;    
  }
  else if((Pot_Comp == 71)||(Pot_Comp == 72))
  {
    Power_Value = 85;    
  }
  else if((Pot_Comp == 73)||(Pot_Comp == 74))
  {
    Power_Value = 86;    
  }
  else if((Pot_Comp == 75)||(Pot_Comp == 76))
  {
    Power_Value = 87;    
  }
  else if((Pot_Comp == 77)||(Pot_Comp == 78))
  {
    Power_Value = 88;    
  }
  else if((Pot_Comp == 79)||(Pot_Comp == 80))
  {
    Power_Value = 89;    
  }   
}


void Choose_Pot5 (int Pot_Comp)                              // Comparador #5  
{
  if((Pot_Comp == 81)||(Pot_Comp == 82))
  {
    Power_Value = 90;    
  }
  else if((Pot_Comp == 83)||(Pot_Comp == 84))
  {
    Power_Value = 91;    
  }
  else if((Pot_Comp == 85)||(Pot_Comp == 86))
  {
    Power_Value = 92;    
  }
  else if((Pot_Comp == 87)||(Pot_Comp == 88))
  {
    Power_Value = 93;    
  }
  else if((Pot_Comp == 89)||(Pot_Comp == 90))
  {
    Power_Value = 94;    
  }
  else if((Pot_Comp == 91)||(Pot_Comp == 92))
  {
    Power_Value = 95;    
  }
  else if((Pot_Comp == 93)||(Pot_Comp == 94))
  {
    Power_Value = 96;    
  }
  else if((Pot_Comp == 95)||(Pot_Comp == 96))
  {
    Power_Value = 97;    
  }
  else if((Pot_Comp == 97)||(Pot_Comp == 98))
  {
    Power_Value = 98;    
  }
  else if((Pot_Comp == 99)||(Pot_Comp == 100))
  {
    Power_Value = 99;    
  }   
}

void Periodo_Util (int FrecC_Util)                         // Obtencion del periodo del ciclo util de la onda de modulacion          
{
  Tutil = 1/FrecC_Util;
}

void Calc_Power (int P_Value, int B_Value, int T_Value)
{
  Val = Bio_Value * P_Value;                               // Si no se quiere manejar el valor del Bioimpedanciometro Z se coloca como un valor estandar en lugar de como una entrada 
  Vrms = sqrt(Val);
  Vamp = Vrms * (sqrt(2*T_Value));                         // Los valores de Factor de uso Fac_Use se calcularon al obtener la formula Vrms = Vamp/(sqrt(2*Tciclo util))   
  Volt_Value = Vamp;
}

void Comparate_Stage(int VoltIn)                          // Etapa de eleccion de tension correspodiente a colocar a la salida de la fuente conmutada
{    
  Voltage_Compare(Volt_Value);
  Matrix_Out(Volt_Chosen);
  Act_Out();  
}

void Voltage_Compare(int Volt_Ref)                    
{
  for (int i=0; i <= 100; i++)                  // Comparador de los valores disponibles de tension con el valor requerido
  { 
    Volt_Comp = abs(Volt_Ref - Volt_level[i]);
    if(Volt_Comp <= Volt_Prev)
    {
      Volt_Prev = Volt_Comp;                    // Valor elegido mas cercano
      Volt_Chosen = byte(i);                    // Ubicacion del valor elegido en la matriz de valores disponibles
    }
    else 
    {
      Volt_Prev = Volt_Prev;
      Volt_Chosen = Volt_Chosen;
    }
  }  
}

void Matrix_Out (byte Selection_Out)
{
  for (int j=0; j <= 7; j++)                    // Conversion de dato binario o bits a datos individual en una matriz  
  {
    Ch_Out[j] = Selection_Out >> j; 
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


   

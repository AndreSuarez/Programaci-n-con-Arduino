#include <Wire.h>

// Varoles de Tension para las diferentes posiciones de los arreglos de Resistencias


#define Pwr_Stage1  9   // Se le dio este roden para identificar fisicamente en la placa las secciones activadas
#define Pwr_Stage2  A0  // Sin embargo su cambio solo implica movimientos estructurales en el codigo 
#define Pwr_Stage3  A1
#define Pwr_Stage4  8
#define Pwr_Stage5  4
#define Pwr_Stage6  3
#define Pwr_Stage7  A2
#define Pwr_Stage8  A3

// Variable Constantes correspodientes a Tensiones de Salida Conmutada

const float V_Conf1 = 12.3; // Son todos los valores obtenidos en tension como resultado de las combinaciones de resistencias   
const float V_Conf2 = 13.5; // en la salida de la fuente conmutada
const float V_Conf3 = 14.7;
const float V_Conf4 = 15.5;
const float V_Conf5 = 16.5;
const float V_Conf6 = 17.5;
const float V_Conf7 = 18.6; 
const float V_Conf8 = 19.5;
const float V_Conf9 = 20.5;
const float V_Conf10 = 21.5;
const float V_Conf11 = 22.5;
const float V_Conf12 = 23.5;
const float V_Conf13 = 24.4;
const float V_Conf14 = 25.5;
const float V_Conf15 = 26.5;
const float V_Conf16 = 27.6;
const float V_Conf17 = 28.4;
const float V_Conf18 = 29.5;
const float V_Conf19 = 30.8;
const float V_Conf20 = 31.6;
const float V_Conf21 = 32.3;
const float V_Conf22 = 35;
const float V_Conf23 = 36.4;
const float V_Conf24 = 37.6;
const float V_Conf25 = 38.9;
const float V_Conf26 = 40.1; 
const float V_Conf27 = 41.2;
const float V_Conf28 = 43.8;
const float V_Conf29 = 46.4;
const float V_Conf30 = 49.1;
const float V_Conf31 = 51.7;
const float V_Conf32 = 54.3;
const float V_Conf33 = 56.9;
const float V_Conf34 = 59.6;

// Variables no Constantes

int Power_Rcv = 0;                // Dato de potencia requerida por la pantalla
int Power_Value = 0;              // Valor de potencia en Watts traducida del valor asociado en la pantalla tactil
int Power_Data;                   // Valor de potencia para asignarlo a la etapa correcta    
int Bio_Value = 0;                // Valor de bioimpedancia recibido para calcular la tension a colocar      
int Volt_Value;                   // Valor de tension requerido a partir de los datos de Bioimpedancia y Potencia desde  la pantalla       
int T_Value;                      // Valor de Ciclo util obtenido
int Tutil;                        // Valor de Periodo util a usar a partir del calculo con frecuencia de modulacion
int Val;                          // Valor intermedio calculo de tension, solo de apoyo
int Vrms;                         // Valor Vrms calculado con los datos ingresados     
int Vamp;                         // Valor DC requerido a la salida de la fuente conmutada     
int Frecuency;                    // Frecuencia de Modulacion       
int Data_Value[2];                // Matriz de Datos obtenidos del sistema via I2C                  
int i = 0;                      

void setup() {

  Wire.begin(8);                // join i2c bus with address #8
  Serial.begin(9600);           // start serial for output
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

  while(Power_Rcv == 0)
  {
    Wire.onReceive(receiveEvent); // register event
    if(Power_Data > 0)
    {
      Power_Rcv = 1;
      i++;
    }
  }

  while(Power_Rcv == 1)
  {
    Wire.onReceive(receiveEvent); // register event
    if(Bio_Value > 0)
    {
      Power_Rcv = 2;
      i = 0;
    }
  }

  while(Power_Rcv == 2)
  {
    Power_Stage(Power_Data);                         // Funcion de asignacion de potencia en Watts         
    Periodo_Util(Frecuency);                         // Funcion de calculo de ciclo util        
    Calc_Power(Power_Value, Bio_Value, T_Value);     // Funcion de calculo de tension para la salida previa de la fuente conmutada           
    Comparate_Stage(Volt_Value);                     // Funcion de calculo de resistencia y asignacion de canales activados para el valor de tension requerido    
  }
}

void receiveEvent(int howMany) {
  while (1 < Wire.available())    // loop through all but the last
  {                               
    Data_Value[i] = Wire.read();      // receive a byte as character
    Serial.print(Power_Value);              // print the character
  }
  if(i == 0)
  {
    Power_Data = Data_Value[i];
  }
  else if(i == 1)
  {
    Bio_Value = Data_Value[i];
  }
  else
  {
    Serial.println("Error de Serie");
  }
}

void Power_Stage (int P_Data)
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



void Calc_Power (int P_Value, int Bio_Value, int T)
{
  Val = Bio_Value * P_Value;                               // Si no se quiere manejar el valor del Bioimpedanciometro Z se coloca como un valor estandar en lugar de como una entrada 
  Vrms = sqrt(Val);
  Vamp = Vrms * (sqrt(2*T));                               // Los valores de Factor de uso Fac_Use se calcularon al obtener la formula Vrms = Vamp/(sqrt(2*Tciclo util))   
  Volt_Value = Vamp;
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

void Periodo_Util (int FrecC_Util)
{
  Tutil = 1/FrecC_Util;
}

void Comparate_Stage(int V_Value)                           // Activador de salida digitales
{
  if((V_Value > 0)&&(V_Value <= V_Conf1)) 
  { 
    digitalWrite(Pwr_Stage1, HIGH);
    digitalWrite(Pwr_Stage2, HIGH);
    digitalWrite(Pwr_Stage3, HIGH);
    digitalWrite(Pwr_Stage4, HIGH);
    digitalWrite(Pwr_Stage5, HIGH);
    digitalWrite(Pwr_Stage6, HIGH);
    digitalWrite(Pwr_Stage7, HIGH);
    digitalWrite(Pwr_Stage8, HIGH);           
  }
  else if((V_Value > V_Conf1)&&(V_Value <= V_Conf2)) 
  {
    digitalWrite(Pwr_Stage1, HIGH);
    digitalWrite(Pwr_Stage2, HIGH);
    digitalWrite(Pwr_Stage3, HIGH);
    digitalWrite(Pwr_Stage4, HIGH);
    digitalWrite(Pwr_Stage5, HIGH);
    digitalWrite(Pwr_Stage6, HIGH);
    digitalWrite(Pwr_Stage7, HIGH);
    digitalWrite(Pwr_Stage8, LOW);           
  }
  else if((V_Value > V_Conf2)&&(V_Value <= V_Conf3)) 
  {
    digitalWrite(Pwr_Stage1, HIGH);
    digitalWrite(Pwr_Stage2, HIGH);
    digitalWrite(Pwr_Stage3, LOW);
    digitalWrite(Pwr_Stage4, HIGH);
    digitalWrite(Pwr_Stage5, HIGH);
    digitalWrite(Pwr_Stage6, HIGH);
    digitalWrite(Pwr_Stage7, HIGH);
    digitalWrite(Pwr_Stage8, LOW);          
  }
  else if((V_Value > V_Conf3)&&(V_Value <= V_Conf4)) 
  {
    digitalWrite(Pwr_Stage1, LOW);
    digitalWrite(Pwr_Stage2, HIGH);
    digitalWrite(Pwr_Stage3, HIGH);
    digitalWrite(Pwr_Stage4, LOW);
    digitalWrite(Pwr_Stage5, HIGH);
    digitalWrite(Pwr_Stage6, HIGH);
    digitalWrite(Pwr_Stage7, HIGH);
    digitalWrite(Pwr_Stage8, LOW);           
  }
  else if((V_Value > V_Conf4)&&(V_Value <= V_Conf5)) 
  {
    digitalWrite(Pwr_Stage1, HIGH);
    digitalWrite(Pwr_Stage2, HIGH);
    digitalWrite(Pwr_Stage3, LOW);
    digitalWrite(Pwr_Stage4, HIGH);
    digitalWrite(Pwr_Stage5, LOW);
    digitalWrite(Pwr_Stage6, HIGH);
    digitalWrite(Pwr_Stage7, HIGH);
    digitalWrite(Pwr_Stage8, LOW);               
  }
  else if((V_Value > V_Conf5)&&(V_Value <= V_Conf6)) 
  {
    digitalWrite(Pwr_Stage1, LOW);
    digitalWrite(Pwr_Stage2, HIGH);
    digitalWrite(Pwr_Stage3, LOW);
    digitalWrite(Pwr_Stage4, LOW);
    digitalWrite(Pwr_Stage5, HIGH);
    digitalWrite(Pwr_Stage6, HIGH);
    digitalWrite(Pwr_Stage7, HIGH);
    digitalWrite(Pwr_Stage8, LOW);             
  }
  else if((V_Value > V_Conf6)&&(V_Value <= V_Conf7)) 
  {
    digitalWrite(Pwr_Stage1, LOW);
    digitalWrite(Pwr_Stage2, HIGH);
    digitalWrite(Pwr_Stage3, HIGH);
    digitalWrite(Pwr_Stage4, HIGH);
    digitalWrite(Pwr_Stage5, HIGH);
    digitalWrite(Pwr_Stage6, LOW);
    digitalWrite(Pwr_Stage7, LOW);
    digitalWrite(Pwr_Stage8, LOW);              
  }
  else if((V_Value > V_Conf7)&&(V_Value <= V_Conf8)) 
  {
    digitalWrite(Pwr_Stage1, HIGH);
    digitalWrite(Pwr_Stage2, HIGH);
    digitalWrite(Pwr_Stage3, LOW);
    digitalWrite(Pwr_Stage4, LOW);
    digitalWrite(Pwr_Stage5, HIGH);
    digitalWrite(Pwr_Stage6, LOW);
    digitalWrite(Pwr_Stage7, HIGH);
    digitalWrite(Pwr_Stage8, LOW);          
  }       
  else if((V_Value > V_Conf8)&&(V_Value <= V_Conf9)) 
  {
    digitalWrite(Pwr_Stage1, LOW);
    digitalWrite(Pwr_Stage2, LOW);
    digitalWrite(Pwr_Stage3, LOW);
    digitalWrite(Pwr_Stage4, LOW);
    digitalWrite(Pwr_Stage5, HIGH);
    digitalWrite(Pwr_Stage6, HIGH);
    digitalWrite(Pwr_Stage7, HIGH);
    digitalWrite(Pwr_Stage8, LOW);         
  }
  else if((V_Value > V_Conf9)&&(V_Value <= V_Conf10)) 
  {
    digitalWrite(Pwr_Stage1, LOW);
    digitalWrite(Pwr_Stage2, HIGH);
    digitalWrite(Pwr_Stage3, LOW);
    digitalWrite(Pwr_Stage4, LOW);
    digitalWrite(Pwr_Stage5, HIGH);
    digitalWrite(Pwr_Stage6, LOW);
    digitalWrite(Pwr_Stage7, HIGH);
    digitalWrite(Pwr_Stage8, LOW);          
  }
  else if((V_Value > V_Conf10)&&(V_Value <= V_Conf11)) 
  {
    digitalWrite(Pwr_Stage1, LOW);
    digitalWrite(Pwr_Stage2, HIGH);
    digitalWrite(Pwr_Stage3, LOW);
    digitalWrite(Pwr_Stage4, HIGH);
    digitalWrite(Pwr_Stage5, HIGH);
    digitalWrite(Pwr_Stage6, LOW);
    digitalWrite(Pwr_Stage7, LOW);
    digitalWrite(Pwr_Stage8, LOW);         
  }
  else if((V_Value > V_Conf11)&&(V_Value <= V_Conf12)) 
  {
    digitalWrite(Pwr_Stage1, HIGH);
    digitalWrite(Pwr_Stage2, LOW);
    digitalWrite(Pwr_Stage3, LOW);
    digitalWrite(Pwr_Stage4, LOW);
    digitalWrite(Pwr_Stage5, LOW);
    digitalWrite(Pwr_Stage6, HIGH);
    digitalWrite(Pwr_Stage7, HIGH);
    digitalWrite(Pwr_Stage8, LOW);         
  }
  else if((V_Value > V_Conf12)&&(V_Value <= V_Conf13)) 
  {
    digitalWrite(Pwr_Stage1, HIGH);
    digitalWrite(Pwr_Stage2, LOW);
    digitalWrite(Pwr_Stage3, LOW);
    digitalWrite(Pwr_Stage4, LOW);
    digitalWrite(Pwr_Stage5, HIGH);
    digitalWrite(Pwr_Stage6, HIGH);
    digitalWrite(Pwr_Stage7, LOW);
    digitalWrite(Pwr_Stage8, LOW);         
  }
  else if((V_Value > V_Conf13)&&(V_Value <= V_Conf14)) 
  {
    digitalWrite(Pwr_Stage1, HIGH);
    digitalWrite(Pwr_Stage2, HIGH);
    digitalWrite(Pwr_Stage3, LOW);
    digitalWrite(Pwr_Stage4, LOW);
    digitalWrite(Pwr_Stage5, LOW);
    digitalWrite(Pwr_Stage6, HIGH);
    digitalWrite(Pwr_Stage7, LOW);
    digitalWrite(Pwr_Stage8, LOW);          
  }
  else if((V_Value > V_Conf16)&&(V_Value <= V_Conf15)) 
  {
    digitalWrite(Pwr_Stage1, HIGH);
    digitalWrite(Pwr_Stage2, HIGH);
    digitalWrite(Pwr_Stage3, LOW);
    digitalWrite(Pwr_Stage4, HIGH);
    digitalWrite(Pwr_Stage5, LOW);
    digitalWrite(Pwr_Stage6, LOW);
    digitalWrite(Pwr_Stage7, LOW);
    digitalWrite(Pwr_Stage8, LOW);         
  }       
  else if((V_Value > V_Conf15)&&(V_Value <= V_Conf16)) 
  {
    digitalWrite(Pwr_Stage1, LOW);
    digitalWrite(Pwr_Stage2, LOW);
    digitalWrite(Pwr_Stage3, LOW);
    digitalWrite(Pwr_Stage4, LOW);
    digitalWrite(Pwr_Stage5, HIGH);
    digitalWrite(Pwr_Stage6, LOW);
    digitalWrite(Pwr_Stage7, HIGH);
    digitalWrite(Pwr_Stage8, LOW);         
  }
  else if((V_Value > V_Conf16)&&(V_Value <= V_Conf17)) 
  {
    digitalWrite(Pwr_Stage1, LOW);
    digitalWrite(Pwr_Stage2, LOW);
    digitalWrite(Pwr_Stage3, LOW);
    digitalWrite(Pwr_Stage4, LOW);
    digitalWrite(Pwr_Stage5, HIGH);
    digitalWrite(Pwr_Stage6, HIGH);
    digitalWrite(Pwr_Stage7, LOW);
    digitalWrite(Pwr_Stage8, LOW);          
  }
  else if((V_Value > V_Conf17)&&(V_Value <= V_Conf18)) 
  {
    digitalWrite(Pwr_Stage1, LOW);
    digitalWrite(Pwr_Stage2, LOW);
    digitalWrite(Pwr_Stage3, HIGH);
    digitalWrite(Pwr_Stage4, LOW);
    digitalWrite(Pwr_Stage5, LOW);
    digitalWrite(Pwr_Stage6, HIGH);
    digitalWrite(Pwr_Stage7, LOW);
    digitalWrite(Pwr_Stage8, LOW);         
  }
  else if((V_Value > V_Conf18)&&(V_Value <= V_Conf19)) 
  {
    digitalWrite(Pwr_Stage1, LOW);
    digitalWrite(Pwr_Stage2, HIGH);
    digitalWrite(Pwr_Stage3, LOW);
    digitalWrite(Pwr_Stage4, LOW);
    digitalWrite(Pwr_Stage5, HIGH);
    digitalWrite(Pwr_Stage6, LOW);
    digitalWrite(Pwr_Stage7, LOW);
    digitalWrite(Pwr_Stage8, LOW);           
  }
  else if((V_Value > V_Conf19)&&(V_Value <= V_Conf20)) 
  {
    digitalWrite(Pwr_Stage1, LOW);
    digitalWrite(Pwr_Stage2, HIGH);
    digitalWrite(Pwr_Stage3, LOW);
    digitalWrite(Pwr_Stage4, HIGH);
    digitalWrite(Pwr_Stage5, LOW);
    digitalWrite(Pwr_Stage6, LOW);
    digitalWrite(Pwr_Stage7, LOW);
    digitalWrite(Pwr_Stage8, LOW);        
  }
  else if((V_Value > V_Conf20)&&(V_Value <= V_Conf21)) 
  {
    digitalWrite(Pwr_Stage1, LOW);
    digitalWrite(Pwr_Stage2, HIGH);
    digitalWrite(Pwr_Stage3, HIGH);
    digitalWrite(Pwr_Stage4, LOW);
    digitalWrite(Pwr_Stage5, LOW);
    digitalWrite(Pwr_Stage6, LOW);
    digitalWrite(Pwr_Stage7, LOW);
    digitalWrite(Pwr_Stage8, LOW);  
  }
  else if((V_Value > V_Conf21)&&(V_Value <= V_Conf22)) 
  {
    digitalWrite(Pwr_Stage1, HIGH);
    digitalWrite(Pwr_Stage2, LOW);
    digitalWrite(Pwr_Stage3, LOW);
    digitalWrite(Pwr_Stage4, LOW);
    digitalWrite(Pwr_Stage5, LOW);
    digitalWrite(Pwr_Stage6, LOW);
    digitalWrite(Pwr_Stage7, HIGH);
    digitalWrite(Pwr_Stage8, LOW);         
  }       
  else if((V_Value > V_Conf22)&&(V_Value <= V_Conf23)) 
  {
    digitalWrite(Pwr_Stage1, HIGH);
    digitalWrite(Pwr_Stage2, LOW);
    digitalWrite(Pwr_Stage3, LOW);
    digitalWrite(Pwr_Stage4, LOW);
    digitalWrite(Pwr_Stage5, LOW);
    digitalWrite(Pwr_Stage6, HIGH);
    digitalWrite(Pwr_Stage7, LOW);
    digitalWrite(Pwr_Stage8, LOW);          
  }
  else if((V_Value > V_Conf23)&&(V_Value <= V_Conf24)) 
  {
    digitalWrite(Pwr_Stage1, HIGH);
    digitalWrite(Pwr_Stage2, LOW);
    digitalWrite(Pwr_Stage3, LOW);
    digitalWrite(Pwr_Stage4, LOW);
    digitalWrite(Pwr_Stage5, HIGH);
    digitalWrite(Pwr_Stage6, LOW);
    digitalWrite(Pwr_Stage7, LOW);
    digitalWrite(Pwr_Stage8, LOW);           
  }
  else if((V_Value > V_Conf24)&&(V_Value <= V_Conf25)) 
  {
    digitalWrite(Pwr_Stage1, HIGH);
    digitalWrite(Pwr_Stage2, LOW);
    digitalWrite(Pwr_Stage3, LOW);
    digitalWrite(Pwr_Stage4, HIGH);
    digitalWrite(Pwr_Stage5, LOW);
    digitalWrite(Pwr_Stage6, LOW);
    digitalWrite(Pwr_Stage7, LOW);
    digitalWrite(Pwr_Stage8, LOW);        
  }
  else if((V_Value > V_Conf25)&&(V_Value <= V_Conf26)) 
  {
    digitalWrite(Pwr_Stage1, HIGH);
    digitalWrite(Pwr_Stage2, LOW);
    digitalWrite(Pwr_Stage3, HIGH);
    digitalWrite(Pwr_Stage4, LOW);
    digitalWrite(Pwr_Stage5, LOW);
    digitalWrite(Pwr_Stage6, LOW);
    digitalWrite(Pwr_Stage7, LOW);
    digitalWrite(Pwr_Stage8, LOW);         
  }
  else if((V_Value > V_Conf26)&&(V_Value <= V_Conf27)) 
  {
    digitalWrite(Pwr_Stage1, HIGH);
    digitalWrite(Pwr_Stage2, HIGH);
    digitalWrite(Pwr_Stage3, LOW);
    digitalWrite(Pwr_Stage4, LOW);
    digitalWrite(Pwr_Stage5, LOW);
    digitalWrite(Pwr_Stage6, LOW);
    digitalWrite(Pwr_Stage7, LOW);
    digitalWrite(Pwr_Stage8, LOW);       
  }
  else if((V_Value > V_Conf27)&&(V_Value <= V_Conf28)) 
  {
    digitalWrite(Pwr_Stage1, LOW);
    digitalWrite(Pwr_Stage2, LOW);
    digitalWrite(Pwr_Stage3, LOW);
    digitalWrite(Pwr_Stage4, LOW);
    digitalWrite(Pwr_Stage5, LOW);
    digitalWrite(Pwr_Stage6, LOW);
    digitalWrite(Pwr_Stage7, LOW);
    digitalWrite(Pwr_Stage8, HIGH);        
  }
  else if((V_Value > V_Conf28)&&(V_Value <= V_Conf29)) 
  {
    digitalWrite(Pwr_Stage1, LOW);
    digitalWrite(Pwr_Stage2, LOW);
    digitalWrite(Pwr_Stage3, LOW);
    digitalWrite(Pwr_Stage4, LOW);
    digitalWrite(Pwr_Stage5, LOW);
    digitalWrite(Pwr_Stage6, LOW);
    digitalWrite(Pwr_Stage7, HIGH);
    digitalWrite(Pwr_Stage8, LOW);            
  } 
  else if((V_Value > V_Conf29)&&(V_Value <= V_Conf30)) 
  {
    digitalWrite(Pwr_Stage1, LOW);
    digitalWrite(Pwr_Stage2, LOW);
    digitalWrite(Pwr_Stage3, LOW);
    digitalWrite(Pwr_Stage4, LOW);
    digitalWrite(Pwr_Stage5, LOW);
    digitalWrite(Pwr_Stage6, HIGH);
    digitalWrite(Pwr_Stage7, LOW);
    digitalWrite(Pwr_Stage8, LOW);           
  }
  else if((V_Value > V_Conf30)&&(V_Value <= V_Conf31)) 
  {
    digitalWrite(Pwr_Stage1, LOW);
    digitalWrite(Pwr_Stage2, LOW);
    digitalWrite(Pwr_Stage3, LOW);
    digitalWrite(Pwr_Stage4, LOW);
    digitalWrite(Pwr_Stage5, HIGH);
    digitalWrite(Pwr_Stage6, LOW);
    digitalWrite(Pwr_Stage7, LOW);
    digitalWrite(Pwr_Stage8, LOW);            
  }
  else if((V_Value > V_Conf31)&&(V_Value <= V_Conf32)) 
  {
    digitalWrite(Pwr_Stage1, LOW);
    digitalWrite(Pwr_Stage2, LOW);
    digitalWrite(Pwr_Stage3, LOW);
    digitalWrite(Pwr_Stage4, HIGH);
    digitalWrite(Pwr_Stage5, LOW);
    digitalWrite(Pwr_Stage6, LOW);
    digitalWrite(Pwr_Stage7, LOW);
    digitalWrite(Pwr_Stage8, LOW);           
  }       
  else if((V_Value > V_Conf32)&&(V_Value <= V_Conf33)) 
  {
    digitalWrite(Pwr_Stage1, LOW);
    digitalWrite(Pwr_Stage2, LOW);
    digitalWrite(Pwr_Stage3, HIGH);
    digitalWrite(Pwr_Stage4, LOW);
    digitalWrite(Pwr_Stage5, LOW);
    digitalWrite(Pwr_Stage6, LOW);
    digitalWrite(Pwr_Stage7, LOW);
    digitalWrite(Pwr_Stage8, LOW);            
  }
  else if((V_Value > V_Conf33)&&(V_Value <= V_Conf34)) 
  {
    digitalWrite(Pwr_Stage1, LOW);
    digitalWrite(Pwr_Stage2, HIGH);
    digitalWrite(Pwr_Stage3, LOW);
    digitalWrite(Pwr_Stage4, LOW);
    digitalWrite(Pwr_Stage5, LOW);
    digitalWrite(Pwr_Stage6, LOW);
    digitalWrite(Pwr_Stage7, LOW);
    digitalWrite(Pwr_Stage8, LOW);          
  }  
}


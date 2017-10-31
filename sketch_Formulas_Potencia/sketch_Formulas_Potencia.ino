#include <Wire.h>

// Varoles de Tension para las diferentes posiciones de los arreglos de Resistencias


#define Pwr_Stage1
#define Pwr_Stage2 
#define Pwr_Stage3
#define Pwr_Stage4
#define Pwr_Stage5
#define Pwr_Stage6
#define Pwr_Stage7
#define Pwr_Stage8

// Variable Constantes correspodientes a Tensiones de Salida Conmutada

const float V_Conf1 = 12.3;
const float V_Conf2 = 13.5;
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

int Power_Rcv = 0;
int Power_Value = 0;
int Power_Data;
int Bio_Value = 0;
int Volt_Value;
int T_Value;
int Tutil;
int Val;
int Vrms;
int Vamp;
int Frecuency;
int Data_Value[2];
int i = 0;

void setup() {

  Wire.begin(8);                // join i2c bus with address #8
  Serial.begin(9600);           // start serial for output
  DDRB = 00000011;
  DDRC = 00001111;
  DDRD = 00011000;
  
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
    Power_Stage(Power_Data); 
    Periodo_Util(Frecuency); 
    Calc_Power(Power_Value, Bio_Value, T_Value);
    Comparate_Stage(Volt_Value);
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
    Choose_Volt1(P_Data);  
  }
  else if((P_Data > 20)&&(P_Data <= 40))
  {
    Choose_Volt2(P_Data);      
  }
  else if((P_Data > 40)&&(P_Data <= 60))
  {
    Choose_Volt3(P_Data);      
  }
  else if((P_Data > 60)&&(P_Data <= 80))
  {
    Choose_Volt4(P_Data);      
  }
  else if((P_Data > 80)&&(P_Data <= 100))
  {
    Choose_Volt5(P_Data);      
  }  
}



void Calc_Power (int P_Value, int Bio_Value, int T)
{
  Val = Bio_Value * P_Value;                               // Si no se quiere manejar el valor del Bioimpedanciometro Z se coloca como un valor estandar en lugar de como una entrada 
  Vrms = sqrt(Val);
  Vamp = Vrms * (sqrt(2*T));                               // Los valores de Factor de uso Fac_Use se calcularon al obtener la formula Vrms = Vamp/(sqrt(2*Tciclo util))   
  Volt_Value = Vamp;
}

void Choose_Volt1 (int Volt_Comp)
{
  if((Volt_Comp == 1)||(Volt_Comp == 2))
  {
    Power_Value = 50;    
  }
  else if((Volt_Comp == 3)||(Volt_Comp == 4))
  {
    Power_Value = 51;    
  }
  else if((Volt_Comp == 5)||(Volt_Comp == 6))
  {
    Power_Value = 52;    
  }
  else if((Volt_Comp == 7)||(Volt_Comp == 8))
  {
    Power_Value = 53;    
  }
  else if((Volt_Comp == 9)||(Volt_Comp == 10))
  {
    Power_Value = 54;    
  }
  else if((Volt_Comp == 11)||(Volt_Comp == 12))
  {
    Power_Value = 55;    
  }
  else if((Volt_Comp == 13)||(Volt_Comp == 14))
  {
    Power_Value = 56;    
  }
  else if((Volt_Comp == 15)||(Volt_Comp == 16))
  {
    Power_Value = 57;    
  }
  else if((Volt_Comp == 17)||(Volt_Comp == 18))
  {
    Power_Value = 58;    
  }
  else if((Volt_Comp == 19)||(Volt_Comp == 20))
  {
    Power_Value = 59;    
  }  
}

void Choose_Volt2 (int Volt_Comp)
{
  if((Volt_Comp == 21)||(Volt_Comp == 22))
  {
    Power_Value = 60;    
  }
  else if((Volt_Comp == 23)||(Volt_Comp == 24))
  {
    Power_Value = 61;    
  }
  else if((Volt_Comp == 25)||(Volt_Comp == 26))
  {
    Power_Value = 62;    
  }
  else if((Volt_Comp == 27)||(Volt_Comp == 28))
  {
    Power_Value = 63;    
  }
  else if((Volt_Comp == 29)||(Volt_Comp == 30))
  {
    Power_Value = 64;    
  }
  else if((Volt_Comp == 31)||(Volt_Comp == 32))
  {
    Power_Value = 65;    
  }
  else if((Volt_Comp == 33)||(Volt_Comp == 34))
  {
    Power_Value = 66;    
  }
  else if((Volt_Comp == 35)||(Volt_Comp == 36))
  {
    Power_Value = 67;    
  }
  else if((Volt_Comp == 37)||(Volt_Comp == 38))
  {
    Power_Value = 68;    
  }
  else if((Volt_Comp == 39)||(Volt_Comp == 40))
  {
    Power_Value = 69;    
  }   
}

void Choose_Volt3 (int Volt_Comp)
{
  if((Volt_Comp == 41)||(Volt_Comp == 42))
  {
    Power_Value = 70;    
  }
  else if((Volt_Comp == 43)||(Volt_Comp == 44))
  {
    Power_Value = 71;    
  }
  else if((Volt_Comp == 45)||(Volt_Comp == 46))
  {
    Power_Value = 72;    
  }
  else if((Volt_Comp == 47)||(Volt_Comp == 48))
  {
    Power_Value = 73;    
  }
  else if((Volt_Comp == 49)||(Volt_Comp == 50))
  {
    Power_Value = 74;    
  }
  else if((Volt_Comp == 51)||(Volt_Comp == 52))
  {
    Power_Value = 75;    
  }
  else if((Volt_Comp == 53)||(Volt_Comp == 54))
  {
    Power_Value = 76;    
  }
  else if((Volt_Comp == 55)||(Volt_Comp == 56))
  {
    Power_Value = 77;    
  }
  else if((Volt_Comp == 57)||(Volt_Comp == 58))
  {
    Power_Value = 78;    
  }
  else if((Volt_Comp == 59)||(Volt_Comp == 60))
  {
    Power_Value = 79;    
  }   
}

void Choose_Volt4 (int Volt_Comp)
{
  if((Volt_Comp == 61)||(Volt_Comp == 62))
  {
    Power_Value = 80;    
  }
  else if((Volt_Comp == 63)||(Volt_Comp == 64))
  {
    Power_Value = 81;    
  }
  else if((Volt_Comp == 65)||(Volt_Comp == 66))
  {
    Power_Value = 82;    
  }
  else if((Volt_Comp == 67)||(Volt_Comp == 68))
  {
    Power_Value = 83;    
  }
  else if((Volt_Comp == 69)||(Volt_Comp == 70))
  {
    Power_Value = 84;    
  }
  else if((Volt_Comp == 71)||(Volt_Comp == 72))
  {
    Power_Value = 85;    
  }
  else if((Volt_Comp == 73)||(Volt_Comp == 74))
  {
    Power_Value = 86;    
  }
  else if((Volt_Comp == 75)||(Volt_Comp == 76))
  {
    Power_Value = 87;    
  }
  else if((Volt_Comp == 77)||(Volt_Comp == 78))
  {
    Power_Value = 88;    
  }
  else if((Volt_Comp == 79)||(Volt_Comp == 80))
  {
    Power_Value = 89;    
  }   
}


void Choose_Volt5 (int Volt_Comp)
{
  if((Volt_Comp == 81)||(Volt_Comp == 82))
  {
    Power_Value = 90;    
  }
  else if((Volt_Comp == 83)||(Volt_Comp == 84))
  {
    Power_Value = 91;    
  }
  else if((Volt_Comp == 85)||(Volt_Comp == 86))
  {
    Power_Value = 92;    
  }
  else if((Volt_Comp == 87)||(Volt_Comp == 88))
  {
    Power_Value = 93;    
  }
  else if((Volt_Comp == 89)||(Volt_Comp == 90))
  {
    Power_Value = 94;    
  }
  else if((Volt_Comp == 91)||(Volt_Comp == 92))
  {
    Power_Value = 95;    
  }
  else if((Volt_Comp == 93)||(Volt_Comp == 94))
  {
    Power_Value = 96;    
  }
  else if((Volt_Comp == 95)||(Volt_Comp == 96))
  {
    Power_Value = 97;    
  }
  else if((Volt_Comp == 97)||(Volt_Comp == 98))
  {
    Power_Value = 98;    
  }
  else if((Volt_Comp == 99)||(Volt_Comp == 100))
  {
    Power_Value = 99;    
  }   
}

void Periodo_Util (int FrecC_Util)
{
  Tutil = 1/FrecC_Util;
}

void Comparate_Stage(int V_Value)
{
  if((V_Value > 0)&&(V_Value <= V_Conf1)) 
  {
    PORTB = 00000011;
    PORTC = 00001111;
    PORTD = 00011000;       
  }
  else if((V_Value > V_Conf1)&&(V_Value <= V_Conf2)) 
  {
    PORTB = 0000011;
    PORTC = 00000111;
    PORTD = 00011000;           
  }
  else if((V_Value > V_Conf2)&&(V_Value <= V_Conf3)) 
  {
    PORTB = 00000011;
    PORTC = 00000101;
    PORTD = 00011000;           
  }
  else if((V_Value > V_Conf3)&&(V_Value <= V_Conf4)) 
  {
    PORTB = 00000000;
    PORTC = 00000111;
    PORTD = 00011000;           
  }
  else if((V_Value > V_Conf4)&&(V_Value <= V_Conf5)) 
  {
    PORTB = 00000011;
    PORTC = 00000101;
    PORTD = 00001000;           
  }
  else if((V_Value > V_Conf5)&&(V_Value <= V_Conf6)) 
  {
    PORTB = 00000000;
    PORTC = 00000101;
    PORTD = 00011000;         
  }
  else if((V_Value > V_Conf6)&&(V_Value <= V_Conf7)) 
  {
    PORTB = 00000001;
    PORTC = 00000011;
    PORTD = 00010000;          
  }
  else if((V_Value > V_Conf7)&&(V_Value <= V_Conf8)) 
  {
    PORTB = 00000010;
    PORTC = 00000101;
    PORTD = 00010000;          
  }       
  else if((V_Value > V_Conf8)&&(V_Value <= V_Conf9)) 
  {
    PORTB = 00000000;
    PORTC = 00000100;
    PORTD = 00011000;          
  }
  else if((V_Value > V_Conf9)&&(V_Value <= V_Conf10)) 
  {
    PORTB = 00000000;
    PORTC = 00000101;
    PORTD = 00010000;          
  }
  else if((V_Value > V_Conf10)&&(V_Value <= V_Conf11)) 
  {
    PORTB = 00000001;
    PORTC = 00000001;
    PORTD = 00010000;         
  }
  else if((V_Value > V_Conf11)&&(V_Value <= V_Conf12)) 
  {
    PORTB = 00000010;
    PORTC = 00000100;
    PORTD = 00010000;          
  }
  else if((V_Value > V_Conf12)&&(V_Value <= V_Conf13)) 
  {
    PORTB = 00000010;
    PORTC = 00000000;
    PORTD = 00011000;          
  }
  else if((V_Value > V_Conf13)&&(V_Value <= V_Conf14)) 
  {
    PORTB = 00000010;
    PORTC = 00000001;
    PORTD = 00001000;          
  }
  else if((V_Value > V_Conf16)&&(V_Value <= V_Conf15)) 
  {
    PORTB = 00000011;
    PORTC = 00000001;
    PORTD = 00000000;          
  }       
  else if((V_Value > V_Conf15)&&(V_Value <= V_Conf16)) 
  {
    PORTB = 00000000;
    PORTC = 00000100;
    PORTD = 00010000;          
  }
  else if((V_Value > V_Conf16)&&(V_Value <= V_Conf17)) 
  {
    PORTB = 00000000;
    PORTC = 00000000;
    PORTD = 00011000;          
  }
  else if((V_Value > V_Conf17)&&(V_Value <= V_Conf18)) 
  {
    PORTB = 00000000;
    PORTC = 00000010;
    PORTD = 00001000;          
  }
  else if((V_Value > V_Conf18)&&(V_Value <= V_Conf19)) 
  {
    PORTB = 00000000;
    PORTC = 00000001;
    PORTD = 00010000;          
  }
  else if((V_Value > V_Conf19)&&(V_Value <= V_Conf20)) 
  {
    PORTB = 00000001;
    PORTC = 00000001;
    PORTD = 00000000;          
  }
  else if((V_Value > V_Conf20)&&(V_Value <= V_Conf21)) 
  {
    PORTB = 00000000;
    PORTC = 00000011;
    PORTD = 00000000;         
  }
  else if((V_Value > V_Conf21)&&(V_Value <= V_Conf22)) 
  {
    PORTB = 00000010;
    PORTC = 00000100;
    PORTD = 00000000;          
  }       
  else if((V_Value > V_Conf22)&&(V_Value <= V_Conf23)) 
  {
    PORTB = 00000010;
    PORTC = 00000000;
    PORTD = 00001000;          
  }
  else if((V_Value > V_Conf23)&&(V_Value <= V_Conf24)) 
  {
    PORTB = 00000010;
    PORTC = 00000000;
    PORTD = 00010000;          
  }
  else if((V_Value > V_Conf24)&&(V_Value <= V_Conf25)) 
  {
    PORTB = 00000011;
    PORTC = 00000000;
    PORTD = 00000000;          
  }
  else if((V_Value > V_Conf25)&&(V_Value <= V_Conf26)) 
  {
    PORTB = 00000010;
    PORTC = 00000010;
    PORTD = 00000000;          
  }
  else if((V_Value > V_Conf26)&&(V_Value <= V_Conf27)) 
  {
    PORTB = 00000010;
    PORTC = 00000001;
    PORTD = 00000000;          
  }
  else if((V_Value > V_Conf27)&&(V_Value <= V_Conf28)) 
  {
    PORTB = 00000000;
    PORTC = 00001000;
    PORTD = 00000000;          
  }
  else if((V_Value > V_Conf28)&&(V_Value <= V_Conf29)) 
  {
    PORTB = 00000000;
    PORTC = 00000100;
    PORTD = 00000000;          
  } 
  else if((V_Value > V_Conf29)&&(V_Value <= V_Conf30)) 
  {
    PORTB = 00000000;
    PORTC = 00000000;
    PORTD = 00001000;          
  }
  else if((V_Value > V_Conf30)&&(V_Value <= V_Conf31)) 
  {
    PORTB = 00000000;
    PORTC = 00000000;
    PORTD = 00010000;           
  }
  else if((V_Value > V_Conf31)&&(V_Value <= V_Conf32)) 
  {
    PORTB = 00000001;
    PORTC = 00000000;
    PORTD = 00000000;         
  }       
  else if((V_Value > V_Conf32)&&(V_Value <= V_Conf33)) 
  {
    PORTB = 00000000;
    PORTC = 00000010;
    PORTD = 00000000;          
  }
  else if((V_Value > V_Conf33)&&(V_Value <= V_Conf34)) 
  {
    PORTB = 00000000;
    PORTC = 00000001;
    PORTD = 00000000;         
  }  
}


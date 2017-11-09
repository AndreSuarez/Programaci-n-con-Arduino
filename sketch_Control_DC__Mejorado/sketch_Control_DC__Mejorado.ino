
#define Pwr_Stage1  9   // Se le dio este roden para identificar fisicamente en la placa las secciones activadas
#define Pwr_Stage2  A0  // Sin embargo su cambio solo implica movimientos estructurales en el codigo 
#define Pwr_Stage3  A1
#define Pwr_Stage4  8
#define Pwr_Stage5  4
#define Pwr_Stage6  3
#define Pwr_Stage7  A2
#define Pwr_Stage8  A3

int Volt_level[100];
int a;
int Volt_Comp;
int Volt_Prev = 5;
int Vt = 40;
byte Volt_Chosen;
int Ch_Out[8];
int Stage_Out[8];

void setup() {

  Assignment_Out();  
}

void loop() {
    
  Voltage_Compare(Vt);
  Matrix_Out(Volt_Chosen);
  Act_Out();  
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
  for (int i=0; i <= 100; i++){
    Volt_Comp = abs(Volt_Ref - Volt_level[i]);
    if(Volt_Comp <= Volt_Prev)
    {
      Volt_Prev = Volt_Comp;
      Volt_Chosen = byte(i);
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
  for (int j=0; j <= 7; j++){
    Ch_Out[j] = Selection_Out >> j; 
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


   

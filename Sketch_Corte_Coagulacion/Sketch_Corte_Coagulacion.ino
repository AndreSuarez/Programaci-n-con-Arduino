
#include <AD5933.h>
#include <Wire.h>
#include <Nextion.h>

#define PlacaRet 8      // Pin para la deteccion de la placa de retorno (Desconectada)
#define Z_On     9      // Pin para la deteccion de impedancia
                         
int Sens = 1;            
int Z; 
int Pin;
long Val;
int Vout;                  
int Cut_coag = 0;           
int Power = 100;         // Potencia asignada por medio de la pantalla Nextion (Entrada al sistema)
boolean SetBio = false;

AD5933 Bio(9600);

void setup() 
{
  Serial.begin(9600);
  Wire.begin();
  pinMode(PlacaRet, OUTPUT);
  pinMode(Z_On, OUTPUT);
}

//////////////////   Ciclo de Sensado   ////////////////////////

void loop() 
{
  
  while(Sens == 1)
  {
    SensorBio();
  }

  while(Cut_coag == 1)
  {
    PowerStage(); 
  }

   
}

void SensorBio ()
{
  int Placa = Bio.impedance();
  if(Paca < 480)
    { 
      Serial.println("Placa detectada ");
      digitalWrite(PlacaRet, HIGH);
      SetBio = true;                              // Switch para cortocircuitar la placa de retorno y realizar medicion entre el lapiz y dicha placa  
    }
    else
    {
      Serial.println("Error Placa Desconectada ");
      digitalWrite(PlacaRet, LOW);
    }
    if(SetBio==1)
    {
      int Impedance = Bio.impedance();
      if(Impedance < 2500)
      {
        Z = Impedance;
        Cut_coag = 1;
        Sens = 0;
        digitalWrite(Z_On, HIGH);
        
      }
      else
      {
        Sens = 1;
        digitalWrite(Z_On, LOW); 
      }
    }  
}

void PowerStage ()
{
  while(Cut_coag == 1)
  {
    CalcPower(Power);    
  }
}

void CalcPower (long Pin)
{
  Val=Z*Pin;
  Vout = sqrt(Val);       
  Sens = 1;
  Cut_coag = 0;
}



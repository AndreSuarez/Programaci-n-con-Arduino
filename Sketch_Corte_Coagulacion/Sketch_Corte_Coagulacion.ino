
#include <AD5933.h>
#include <Wire.h>
#include <Nextion.h>

#define AlarmaRet 7      // Pin para la deteccion de la alarma de la placa de retorno (Desconectada)
                         
int Sens = 1;            
int Z; 
int Pin;
int Vout;                  
int Cut_coag = 0;           
int Power = 100;         // Potencia asignada por medio de la pantalla Nextion (Entrada al sistema)
boolean SetBio = false;

AD5933 Bio(9600);

void setup() 
{
  Serial.begin(9600);
  Wire.begin();
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
  if(Placa < 480)
    { 
      Serial.println("Placa detectada ");
      digitalWrite(AlarmaRet, LOW);
      SetBio = true;                              // Switch para cortocircuitar la placa de retorno y realizar medicion entre el lapiz y dicha placa  
    }
    else
    {
      Serial.println("Error Placa Desconectada ");
      digitalWrite(AlarmaRet, HIGH);
    }
    if(SetBio)
    {
      int Impedance = Bio.impedance();
      if(Impedance < 2500)
      {
        Z = Impedance;
        Cut_coag = 1;
        Sens = 0;
      }
      else
      {
        Sens = 1; 
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

void CalcPower (int Pin)
{
  Vout = sqrt(Z*Pin);    
}



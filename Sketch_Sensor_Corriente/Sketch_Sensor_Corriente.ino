//Aplicando Filtro a las lecturas del ACS712
//usaremos la media aritmética de varias lecturas consecutivas, 
//simplemente hay que sumar la lecturas y dividirlas en el número de muestras.
//La cantidad de muestras para calcular el promedio depende del nivel de ruido que tengan.

#include <Wire.h>         //libreria para comunicacion I2C

#define Off_Current  8
#define Condition_Up 9

float Sensibilidad=0.1;       //sensibilidad en voltios dependiendo el sensor 20A
float I_Current;
float Idc;
float Idc2;
byte Dato_Corriente;
int Activation;
int Cut_wait = 0;

void setup() {
  Serial.begin(9600);
//  Wire.begin(7);                  //unirse al bus I2C con la dirección # 7
//  Wire.onRequest(requestSens);    
  
  pinMode(Off_Current,OUTPUT);
  
}

void loop() {
  
  Activation = digitalRead(Condition_Up);
  if(Activation == HIGH)
  {   
    if(Cut_wait == 0)
    {
      Idc = CURRENT_C_REQUEST(100);               //obtenemos la corriente promedio de 500 muestras
      if(Idc >= 1)
      {
        digitalWrite(Off_Current, LOW);
        Cut_wait = 1;
        Idc2 = 2;
      }
    }  
    if(Cut_wait == 1)
      {
        Idc2 = CURRENT_C_REQUEST(50);               //obtenemos la corriente promedio de 500 muestras 
        if(Idc2 <= 1)
        {
          digitalWrite(Off_Current, HIGH); 
          Idc = 0;
          Cut_wait = 0; 
          delay(10); 
        }      
      }
   } 
}

//función que se ejecuta cuando los datos son solicitados por el maestro
// esta función está registrada como un evento, vea setup ()

float CURRENT_C_REQUEST(int numeroMuestras)
{
  float VoltajeSensorCUR = 0;
  float intensidad_Cur = 0;

  for(int i=0;i<numeroMuestras;i++)                     //ciclo donde sumamos las 500 muestras
  {
    VoltajeSensorCUR = analogRead(A3) * (5.0/1023.0);              //lee el canal A0 y calcula el valor en tension 
    intensidad_Cur=intensidad_Cur+((VoltajeSensorCUR-2.55)/Sensibilidad);    //calculamos corriente y sumamos
  }
  intensidad_Cur= intensidad_Cur/numeroMuestras;
  I_Current = (intensidad_Cur - 0.08);
  
  return(I_Current);
}

//void requestSens ()
//{
//  Wire.write(Dato_Corriente); //responder con mensaje de 6 bytes como esperaba el maestro
//}
//
//void Salida_Corriente ()
//{
//  Dato_Corriente =  Activation; 
//}



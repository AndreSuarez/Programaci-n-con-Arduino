//Aplicando Filtro a las lecturas del ACS712
//usaremos la media aritmética de varias lecturas consecutivas, 
//simplemente hay que sumar la lecturas y dividirlas en el número de muestras.
//La cantidad de muestras para calcular el promedio depende del nivel de ruido que tengan.

#include <Wire.h>         //libreria para comunicacion I2C

#define Off_Current  8
#define Condition_Up 9

float Sensibilidad=0.1;       // Sensibilidad en voltios dependiendo el sensor 20A
float I_Current;              // Salida o retorno de la funcion donde se obtiene el valor promedio de corriente
float Idc;                    // Primer valor de corriente correspondiente al valor donde se registra que se comienza a cortar
float Idc2;                   // Segundo valor de corriente correspondiente al valor donde se registra que se dejo de cortar
byte Dato_Corriente;          // Dato utilizado unicamente si se realiza comunicacion I2C
int Activation;               // Dato de entrada donde se activa el comienzo del senso de corriente, pin digital
int Cut_wait = 0;             // Dato de estado, en cero es esperando empezar corte, en uno es esperando terminar corte
int No_Cut = 0;               // Esta variable es a tener en cuenta por si no hay corte no dejar el equipo en estado de continuidad con el trafo
int No_Stop = 0;              // Esta variable es mas de proteccion para que despues de X segundos se deje de usar el equipo sin sobrecalentarlo o dañarlo

void setup() {
  Serial.begin(9600);
//  Wire.begin(7);                  //unirse al bus I2C con la dirección # 7
//  Wire.onRequest(requestSens);    
  
  pinMode(Off_Current,OUTPUT);
  
}

void loop() {

  digitalWrite(Off_Current, LOW);                 // Estado por defecto de la salida, en bajo para no desactivar el proceso de corte o coagulacion (puede ir en el void setup)
  Activation = digitalRead(Condition_Up);         // Leemos el puerto donde nos dicen cuando comenzar a sensar
  if(Activation == HIGH)                          // Necesitamos un valor en alto para comenzar el proceso    
  {   
    if(Cut_wait == 0)                             // Esta variable por defecto esta en cero y al completarse cada proceso se reinicia, es decir este paso es automatico, primer loop      
    {                                               
      Idc = CURRENT_C_REQUEST(100);               // Obtenemos la corriente promedio de 100 muestras
      No_Cut++;                                   // Variable de "tiempo de espera" despues de un numero determinado de sensos donde no se haga nada nos saca del estado actual
      if(Idc >= 1)                                // Detectamos cuando la corriente supera 1 [A], la corriente de corte q estaba detectando el sensor era de aprox. 2,5 [A]
      {
        digitalWrite(Off_Current, LOW);           // Esta es la salida y al estar en cero quiere decir q la maquina de estados aun sigue en corte o coagulacion
        Cut_wait = 1;                             // Cambia de loop para detectar cuando se deja de cortar
        Idc2 = 2;                                 // Este valor es por defecto para que en el otro loop no nos saque inmediatamente, pero podria ser cualquier valor superior a 1  
      }
      if(No_Cut == 100)                           // Si llegados X ciclos de sensado no hay corriente pasa a sacarlo del estado actual (Puse 100 pero pueden ser mas o menos)
      {
        digitalWrite(Off_Current, LOW);           // Esta es la salida y al estar en cero quiere decir q la maquina de estados aun sigue en corte o coagulacion                            // Cambia de loop para detectar cuando se deja de cortar
        Idc2 = 2;                                 // Este valor es por defecto para que en el otro loop no nos saque inmediatamente, pero podria ser cualquier valor superior a 1 
        No_Cut = 0;                               // Reiniciamos esta variable para el siguiente ciclo de sensado
      }
    }  
    if(Cut_wait == 1)                             // Segundo loop donde detectamos el momento de desactivacion del estado  
      {
        Idc2 = CURRENT_C_REQUEST(50);             // Obtenemos de nuevo la corriente promedio pero de 50 muestras 
        No_Stop++;                                // Variable de "tiempo maximo de uso", si no se deja de usar despues de un numero determinado de segundos igual nos saca del estado actual
        if(Idc2 <= 1)                             // Al decrecer el valor de corriente a menos de 1[A] implica que se dejo de cortar o coagular
        {
          digitalWrite(Off_Current, HIGH);        // Señal de salida para la maquina de estados, con este valor si se sale del estado actual
          Idc = 0;                                // Reiniciamos la variable para el siguiente ciclo de sensado
          Cut_wait = 0;                           // Reiniciamos la variable para el siguiente ciclo de sensado
          delay(10);                              // Tiempo de interrupcion se puede usar o no dependiendo de la velocidad requerida
        } 
        if(No_Stop == 100)                        // Esta funcion es la que protegeria al equipo de no cortar o coagular mas del tiempo donde se dañaria el equipo
        {
          digitalWrite(Off_Current, HIGH);        // Señal de salida para la maquina de estados, con este valor si se sale del estado actual
          Idc = 0;                                // Reiniciamos la variable para el siguiente ciclo de sensado
          Cut_wait = 0;                           // Reiniciamos la variable para el siguiente ciclo de sensado 
          No_Stop = 0;                            // Reiniciamos esta variable para el siguiente ciclo de sensado      
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



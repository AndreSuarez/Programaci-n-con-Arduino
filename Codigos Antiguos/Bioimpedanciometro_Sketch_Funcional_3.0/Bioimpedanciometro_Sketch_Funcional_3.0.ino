
/*  

Para escribir datos se utiliza la misma plantilla, al igual que para leer solo cambian
la direccion de los registros y los datos a transmitir o recibir. (1 plantilla para leer
y 1 para escribir). 

*/

#include <Wire.h>
                      /* Direccion del AD5933 */
                      
#define Slave_Add   B0001101    // La dirección asignada por defecto al AD5933 que es nuestro dispositivo esclavo 

                      /* Comandos Principales */
                      
#define Block_W        0xA0        // Comando para escribir en el AD5933 
#define Block_R        0xA1        // Comando para leer del AD5933 
#define Set_AddPointer 0xB0        // Comando para seleccionar el registro a leer 
#define VRI_Data       0x02        // Comando de confimacion de Dato valido en el registro de Estado
#define FS_Valid       0x04        // Finalizacion del Barrido en Frecuencia en el registro de Estado
#define Start_Process  0x10        // Dato en el registro de control para comenzar proceso
#define Standby_Mode   0xB0        // Inicializacion de la alimentacion del sistema 
#define Sweep_Active   0x20        // Activar Barrido de Frecuencia
#define Incr_Frequency 0x30        // Pasar de un valor de frecuencia al siguiente
#define Gain_X1        0x01        // Establecimiento de Ganancia en un Factor de x1
#define Port_OutD      0x40        // Puertos de Salida D
#define Port_OutC      0x03        // Puertos de Salida C
#define Rst            0x01        // Reset para comenzar el sensado de la Z desconocida sin desenergizar   

                        /* Bytes de Registros */
                        
byte Byte_Control1 = 0x80;        // Byte de Control 1  
byte Byte_Control2 = 0x81;        // Byte de Control 2  
byte Start_Freq1   = 0x82;        // Byte 1 frecuencia de inicio de Sensado  
byte Start_Freq2   = 0x83;        // Byte 2 frecuencia de inicio de Sensado 
byte Start_Freq3   = 0x84;        // Byte 3 frecuencia de inicio de Sensado  
byte Freq_incre1   = 0x85;        // Byte 1 para los increments de frecuencia  
byte Freq_incre2   = 0x86;        // Byte 2 para los increments de frecuencia
byte Freq_incre3   = 0x87;        // Byte 3 para los increments de frecuencia
byte Num_incre1    = 0x88;        // Byte 1 para el # increments de frecuencia
byte Num_incre2    = 0x89;        // Byte 2 para el # increments de frecuencia
byte Set_cycles1   = 0x8A;        // Byte 1 para el # de ciclos para cada Sensado
byte Set_cycles2   = 0x8B;        // Byte 2 para el # de ciclos para cada Sensado
byte Status        = 0x8F;        // Byte que confirma medida Sensada
byte Temp_Data1    = 0x92;        // Byte 1 para el dato de temperatura
byte Temp_Data2    = 0x93;        // Byte 2 para el dato de temperatura
byte Real_Data1    = 0x94;        // Byte 1 dato Real 
byte Real_Data2    = 0x95;        // Byte 2 dato Real 
byte Imag_Data1    = 0x96;        // Byte 1 dato Imaginario 
byte Imag_Data2    = 0x97;        // Byte 2 dato Imaginario 

                      /* Datos a Ingresar */
                      
int a;                            // Valor de comparacion para una resistencia muy grande
int b;                            // Valor de comparacion para una resistencia medible
int i;                            // Valor del ciclo en proceso   
int j = 0;                        // Valor de la etapa (numero de sensado)
int h;                            // Valor para el numero de veces valido, por el cual se establece una impedancia o muy grande o medible
int k;                            // Este reemplaza a j en el barrido previo al sensado
int m = 0;                        // Valor para hallar el promedio de las magnitudes sensadas
int n = 0;                        // Valor para sacar del estado donde salen magnitudes medibles pero deja repentinamente en circuito abierto en medio del proceso
int conf = 0;                     // Valor de Estado del Ciclo (Todo el proceso se puede colocar en el void setup pues sera realizado una sola vez)
int Prev_State = 0;               // Proceso para evitar demorarse en resistencias demasiado grandes
int State;                        // Dato de confirmacion para lectura de registros 
int Reg_Data[4]= {Real_Data1, Real_Data2, Imag_Data1, Imag_Data2};  // Matriz de direccion de datos (Reales e Imaginarios)
int Dato[4];                       // Matriz acomuladora de datos (Reales e Imaginarios)
int Valid_Sweep;
int Conf_State;
long Frec_I = 0x2E147B;            // Frecuencia inicial de 90KHz
long Frec_F = 0x28F5C;             // Frecuencia de resolucion de 5KHz    
byte Frec_lower;                   // Byte menor del dato de frecuencia a ingresar  
byte Frec_med;                     // Byte medio del dato de frecuencia a ingresar                      
byte Frec_upper;                   // Byte mayor del dato de frecuencia a ingresar  
byte IF_lower;                     // Byte menor del dato de incremento de frecuencia a ingresar
byte IF_med;                       // Byte medio del dato de incremento de frecuencia a ingresar
byte IF_upper;                     // Byte mayor del dato de incremento de frecuencia a ingresar 
byte NI_lower;                     // Byte menor del dato del numero de incrementos de frecuencia a ingresar 
byte NI_upper;                     // Byte mayor del dato del numero de incrementos de frecuencia a ingresar
int Dato_Real[9];
int Dato_R;
int Dato_I;  
long Mag_Prev;
long Magnitude[3];
long Mag[3];
long Valf_Mag;
word Gain_Ref;
byte Cycles = 0x14;
int Rref = 470;
long B_Prev;
long Bio_impedance;
float EC3 = -0.5553;
float EC1 = -2.7232;
float EC2 = -0.9145;
float EC4 = -0.2034;
int Stage = 0;
int SW = 2;
 
void setup() 
{
  Serial.begin(9600);   // 
  Wire.begin();
  Program_Port(); 
  
}

//////////////////   Ciclo de Sensado   ////////////////////////

void loop() 
{
  Impedance();
  Stage = 0;
  conf = 0;
}

//////////////////   Fin del Ciclo Principal    /////////////////////

void Impedance ()
{ 

  while(Stage < 1)
  {
    Imp_Teor();
  }
}

void Imp_Teor ()
{

  Value_Imp();
  B_Prev = (Valf_Mag * EC3)+11538 ;
  if(B_Prev <= 330)
  {
    Bio_impedance = 0;
  }  
  else if((B_Prev <= 392)&&(B_Prev > 330))
  {
    Bio_impedance = (Valf_Mag * EC1)+ 54948;
  }
  else if((B_Prev <= 548)&&(B_Prev > 392))
  {
    Bio_impedance = (Valf_Mag * EC2)+ 18644;    
  }
  else if((B_Prev >= 799)&&(B_Prev < 3000))
  {
    Bio_impedance = (Valf_Mag * EC4)+ 4745.7;    
  }
  else if(B_Prev >= 3000)
  {
    Bio_impedance = 10000;    
  }  
  else
  {
    Bio_impedance = B_Prev;
  }
  Serial.print("Valor de Bioimpedancia: ");
  Serial.print(Bio_impedance);
  Serial.println(" Ohms");
  Stage = 1;
}



void Value_Imp()
{

  while(conf<6)
  {
    if((conf==0)||(conf==2)||(conf==4)||(conf==6))
    {
      Init_Command();
      delay(14);
    }
    
    if(conf == 1)
    { 
      digitalWrite(SW, HIGH);
      Taking_Data();
      Set_CR1(Rst);
    } 
    
    if((conf == 3)||(conf==5)||(conf==7))
    { 
      digitalWrite(SW, LOW);      
      Taking_Data();
    }
    conf++;      
  } 
}

void Program_Port()
{
  pinMode(SW, OUTPUT);
}

void Taking_Data ()
{
  AddressPointer(Status);
  ReadCommand_Status();
  Conf_State = (State & 0x02);
  while(Conf_State == VRI_Data)
  {
    for(i=0; i<4; i++)
    {
      AddressPointer(Reg_Data[i]);                             // Selecciono uno a uno los registros donde estan los datos reales e imaginarios
      delay(10);
      ReadCommand();                                // Leo uno a uno los datos almacenados y los coloco en una matriz
      if(i==3)
      {
        Set_CR(Incr_Frequency);
        if(Prev_State == 0)
        {
          Valid_Data(); 
        }
        else if(Prev_State == 1)
        {         
          Mag_Data2(); 
        }
        else if(Prev_State == 2)
        {               
          Mag_Data1();          
        }
        if(j==3)                                        
        {                                                
          AddressPointer(Status);
          ReadCommand_Status();
          Valid_Sweep = (State & 0x04);   
          if(Valid_Sweep == FS_Valid)
          {
            Mag[m] = (Magnitude[0]+ Magnitude[1]+Magnitude[2])/3;
            if(conf > 1)
            {
              Add_Prom();
            }
            Conf_State =  0x00;
            Prev_State = 0;
            j=0;
            a=0;
            b=0;
          }
        }  
      }      
    }
  }
}

void Init_Command ()
{

  WriteCommand_Freq();
  Increment_Freq();
  NumberIncrement();
  CycleNumber();
  Set_CR(Standby_Mode);
  Set_CR(Gain_X1);   
  Set_CR(Start_Process);
  Set_CR(Sweep_Active);
  delay(10);
  
}


void WriteCommand_Freq()                  // Funcion para escribir la frecuencia requerida
{
  Frec_lower = 0x7B;
  Frec_med = 0x14;
  Frec_upper = 0x2E;  
  Wire.beginTransmission(Slave_Add);  // Envia los 7 bits de direccion del AD5933 + el bit de escritura (0), para comenzar
  Wire.write(Start_Freq1);
  Wire.write(Frec_upper);
  Wire.endTransmission();
    
  Wire.beginTransmission(Slave_Add);  // Envia los 7 bits de direccion del AD5933 + el bit de escritura (0), para comenzar
  Wire.write(Start_Freq2);
  Wire.write(Frec_med);
  Wire.endTransmission(); 

  Wire.beginTransmission(Slave_Add);  // Envia los 7 bits de direccion del AD5933 + el bit de escritura (0), para comenzar
  Wire.write(Start_Freq3);
  Wire.write(Frec_lower);
  Wire.endTransmission(); 
}

void Increment_Freq()                  // Funcion para la resolucion de los incrementos de la frecuencia
{
  IF_lower = 0x5C;
  IF_med = 0x8F;
  IF_upper = 0x02;  

  Wire.beginTransmission(Slave_Add);  // Envia los 7 bits de direccion del AD5933 + el bit de escritura (0), para comenzar
  Wire.write(Freq_incre1);
  Wire.write(IF_upper);
  Wire.endTransmission(); 
    
  Wire.beginTransmission(Slave_Add);  // Envia los 7 bits de direccion del AD5933 + el bit de escritura (0), para comenzar
  Wire.write(Freq_incre2);
  Wire.write(IF_med);
  Wire.endTransmission(); 

  Wire.beginTransmission(Slave_Add);  // Envia los 7 bits de direccion del AD5933 + el bit de escritura (0), para comenzar
  Wire.write(Freq_incre3);
  Wire.write(IF_lower);
  Wire.endTransmission(); 
}

void NumberIncrement()                 // Funcion para establecer el numero de incrementos
{
  NI_lower = 0x02;
  NI_upper = 0x00;  
    
  Wire.beginTransmission(Slave_Add);  // Envia los 7 bits de direccion del AD5933 + el bit de escritura (0), para comenzar
  Wire.write(Num_incre1);
  Wire.write(NI_upper);
  Wire.endTransmission(); 

  Wire.beginTransmission(Slave_Add);  // Envia los 7 bits de direccion del AD5933 + el bit de escritura (0), para comenzar
  Wire.write(Num_incre2);
  Wire.write(NI_lower);
  Wire.endTransmission(); 
}

void CycleNumber()
{
  Wire.beginTransmission(Slave_Add);  // Envia los 7 bits de direccion del AD5933 + el bit de escritura (0), para comenzar
  Wire.write(Set_cycles2);
  Wire.write(Cycles);
  Wire.endTransmission();
}   

void AddressPointer (byte Reg_Address)                  // Funcion que ubica en el registro la direccion posterior a leer
{
  
  Wire.beginTransmission(Slave_Add);  // Envia los 7 bits de direccion del AD5933 + el bit de escritura (0), para comenzar
  Wire.write(Set_AddPointer);
  Wire.write(Reg_Address);
  Wire.endTransmission(); 
}

void ReadCommand ()                // Funcion para leer los registros de los datos reales e imaginarios sensados
{
  Wire.requestFrom(Slave_Add,1);    // request 6 bytes from slave device #2
  while(Wire.available())    // slave may send less than requested
  { 
    Dato[i] = Wire.read();    // receive a byte as character
  }   
}

void Set_CR (byte Control)                     // Coloca en el registro de Control datos para avanzar en el proceso
{
  
  Wire.beginTransmission(Slave_Add);  // Envia los 7 bits de direccion del AD5933 + el bit de escritura (0), para comenzar
  Wire.write(Byte_Control1);
  Wire.write(Control);
  Wire.endTransmission();   
 
}

void Set_CR1 (byte Control)                     // Coloca en el registro de Control datos para avanzar en el proceso
{
  
  Wire.beginTransmission(Slave_Add);  // Envia los 7 bits de direccion del AD5933 + el bit de escritura (0), para comenzar
  Wire.write(Byte_Control2);
  Wire.write(Control);
  Wire.endTransmission();   
 
}

void ReadCommand_Status ()          // Lee unicamente el registro de Estado para obtener datos de confirmacion
{
  Wire.requestFrom(Slave_Add,1);    // request 6 bytes from slave device #2
  while(Wire.available())    // slave may send less than requested
  { 
    State = Wire.read();    // receive a byte as character
  }  
  delay(10);
}

void Mag_Data1 ()
{
  Dato_R = (Dato[0]<<8)|(Dato[1]);
  Dato_I = (Dato[2]<<8)|(Dato[3]);   
  Mag_Prev = (pow(Dato_R,2))+(pow(Dato_I,2));
  Magnitude[j] = sqrt(Mag_Prev); 
  if(Magnitude[j] >= 8000)
  {
    j++;
  }
  if(Magnitude[j]<=1000)
  {
    n++;
    if(n == 3)
    {
      Prev_State = 0;
      a=0;
      b=0;
      n=0;
      j=0;
      conf = 1;
    }
  }
}

void Mag_Data2 ()
{
  Magnitude[0] = 8000;
  Magnitude[1] = 8000;
  Magnitude[2] = 8000;  
  j = 3;  
}

void Valid_Data ()
{
  Dato_Real[h] = (Dato[0]<<8)|(Dato[1]);
  if(Dato_Real[h] <= 1000)
  {
    a++;     
  }
  else if(Dato_Real[h] > 1000)
  {
    b++;     
  }
  k++;
  if(k==3)
  {        
    AddressPointer(Status);
    ReadCommand_Status();
    Valid_Sweep = (State & 0x04);
      if(Valid_Sweep == FS_Valid)
      {
        Conf_State =  0x00;
        k=0;
      }       
  }   
  h++;
  if(h == 6)
  {
    if(a >= b)
    {
      Prev_State = 1;
      h=0;   
    }
    else if(a < b)
    {
      Prev_State = 2;
      h=0;    
    }
  }
}

void Add_Prom ()
{
  m++;
  if(m == 3)
  {
    Valf_Mag = (Mag[0]+Mag[1]+Mag[2])/3;
    m=0;  
  }
}
















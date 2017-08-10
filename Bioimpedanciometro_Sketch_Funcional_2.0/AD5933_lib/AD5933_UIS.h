#ifndef AD5933_UIS_h
#define AD5933_UIS_h

/**
 * Includes
 */
#include <Arduino.h>
#include <Wire.h>

/**
 * Datos Importantes (Registros y Variables Fijas)
 */

#define Slave_Add      (B0001101)   // La dirección asignada por defecto al AD5933 que es nuestro dispositivo esclavo 
#define Block_W        (0xA0)        // Comando para escribir en el AD5933 
#define Block_R        (0xA1)        // Comando para leer del AD5933 
#define Set_AddPointer (0xB0)        // Comando para seleccionar el registro a leer 
#define VRI_Data       (0x02)        // Comando de confimacion de Dato valido en el registro de Estado
#define FS_Valid       (0x04)        // Finalizacion del Barrido en Frecuencia en el registro de Estado
#define Start_Process  (0x10)        // Dato en el registro de control para comenzar proceso
#define Standby_Mode   (0xB0)        // Inicializacion de la alimentacion del sistema 
#define Sweep_Active   (0x20)        // Activar Barrido de Frecuencia
#define Incr_Frequency (0x30)        // Pasar de un valor de frecuencia al siguiente
#define Gain_X1        (0x01)        // Establecimiento de Ganancia en un Factor de x1
#define Port_OutD      (0x40)        // Puertos de Salida D
#define Port_OutC      (0x03)        // Puertos de Salida C
#define Switch_Ch1     (0x40)        // Activacion del canal 1 del switch
#define Switch_Ch2     (0x00)        // Activacion del canal 2 del switch 
#define Reset          (0x01)        // Reset para comenzar el sensado de la Z desconocida sin desenergizar   

                        /** Registros */
                        
#define Byte_Control1  (0x80)        // #define de Control 1  
#define Byte_Control2  (0x81)        // #define de Control 2  
#define Start_Freq1    (0x82)        // #define 1 frecuencia de inicio de Sensado  
#define Start_Freq2    (0x83)        // #define 2 frecuencia de inicio de Sensado 
#define Start_Freq3    (0x84)        // #define 3 frecuencia de inicio de Sensado  
#define Freq_incre1    (0x85)        // #define 1 para los increments de frecuencia  
#define Freq_incre2    (0x86)        // #define 2 para los increments de frecuencia
#define Freq_incre3    (0x87)        // #define 3 para los increments de frecuencia
#define Num_incre1     (0x88)        // #define 1 para el # increments de frecuencia
#define Num_incre2     (0x89)        // #define 2 para el # increments de frecuencia
#define Set_cycles1    (0x8A)        // #define 1 para el # de ciclos para cada Sensado
#define Set_cycles2    (0x8B)        // #define 2 para el # de ciclos para cada Sensado
#define Status         (0x8F)        // #define que confirma medida Sensada
#define Temp_Data1     (0x92)        // #define 1 para el dato de temperatura
#define Temp_Data2     (0x93)        // #define 2 para el dato de temperatura
#define Real_Data1     (0x94)        // #define 1 dato Real 
#define Real_Data2     (0x95)        // #define 2 dato Real 
#define Imag_Data1     (0x96)        // #define 1 dato Imaginario 
#define Imag_Data2     (0x97)        // #define 2 dato Imaginario 

class AD5933_UIS {
    public:
        
		void Impedance();
		
    private:
		
		void Program_Port();
        void Init_Command();
		void Taking_Data();
		void Set_CR1(byte);
		void AddressPointer(byte);
		void ReadCommand_Status();
		void ReadCommand();
		void Set_CR(byte);
		void Mag_Data();
		int i ;                            // Valor del ciclo en proceso    
		int conf;                      // Valor de Estado del Ciclo (Todo el proceso se puede colocar en el void setup pues sera realizado una sola vez)
		int State;                         // Dato de confirmacion para lectura de registros 
		int Reg_Data[4]= {Real_Data1, Real_Data2, Imag_Data1, Imag_Data2};  // Matriz de direccion de datos (Reales e Imaginarios)
		int Dato[4];                       // Matriz acomuladora de datos (Reales e Imaginarios)
		int Valid_Sweep;
		int Conf_State;    
		int Dato_R;
		int Dato_I;	
		
};

#endif





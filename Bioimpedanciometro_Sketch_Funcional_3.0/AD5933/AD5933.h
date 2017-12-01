#ifndef AD5933_h
#define AD5933_h

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
#define Rst            (0x01)        // Reset para comenzar el sensado de la Z desconocida sin desenergizar   

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

class AD5933 {
    public:
        AD5933(int St);
		long impedance();
		
    private:
		
		void Program_Port();
        void Init_Command();
		void Taking_Data();
		void Value_Imp();
		long Imp_Teor();
		void Set_CR1(byte);
		void AddressPointer(byte);
		void ReadCommand_Status();
		void ReadCommand();
		void Set_CR(byte);
		void Mag_Data1();
	    void WriteCommand_Freq();
	    void Increment_Freq();
	    void NumberIncrement();
	    void CycleNumber();
		void Mag_Data2();
		void Valid_Data();
		void Add_Prom();
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
		long BioImpedance;
		float EC3 = -0.5553;
		float EC1 = -2.7232;
		float EC2 = -0.9145;
		float EC4 = -0.2034;
		int Stage = 0;
		int _Stage ;
		int St;
		int SW = 2;
		
};

#endif





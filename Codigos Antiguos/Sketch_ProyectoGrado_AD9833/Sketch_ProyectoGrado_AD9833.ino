/////////////////////////////////////////////// SKETCH AD9833 ////////////////////////////////////////////////////////

#include <SPI.h>

/*
 * Se definen los registros para el integrado AD9833 de acuerdo a lo planteado en el datasheet
*/

#define  REG_COMP          0x2000     // Bit para el registro completo (MSB y LSB)
#define  REG_FSELECT_0     0x0000     // Bit para el registro seleccion FREQ0
#define  REG_PSELECT_0     0x0000     // Bit para el registro seleccion PHASE0
#define  REG_FSELECT_1     0x0800     // Bit para el registro seleccion FREQ1
#define  REG_PSELECT_1     0x0400     // Bit para el registro seleccion PHASE1
#define  REG_RESET         0x0100     // Bit para el registro del reset
#define  REG_SLEEP_1       0x0080     // Bit registro para desactivar MCLK
#define  REG_SLEEP_12      0x0040     // Bit registro para desactivar DAC interno
#define  REG_OPBITEN       0x0020     // Bit registro. Si D5=1 VOUT esta conectado a MSB del DAC, si D5=0 esta conectado a la salida de DAC
#define  REG_DIV2          0x0008     // Bit registro. Si D5=1 y D3=1 VOUT esta conectado a MSB del DAC, si D5=1 y D3=0 VOUT esta conectado a MSB/2 del DAC
#define  REG_TRIANG        0x0002     // Bit regstro para seleccionar el tipo de onda triangular
#define  REG_SENO          0x0000     // Bit regstro para seleccionar el tipo de onda senoidal

// Bits de Registro completo para la implementación del tipo de onda
#define SENOIDAL                 (REG_COMP | REG_SENO)
#define TRIANGULAR               (REG_COMP | REG_TRIANG)
#define CUADRADA                 (REG_COMP | REG_OPBITEN)
#define CUADRADA_RAPIDA          (CUADRADA | REG_DIV2)

// Bits de Registro para seleccion de Fase y Frecuencia

#define FREQ0                0x4000
#define PHASE0               0xC000
#define FREQ1                0x8000
#define PHASE1               0xE000
#define REF_FREQ             25000000.0
#define SPI_VEL_RELOJ        12000000.0 


/*
 * Para el Arduino Mega= FSYNC = 53; CLK = 52; DATA = 51;
 * Para el arduino Uno=  FSYNC = 10; CLK = 13; DATA = 11;
 */

const int FSYNC = 53;                       // Pines para comunicación SPI 
const int CLK = 52;                         
const int DATA = 51;

// Seleccion de Onda por Botones
int b0=1;
int b1=0;
int b2=0;
int b3=0;
int b4=0;
int FORMA;

// Seleccion del valor de Frecuencia

int Frec = 1000000;

void setup() {
  
/// Iniciando comunicación serial ///
  
//  Serial.begin(115200);

  SPI.begin();
  
  T_ONDA(FORMA,Frec);
  delay(1);

}
/// Ciclo para definir /// 
void loop() {
  if (b0 == 1)
  {
    FORMA = SENOIDAL;
  }
  else if (b1==1)
  {
    FORMA = TRIANGULAR;
  }
  else if (b2==1)
  {
    FORMA = CUADRADA;
  }
  else if (b3==1)
  {
    FORMA = CUADRADA_RAPIDA;
  } 
  else
  {
    FORMA = REG_SENO;
  }  
}
/// Escritura en Registros para la forma y frecuencia de la Onda ///

void T_ONDA(int FormaOnda, long Frecuencia) {
  long Dato_frec = Frecuencia * pow(2, 28) / REF_FREQ;
  int Frec_MSB = (int)(Dato_frec >> 14) | FREQ0;
  int Frec_LSB = (int)(Dato_frec & 0x3FFF) | FREQ0;

  EscRegistro(0x2100);   
  EscRegistro(Frec_LSB);                  // Escribir los 16 bits mas bajos para los registros del AD9833
  EscRegistro(Frec_MSB);                  // Escribir los 16 bits mas altos para los registros del AD9833
  EscRegistro(PHASE0);                    // Registro de Fase
  EscRegistro(FormaOnda);                 // Salida y eleccion del Tipo de Onda

}

void EscRegistro(int Dato) { 
  
  // Transmision SPI para los registros del AD9833
  
  SPI.setDataMode(SPI_MODE2);       
  
  digitalWrite(FSYNC, LOW);           // Colocar FSYNC en bajo para poder escribir los registros del AD9833
  delayMicroseconds(100);              // Darle al AD9833 para estar listo en la recepción de datos.
  
  SPI.transfer(highByte(Dato));        // Cada registro del AD9833 es de 32 bits y estos estan catalogados en paquetes hexagesimales de 16 Bit.
  SPI.transfer(lowByte(Dato));         // Y cada uno de estos paquetes se trasmiten en Bytes es decir 2 Bytes = 16 Bits (Un paquete a transmitir).

  digitalWrite(FSYNC, HIGH);          //Escritura completa. FSYNC en alto de nuevo.
}



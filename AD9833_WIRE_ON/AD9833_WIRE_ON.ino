
#include <Wire.h>
#include <SPI.h>

#define selector 6
#define PWM_pin 5

const int slaveSelectPin_DDS1 = 14;

  //* CS - to digital pin 10  (SS pin)
  //* SDI - to digital pin 11 (MOSI pin)
  //* CLK - to digital pin 13 (SCK pin)

#define SPI_CLOCK_SPEED      12000000

// Variables para Tipo de Corte
int Kind_Cut_Coag;

void setup() {

  Wire.begin(7);
  pinMode(selector,OUTPUT);
  digitalWrite(selector,HIGH);

// MODIFICAR LA FRECUENCIA DEL PWM DEL PIN 9 (PIN DIGITAL 5 DE ARDUINO)

  //TCCR0B = _BV(CS00);             // Frecuencia de 31372 Hz para el pin PWM
  //TCCR0B = _BV(CS01);             // Frecuencia de 3921 Hz para el pin PWM
  //TCCR0B = _BV(CS00) | _BV(CS01); // Frecuencia de 490 Hz para el pin PWM, Valor por defecto en el Microprocesador
  //TCCR0B = _BV(CS02);             // Frecuencia de 122 Hz para el pin PWM
  //TCCR0B = _BV(CS00) | _BV(CS02); // Frecuencia de 30 Hz para el pin PWM
  
  pinMode(slaveSelectPin_DDS1,OUTPUT);
  // initialize SPI:
  SPI.begin();
  delay(10);
  for (int hola = 1; hola < 10; hola++) {
    SET_DDS_1();
    delay(20);
    }
  Serial.begin(9600);
}

void loop() {
  Wire.onReceive(receiveEvent); // register event 
  if(Kind_Cut_Coag == 1)
  {
    analogWrite(PWM_pin,255);
    digitalWrite(selector,LOW);    
  }
  if(Kind_Cut_Coag == 2)
  {
    analogWrite(PWM_pin,242);
    digitalWrite(selector,LOW);    
  }
  if(Kind_Cut_Coag == 3)
  {
    digitalWrite(selector,HIGH);    
  }  
  if(Kind_Cut_Coag == 4)
  {
    analogWrite(PWM_pin,153);
    digitalWrite(selector,LOW);    
  }
  if(Kind_Cut_Coag == 5)
  {
    analogWrite(PWM_pin,127);
    digitalWrite(selector,LOW);    
  }
  if(Kind_Cut_Coag == 6)
  {
    analogWrite(PWM_pin,102);
    digitalWrite(selector,LOW);        
  }  

}

void receiveEvent(int howMany) {
  while (0 < Wire.available()) { // loop through all but the last

   Kind_Cut_Coag = Wire.read(); // receive byte as a character
   delay(10);
  }
}

void SET_DDS_1() {

  // take the SS pin low to select the chip:
  SPI.beginTransaction(SPISettings(SPI_CLOCK_SPEED, MSBFIRST, SPI_MODE2));   
  
  digitalWrite(slaveSelectPin_DDS1, LOW);
  //  send in the address and value via SPI:
  
  SPI.transfer16(0x2128); //Trama de configuración y activación del reset
  SPI.transfer16(0x6707); //01 + dato de frecuencia que son 14 bits los menos significativos
  SPI.transfer16(0x40C4); //01 + dato de frecuencia que son 14 bits los mas significativos
  SPI.transfer16(0xC000); //Fase, No mover se va a dejar en cero, en este caso no importa =)
  SPI.transfer16(0x2028); //Trama de configuración y desactivación del reset
  
  // take the SS pin high to de-select the chip:
  digitalWrite(slaveSelectPin_DDS1, HIGH);
  SPI.endTransaction();
}

void SELECCION_MUX(){

  
  }



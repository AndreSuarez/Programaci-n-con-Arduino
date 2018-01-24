#include <Wire.h>

int Bio_Value = 800;
byte Bio_ValF;
byte Bio_ValS;
int Pot_Value= 90;
int Ciclo_Value = 2;
int Pass_Value = 1;

void setup() 
{
  Serial.begin(9600);
  Wire.begin(); // join i2c bus (address optional for master)
}

void loop() 
{
  
  Bio_Comm(); 
  Wire.beginTransmission(9); // transmit to device #9
  Wire.write(Pot_Value);              // sends one byte
  Wire.write(Bio_ValF);              // sends one byte
  Wire.write(Bio_ValS);              // sends one byte
  Wire.write(Ciclo_Value);              // sends one byte
  Wire.write(Pass_Value);
  Wire.endTransmission();    // stop transmitting
  for(int i=0; i<1000; i++)
  {
    Serial.println("Terminado");  
  }
}

void Bio_Comm()
{ 
  Serial.println(Bio_Value);
  Bio_ValF = (Bio_Value >> 8)&(11111111);
  Serial.println(Bio_ValF);
  Bio_ValS = Bio_Value; 
  Serial.println(Bio_ValS); 
}

